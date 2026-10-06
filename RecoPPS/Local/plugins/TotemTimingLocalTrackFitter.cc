/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *   Mateusz Szpyrka (mateusz.szpyrka@cern.ch)
 *   Christopher Misan (krzysztof.misan@cern.ch)
 *
 ****************************************************************************/

#include <memory>
#include <utility>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemTimingDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemTimingLocalTrack.h"

#include "RecoPPS/Local/interface/TotemTimingTrackRecognition.h"
template <typename T>
class TotemTimingLocalTrackFitter : public edm::stream::EDProducer<> {
public:
  explicit TotemTimingLocalTrackFitter(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  static edm::ParameterSetDescription fillDescriptionsShared(edm::ConfigurationDescriptions&);
  void produce(edm::Event&, const edm::EventSetup&) override;

  const edm::EDGetTokenT<std::vector<TotemTimingRecHit> > recHitsToken_;
  const int maxPlaneActiveChannels_;
  const edm::ParameterSet trk_algo_params_;
  std::map<CTPPSDetId, TotemTimingTrackRecognition> trk_algo_map_;
};
template <typename T>
TotemTimingLocalTrackFitter<T>::TotemTimingLocalTrackFitter(const edm::ParameterSet& iConfig)
    : recHitsToken_(consumes<std::vector<TotemTimingRecHit> >(iConfig.getParameter<edm::InputTag>("recHitsTag"))),
      maxPlaneActiveChannels_(iConfig.getParameter<int>("maxPlaneActiveChannels")),
      trk_algo_params_(iConfig.getParameter<edm::ParameterSet>("trackingAlgorithmParams")) {
  produces<std::vector<TotemTimingLocalTrack> >();
}
template <typename T>
void TotemTimingLocalTrackFitter<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  auto pOut = std::make_unique<std::vector<TotemTimingLocalTrack> >();

  edm::Handle<std::vector<TotemTimingRecHit> > recHits;
  iEvent.getByToken(recHitsToken_, recHits);

  std::map<T, int> planeActivityMap;
  std::unordered_map<CTPPSDetId, std::vector<const TotemTimingRecHit*> > recHitsMap;

  auto motherId = [](const auto& detid) {
    T out(detid);
    out.setChannel(0);
    return out;
  };

  for (const auto& rechit : *recHits) {
    const auto rechit_detid = rechit.detId();
    recHitsMap[rechit_detid].emplace_back(&rechit);
    ++planeActivityMap[motherId(rechit_detid)];
  }

  // feed hits to the track producers
  for (const auto& [raw_detid, rechits] : recHitsMap) {
    T detid(raw_detid.arm(), raw_detid.station(), raw_detid.rp());
    // if algorithm is not found, build it
    if (trk_algo_map_.count(detid) == 0)
      trk_algo_map_.insert(std::make_pair(detid, trk_algo_params_));

    auto detId = motherId(raw_detid);
    if (planeActivityMap[detId] > maxPlaneActiveChannels_)
      continue;

    detId.setPlane(0);
    for (const auto& hit : rechits) {
      if (trk_algo_map_.find(detId) == trk_algo_map_.end())
        throw cms::Exception("TotemTimingLocalTrackFitter")
            << "Invalid detId for rechit: arm=" << detId.arm() << ", rp=" << detId.rp();
      trk_algo_map_.at(detId).addHit(*hit);
    }
  }

  // retrieves tracks for all hit sets
  for (auto& [detid, trk_algo] : trk_algo_map_)
    trk_algo.produceTracks(detid, *pOut);

  iEvent.put(std::move(pOut));

  // remove all hits from the track producers to prepare for the next event
  for (auto& trk_algo_entry : trk_algo_map_)
    trk_algo_entry.second.clear();
}
template <typename T>
edm::ParameterSetDescription TotemTimingLocalTrackFitter<T>::fillDescriptionsShared(
    edm::ConfigurationDescriptions& descr) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("recHitsTag", edm::InputTag("totemTimingRecHits"))
      ->setComment("input rechits collection to retrieve");
  desc.add<int>("maxPlaneActiveChannels", 2)->setComment("threshold for discriminating noisy planes");

  edm::ParameterSetDescription trackingAlgoParams;
  trackingAlgoParams.add<double>("threshold", 1.5)
      ->setComment("minimal number of rechits to be observed before launching the track recognition algorithm");
  trackingAlgoParams.add<double>("thresholdFromMaximum", 0.5)
      ->setComment("threshold relative to hit profile function local maximum for determining the width of the track");
  trackingAlgoParams.add<double>("resolution", 0.01 /* mm */)
      ->setComment("spatial resolution on the horizontal coordinate (in mm)");
  trackingAlgoParams.add<double>("sigma", 0.)
      ->setComment("pixel efficiency function parameter determining the smoothness of the step");
  trackingAlgoParams.add<double>("tolerance", 0.1 /* mm */)
      ->setComment("tolerance used for checking if the track contains certain hit");

  trackingAlgoParams.add<std::string>("pixelEfficiencyFunction", "(x>[0]-0.5*[1]-0.05)*(x<[0]+0.5*[1]-0.05)+0*[2]")
      ->setComment(
          "efficiency function for single pixel\n"
          "can be defined as:\n"
          " * Precise: "
          "(TMath::Erf((x-[0]+0.5*([1]-0.05))/([2]/4)+2)+1)*TMath::Erfc((x-[0]-0.5*([1]-0.05))/([2]/4)-2)/4\n"
          " * Fast: "
          "(x>[0]-0.5*([1]-0.05))*(x<[0]+0.5*([1]-0.05))+((x-[0]+0.5*([1]-0.05)+[2])/"
          "[2])*(x>[0]-0.5*([1]-0.05)-[2])*(x<[0]-0.5*([1]-0.05))+(2-(x-[0]-0.5*([1]-0.05)+[2])/"
          "[2])*(x>[0]+0.5*([1]-0.05))*(x<[0]+0.5*([1]-0.05)+[2])\n"
          " * Legacy: (1/(1+exp(-(x-[0]+0.5*([1]-0.05))/[2])))*(1/(1+exp((x-[0]-0.5*([1]-0.05))/[2])))\n"
          " * Default (sigma ignored): (x>[0]-0.5*[1]-0.05)*(x<[0]+0.5*[1]-0.05)+0*[2]\n"
          "with:\n"
          "  [0]: centre of pad\n"
          "  [1]: width of pad\n"
          "  [2]: sigma: distance between efficiency ~100 -> 0 outside width");

  trackingAlgoParams.add<double>("yPosition", 0.0)->setComment("vertical offset of the outcoming track centre");
  trackingAlgoParams.add<double>("yWidth", 0.0)->setComment("vertical track width");
  desc.add<edm::ParameterSetDescription>("trackingAlgorithmParams", trackingAlgoParams)
      ->setComment("list of parameters associated to the track recognition algorithm");
  return desc;
}
template <>
void TotemTimingLocalTrackFitter<TotemTimingDetId>::fillDescriptions(edm::ConfigurationDescriptions& descr) {
  auto desc = fillDescriptionsShared(descr);
  descr.add("totemTimingLocalTracks", desc);
}

template <>
void TotemTimingLocalTrackFitter<CTPPSDiamondDetId>::fillDescriptions(edm::ConfigurationDescriptions& descr) {
  auto desc = fillDescriptionsShared(descr);
  descr.add("diamondSampicLocalTracks", desc);
}
template class TotemTimingLocalTrackFitter<CTPPSDiamondDetId>;
template class TotemTimingLocalTrackFitter<TotemTimingDetId>;
DEFINE_FWK_MODULE(TotemTimingLocalTrackFitter<TotemTimingDetId>);
DEFINE_FWK_MODULE(TotemTimingLocalTrackFitter<CTPPSDiamondDetId>);
