/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
*   Laurent Forthomme
*
****************************************************************************/

#include "DQMServices/Core/interface/DQMOneEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "DataFormats/Provenance/interface/EventRange.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/CTPPSDigi/interface/TotemVFATStatus.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"
#include "DataFormats/TotemReco/interface/TotemT2RecHit.h"

namespace dds {
  struct Cache {
    std::unordered_map<CTPPSDetId, std::unique_ptr<TH2D>> hitDistribution2dMap;
  };
}

class TotemT2DQMSource : public DQMOneEDAnalyzer<edm::LuminosityBlockCache<dds::Cache>> {
public:
  TotemT2DQMSource(const edm::ParameterSet&);

  void dqmBeginRun(const edm::Run&, const edm::EventSetup&) override;
  void bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  std::shared_ptr<dds::Cache> globalBeginLuminosityBlock(const edm::LuminosityBlock&,
                                                         const edm::EventSetup&) const override;
  void globalEndLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) override;

private:
  edm::EDGetTokenT<edm::DetSetVector<TotemVFATStatus>> statusToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemT2Digi>> digisToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemT2RecHit>> rechitsToken_;

  edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geometryRunToken_;
};

TotemT2DQMSource::TotemT2DQMSource(const edm::ParameterSet& iConfig)
    : geometryRunToken_(esConsumes<CTPPSGeometry, VeryForwardRealGeometryRecord, edm::Transition::BeginRun>()) {
}

void TotemT2DQMSource::dqmBeginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
}

void TotemT2DQMSource::bookHistograms(DQMStore::IBooker&, const edm::Run&, const edm::EventSetup&) {}

void TotemT2DQMSource::analyze(const edm::Event&, const edm::EventSetup&) {}

std::shared_ptr<dds::Cache> TotemT2DQMSource::globalBeginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) const {
  auto cache = std::make_shared<dds::Cache>();
  return cache;
}

void TotemT2DQMSource::globalEndLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}
