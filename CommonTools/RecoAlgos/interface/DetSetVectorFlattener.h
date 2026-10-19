#ifndef CommonTools_RecoAlgos_DetSetVectorFlattener_h
#define CommonTools_RecoAlgos_DetSetVectorFlattener_h

#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

/** \class DetSetVectorFlattener
 *
 * Framework module that produces a flattened STL vector
 * of objects from a DetSetVector input.
 *
 * \author Laurent Forthomme, AGH
 *
 */

namespace converter::helper {
  template <typename ItemType, typename DetIdType>
  concept HasSetDetId = requires(ItemType& item, const DetIdType& id) { item.setDetId(id); };

  template <typename ItemType, typename DetIdType>
    requires HasSetDetId<ItemType, DetIdType>
  class DetSetVectorFlattener : public edm::global::EDProducer<> {
  public:
    explicit DetSetVectorFlattener(const edm::ParameterSet& iConfig)
        : input_token_{consumes<edm::DetSetVector<ItemType> >(iConfig.getParameter<edm::InputTag>("input"))} {
      produces<std::vector<ItemType> >();
    }

    static void fillDescriptions(edm::ConfigurationDescriptions& descr) {
      edm::ParameterSetDescription desc;
      desc.add<edm::InputTag>("input", edm::InputTag{})->setComment("input collection to retrieve");
      descr.addWithDefaultLabel(desc);
    }

  private:
    void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const override {
      auto pOut = std::make_unique<std::vector<ItemType> >();
      for (const auto& [detid, items] : iEvent.get(input_token_))
        for (const auto& item : items)
          pOut->emplace_back(item).setDetId(DetIdType{detid});
      iEvent.put(std::move(pOut));
    }

    const edm::EDGetTokenT<edm::DetSetVector<ItemType> > input_token_;
  };
}  // namespace converter::helper

#endif
