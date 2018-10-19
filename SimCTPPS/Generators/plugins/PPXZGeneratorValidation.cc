/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include <map>

//----------------------------------------------------------------------------------------------------

class PPXZGeneratorValidation : public edm::one::EDAnalyzer<>
{
  public:
    explicit PPXZGeneratorValidation(const edm::ParameterSet&);
    ~PPXZGeneratorValidation();

  private:
    virtual void beginJob() override;

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    edm::EDGetTokenT<edm::HepMCProduct> hepMCToken_;
    edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> recoTracksToken_;

    std::string outputFile;

    struct Plots
    {
      TH1D *h_m_Z = NULL;
      TH1D *h_m_XZ;
      TH1D *h_p_z_LAB_2p;

      TH1D *h_p_T_X, *h_p_z_X, *h_p_tot_X, *h_theta_X, *h_eta_X;
      TH1D *h_p_T_Z, *h_p_z_Z, *h_p_tot_Z, *h_theta_Z, *h_eta_Z;

      TH1D *h_p_T_l_pl, *h_p_z_l_pl, *h_p_tot_l_pl, *h_theta_l_pl, *h_eta_l_pl;
      TH1D *h_p_T_l_mi, *h_p_z_l_mi, *h_p_tot_l_mi, *h_theta_l_mi, *h_eta_l_mi;

      void init()
      {
        h_m_Z = new TH1D("", ";m_{Z}   (GeV)", 100, 80., 100.);
        h_m_XZ = new TH1D("", ";m_{XZ}   (GeV)", 100, 1200., 1500.);
        h_p_z_LAB_2p = new TH1D("", ";p_{z}(2 protons)   (GeV)", 100, -2000., +2000.);

        h_p_T_X = new TH1D("", "p_{T}(X)   (GeV)", 100, -10., 170.);
        h_p_z_X = new TH1D("", "p_{z}(X)   (GeV)", 100, -1500., 1500.);
        h_p_tot_X = new TH1D("", "p(X)   (GeV)", 100, -300., 1450.);
        h_theta_X = new TH1D("", "theta(X)", 100, -0.4, 4.5);
        h_eta_X = new TH1D("", "eta(X)", 100, -8., 8.);

        h_p_T_Z = new TH1D("", "p_{T}(Z)   (GeV)", 100, -10., 180.);
        h_p_z_Z = new TH1D("", "p_{z}(Z)   (GeV)", 100, -300., 300.);
        h_p_tot_Z = new TH1D("", "p(Z)   (GeV)", 100, -50., 300.);
        h_theta_Z = new TH1D("", "theta(Z)", 100, -0.4, 4.5);
        h_eta_Z = new TH1D("", "eta(Z)", 100, -5., 5.);

        h_p_T_l_pl = new TH1D("", "p_{T}(l_pl)   (GeV)", 100, -10., 180.);
        h_p_z_l_pl = new TH1D("", "p_{z}(l_pl)   (GeV)", 100, -300., 300.);
        h_p_tot_l_pl = new TH1D("", "p(l_pl)   (GeV)", 100, -50., 300.);
        h_theta_l_pl = new TH1D("", "theta(l_pl)", 100, -0.4, 4.5);
        h_eta_l_pl = new TH1D("", "eta(l_pl)", 100, -5., 5.);

        h_p_T_l_min = new TH1D("", "p_{T}(l_min)   (GeV)", 100, -10., 180.);
        h_p_z_l_min = new TH1D("", "p_{z}(l_min)   (GeV)", 100, -300., 300.);
        h_p_tot_l_min = new TH1D("", "p(l_min)   (GeV)", 100, -50., 300.);
        h_theta_l_min = new TH1D("", "theta(l_min)", 100, -0.4, 4.5);
        h_eta_l_min = new TH1D("", "eta(l_min)", 100, -5., 5.);
      }

      void fill(const CLHEP::HepLorentzVector &momentum_p1, const CLHEP::HepLorentzVector &momentum_p2,
        const CLHEP::HepLorentzVector &momentum_X, const CLHEP::HepLorentzVector &momentum_Z,
        const CLHEP::HepLorentzVector &momentum_l_pl, const CLHEP::HepLorentzVector &momentum_l_mi)
      {
        if (h_m_Z == NULL)
          init();

        h_m_Z->Fill(momentum_Z.m());
        h_m_XZ->Fill((momentum_Z + momentum_X).m());
        h_p_z_LAB_2p->Fill((momentum_p1 + momentum_p2).z());

        h_p_T_X->Fill(momentum_X.perp());
        h_p_z_X->Fill(momentum_X.z());
        h_p_tot_X->Fill(momentum_X.mag());
        h_theta_X->Fill(momentum_X.theta());
        h_eta_X->Fill(momentum_X.et());

        h_p_T_Z->Fill(momentum_Z.perp());
        h_p_z_Z->Fill(momentum_Z.z());
        h_p_tot_Z->Fill(momentum_Z.mag());
        h_theta_Z->Fill(momentum_Z.theta());
        h_eta_Z->Fill(momentum_Z.et());

        h_p_T_l_pl->Fill(momentum_l_pl.perp());
        h_p_z_l_pl->Fill(momentum_l_pl.z());
        h_p_tot_l_pl->Fill(momentum_l_pl.mag());
        h_theta_l_pl->Fill(momentum_l_pl.theta());
        h_eta_l_pl->Fill(momentum_l_pl.et());

        h_p_T_l_mi->Fill(momentum_l_mi.perp());
        h_p_z_l_mi->Fill(momentum_l_mi.z());
        h_p_tot_l_mi->Fill(momentum_l_mi.mag());
        h_theta_l_mi->Fill(momentum_l_mi.theta());
        h_eta_l_mi->Fill(momentum_l_mi.et());
      }

      void write() const
      {
        h_m_Z->Write("h_m_Z");
        h_m_XZ->Write("h_m_XZ");
        h_p_z_LAB_2p->Write("h_p_z_LAB_2p");

        h_p_T_X->Write("h_p_T_X");
        h_p_z_X->Write("h_p_z_X");
        h_p_tot_X->Write("h_p_tot_X");
        h_theta_X->Write("h_theta_X");
        h_eta_X->Write("h_eta_X");

        h_p_T_Z->Write("h_p_T_Z");
        h_p_z_Z->Write("h_p_z_Z");
        h_p_tot_Z->Write("h_p_tot_Z");
        h_theta_Z->Write("h_theta_Z");
        h_eta_Z->Write("h_eta_Z");

        h_p_T_l_pl->Write("h_p_T_l_pl");
        h_p_z_l_pl->Write("h_p_z_l_pl");
        h_p_tot_l_pl->Write("h_p_tot_l_pl");
        h_theta_l_pl->Write("h_theta_l_pl");
        h_eta_l_pl->Write("h_eta_l_pl");

        h_p_T_l_mi->Write("h_p_T_l_mi");
        h_p_z_l_mi->Write("h_p_z_l_mi");
        h_p_tot_l_mi->Write("h_p_tot_l_mi");
        h_theta_l_mi->Write("h_theta_l_mi");
        h_eta_l_mi->Write("h_eta_l_mi");
      }
    };

    Plots plotsBeforeSimulation, plotsAfterSimulation;
};

//----------------------------------------------------------------------------------------------------

PPXZGeneratorValidation::PPXZGeneratorValidation( const edm::ParameterSet& iConfig ) :
  hepMCToken_( consumes< edm::HepMCProduct >( iConfig.getParameter<edm::InputTag>( "hepMCTag" ) ) ),
  recoTracksToken_( consumes< std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "recoTracksTag" ) ) ),
  outputFile( iConfig.getParameter<std::string>("outputFile") )
{
}

//----------------------------------------------------------------------------------------------------

PPXZGeneratorValidation::~PPXZGeneratorValidation()
{
}

//----------------------------------------------------------------------------------------------------

void PPXZGeneratorValidation::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  // get input
  edm::Handle<edm::HepMCProduct> hHepMCProduct;
  iEvent.getByToken(hepMCToken_, hHepMCProduct);

  edm::Handle< std::vector<CTPPSLocalTrackLite> > hRecoTracks;
  iEvent.getByToken(recoTracksToken_, hRecoTracks);

  // process HepMC record
  CLHEP::HepLorentzVector momentum_p1, momentum_p2, momentum_X, momentum_Z, momentum_l_pl, momentum_l_mi;

  // TODO

  // process tracks
  bool protonIn45 = false, protonIn56 = false;
  for (const auto& tr : *hRecoTracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.arm() == 0) protonIn45 = true;
    if (rpId.arm() == 0) protonIn56 = true;
  }

  // fill plots
  plotsBeforeSimulation.Fill(momentum_p1, momentum_p2, momentum_X, momentum_Z, momentum_l_pl, momentum_l_mi);

  if (protonIn45 && protonIn56)
    plotsAfterSimulation.Fill(momentum_p1, momentum_p2, momentum_X, momentum_Z, momentum_l_pl, momentum_l_mi);
}

//----------------------------------------------------------------------------------------------------

void PPXZGeneratorValidation::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void PPXZGeneratorValidation::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");
  
  gDirectory->mkdir("before simulation");
  plotsBeforeSimulation.Write();
  
  gDirectory->mkdir("after simulation");
  plotsAfter.Write();

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PPXZGeneratorValidation);
