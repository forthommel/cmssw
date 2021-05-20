/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
*   Laurent Forthomme
*
****************************************************************************/

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "Geometry/ForwardGeometry/interface/TotemGeometry.h"
#include "Geometry/Records/interface/TotemGeometryRcd.h"

#include "DataFormats/Provenance/interface/EventRange.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"
#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

class TotemT2DQMSource : public DQMEDAnalyzer {
public:
  TotemT2DQMSource(const edm::ParameterSet&);

  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  edm::ESGetToken<TotemGeometry, TotemGeometryRcd> geometryRunToken_;
  const TotemGeometry* geom_;

  edm::EDGetTokenT<edm::DetSetVector<TotemVFATStatus>> statusToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemT2Digi>> digisToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemT2RecHit>> rechitsToken_;

  struct ArmPlots {
    ArmPlots() = default;
    ArmPlots(DQMStore::IBooker& iBooker, const TotemT2DetId& detid) {
      std::string path, title;
      detid.armName(path, TotemT2DetId::nPath);
      detid.armName(title, TotemT2DetId::nFull);
      iBooker.setCurrentFolder(path);
      std::cout<<detid<<"::"<<path<<"::"<<title<<std::endl;
      digiSummaryPlot = iBooker.book2D("Arm summary (digi)",
                                       title + " digis in station;Plane number;Tile number",
                                       TotemT2DetId::maxPlane + 1,
                                       -0.5,
                                       TotemT2DetId::maxPlane + 0.5,
                                       TotemT2DetId::maxChannel + 1,
                                       -0.5,
                                       TotemT2DetId::maxChannel + 0.5);
      rechitSummaryPlot = iBooker.book2D("Arm summary (rechit)",
                                         title + " rechits in station;Plane number;Tile number",
                                         TotemT2DetId::maxPlane + 1,
                                         -0.5,
                                         TotemT2DetId::maxPlane + 0.5,
                                         TotemT2DetId::maxChannel + 1,
                                         -0.5,
                                         TotemT2DetId::maxChannel + 0.5);
    }
    //std::unordered_map<unsigned int, MonitorElement*> activity_per_bx;
    MonitorElement* digiSummaryPlot = nullptr;
    MonitorElement* rechitSummaryPlot = nullptr;
  };
  std::unordered_map<unsigned short, ArmPlots> arm_plots_;
};

TotemT2DQMSource::TotemT2DQMSource(const edm::ParameterSet& iConfig)
    : geometryRunToken_(esConsumes<TotemGeometry, TotemGeometryRcd, edm::Transition::BeginRun>()),
      statusToken_(consumes<edm::DetSetVector<TotemVFATStatus>>(iConfig.getParameter<edm::InputTag>("statusLabel"))),
      digisToken_(consumes<edm::DetSetVector<TotemT2Digi>>(iConfig.getParameter<edm::InputTag>("digiLabel"))),
      rechitsToken_(consumes<edm::DetSetVector<TotemT2RecHit>>(iConfig.getParameter<edm::InputTag>("rechitsLabel"))) {}

void TotemT2DQMSource::dqmBeginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
  geom_ = &iSetup.getData(geometryRunToken_);
}

void TotemT2DQMSource::bookHistograms(DQMStore::IBooker& iBooker, const edm::Run&, const edm::EventSetup&) {
  for (unsigned short arm = 0; arm <= TotemT2DetId::maxArm; ++arm) {
    const TotemT2DetId arm_id(arm, 0);
    arm_plots_[arm_id] = ArmPlots(iBooker, arm_id);
  }
}

void TotemT2DQMSource::analyze(const edm::Event& iEvent, const edm::EventSetup&) {
  edm::Handle<edm::DetSetVector<TotemT2Digi>> hDigis;
  iEvent.getByToken(digisToken_, hDigis);

  edm::Handle<edm::DetSetVector<TotemT2RecHit>> hRechits;
  iEvent.getByToken(rechitsToken_, hRechits);

  for (const auto& digis : *hDigis) {
    const TotemT2DetId detid(digis.detId());
    auto& arm_plots = arm_plots_.at(detid.armId());
    arm_plots.digiSummaryPlot->getTH2F()->Fill(detid.plane(), detid.channel(), digis.size());
  }

  for (const auto& rechits : *hRechits) {
    const TotemT2DetId detid(rechits.detId());
    auto& arm_plots = arm_plots_.at(detid.armId());
    arm_plots.rechitSummaryPlot->getTH2F()->Fill(detid.plane(), detid.channel(), rechits.size());
  }
}

DEFINE_FWK_MODULE(TotemT2DQMSource);
