#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Sources/interface/ProducerSourceFromFiles.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "EventFilter/DateRawToDigi/interface/event.h"
#include "EventFilter/DateRawToDigi/interface/AliRawReaderDate.h"
//#include "EventFilter/DateRawToDigi/interface/TDATEEventParser.h"

#include <array>

class DateSource : public edm::ProducerSourceFromFiles
{
  public:
    explicit DateSource( const edm::ParameterSet&, const edm::InputSourceDescription& );
    ~DateSource() override = default;

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
    std::array<unsigned char,1000> data_;
    size_t fileId_;
};

DateSource::DateSource( const edm::ParameterSet& params, const edm::InputSourceDescription& desc ) :
  ProducerSourceFromFiles( params, desc, false ),
  reader_( new AliRawReaderDate( fileNames().begin()->c_str() ) ),
  fileId_( 0 )
{
  //produces<ThingCollection>();
  //reader_->ReadHeader();
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

  if ( !reader_->NextEvent() && fileId_ < fileNames().size()-1 ) {
  //if ( !reader_->ReadNextData( data_ ) && fileId_ < fileNames().size()-1 ) {
    reader_.reset( new AliRawReaderDate( fileNames()[++fileId_].c_str() ) );
    //reader_->ReadHeader();
    //reader_->NextEvent();
  }
  //eventHeaderStruct header;
  if ( reader_->GetType() != 1 ) { //FIXME
    reader_->DumpData();
  }
  const unsigned int* event_id = reader_->GetEventId();
  edm::LogError("DateSource")
    << reader_->CheckData() << "::"
    << reader_->GetType() << "|"
    << reader_->GetRunNumber() << "|"
    << EVENT_ID_GET_BUNCH_CROSSING( event_id ) << "|"
    << EVENT_ID_GET_BURST_NB( event_id ) << "|"
    << EVENT_ID_GET_NB_IN_BURST( event_id ) << "|"
    << event_id[0] << "|"
    << reader_->GetTimestamp() << "|"
    << reader_->GetEquipmentSize() << "|"
    << reader_->GetEquipmentType() << "|"
    << reader_->GetEquipmentId() << "|"
    << reader_->GetEquipmentElementSize() << "|"
    << reader_->GetEquipmentHeaderSize();

  //iEvent.put( std::move( result ) );
}

DEFINE_FWK_INPUT_SOURCE( DateSource );

