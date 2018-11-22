#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Sources/interface/ProducerSourceFromFiles.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "EventFilter/DateRawToDigi/interface/AliRawReaderDate.h"

class DateSource : public edm::ProducerSourceFromFiles
{
  public:
    explicit DateSource( const edm::ParameterSet&, const edm::InputSourceDescription& );
    ~DateSource() override {}

    bool setRunAndEventInfo( edm::EventID&, edm::TimeValue_t&, edm::EventAuxiliary::ExperimentType& ) override;
    void produce( edm::Event& ) override;

  private:
    //void endJob() override {}
    //void readRun_( edm::RunPrincipal& runPrincipal ) override;
    //void readLuminosityBlock_( edm::LuminosityBlockPrincipal& lumiPrincipal ) override;
    //void readEvent_( edm::EventPrincipal& eventPrincipal ) override;
    //std::shared_ptr<edm::RunAuxiliary> readRunAuxiliary_() override;
    //std::shared_ptr<edm::LuminosityBlockAuxiliary> readLuminosityBlockAuxiliary_() override;

    std::unique_ptr<AliRawReaderDate> reader_;
    size_t fileId_;
};

DateSource::DateSource( const edm::ParameterSet& params, const edm::InputSourceDescription& desc ) :
  ProducerSourceFromFiles( params, desc, false ),
  reader_( new AliRawReaderDate( fileNames().begin()->c_str() ) ),
  fileId_( 0 )
{
  //produces<ThingCollection>();
}

bool
DateSource::setRunAndEventInfo( edm::EventID& id, edm::TimeValue_t& timeval, edm::EventAuxiliary::ExperimentType& exper )
{
  return true;
}

void
DateSource::produce( edm::Event& iEvent )
{
  //auto result = std::make_unique<ThingCollection>();

  reader_->NextEvent();
  edm::LogError("DateSource")
    << reader_->GetRunNumber() << "|"
    << *reader_->GetEventId() << "|"
    << reader_->GetTimestamp() << "|"
    << reader_->GetEquipmentSize() << "|"
    << reader_->GetEquipmentType() << "|"
    << reader_->GetEquipmentId() << "|"
    << reader_->GetEquipmentElementSize() << "|"
    << reader_->GetEquipmentHeaderSize();

  //iEvent.put( std::move( result ) );
}

DEFINE_FWK_INPUT_SOURCE( DateSource );

