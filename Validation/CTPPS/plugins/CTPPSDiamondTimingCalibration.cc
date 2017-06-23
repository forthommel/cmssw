// -*- C++ -*-
//
// Package:    Validation/CTPPS
// Class:      CTPPSDiamondTimingCalibration
// 
/**\class CTPPSDiamondTimingCalibration CTPPSDiamondTimingCalibration.cc Validation/CTPPS/plugins/CTPPSDiamondTimingCalibration.cc

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

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"

#include "DataFormats/Common/interface/DetSetVector.h"

// database connection
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSTimingCalibration.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

// plotting
#include "TH1.h"
#include "TH2.h"
#include "TFitResult.h"

class CTPPSDiamondTimingCalibration : public edm::one::EDAnalyzer<edm::one::SharedResources>
{
  public:
    explicit CTPPSDiamondTimingCalibration( const edm::ParameterSet& );
    ~CTPPSDiamondTimingCalibration();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;

    static const unsigned short num_arms = 2, num_planes = 4, num_channels = 12;

    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondRecHit> > diamRecHitsToken_;
    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondLocalTrack> > diamTracksToken_;
    int timeSlice_;

    TH1D* h_lead[num_arms][num_planes][num_channels], *h_lead_ts[num_arms][num_planes][num_channels];

    double m_mpv[num_arms][num_planes][num_channels];
    double m_sigma[num_arms][num_planes][num_channels];
    TH1D* h_mpv_arm[num_arms], *h_sigma_arm[num_arms];
    TH1D* h_mpv_plane[num_arms][num_planes], *h_sigma_plane[num_arms][num_planes];

};

CTPPSDiamondTimingCalibration::CTPPSDiamondTimingCalibration( const edm::ParameterSet& iConfig ) :
  diamRecHitsToken_( consumes< edm::DetSetVector<CTPPSDiamondRecHit> >( iConfig.getParameter<edm::InputTag>( "diamondRecHitsTag" ) ) ),
  diamTracksToken_( consumes< edm::DetSetVector<CTPPSDiamondLocalTrack> >( iConfig.getParameter<edm::InputTag>( "diamondLocalTracksTag" ) ) ),
  timeSlice_( iConfig.getParameter<int>( "OOTindex" ) )
{
  edm::Service<TFileService> fs;
  for ( unsigned short arm_i=0; arm_i<num_arms; arm_i++ ) {
    TFileDirectory arm_dir = fs->mkdir( Form( "arm %d", arm_i ) );
    h_mpv_arm[arm_i] = arm_dir.make<TH1D>( Form( "lead_mpv_arm%u", arm_i ), Form( "Arm %u;%u #times plane + channel number;MPV from Landau fit (ns)", arm_i, num_channels ), num_planes*num_channels, 0., num_planes*num_channels );
    h_sigma_arm[arm_i] = arm_dir.make<TH1D>( Form( "lead_sigma_arm%u", arm_i ), Form( "Arm %u;%u #times plane + channel number;Sigma from Landau fit (ns)", arm_i , num_channels ), num_planes*num_channels, 0., num_planes*num_channels );
    for ( unsigned short pl_i=0; pl_i<num_planes; pl_i++ ) {
      TFileDirectory pl_dir = arm_dir.mkdir( Form( "plane %d", pl_i ) );
      h_mpv_plane[arm_i][pl_i] = pl_dir.make<TH1D>( Form( "lead_mpv_arm%u_plane%u", arm_i, pl_i ), Form( "Arm %d, plane %d;Channel number;MPV from Landau fit (ns)", arm_i, pl_i ), num_channels, 0., num_channels );
      h_sigma_plane[arm_i][pl_i] = pl_dir.make<TH1D>( Form( "lead_sigma_arm%u_plane%u", arm_i, pl_i ), Form( "Arm %d, plane %d;Channel number;Sigma from Landau fit (ns)", arm_i, pl_i ), num_channels, 0., num_channels );
      for ( unsigned short ch_i=0; ch_i<num_channels; ch_i++ ) {
        h_lead[arm_i][pl_i][ch_i] = pl_dir.make<TH1D>( Form( "lead_arm%u_plane%u_ch%u", arm_i, pl_i, ch_i ), ";Leading edge (ns);Entries", 4000, -100., 300. );
        h_lead_ts[arm_i][pl_i][ch_i] = pl_dir.make<TH1D>( Form( "lead_arm%u_plane%u_ch%u_ts%d", arm_i, pl_i, ch_i, timeSlice_ ), ";Leading edge (ns);Entries", 2500, 0., 25. );
      }
    }
  }
}


CTPPSDiamondTimingCalibration::~CTPPSDiamondTimingCalibration()
{}

void
CTPPSDiamondTimingCalibration::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Handle< edm::DetSetVector<CTPPSDiamondRecHit> > rechits;
  iEvent.getByToken( diamRecHitsToken_, rechits );

  std::vector<CTPPSDiamondRecHit> rechits_45, rechits_56;

  for ( const auto& rechits_ds : *rechits ) {
    const CTPPSDiamondDetId detid( rechits_ds.detId() );
    const unsigned short arm = detid.arm(), plane = detid.plane(), channel = detid.channel();
    for ( const auto& rechit : rechits_ds ) {
      h_lead[arm][plane][channel]->Fill( rechit.getT()+25.*rechit.getOOTIndex() );
      if ( rechit.getOOTIndex()==timeSlice_ ) {
        h_lead_ts[arm][plane][channel]->Fill( rechit.getT() );
      }
    }
  }
}

void 
CTPPSDiamondTimingCalibration::beginJob()
{}

void 
CTPPSDiamondTimingCalibration::endJob() 
{
  std::shared_ptr<CTPPSTimingCalibration> pOut( new CTPPSTimingCalibration );

  edm::Service<cond::service::PoolDBOutputService> poolService;
  if ( !poolService.isAvailable() ) {
    throw cms::Exception("CTPPSDiamondTimingCalibration") << "PoolDBService required.";
  }

  for ( unsigned short arm_i=0; arm_i<num_arms; arm_i++ ) {
    for ( unsigned short pl_i=0; pl_i<num_planes; pl_i++ ) {
      for ( unsigned short ch_i=0; ch_i<num_channels; ch_i++ ) {
        if ( h_lead_ts[arm_i][pl_i][ch_i]->GetEntries()==0 ) continue; // skip empty spectra
        TFitResultPtr res = h_lead_ts[arm_i][pl_i][ch_i]->Fit( "landau", "S" );
        if ( !res->IsValid() ) continue;
        m_mpv[arm_i][pl_i][ch_i] = res->Parameter( 1 );
        m_sigma[arm_i][pl_i][ch_i] = res->Parameter( 2 );

        h_mpv_plane[arm_i][pl_i]->SetBinContent( ch_i, m_mpv[arm_i][pl_i][ch_i] );
        h_sigma_plane[arm_i][pl_i]->SetBinContent( ch_i, m_sigma[arm_i][pl_i][ch_i] );
        h_mpv_arm[arm_i]->SetBinContent( num_channels*pl_i+ch_i, m_mpv[arm_i][pl_i][ch_i] );
        h_sigma_arm[arm_i]->SetBinContent( num_channels*pl_i+ch_i, m_sigma[arm_i][pl_i][ch_i] );
      }
    }
  }

  poolService->writeOne( pOut.get(), poolService->currentTime(), "CTPPSTimingCalibrationRcd" );
}

void
CTPPSDiamondTimingCalibration::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

DEFINE_FWK_MODULE( CTPPSDiamondTimingCalibration );
