// -*- C++ -*-
//
// Package:    Validation/CTPPS
// Class:      CTPPSReconstruction
// 
/**\class CTPPSReconstruction CTPPSReconstruction.cc Validation/CTPPS/test/CTPPSReconstruction.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Fri, 7 Jul 2017 14:22:04 GMT
//
//
//
//
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/Proton.h"

#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"

#include "TH1D.h"
#include "TH2D.h"

#include <map>

class CTPPSReconstruction : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
    explicit CTPPSReconstruction( const edm::ParameterSet& );
    ~CTPPSReconstruction();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;

    edm::EDGetTokenT< edm::View<reco::GenParticle> > genPartToken_;
    edm::EDGetTokenT< edm::View<CTPPSLocalTrackLite> > tracksToken_;
    edm::EDGetTokenT< edm::View<reco::Proton> > protonsToken_;

    //edm::ParameterSet beamConditions_;
    double sqrtS_;
    std::vector<edm::ParameterSet> detectorPackages_;

    std::map<unsigned int,TH2D*> m_rp_h2_y_vs_x_;
    std::map<unsigned int,TH2D*> m_rp_h2_xireco_vs_xigen_;
};

CTPPSReconstruction::CTPPSReconstruction( const edm::ParameterSet& iConfig ) :
  genPartToken_( consumes< edm::View<reco::GenParticle> >( iConfig.getParameter<edm::InputTag>( "genPartTag" ) ) ),
  tracksToken_ ( consumes< edm::View<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "potsTracksTag" ) ) ),
  protonsToken_( consumes< edm::View<reco::Proton> >( iConfig.getParameter<edm::InputTag>( "protonsTag" ) ) ),
  sqrtS_           ( iConfig.getParameter<edm::ParameterSet>( "beamConditions" ).getParameter<double>( "sqrtS" ) ),
  detectorPackages_( iConfig.getParameter< std::vector<edm::ParameterSet> >( "detectorPackages" ) )
{
  usesResource( "TFileService" );

  // prepare output
  edm::Service<TFileService> fs;

  // prepare plots - hit distributions
  TFileDirectory hm_dir = fs->mkdir( "hitmaps" );
  for ( const auto& det : detectorPackages_ ) {
    const TotemRPDetId pot_id( det.getParameter<unsigned int>( "potId" ) );
    m_rp_h2_y_vs_x_.insert( std::make_pair( pot_id, hm_dir.make<TH2D>( Form( "h2_rp_hits_arm%d_rp%d", pot_id.arm(), pot_id.rp() ) , ";x;y", 300, 0., 30E-3, 200, -10E-3, +10E-3 ) ) );
    m_rp_h2_xireco_vs_xigen_.insert( std::make_pair( pot_id, hm_dir.make<TH2D>( Form( "h2_rp_xibal_arm%d_rp%d", pot_id.arm(), pot_id.rp() ), ";#xi_{gen};#xi_{reco}", 100, 0., 0.5, 100, 0., 0.5 ) ) );
  }
}

CTPPSReconstruction::~CTPPSReconstruction()
{}

void
CTPPSReconstruction::analyze( const edm::Event& iEvent, const edm::EventSetup& )
{
  edm::Handle< edm::View<reco::GenParticle> > genParticles;
  iEvent.getByToken( genPartToken_, genParticles );

  std::vector<reco::GenParticle> sim_protons_45, sim_protons_56;

  for ( const auto& part : *genParticles ) {
    if ( part.status()==1 && part.pdgId()==2212 ) {
      if ( part.pz()>0. ) sim_protons_45.push_back( part );
      if ( part.pz()<0. ) sim_protons_56.push_back( part );
    }
  }
  std::cout << "number of simulated outgoing protons in the event:" << std::endl;
  std::cout << "  sector 45: " << sim_protons_45.size() << std::endl;
  std::cout << "  sector 56: " << sim_protons_56.size() << std::endl;

  edm::Handle< edm::View<reco::Proton> > reco_protons;
  iEvent.getByToken( protonsToken_, reco_protons );

  for ( const auto& gen_pro : sim_protons_45 ) {
    const double gen_xi = 1.-gen_pro.energy()/( sqrtS_*0.5 );
    for ( const auto& proton : *reco_protons ) {
      const CTPPSDetId detid( proton.pot() );
      if ( detid.arm()!=0 ) continue;
      unsigned short pot_id = 100*detid.arm()+detid.rp();
      //std::cout << "proton track reconstructed in pot: " << pot_id << std::endl;
      m_rp_h2_xireco_vs_xigen_[proton.pot()]->Fill( gen_xi, proton.xi() );
    }
  }

  edm::Handle< edm::View<CTPPSLocalTrackLite> > tracks;
  iEvent.getByToken( tracksToken_, tracks );

  for ( const auto& trk : *tracks ) {
    const TotemRPDetId det_id( trk.getRPId() );
    m_rp_h2_y_vs_x_[det_id.rawId()]->Fill( trk.getX(), trk.getY() );
  }

}

void
CTPPSReconstruction::beginJob()
{}

void
CTPPSReconstruction::endJob()
{}

void
CTPPSReconstruction::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

//define this as a plug-in
DEFINE_FWK_MODULE( CTPPSReconstruction );

