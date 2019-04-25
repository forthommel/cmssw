/****************************************************************************
 *
 * This is a part of TOTEM/PPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "CondFormats/CTPPSReadoutObjects/interface/PPSTimingCalibration.h"
#include "CondFormats/DataRecord/interface/PPSTimingCalibrationRcd.h"

#include <memory>

class PPSTimingCalibrationWriter : public edm::one::EDAnalyzer<>
{
  public:
    explicit PPSTimingCalibrationWriter( const edm::ParameterSet& );

    static void fillDescriptions( edm::ConfigurationDescriptions& );

  private:
    void beginJob() override {}
    void analyze( const edm::Event&, const edm::EventSetup& ) override;
    void endJob() override {}

    std::string payloadTag_;
    unsigned int fromRunIOV_;
};

PPSTimingCalibrationWriter::PPSTimingCalibrationWriter( const edm::ParameterSet& iConfig ) :
  payloadTag_( iConfig.getParameter<std::string>( "payloadTag" ) ),
  fromRunIOV_( iConfig.getParameter<unsigned int>( "fromRunIOV" ) )
{}

void
PPSTimingCalibrationWriter::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  // get timing calibration parameters
  edm::ESHandle<PPSTimingCalibration> hTimingCalib;
  iSetup.get<PPSTimingCalibrationRcd>().get( hTimingCalib );

  // store the calibration into a DB object
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if ( poolDbService.isAvailable() ) {
    const edm::IOVSyncValue start( edm::EventID( fromRunIOV_, 1, 0 ) );
    const cond::time::Time_t start_time = cond::time::fromIOVSyncValue( start, cond::TimeType::runnumber );
    if ( poolDbService->isNewTagRequest( payloadTag_ ) )
      poolDbService->createNewIOV<const PPSTimingCalibration>( hTimingCalib.product(), start_time, poolDbService->endOfTime(), payloadTag_ );
    else
      poolDbService->appendSinceTime<const PPSTimingCalibration>( hTimingCalib.product(), start_time, payloadTag_ );
  }
  else
    throw cms::Exception("PPSTimingCalibrationWriter") << "PoolDBService required.";
}

void
PPSTimingCalibrationWriter::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.add<std::string>( "payloadTag", "PPSTimingCalibrationRcd" )
    ->setComment( "record tag for the payload" );
  desc.add<unsigned int>( "fromRunIOV", 0 )
    ->setComment( "start of validity period (run number)" );

  descriptions.add( "ppsTimingCalibrationWriter", desc );
}

DEFINE_FWK_MODULE( PPSTimingCalibrationWriter );

