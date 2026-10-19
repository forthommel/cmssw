/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHitLite.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

class CTPPSPixelRecHitFlattener : public edm::global::EDProducer<> {
public:
  explicit CTPPSPixelRecHitFlattener(const edm::ParameterSet& iConfig)
      : input_token_{consumes<edm::DetSetVector<CTPPSPixelRecHit> >(iConfig.getParameter<edm::InputTag>("input"))} {
    produces<std::vector<CTPPSPixelRecHitLite> >();
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descr) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("input", edm::InputTag{})->setComment("input collection to retrieve");
    descr.addWithDefaultLabel(desc);
  }

private:
  void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const override {
    auto pOut = std::make_unique<std::vector<CTPPSPixelRecHitLite> >();
    for (const auto& [detid, items] : iEvent.get(input_token_))
      for (const auto& item : items)
        pOut->emplace_back(item);
    iEvent.put(std::move(pOut));
  }

  const edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelRecHit> > input_token_;
};

DEFINE_FWK_MODULE(CTPPSPixelRecHitFlattener);
