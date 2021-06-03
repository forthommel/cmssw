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
      digiOccupancyPlot = iBooker.book2D("Arm occupancy (digi)",
                                         title + " digis in station;Plane number;Tile number",
                                         TotemT2DetId::maxPlane + 1,
                                         -0.5,
                                         TotemT2DetId::maxPlane + 0.5,
                                         TotemT2DetId::maxChannel + 1,
                                         -0.5,
                                         TotemT2DetId::maxChannel + 0.5);
      rechitOccupancyPlot = iBooker.book2D("Arm occupancy (rechit)",
                                           title + " rechits in station;Plane number;Tile number",
                                           TotemT2DetId::maxPlane + 1,
                                           -0.5,
                                           TotemT2DetId::maxPlane + 0.5,
                                           TotemT2DetId::maxChannel + 1,
                                           -0.5,
                                           TotemT2DetId::maxChannel + 0.5);
    }
    //std::unordered_map<unsigned int, MonitorElement*> activity_per_bx;
    MonitorElement* digiOccupancyPlot = nullptr;
    MonitorElement* rechitOccupancyPlot = nullptr;
  };
  std::unordered_map<unsigned short, ArmPlots> arm_plots_;
  struct PlanePlots {
    PlanePlots() = default;
    PlanePlots(DQMStore::IBooker& iBooker, const TotemT2DetId& detid) {
      std::string path, title;
      detid.planeName(path, TotemT2DetId::nPath);
      detid.planeName(title, TotemT2DetId::nFull);
      iBooker.setCurrentFolder(path);
      rechitsPositionPlot = iBooker.book2D(
          "Plane occupancy (rechits)", title + " rechits in plane;x (mm);y (mm)", 20, -100., 100., 20, -100., 100.);
    }
    MonitorElement* rechitsPositionPlot = nullptr;
  };
  std::unordered_map<unsigned short, PlanePlots> planes_plots_;
  struct TilePlots {
    TilePlots() = default;
    TilePlots(DQMStore::IBooker& iBooker, const TotemT2DetId& detid) {
      std::string path, title;
      detid.channelName(path, TotemT2DetId::nPath);
      detid.channelName(title, TotemT2DetId::nFull);
      iBooker.setCurrentFolder(path);
      rechitsMeanT0Plot = iBooker.book1D(
          "Mean time of arrival (rechits)", title + " rechits in tile;Time of arrival (s);Entries", 20, -10., 10.);
      rechitsTotVsT0Plot = iBooker.book2D("ToT vs. time of arrival (rechits)",
                                          title + " rechits in tile;Time of arrival (s);Time over threshold (s)",
                                          100,
                                          -10.,
                                          10.,
                                          100,
                                          0.,
                                          20.);
    }
    MonitorElement* rechitsMeanT0Plot = nullptr;
    MonitorElement* rechitsTotVsT0Plot = nullptr;
  };
  std::unordered_map<unsigned short, TilePlots> tiles_plots_;
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
    for (unsigned short pl = 0; pl <= TotemT2DetId::maxPlane; ++pl) {
      const TotemT2DetId pl_id(arm, pl);
      planes_plots_[pl_id] = PlanePlots(iBooker, pl_id);
      for (unsigned short tile = 0; tile <= TotemT2DetId::maxChannel; ++tile) {
        const TotemT2DetId tile_id(arm, pl, tile);
        tiles_plots_[tile_id] = TilePlots(iBooker, tile_id);
      }
    }
  }
}

void TotemT2DQMSource::analyze(const edm::Event& iEvent, const edm::EventSetup&) {
  for (const auto& digis : iEvent.get(digisToken_)) {
    const TotemT2DetId detid(digis.detId());
    auto& arm_plots = arm_plots_.at(detid.armId());
    arm_plots.digiOccupancyPlot->getTH2F()->Fill(detid.plane(), detid.channel(), digis.size());
  }

  for (const auto& rechits : iEvent.get(rechitsToken_)) {
    const TotemT2DetId detid(rechits.detId());
    auto& arm_plots = arm_plots_.at(detid.armId());
    arm_plots.rechitOccupancyPlot->getTH2F()->Fill(detid.plane(), detid.channel(), rechits.size());
    for (const auto& rechit : rechits) {
      auto& pl_plots = planes_plots_.at(detid.planeId());
      pl_plots.rechitsPositionPlot->getTH2F()->Fill(rechit.centre().x(), rechit.centre().y());
      std::cout << detid << ":" << rechit.centre().x() << "," << rechit.centre().y() << std::endl;
      auto& tile_plots = tiles_plots_.at(detid);
      tile_plots.rechitsMeanT0Plot->getTH1F()->Fill(rechit.time());
      tile_plots.rechitsTotVsT0Plot->getTH2F()->Fill(rechit.time(), rechit.toT());
    }
  }
}

DEFINE_FWK_MODULE(TotemT2DQMSource);
