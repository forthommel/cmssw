/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

template <typename T>
class DetSetVectorFlattener : public edm::global::EDProducer<> {
public:
  explicit DetSetVectorFlattener(const edm::ParameterSet& iConfig)
      : input_token_{consumes<edm::DetSetVector<T> >(iConfig.getParameter<edm::InputTag>("input"))} {
    produces<std::vector<T> >();
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descr) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("input", edm::InputTag{})->setComment("input collection to retrieve");
    descr.addWithDefaultLabel(desc);
  }

private:
  void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const override {
    auto pOut = std::make_unique<std::vector<T> >();
    for (auto& [detid, items] : iEvent.get(input_token_))
      for (auto& item : items) {
        const_cast<T&>(item).setDetId(CTPPSDetId{detid});
        pOut->push_back(item);
      }
    iEvent.put(std::move(pOut));
  }

  const edm::EDGetTokenT<edm::DetSetVector<T> > input_token_;
};

#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"

using CTPPSDiamondRecHitFlattener = DetSetVectorFlattener<CTPPSDiamondRecHit>;
DEFINE_FWK_MODULE(CTPPSDiamondRecHitFlattener);
using CTPPSDiamondLocalTrackFlattener = DetSetVectorFlattener<CTPPSDiamondLocalTrack>;
DEFINE_FWK_MODULE(CTPPSDiamondLocalTrackFlattener);
using CTPPSPixelRecHitFlattener = DetSetVectorFlattener<CTPPSPixelRecHit>;
DEFINE_FWK_MODULE(CTPPSPixelRecHitFlattener);
using CTPPSPixelLocalTrackFlattener = DetSetVectorFlattener<CTPPSPixelLocalTrack>;
DEFINE_FWK_MODULE(CTPPSPixelLocalTrackFlattener);
