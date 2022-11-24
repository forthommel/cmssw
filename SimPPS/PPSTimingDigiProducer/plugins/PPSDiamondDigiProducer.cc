/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 * Based on work by:
 *   Morteza Pourkazemi
 *   Seyed Mohsen Etesami
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"

class PPSDiamondDigiProducer : public edm::stream::EDProducer<> {
public:
  explicit PPSDiamondDigiProducer(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  const edm::EDGetTokenT<CrossingFrame<PSimHit> > crossingFrameToken_;
  const double time_to_digi_time_;
  edm::EDPutTokenT<edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiToken_;
};

PPSDiamondDigiProducer::PPSDiamondDigiProducer(const edm::ParameterSet& iConfig)
    : crossingFrameToken_(consumes<CrossingFrame<PSimHit> >(iConfig.getParameter<edm::InputTag>("inputs"))),
      time_to_digi_time_(iConfig.getParameter<double>("timeToDigiTime")),
      diamondDigiToken_(produces<edm::DetSetVector<CTPPSDiamondDigi> >()) {
  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable())
    throw cms::Exception("Configuration")
        << "This class requires the RandomNumberGeneratorService\n"
           "which is not present in the configuration file.  You must add the service\n"
           "in the configuration file or remove the modules that require it.";
}

void PPSDiamondDigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Service<edm::RandomNumberGenerator> rng;
  // at this stage, we already checked the service was part of the process
  auto* rnd = &rng->getEngine(iEvent.streamID());

  // first "massage" the input collection to classify by diamond channel
  std::unordered_map<unsigned int, std::vector<PSimHit> > simhits;
  MixCollection<PSimHit> allHits{iEvent.getHandle(crossingFrameToken_).product()};
  for (const auto& simhit : allHits)
    simhits[simhit.detUnitId()].emplace_back(simhit);

  // then iterate on all channels
  edm::DetSetVector<CTPPSDiamondDigi> digis;  // output collection
  for (const auto& rh_vs_detid : simhits) {
    auto& ds = digis.find_or_insert(CTPPSDiamondDetId(rh_vs_detid.first));
    for (const auto& simhit : rh_vs_detid.second) {
      unsigned int leading_edge = simhit.timeOfFlight() * time_to_digi_time_;
      unsigned int tot = 0;
      unsigned int trailing_edge = leading_edge + tot;
      unsigned int thr_voltage = 0;
      bool multi_hits = false;
      unsigned short hptdc_err = 0;
      ds.emplace_back(leading_edge, trailing_edge, thr_voltage, multi_hits, hptdc_err);
    }
  }
  iEvent.emplace(diamondDigiToken_, std::move(digis));  // store output collection to event
}

void PPSDiamondDigiProducer::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::endStream() {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("inputs", edm::InputTag("mix", "g4SimHitsCTPPSTimingHits"));
  desc.add<double>("timeToDigiTime", 1024 / 25.e-9)
      ->setComment("conversion factor between hit time of arrival (in seconds) and HPTDC bin size");
  descriptions.add("PPSDiamondDigitizer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PPSDiamondDigiProducer);
