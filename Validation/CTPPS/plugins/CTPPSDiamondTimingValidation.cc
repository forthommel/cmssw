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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include "TH1.h"
#include "TH2.h"

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

    static const float ns_to_s_, m_to_cm_;

    edm::EDGetTokenT<reco::BeamSpot > beamSpotToken_;
    edm::EDGetTokenT< edm::View<reco::Vertex> > verticesToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > diamRecHitsToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamTracksToken_;

    TH1D* h_vtxz_diff_ootm1_, *h_vtxz_diff_oot0_, *h_vtxz_diff_ootp1_;
    TH2D* h2_vtxz_ootm1_, *h2_vtxz_oot0_, *h2_vtxz_ootp1_;
};

const float CTPPSDiamondTimingValidation::ns_to_s_ = CLHEP::nanosecond / CLHEP::second;
const float CTPPSDiamondTimingValidation::m_to_cm_ = CLHEP::m / CLHEP::cm;

CTPPSDiamondTimingValidation::CTPPSDiamondTimingValidation( const edm::ParameterSet& iConfig ) :
  beamSpotToken_( consumes<reco::BeamSpot>( iConfig.getParameter<edm::InputTag>( "beamSpotTag" ) ) ),
  verticesToken_( consumes< edm::View<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "verticesTag" ) ) ),
  diamRecHitsToken_( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >( iConfig.getParameter<edm::InputTag>( "diamondRecHitsTag" ) ) ),
  diamTracksToken_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( iConfig.getParameter<edm::InputTag>( "diamondLocalTracksTag" ) ) )
{
  edm::Service<TFileService> fs;
  h_vtxz_diff_ootm1_ = fs->make<TH1D>( "vtxz_diff_ootm1", "OOT-1;z_{pp} - vertex z (cm);Events", 400, -2., 2. );
  h_vtxz_diff_oot0_ = fs->make<TH1D>( "vtxz_diff_oot0", "no OOT;z_{pp} - vertex z (cm);Events", 400, -2., 2. );
  h_vtxz_diff_ootp1_ = fs->make<TH1D>( "vtxz_diff_ootp1", "OOT+1;z_{pp} - vertex z (cm);Events", 400, -2., 2. );
  h2_vtxz_ootm1_ = fs->make<TH2D>( "vtxz_corr_ootm1", "OOT-1;Vertex z (cm);z_{pp} (cm)", 400, -2., 2., 400, -2., 2. );
  h2_vtxz_oot0_ = fs->make<TH2D>( "vtxz_corr_oot0", "no OOT;Vertex z (cm);z_{pp} (cm)", 400, -2., 2., 400, -2., 2. );
  h2_vtxz_ootp1_ = fs->make<TH2D>( "vtxz_corr_ootp1", "OOT+1;Vertex z (cm);z_{pp} (cm)", 400, -2., 2., 400, -2., 2. );
}


CTPPSDiamondTimingValidation::~CTPPSDiamondTimingValidation()
{}

void
CTPPSDiamondTimingValidation::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByToken( beamSpotToken_, beamspot );

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

  std::vector<double> reco_z_ootm1, reco_z_oot0, reco_z_ootp1;
  for ( const auto& rh_45 : rechits_45 ) {
    const double t_45 = rh_45.getT();
    for ( const auto& rh_56 : rechits_56 ) {
      if ( rh_56.getOOTIndex()!=rh_45.getOOTIndex() ) continue; //FIXME too tight?
      const double t_56 = rh_56.getT();
      const double t_diff = ( t_45-t_56 ) * ns_to_s_ * CLHEP::c_light * 0.5 * m_to_cm_;
      if ( rh_56.getOOTIndex()==-1 ) reco_z_ootm1.push_back( t_diff );
      if ( rh_56.getOOTIndex()==0 ) reco_z_oot0.push_back( t_diff );
      if ( rh_56.getOOTIndex()==+1 ) reco_z_ootp1.push_back( t_diff );
    }
  }

  for ( const auto& vtx : *vertices ) {
    //const double vtx_z = vtx.z() - beamspot->z0();
    const double vtx_z = vtx.z();
    for ( const auto& z_pp : reco_z_ootm1 ) {
      //std::cout << vtx_z << " >> " << z_pp << std::endl;
      h_vtxz_diff_ootm1_->Fill( z_pp-vtx_z );
      h2_vtxz_ootm1_->Fill( vtx_z, z_pp );
    }
    for ( const auto& z_pp : reco_z_oot0 ) {
      h_vtxz_diff_oot0_->Fill( z_pp-vtx_z );
      h2_vtxz_oot0_->Fill( vtx_z, z_pp );
    }
    for ( const auto& z_pp : reco_z_ootp1 ) {
      h_vtxz_diff_ootp1_->Fill( z_pp-vtx_z );
      h2_vtxz_ootp1_->Fill( vtx_z, z_pp );
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
