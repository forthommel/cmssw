/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

class TotemT2DigiProducer : public edm::stream::EDProducer<> {
public:
  explicit TotemT2DigiProducer(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  edm::EDGetTokenT<edm::HepMCProduct> evtinToken_;
};

TotemT2DigiProducer::TotemT2DigiProducer(const edm::ParameterSet& iConfig) :
  evtinToken_(consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("input"))) {
  produces<edm::DetSetVector<TotemT2Digi>>();
}

void TotemT2DigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<edm::HepMCProduct> evt_in;
  iEvent.getByToken(evtinToken_, evt_in);
  const auto& evt = evt_in->GetEvent();

}

void TotemT2DigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TotemT2DigiProducer);
