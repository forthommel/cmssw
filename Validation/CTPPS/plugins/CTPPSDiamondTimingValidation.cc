// -*- C++ -*-
//
// Package:    Validation/CTPPS
// Class:      CTPPSDiamondTimingValidation
// 
/**\class CTPPSDiamondTimingValidation CTPPSDiamondTimingValidation.cc Validation/CTPPS/plugins/CTPPSDiamondTimingValidation.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Sat, 03 Jun 2017 12:17:50 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CTPPSDiamondTimingValidation : public edm::one::EDAnalyzer<edm::one::SharedResources>
{
  public:
    explicit CTPPSDiamondTimingValidation( const edm::ParameterSet& );
    ~CTPPSDiamondTimingValidation();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;

};

CTPPSDiamondTimingValidation::CTPPSDiamondTimingValidation( const edm::ParameterSet& iConfig )
{
}


CTPPSDiamondTimingValidation::~CTPPSDiamondTimingValidation()
{}

void
CTPPSDiamondTimingValidation::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
}

void 
CTPPSDiamondTimingValidation::beginJob()
{}

void 
CTPPSDiamondTimingValidation::endJob() 
{}

void
CTPPSDiamondTimingValidation::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

DEFINE_FWK_MODULE( CTPPSDiamondTimingValidation );
