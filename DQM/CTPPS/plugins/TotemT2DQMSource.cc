/****************************************************************************
 *
 * This is a part of TOTEM offline software.
 * Author:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"
#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

#include "DQM/CTPPS/interface/TotemT2Segmentation.h"

#include <string>

class TotemT2DQMSource : public DQMEDAnalyzer {
public:
  TotemT2DQMSource(const edm::ParameterSet&);
  ~TotemT2DQMSource() override;

protected:
  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  edm::EDGetTokenT<edm::DetSetVector<TotemT2Digi>> digiToken_;
  edm::EDGetTokenT<edmNew::DetSetVector<TotemT2RecHit>> rechitToken_;

  MonitorElement* m_digis_mult_[TotemT2DetId::maxPlane / 2];
};

TotemT2DQMSource::TotemT2DQMSource(const edm::ParameterSet& iConfig)
    : digiToken_(consumes<edm::DetSetVector<TotemT2Digi>>(iConfig.getParameter<edm::InputTag>("digisTag"))),
      rechitToken_(consumes<edmNew::DetSetVector<TotemT2RecHit>>(iConfig.getParameter<edm::InputTag>("rechitsTag"))) {}

TotemT2DQMSource::~TotemT2DQMSource() {}

void TotemT2DQMSource::dqmBeginRun(const edm::Run&, const edm::EventSetup&) {}

void TotemT2DQMSource::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {
  ibooker.cd();
  ibooker.setCurrentFolder("CTPPS/TotemT2");

  for (unsigned int pl = 0; pl < TotemT2DetId::maxPlane / 2; ++pl)
    m_digis_mult_[pl] =
        ibooker.book2D("digis multiplicity (plane " + std::to_string(pl) + ")", "x;y", 100, 0., 100., 100, 0., 100.);
  //std::string stnd;
  //TotemT2DetId(ID.stationId()).stationName(stnd, CTPPSDetId::nPath);
  //ibooker.setCurrentFolder(stnd);
}

void TotemT2DQMSource::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  for (const auto& ds_digis : iEvent.get(digiToken_)) {
    const TotemT2DetId detid(ds_digis.detId());
    for (const auto& digi : ds_digis) {
      TotemT2Segmentation(m_digis_mult_[detid.plane()]->getTH2D()).fill(detid);
    }
  }
}

DEFINE_FWK_MODULE(TotemT2DQMSource);
