#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Sources/interface/ProducerSourceFromFiles.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/DateDigi/interface/DateEvent.h"

#include <fstream>
#include <array>

class DateSource : public edm::ProducerSourceFromFiles
{
  public:
    explicit DateSource( const edm::ParameterSet&, const edm::InputSourceDescription& );
    ~DateSource() override;

    bool setRunAndEventInfo( edm::EventID&, edm::TimeValue_t&, edm::EventAuxiliary::ExperimentType& ) override;
    void produce( edm::Event& ) override;

  private:
    //void endJob() override {}
    //void readRun_( edm::RunPrincipal& runPrincipal ) override;
    //void readLuminosityBlock_( edm::LuminosityBlockPrincipal& lumiPrincipal ) override;
    //void readEvent_( edm::EventPrincipal& eventPrincipal ) override;
    //std::shared_ptr<edm::RunAuxiliary> readRunAuxiliary_() override;
    //std::shared_ptr<edm::LuminosityBlockAuxiliary> readLuminosityBlockAuxiliary_() override;

    std::ifstream file_;
    size_t fileId_;

    date::Event::Header header_;
};

DateSource::DateSource( const edm::ParameterSet& params, const edm::InputSourceDescription& desc ) :
  ProducerSourceFromFiles( params, desc, false ),
  file_( fileNames()[0], std::ios::in | std::ios::binary ),
  fileId_( 0 )
{
  //produces<ThingCollection>();
  std::cout << "---> " << fileNames()[0] << std::endl;
  // first read the header
  file_.read( reinterpret_cast<char*>( &header_ ), sizeof( header_ ) );
  //edm::LogError("DateSource") << header_;
  std::cout << sizeof( header_ ) << "--> " << header_.eventSize << std::endl;
  std::cout << header_;
}

DateSource::~DateSource()
{
  file_.close();
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

  /*
  edm::LogError("DateSource")
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
*/
  //iEvent.put( std::move( result ) );
}

DEFINE_FWK_INPUT_SOURCE( DateSource );

