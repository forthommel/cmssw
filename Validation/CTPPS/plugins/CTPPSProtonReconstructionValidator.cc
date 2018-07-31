/****************************************************************************
 *
 * This is a part of CTPPS validation software
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TGraphErrors.h"

#include <map>
#include <string>

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstructionValidator : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSProtonReconstructionValidator( const edm::ParameterSet& );
    ~CTPPSProtonReconstructionValidator();

  private:
    virtual void beginJob() override;

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    void FillPlots(unsigned int meth_idx, unsigned int idx, const reco::ProtonTrack &rec_pr,
      const HepMC::FourVector &vtx, const HepMC::FourVector &mom);

    edm::EDGetTokenT<edm::HepMCProduct> tokenHepMCBeforeSmearing;
    edm::EDGetTokenT<edm::HepMCProduct> tokenHepMCAfterSmearing;
    edm::EDGetTokenT<std::vector<reco::ProtonTrack>> tokenRecoProtons;

    std::string outputFile;

    static double CalculateT(double xi, double th_x, double th_y)
    {
      const double m = 0.938; // GeV
      const double p = 6500.; // GeV

	  const double t0 = 2.*m*m + 2.*p*p*(1.-xi) - 2.*sqrt( (m*m + p*p) * (m*m + p*p*(1.-xi)*(1.-xi)) );
      const double th = sqrt(th_x * th_x + th_y * th_y);
	  const double S = sin(th/2.);
	  return t0 - 4. * p*p * (1.-xi) * S*S;
    }

    struct PlotGroup
    {
      TH1D *h_de_xi = NULL;
      TProfile *p_de_xi_vs_xi_simu;
      TH2D *h_xi_reco_vs_xi_simu;

      TH1D *h_de_th_x = NULL;
      TProfile *p_de_th_x_vs_xi_simu;

      TH1D *h_de_th_y = NULL;
      TProfile *p_de_th_y_vs_xi_simu;

      TH1D *h_de_vtx_y = NULL;
      TProfile *p_de_vtx_y_vs_xi_simu;

      TH1D *h_de_t = NULL;
      TProfile *p_de_t_vs_xi_simu;
      TProfile *p_de_t_vs_t_simu;

      void Init()
      {
        h_de_xi = new TH1D("", ";#xi_{reco} - #xi_{simu}", 100, 0., 0.);
        p_de_xi_vs_xi_simu = new TProfile("", ";#xi_{simu};#xi_{reco} - #xi_{simu}", 19, 0.015, 0.205);
        h_xi_reco_vs_xi_simu = new TH2D("", ";#xi_{simu};#xi_{reco}", 100, 0., 0.30, 100, 0., 0.30);

        h_de_th_x = new TH1D("", ";#theta_{x,reco} - #theta_{x,simu}", 100, 0., 0.);
        p_de_th_x_vs_xi_simu = new TProfile("", ";#xi_{simu};#theta_{x,reco} - #theta_{x,simu}", 19, 0.015, 0.205);

        h_de_th_y = new TH1D("", ";#theta_{y,reco} - #theta_{y,simu}", 100, 0., 0.);
        p_de_th_y_vs_xi_simu = new TProfile("", ";#xi_{simu};#theta_{y,reco} - #theta_{y,simu}", 19, 0.015, 0.205);

        h_de_vtx_y = new TH1D("", ";vtx_{y,reco} - vtx_{y,simu}   (mm)", 100, 0., 0.);
        p_de_vtx_y_vs_xi_simu = new TProfile("", ";#xi_{simu};vtx_{y,reco} - vtx_{y,simu} (mm)", 19, 0.015, 0.205);

        h_de_t = new TH1D("", ";t_{reco} - t_{simu}", 100, -1., +1.);
        p_de_t_vs_xi_simu = new TProfile("", ";xi_{simu};t_{reco} - t_{simu}", 19, 0.015, 0.205);
        p_de_t_vs_t_simu = new TProfile("", ";t_{simu};t_{reco} - t_{simu}", 20, 0., 5.);
      }

      static TGraphErrors* ProfileToRMSGraph(TProfile *p, const std::string &name = "")
      {
          TGraphErrors *g = new TGraphErrors();
          g->SetName(name.c_str());

          for (int bi = 1; bi <= p->GetNbinsX(); ++bi)
          {
              double c = p->GetBinCenter(bi);
              double w = p->GetBinWidth(bi);

              double N = p->GetBinEntries(bi);
              double Sy = p->GetBinContent(bi) * N;
              double Syy = p->GetSumw2()->At(bi);

              double si_sq = Syy/N - Sy*Sy/N/N;
              double si = (si_sq >= 0.) ? sqrt(si_sq) : 0.;
              double si_unc_sq = si_sq / 2. / N;	// Gaussian approximation
              double si_unc = (si_unc_sq >= 0.) ? sqrt(si_unc_sq) : 0.;

              int idx = g->GetN();
              g->SetPoint(idx, c, si);
              g->SetPointError(idx, w/2., si_unc);
          }

          return g;
      }

      void Write() const
      {
        h_xi_reco_vs_xi_simu->Write("h_xi_reco_vs_xi_simu");
        h_de_xi->Write("h_de_xi");
        p_de_xi_vs_xi_simu->Write("p_de_xi_vs_xi_simu");
        ProfileToRMSGraph(p_de_xi_vs_xi_simu, "g_rms_de_xi_vs_xi_simu")->Write();

        h_de_th_x->Write("h_de_th_x");
        p_de_th_x_vs_xi_simu->Write("p_de_th_x_vs_xi_simu");
        ProfileToRMSGraph(p_de_th_x_vs_xi_simu, "g_rms_de_th_x_vs_xi_simu")->Write();

        h_de_th_y->Write("h_de_th_y");
        p_de_th_y_vs_xi_simu->Write("p_de_th_y_vs_xi_simu");
        ProfileToRMSGraph(p_de_th_y_vs_xi_simu, "g_rms_de_th_y_vs_xi_simu")->Write();

        h_de_vtx_y->Write("h_de_vtx_y");
        p_de_vtx_y_vs_xi_simu->Write("p_de_vtx_y_vs_xi_simu");
        ProfileToRMSGraph(p_de_vtx_y_vs_xi_simu, "g_rms_de_vtx_y_vs_xi_simu")->Write();

        h_de_t->Write("h_de_t");
        p_de_t_vs_xi_simu->Write("p_de_t_vs_xi_simu");
        ProfileToRMSGraph(p_de_t_vs_xi_simu, "g_rms_de_t_vs_xi_simu")->Write();
        p_de_t_vs_t_simu->Write("p_de_t_vs_t_simu");
        ProfileToRMSGraph(p_de_t_vs_t_simu, "g_rms_de_t_vs_t_simu")->Write();
      }
    };

    std::map<unsigned int, std::map<unsigned int, PlotGroup>> plots;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;
using namespace HepMC;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionValidator::CTPPSProtonReconstructionValidator(const edm::ParameterSet& iConfig) :
  tokenHepMCBeforeSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagHepMCBeforeSmearing")) ),
  tokenHepMCAfterSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagHepMCAfterSmearing")) ),
  tokenRecoProtons( consumes<std::vector<reco::ProtonTrack>>(iConfig.getParameter<edm::InputTag>("tagRecoProtons")) ),
  outputFile(iConfig.getParameter<string>("outputFile"))
{
}

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionValidator::~CTPPSProtonReconstructionValidator()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  //printf("------------------------ run=%u, event=%llu -----------------------\n", iEvent.id().run(), iEvent.id().event());

  // get input
  edm::Handle<edm::HepMCProduct> hHepMCBeforeSmearing;
  iEvent.getByToken(tokenHepMCBeforeSmearing, hHepMCBeforeSmearing);
  HepMC::GenEvent *hepMCEventBeforeSmearing = (HepMC::GenEvent *) hHepMCBeforeSmearing->GetEvent();

  edm::Handle<edm::HepMCProduct> hHepMCAfterSmearing;
  iEvent.getByToken(tokenHepMCAfterSmearing, hHepMCAfterSmearing);
  HepMC::GenEvent *hepMCEventAfterSmearing = (HepMC::GenEvent *) hHepMCAfterSmearing->GetEvent();

  edm::Handle<vector<reco::ProtonTrack>> hRecoProtons;
  iEvent.getByToken(tokenRecoProtons, hRecoProtons);

  // extract vertex position
  bool vertex_set = false;
  FourVector vtx;
  for (auto it = hepMCEventAfterSmearing->vertices_begin(); it != hepMCEventAfterSmearing->vertices_end(); ++it)
  {
    if (vertex_set)
    {
      printf("ERROR: multiple vertices found.\n");
      return;
    }

    vertex_set = true;
    vtx = (*it)->position();
  }

  // extract protons
  bool proton_45_set = false;
  bool proton_56_set = false;
  FourVector mom_45, mom_56;

  for (auto it = hepMCEventBeforeSmearing->particles_begin(); it != hepMCEventBeforeSmearing->particles_end(); ++it)
  {
    const auto &part = *it;

    // accept only stable non-beam protons
    if (part->pdg_id() != 2212)
      continue;

    if (part->status() != 1)
      continue;

    if (part->is_beam())
      continue;

    const auto &mom = part->momentum();

    if (mom.e() < 4500.)
      continue;

    // TODO
    //printf("    status = %u, mom = %E, %E, %E\n", part->status(), mom.x(), mom.y(), mom.z());

    if (mom.z() > 0)
    {
      // 45
      if (proton_45_set)
      {
        printf("ERROR: multiple protons in sector 45 found.\n");
        return;
      }

      proton_45_set = true;
      mom_45 = mom;
    } else {
      // 56
      if (proton_56_set)
      {
        printf("ERROR: multiple protons in sector 56 found.\n");
        return;
      }

      proton_56_set = true;
      mom_56 = mom;
    }
  }

  // TODO
  //printf("proton_45_set=%u, proton_56_set=%u\n", proton_45_set, proton_56_set);

  // do comparison
  for (const auto &rec_pr : *hRecoProtons)
  {
    if (! rec_pr.valid())
      continue;

    unsigned int idx;

    bool mom_set = false;
    FourVector mom;

    if (rec_pr.lhcSector == reco::ProtonTrack::sector45)
    {
      idx = 0;
      mom_set = proton_45_set;
      mom = mom_45;
    } else {
      idx = 1;
      mom_set = proton_56_set;
      mom = mom_56;
    }

    if (! mom_set)
      continue;

    unsigned int meth_idx;

    if (rec_pr.method == reco::ProtonTrack::rmSingleRP)
    {
      meth_idx = 0;

      CTPPSDetId rpId(* rec_pr.contributingRPIds.begin());
      idx = 100*rpId.arm() + 10*rpId.station() + rpId.rp();
    } else {
      meth_idx = 1;
    }

    FillPlots(meth_idx, idx, rec_pr, vtx, mom);
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::FillPlots(unsigned int meth_idx, unsigned int idx, const reco::ProtonTrack &rec_pr,
      const HepMC::FourVector &vtx, const HepMC::FourVector &mom)
{
  const double p_nom = 6500.;
  const double xi_simu = (p_nom - mom.e()) / p_nom;
  const double th_x_simu = mom.x() / mom.rho();
  const double th_y_simu = mom.y() / mom.rho();
  const double vtx_y_simu = vtx.y();  // mm
  const double t_simu = - CalculateT(xi_simu, th_x_simu, th_y_simu);

  const double xi_reco = rec_pr.xi();
  const double th_x_reco = rec_pr.direction().x() / rec_pr.direction().mag();
  const double th_y_reco = rec_pr.direction().y() / rec_pr.direction().mag();
  const double vtx_y_reco = rec_pr.vertex().y();  // mm
  const double t_reco = - CalculateT(xi_reco, th_x_reco, th_y_reco);

  /*
  printf("- SIMU: xi=%.4f, th_x=%.3E, th_y=%.3E, vtx_y=%.3E; RECO: xi=%.4f, th_x=%.3E, th_y=%.3E, vtx_y=%.3E\n",
    xi_simu, th_x_simu, th_y_simu, vtx_y_simu,
    xi_reco, th_x_reco, th_y_reco, vtx_y_reco
  );

  if (meth_idx == 0 && (xi_reco - xi_simu) < -0.10)
  {
    printf("- SIMU: xi=%.4f, th_x=%.3E, th_y=%.3E, vtx_y=%.3E; RECO: xi=%.4f, th_x=%.3E, th_y=%.3E, vtx_y=%.3E\n",
      xi_simu, th_x_simu, th_y_simu, vtx_y_simu,
      xi_reco, th_x_reco, th_y_reco, vtx_y_reco);
  }

  if (meth_idx == 1 && fabs(xi_reco - xi_simu) > 0.01)
    printf("   PROBLEM: xi diff too large\n");
  */

  auto &p = plots[meth_idx][idx];
  if (p.h_de_xi == NULL)
    p.Init();

  p.h_xi_reco_vs_xi_simu->Fill(xi_simu, xi_reco);
  p.h_de_xi->Fill(xi_reco - xi_simu);
  p.p_de_xi_vs_xi_simu->Fill(xi_simu, xi_reco - xi_simu);

  p.h_de_th_x->Fill(th_x_reco - th_x_simu);
  p.p_de_th_x_vs_xi_simu->Fill(xi_simu, th_x_reco - th_x_simu);

  p.h_de_th_y->Fill(th_y_reco - th_y_simu);
  p.p_de_th_y_vs_xi_simu->Fill(xi_simu, th_y_reco - th_y_simu);

  p.h_de_vtx_y->Fill(vtx_y_reco - vtx_y_simu);
  p.p_de_vtx_y_vs_xi_simu->Fill(xi_simu, vtx_y_reco - vtx_y_simu);

  p.h_de_t->Fill(t_reco - t_simu);
  p.p_de_t_vs_xi_simu->Fill(xi_simu, t_reco - t_simu);
  p.p_de_t_vs_t_simu->Fill(t_simu, t_reco - t_simu);
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  for (const auto &mit : plots)
  {
    string method = (mit.first == 0) ? "single rp" : "multi rp";
    TDirectory *d_method = f_out->mkdir(method.c_str());

    for (const auto &eit : mit.second)
    {
      char buf[20];
      sprintf(buf, "%i", eit.first);
    
      TDirectory *d_element = d_method->mkdir(buf);
      
      gDirectory = d_element;
      eit.second.Write();
    }
  }

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstructionValidator);
