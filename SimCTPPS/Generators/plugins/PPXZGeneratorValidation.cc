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

#include "CLHEP/Vector/LorentzVector.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "SimCTPPS/Generators/plugins/particle_ids.h"

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

      TH2D *h_xi2_vs_xi1;

      TH1D *h_p_T_X, *h_p_z_X, *h_p_tot_X, *h_theta_X, *h_eta_X;

      TH1D *h_p_T_X_pr1, *h_p_z_X_pr1, *h_p_tot_X_pr1, *h_theta_X_pr1, *h_eta_X_pr1;
      TH1D *h_p_T_X_pr2, *h_p_z_X_pr2, *h_p_tot_X_pr2, *h_theta_X_pr2, *h_eta_X_pr2;

      TH1D *h_p_T_Z, *h_p_z_Z, *h_p_tot_Z, *h_theta_Z, *h_eta_Z;

      TH1D *h_p_T_l_pl, *h_p_z_l_pl, *h_p_tot_l_pl, *h_theta_l_pl, *h_eta_l_pl;
      TH1D *h_p_T_l_mi, *h_p_z_l_mi, *h_p_tot_l_mi, *h_theta_l_mi, *h_eta_l_mi;

      TH1D *h_angle_X_Z, *h_angle_l_pl_l_mi, *h_angle_X_pr1_X_pr2;

      void init()
      {
        h_m_Z = new TH1D("", ";m_{Z}   (GeV)", 100, 80., 100.);
        h_m_XZ = new TH1D("", ";m_{XZ}   (GeV)", 200, 800., 2000.);
        h_p_z_LAB_2p = new TH1D("", ";p_{z}(2 protons)   (GeV)", 100, -2000., +2000.);

        h_xi2_vs_xi1 = new TH2D("", ";#xi_{1};#xi_{2}", 50., 0., 0.20, 50., 0., 0.20);

        h_p_T_X = new TH1D("", "p_{T}(X)   (GeV)", 100, 0., 170.);
        h_p_z_X = new TH1D("", "p_{z}(X)   (GeV)", 100, -1500., 1500.);
        h_p_tot_X = new TH1D("", "p(X)   (GeV)", 100, 0., 1500.);
        h_theta_X = new TH1D("", "theta(X)", 100, -0.1, 3.3);
        h_eta_X = new TH1D("", "eta(X)", 100, -8., 8.);

        h_p_T_X_pr1 = new TH1D("", "p_{T}(X_pr1)   (GeV)", 100, 0., 1500.);
        h_p_z_X_pr1 = new TH1D("", "p_{z}(X_pr1)   (GeV)", 100, -1500., 1500.);
        h_p_tot_X_pr1 = new TH1D("", "p(X_pr1)   (GeV)", 100, 0., 1500.);
        h_theta_X_pr1 = new TH1D("", "theta(X_pr1)", 100, -0.1, 3.3);
        h_eta_X_pr1 = new TH1D("", "eta(X_pr1)", 100, -5., 5.);

        h_p_T_X_pr2 = new TH1D("", "p_{T}(X_pr2)   (GeV)", 100, 0., 1500.);
        h_p_z_X_pr2 = new TH1D("", "p_{z}(X_pr2)   (GeV)", 100, -1500., 1500.);
        h_p_tot_X_pr2 = new TH1D("", "p(X_pr2)   (GeV)", 100, 0., 1500.);
        h_theta_X_pr2 = new TH1D("", "theta(X_pr2)", 100, -0.1, 3.3);
        h_eta_X_pr2 = new TH1D("", "eta(X_pr2)", 100, -5., 5.);

        h_p_T_Z = new TH1D("", "p_{T}(Z)   (GeV)", 100, 0., 180.);
        h_p_z_Z = new TH1D("", "p_{z}(Z)   (GeV)", 100, -300., 300.);
        h_p_tot_Z = new TH1D("", "p(Z)   (GeV)", 100, 0., 300.);
        h_theta_Z = new TH1D("", "theta(Z)", 100, -0.1, 3.3);
        h_eta_Z = new TH1D("", "eta(Z)", 100, -5., 5.);

        h_p_T_l_pl = new TH1D("", "p_{T}(l_pl)   (GeV)", 100, 0., 180.);
        h_p_z_l_pl = new TH1D("", "p_{z}(l_pl)   (GeV)", 100, -300., 300.);
        h_p_tot_l_pl = new TH1D("", "p(l_pl)   (GeV)", 100, 0., 300.);
        h_theta_l_pl = new TH1D("", "theta(l_pl)", 100, -0.1, 3.3);
        h_eta_l_pl = new TH1D("", "eta(l_pl)", 100, -5., 5.);

        h_p_T_l_mi = new TH1D("", "p_{T}(l_mi)   (GeV)", 100, 0., 180.);
        h_p_z_l_mi = new TH1D("", "p_{z}(l_mi)   (GeV)", 100, -300., 300.);
        h_p_tot_l_mi = new TH1D("", "p(l_mi)   (GeV)", 100, 0., 300.);
        h_theta_l_mi = new TH1D("", "theta(l_mi)", 100, -0.1, 3.3);
        h_eta_l_mi = new TH1D("", "eta(l_mi)", 100, -5., 5.);

        h_angle_X_Z = new TH1D("", "angle(X, Z)", 100, 0., M_PI);
        h_angle_l_pl_l_mi = new TH1D("", "angle(l_pl, l_mi)", 100, 0., M_PI);
        h_angle_X_pr1_X_pr2 = new TH1D("", "angle(X_pr1, X_pr2)", 100, 0., M_PI);
      }

      void fill(const CLHEP::HepLorentzVector &momentum_p1, const CLHEP::HepLorentzVector &momentum_p2,
        const CLHEP::HepLorentzVector &momentum_X,
        const CLHEP::HepLorentzVector &momentum_X_pr1, const CLHEP::HepLorentzVector &momentum_X_pr2,
        const CLHEP::HepLorentzVector &momentum_Z,
        const CLHEP::HepLorentzVector &momentum_l_pl, const CLHEP::HepLorentzVector &momentum_l_mi)
      {
        if (h_m_Z == NULL)
          init();

        h_m_Z->Fill(momentum_Z.m());
        h_m_XZ->Fill((momentum_Z + momentum_X).m());
        h_p_z_LAB_2p->Fill((momentum_p1 + momentum_p2).z());

        const double p_beam = (momentum_p1 + momentum_p2 + momentum_Z + momentum_X).m() / 2.;
        const double xi1 = 1. - momentum_p1.t() / p_beam;
        const double xi2 = 1. - momentum_p2.t() / p_beam;
        h_xi2_vs_xi1->Fill(xi1, xi2);

        h_p_T_X->Fill(momentum_X.perp());
        h_p_z_X->Fill(momentum_X.z());
        h_p_tot_X->Fill(momentum_X.rho());
        h_theta_X->Fill(momentum_X.theta());
        h_eta_X->Fill(momentum_X.pseudoRapidity());

        h_p_T_X_pr1->Fill(momentum_X_pr1.perp());
        h_p_z_X_pr1->Fill(momentum_X_pr1.z());
        h_p_tot_X_pr1->Fill(momentum_X_pr1.rho());
        h_theta_X_pr1->Fill(momentum_X_pr1.theta());
        h_eta_X_pr1->Fill(momentum_X_pr1.pseudoRapidity());

        h_p_T_X_pr2->Fill(momentum_X_pr2.perp());
        h_p_z_X_pr2->Fill(momentum_X_pr2.z());
        h_p_tot_X_pr2->Fill(momentum_X_pr2.rho());
        h_theta_X_pr2->Fill(momentum_X_pr2.theta());
        h_eta_X_pr2->Fill(momentum_X_pr2.pseudoRapidity());

        h_p_T_Z->Fill(momentum_Z.perp());
        h_p_z_Z->Fill(momentum_Z.z());
        h_p_tot_Z->Fill(momentum_Z.rho());
        h_theta_Z->Fill(momentum_Z.theta());
        h_eta_Z->Fill(momentum_Z.pseudoRapidity());

        h_p_T_l_pl->Fill(momentum_l_pl.perp());
        h_p_z_l_pl->Fill(momentum_l_pl.z());
        h_p_tot_l_pl->Fill(momentum_l_pl.rho());
        h_theta_l_pl->Fill(momentum_l_pl.theta());
        h_eta_l_pl->Fill(momentum_l_pl.pseudoRapidity());

        h_p_T_l_mi->Fill(momentum_l_mi.perp());
        h_p_z_l_mi->Fill(momentum_l_mi.z());
        h_p_tot_l_mi->Fill(momentum_l_mi.rho());
        h_theta_l_mi->Fill(momentum_l_mi.theta());
        h_eta_l_mi->Fill(momentum_l_mi.pseudoRapidity());

        h_angle_X_Z->Fill(momentum_X.angle(momentum_Z));
        h_angle_l_pl_l_mi->Fill(momentum_l_pl.angle(momentum_l_mi));
        h_angle_X_pr1_X_pr2->Fill(momentum_X_pr1.angle(momentum_X_pr2));
      }

      void write() const
      {
        if (!h_m_Z)
        {
          printf("ERROR in Plots::write > object not initialised.\n");
          return;
        }

        h_m_Z->Write("h_m_Z");
        h_m_XZ->Write("h_m_XZ");
        h_p_z_LAB_2p->Write("h_p_z_LAB_2p");

        h_xi2_vs_xi1->Write("h_xi2_vs_xi1");

        h_p_T_X->Write("h_p_T_X");
        h_p_z_X->Write("h_p_z_X");
        h_p_tot_X->Write("h_p_tot_X");
        h_theta_X->Write("h_theta_X");
        h_eta_X->Write("h_eta_X");

        h_p_T_X_pr1->Write("h_p_T_X_pr1");
        h_p_z_X_pr1->Write("h_p_z_X_pr1");
        h_p_tot_X_pr1->Write("h_p_tot_X_pr1");
        h_theta_X_pr1->Write("h_theta_X_pr1");
        h_eta_X_pr1->Write("h_eta_X_pr1");

        h_p_T_X_pr2->Write("h_p_T_X_pr2");
        h_p_z_X_pr2->Write("h_p_z_X_pr2");
        h_p_tot_X_pr2->Write("h_p_tot_X_pr2");
        h_theta_X_pr2->Write("h_theta_X_pr2");
        h_eta_X_pr2->Write("h_eta_X_pr2");

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

        h_angle_X_Z->Write("h_angle_X_Z");
        h_angle_l_pl_l_mi->Write("h_angle_l_pl_l_mi");
        h_angle_X_pr1_X_pr2->Write("h_angle_X_pr1_X_pr2");
      }
    };

    Plots plotsBeforeSimulation, plotsAfterSimulation;

    inline CLHEP::HepLorentzVector convertTo(const HepMC::FourVector &v)
    {
      return CLHEP::HepLorentzVector(v.x(), v.y(), v.z(), v.t());
    }
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
  CLHEP::HepLorentzVector momentum_p1, momentum_p2, momentum_X, momentum_X_pr1, momentum_X_pr2, momentum_Z, momentum_l_pl, momentum_l_mi;

  // loop over event vertices
  auto evt = hHepMCProduct->GetEvent();
  for (auto it_vtx = evt->vertices_begin(); it_vtx != evt->vertices_end(); ++it_vtx)
  {
    auto vtx = *it_vtx;

    // loop over outgoing particles
    for (auto it_part = vtx->particles_out_const_begin(); it_part != vtx->particles_out_const_end(); ++it_part)
    {
      auto part = *it_part;

      if (part->pdg_id() == particleId_p)
      {
        if (part->momentum().z() > 0)
          momentum_p1 = convertTo(part->momentum());
        else
          momentum_p2 = convertTo(part->momentum());
      }

      if (part->pdg_id() == particleId_X)
        momentum_X = convertTo(part->momentum());

      if (part->pdg_id() == particleId_X_pr1)
        momentum_X_pr1 = convertTo(part->momentum());

      if (part->pdg_id() == particleId_X_pr2)
        momentum_X_pr2 = convertTo(part->momentum());

      if (part->pdg_id() == particleId_Z)
        momentum_Z = convertTo(part->momentum());

      if (part->pdg_id() == particleId_e_pl || part->pdg_id() == particleId_mu_pl)
        momentum_l_pl = convertTo(part->momentum());

      if (part->pdg_id() == particleId_e_mi || part->pdg_id() == particleId_mu_mi)
        momentum_l_mi = convertTo(part->momentum());
    }
  }

  // process tracks
  bool protonIn45 = false, protonIn56 = false;
  for (const auto& tr : *hRecoTracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.arm() == 0) protonIn45 = true;
    if (rpId.arm() == 1) protonIn56 = true;
  }

  // fill plots
  plotsBeforeSimulation.fill(momentum_p1, momentum_p2, momentum_X, momentum_X_pr1, momentum_X_pr2, momentum_Z, momentum_l_pl, momentum_l_mi);

  if (protonIn45 && protonIn56)
    plotsAfterSimulation.fill(momentum_p1, momentum_p2, momentum_X, momentum_X_pr1, momentum_X_pr2, momentum_Z, momentum_l_pl, momentum_l_mi);
}

//----------------------------------------------------------------------------------------------------

void PPXZGeneratorValidation::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void PPXZGeneratorValidation::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");
  
  gDirectory = f_out->mkdir("before simulation");
  plotsBeforeSimulation.write();
  
  gDirectory = f_out->mkdir("after simulation");
  plotsAfterSimulation.write();

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PPXZGeneratorValidation);
