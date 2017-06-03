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

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

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

    edm::EDGetTokenT< edm::View<reco::Vertex> > verticesToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > diamRecHitsToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamTracksToken_;
};

CTPPSDiamondTimingValidation::CTPPSDiamondTimingValidation( const edm::ParameterSet& iConfig ) :
  verticesToken_( consumes< edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "verticesTag" ) ) ),
  diamRecHitsToken_( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >( iConfig.getParameter<edm::InputTag>( "diamondRecHitsTag" ) ) ),
  diamTracksToken_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( iConfig.getParameter<edm::InputTag>( "diamondLocalTracksTag" ) ) )
{
}


CTPPSDiamondTimingValidation::~CTPPSDiamondTimingValidation()
{}

void
CTPPSDiamondTimingValidation::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Handle< edm::View<reco::Vertex> > vertices;
  iEvent.getByToken( verticesToken_, vertices );

  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > rechits;
  iEvent.getByToken( diamRecHitsToken_, rechits );

  std::vector<CTPPSDiamondRecHit> rechits_45, rechits_56;

  for ( const auto& rechits_ds : *rechits ) {
    const CTPPSDiamondDetId detid( rechits_ds.detId() );
    const unsigned short arm = detid.arm();
    for ( const auto& rechit : rechits_ds ) {
      if ( arm==0 ) rechits_45.push_back( rechit );
      if ( arm==1 ) rechits_56.push_back( rechit );
    }
  }

  std::vector<double> reco_z;
  for ( const auto& rh_45 : rechits_45 ) {
    const double t_45 = rh_45.getT();
    for ( const auto& rh_56 : rechits_56 ) {
      const double t_56 = rh_56.getT();
      reco_z.push_back( ( t_45-t_56 ) / CLHEP::ns * CLHEP::c_light * 0.5 );
    }
  }

  for ( const auto& vtx : *vertices ) {
    const double vtx_z = vtx.z();
    for ( const auto& z_pp : reco_z ) {
      std::cout << vtx_z << " >> " << z_pp << std::endl;
    }
  }
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
