// Original Author:  Jan Kašpar

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"

#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"

//----------------------------------------------------------------------------------------------------

/**
 * \brief Provides LHCInfo data necessary for CTPPS reconstruction (and direct simulation).
 **/
class CTPPSLHCInfoESSourceJSON: public edm::ESProducer, public edm::EventSetupRecordIntervalFinder
{
  public:
    CTPPSLHCInfoESSourceJSON(const edm::ParameterSet &);

    ~CTPPSLHCInfoESSourceJSON() {};

    std::unique_ptr<LHCInfo> produce(const LHCInfoRcd &);

  private:
    struct InputData
    {
      double beamEnergy;
      double xangle;
      std::vector<edm::LuminosityBlockRange> ranges;
    };

    std::vector<InputData> inputData_;

    signed int currentBlockIdx_;

    void setIntervalFor(const edm::eventsetup::EventSetupRecordKey&, const edm::IOVSyncValue&, edm::ValidityInterval&) override;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

CTPPSLHCInfoESSourceJSON::CTPPSLHCInfoESSourceJSON(const edm::ParameterSet& conf) :
  currentBlockIdx_(-1)
{
  for (const auto &ps : conf.getParameter<std::vector<edm::ParameterSet>>("configuration"))
  {
    InputData id;
    id.beamEnergy = ps.getParameter<double>("beamEnergy");
    id.xangle = ps.getParameter<double>("xangle");
    id.ranges = ps.getParameter<std::vector<edm::LuminosityBlockRange>>("ranges");
    inputData_.emplace_back(std::move(id));
  }

  setWhatProduced(this);
  findingRecord<LHCInfoRcd>();
}

//----------------------------------------------------------------------------------------------------

void CTPPSLHCInfoESSourceJSON::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &key,
  const edm::IOVSyncValue& iosv, edm::ValidityInterval& oValidity)
{
  currentBlockIdx_ = -1;

  for (unsigned int bi = 0; bi < inputData_.size(); ++bi)
  {
    const auto &block = inputData_[bi];

    for (const auto &range : block.ranges)
    {
      edm::LuminosityBlockID eventLBID(iosv.eventID().run(), iosv.eventID().luminosityBlock());

      if (edm::contains(range, eventLBID))
      {
        currentBlockIdx_ = bi;

        const edm::EventID start(range.startRun(), range.startLumi(), 1);
        const edm::EventID end(range.endRun(), range.endLumi(), edm::EventID::maxEventNumber());
        oValidity = edm::ValidityInterval(edm::IOVSyncValue(start), edm::IOVSyncValue(end));

        return;
      }
    }
  }

  if (currentBlockIdx_ < 0)
  {
    edm::LogWarning("CTPPSLHCInfoESSourceJSON") << "No configuration found for event " << iosv.eventID() <<
      ", default values will be used.";

    const edm::EventID start(iosv.eventID().run(), iosv.eventID().luminosityBlock(), 1);
    const edm::EventID end(iosv.eventID().run(), iosv.eventID().luminosityBlock(), 2);
    oValidity = edm::ValidityInterval(edm::IOVSyncValue(start), edm::IOVSyncValue(end));
  }
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<LHCInfo> CTPPSLHCInfoESSourceJSON::produce(const LHCInfoRcd &)
{
  auto output = std::make_unique<LHCInfo>();

  // defaults
  output->setEnergy(0.);
  output->setCrossingAngle(0.);

  if (currentBlockIdx_ >= 0)
  {
    auto const &data = inputData_[currentBlockIdx_];
    output->setEnergy(data.beamEnergy);
    output->setCrossingAngle(data.xangle);
  }

  return output;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_EVENTSETUP_SOURCE(CTPPSLHCInfoESSourceJSON);
