// -*- C++ -*-
//
// Class:      RetrieveCTPPSBeamParameters
//
// Description: Test analyzer for reading CTPPS beam parameters condition data
//
//              Simple analyzer that retrieves CTTPSBeamParameters record from a sql 
//              database file, as a test of offline conditions implementation.
//
// Implementation:
//     [Notes on implementation]
//
//
// Original Author:  Wagner De Paula Carvalho
//         Created:  Wed, 21 Nov 2018 17:35:07 GMT
//
//==================================================================================

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
// #include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSBeamParameters.h"
#include "CondFormats/DataRecord/interface/CTPPSBeamParametersRcd.h"

#include <stdint.h>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


// class RetrieveCTPPSBeamParameters : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
class RetrieveCTPPSBeamParameters : public edm::one::EDAnalyzer<>  {
   public:
      explicit RetrieveCTPPSBeamParameters(const edm::ParameterSet&);
      ~RetrieveCTPPSBeamParameters();

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::string _label ;
};

//---------------------------------------------------------------------------------------

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

RetrieveCTPPSBeamParameters::RetrieveCTPPSBeamParameters(const edm::ParameterSet& iConfig) {}

RetrieveCTPPSBeamParameters::~RetrieveCTPPSBeamParameters() {}

// ------------ method called for each event  ------------
void RetrieveCTPPSBeamParameters::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   edm::ESHandle<CTPPSBeamParameters> pSetup;
   iSetup.get<CTPPSBeamParametersRcd>().get(_label,pSetup);

   const CTPPSBeamParameters* pInfo = pSetup.product();
   
   edm::LogInfo("CTPPSBeamParameters") << "\n" << *pInfo << "\n" ;
}


// ------------ method called once each job just before starting event loop  ------------
void
RetrieveCTPPSBeamParameters::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
RetrieveCTPPSBeamParameters::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(RetrieveCTPPSBeamParameters);
