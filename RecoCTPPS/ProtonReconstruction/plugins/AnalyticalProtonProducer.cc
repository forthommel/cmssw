/****************************************************************************
 * Authors:
 *   Frigyes Nemes
 *   Laurent Forthomme
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLiteFwd.h"

#include "DataFormats/ProtonReco/interface/ForwardProton.h"
#include "DataFormats/ProtonReco/interface/ForwardProtonFwd.h"

#include "CondFormats/DataRecord/interface/CTPPSOpticsRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/LHCOpticalFunctionsSetCollection.h"

class AnalyticalProtonProducer : public edm::stream::EDProducer<> {
public:
  explicit AnalyticalProtonProducer(const edm::ParameterSet&);
  ~AnalyticalProtonProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  edm::EDGetTokenT<CTPPSLocalTrackLiteCollection> tracksToken_;
  const std::string opticsTag_;
};

AnalyticalProtonProducer::AnalyticalProtonProducer(const edm::ParameterSet& iConfig)
    : tracksToken_(consumes<CTPPSLocalTrackLiteCollection>(iConfig.getParameter<edm::InputTag>("tracksTag"))),
      opticsTag_(iConfig.getParameter<std::string>("opticalFunctionsTag")) {
  produces<reco::ForwardProtonCollection>();
}

void AnalyticalProtonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  //--- retrieve the input collection
  edm::Handle<CTPPSLocalTrackLiteCollection> hTracks;
  iEvent.getByToken(tracksToken_, hTracks);
  //--- avoid launching the reconstruction if no local tracks in event
  if (hTracks->empty())
    return;

  //--- retrieve the conditions
  edm::ESHandle<LHCOpticalFunctionsSetCollection> hOpticalFunctions;
  iSetup.get<CTPPSOpticsRcd>().get(opticsTag_, hOpticalFunctions);

  //--- book the output
  auto pOut = std::make_unique<reco::ForwardProtonCollection>();

  iEvent.put(std::move(pOut));
}

void AnalyticalProtonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("tracksTag", edm::InputTag("ctppsLocalTrackLiteProducer"))
      ->setComment("input local tracks collection");
  desc.add<std::string>("opticalFunctionsTag", "")->setComment("condition database tag for optical functions");
  descriptions.add("ppsProtonsAnalytical", desc);
}

DEFINE_FWK_MODULE(AnalyticalProtonProducer);
