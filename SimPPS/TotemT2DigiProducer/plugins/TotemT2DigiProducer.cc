/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/ProducesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "SimGeneral/MixingModule/interface/DigiAccumulatorMixMod.h"
#include "SimGeneral/MixingModule/interface/DigiAccumulatorMixModFactory.h"
#include "SimGeneral/MixingModule/interface/PileUpEventPrincipal.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

class TotemT2DigiProducer : public DigiAccumulatorMixMod {
public:
  explicit TotemT2DigiProducer(const edm::ParameterSet&, edm::ProducesCollector, edm::ConsumesCollector&);

  //static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void initializeEvent(const edm::Event&, const edm::EventSetup&) override;
  void accumulate(const edm::Event&, const edm::EventSetup&) override;
  void accumulate(const PileUpEventPrincipal&, const edm::EventSetup&, const edm::StreamID&) override;
  void finalizeEvent(edm::Event&, const edm::EventSetup&) override;

  void accumulateHits(const std::vector<PCaloHit>&, int);

  edm::InputTag prodTag_;
};

TotemT2DigiProducer::TotemT2DigiProducer(const edm::ParameterSet& iConfig, edm::ProducesCollector pColl, edm::ConsumesCollector& iCons) :
  prodTag_{iConfig.getParameter<edm::InputTag>("hitsCollection")} {
  iCons.consumes<std::vector<PCaloHit> >(prodTag_);
  pColl.produces<edm::DetSetVector<TotemT2Digi>>();
}

void TotemT2DigiProducer::initializeEvent(const edm::Event&, const edm::EventSetup&) {
}

void TotemT2DigiProducer::accumulate(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<std::vector<PCaloHit> > hHits;
  iEvent.getByLabel(prodTag_, hHits);
  accumulateHits(*hHits, 0);
}

void TotemT2DigiProducer::accumulate(const PileUpEventPrincipal& iPU, const edm::EventSetup& iSetup, const edm::StreamID& iStream) {
  edm::Handle<std::vector<PCaloHit> > hHits;
  iPU.getByLabel(prodTag_, hHits);
  accumulateHits(*hHits, iPU.bunchCrossing());
}

void TotemT2DigiProducer::finalizeEvent(edm::Event& iEvent, const edm::EventSetup&) {
  // output digis
  auto digis = std::make_unique<edm::DetSetVector<TotemT2Digi>>();

  //...

  iEvent.put(std::move(digis));
}

void TotemT2DigiProducer::accumulateHits(const std::vector<PCaloHit>& input, int bx) {

}

/*void TotemT2DigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("hitsCollection", edm::InputTag("mix", "g4SimHitTotemT2Hits"));
  descriptions.addWithDefaultLabel(desc);
  descriptions.setComment("This producer produces a set of Totem T2 DIGIs from an input SimHit collection.");
}*/

//define this as a plug-in
DEFINE_DIGI_ACCUMULATOR(TotemT2DigiProducer);
