/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

class TotemT2Validator : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TotemT2Validator(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  edm::EDGetTokenT<edm::DetSetVector<TotemT2RecHit> > rechitsToken_;
};

TotemT2Validator::TotemT2Validator(const edm::ParameterSet& iConfig)
    : rechitsToken_(consumes<edm::DetSetVector<TotemT2RecHit> >(iConfig.getParameter<edm::InputTag>("rechitsTag"))) {
}

void TotemT2Validator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  for (const auto& ds : iEvent.get(rechitsToken_)) {
    const TotemT2DetId detid(ds.detId());
    for (const auto& rechit : ds) {
    }
  }
}

void TotemT2Validator::beginJob() {
}

void TotemT2Validator::endJob() {
}

void TotemT2Validator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("rechitsTag", edm::InputTag("totemT2RecHits"));
  descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TotemT2Validator);
