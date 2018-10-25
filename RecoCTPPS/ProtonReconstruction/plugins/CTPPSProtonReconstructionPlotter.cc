/****************************************************************************
*
* Authors:
*  Jan Kašpar (jan.kaspar@gmail.com)
*
****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstructionPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSProtonReconstructionPlotter(const edm::ParameterSet&);
    ~CTPPSProtonReconstructionPlotter() {}

  private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> tokenTracks;
    edm::EDGetTokenT<std::vector<reco::ProtonTrack>> tokenRecoProtons;

    unsigned int rpId_45_N, rpId_45_F;
    unsigned int rpId_56_N, rpId_56_F;

    std::string outputFile;

    signed int maxNonEmptyEvents;

    static void ProfileToRMSGraph(TProfile *p, TGraphErrors *g)
    {
        for (int bi = 1; bi <= p->GetNbinsX(); ++bi)
        {
            double c = p->GetBinCenter(bi);

            double N = p->GetBinEntries(bi);
            double Sy = p->GetBinContent(bi) * N;
            double Syy = p->GetSumw2()->At(bi);

            double si_sq = Syy/N - Sy*Sy/N/N;
            double si = (si_sq >= 0.) ? sqrt(si_sq) : 0.;
            double si_unc_sq = si_sq / 2. / N;	// Gaussian approximation
            double si_unc = (si_unc_sq >= 0.) ? sqrt(si_unc_sq) : 0.;

            int idx = g->GetN();
            g->SetPoint(idx, c, si);
            g->SetPointError(idx, 0., si_unc);
        }
    }

    static double CalculateT(double xi, double th_x, double th_y)
    {
      const double m = 0.938; // GeV
      const double p = 6500.; // GeV

	  const double t0 = 2.*m*m + 2.*p*p*(1.-xi) - 2.*sqrt( (m*m + p*p) * (m*m + p*p*(1.-xi)*(1.-xi)) );
      const double th = sqrt(th_x * th_x + th_y * th_y);
	  const double S = sin(th/2.);
	  return t0 - 4. * p*p * (1.-xi) * S*S;
    }

    struct SingleRPPlots
    {
      TH1D *h_xi = NULL;

      TH2D *h2_th_y_vs_xi = NULL;
      TProfile *p_th_y_vs_xi = NULL;

      void Init()
      {
        h_xi = new TH1D("", ";#xi", 100, 0., 0.25);

        h2_th_y_vs_xi = new TH2D("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.25, 100, -500E-6, +500E-6);
        p_th_y_vs_xi = new TProfile("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.25);
      }

      void Fill(const reco::ProtonTrack &p)
      {
        if (!h_xi)
          Init();

        if (p.valid())
        {
          h_xi->Fill(p.xi());

          const double th_y = p.direction().y() / p.direction().mag();
          h2_th_y_vs_xi->Fill(p.xi(), th_y);
          p_th_y_vs_xi->Fill(p.xi(), th_y);
        }
      }

      void Write() const
      {
        h_xi->Write("h_xi");

        h2_th_y_vs_xi->Write("h2_th_y_vs_xi");
        p_th_y_vs_xi->Write("p_th_y_vs_xi");
      }
    };

    std::map<unsigned int, SingleRPPlots> singleRPPlots;

    struct MultiRPPlots
    {
      TH1D *h_xi=NULL, *h_th_x=NULL, *h_th_y=NULL, *h_vtx_y=NULL, *h_t=NULL, *h_chi_sq=NULL, *h_chi_sq_norm=NULL;
      TH1D *h_t_xi_range1=NULL, *h_t_xi_range2=NULL, *h_t_xi_range3=NULL;
      TH2D *h2_th_x_vs_xi = NULL, *h2_th_y_vs_xi = NULL, *h2_vtx_y_vs_xi = NULL, *h2_t_vs_xi;
      TProfile *p_th_x_vs_xi = NULL, *p_th_y_vs_xi = NULL, *p_vtx_y_vs_xi = NULL;

      void Init()
      {
        std::vector<double> v_t_bin_edges;
        for (double t = 0; t <= 5.; )
        {
          v_t_bin_edges.push_back(t);
          const double de_t = 0.05 + 0.09 * t + 0.02 * t*t;
          t += de_t;
        }

        double *t_bin_edges = new double[v_t_bin_edges.size()];
        for (unsigned int i = 0; i < v_t_bin_edges.size(); ++i)
          t_bin_edges[i] = v_t_bin_edges[i];

        h_chi_sq = new TH1D("", ";#chi^{2}", 100, 0., 0.);
        h_chi_sq_norm = new TH1D("", ";#chi^{2}/ndf", 100, 0., 5.);

        h_xi = new TH1D("", ";#xi", 100, 0., 0.25);

        h_th_x = new TH1D("", ";#theta_{x}   (rad)", 100, -500E-6, +500E-6);
        h_th_y = new TH1D("", ";#theta_{y}   (rad)", 100, -500E-6, +500E-6);

        h_vtx_y = new TH1D("", ";vtx_{y}   (mm)", 100, -2., +2.);

        h_t = new TH1D("", ";|t|   (GeV^2)", v_t_bin_edges.size() - 1, t_bin_edges);
        h_t_xi_range1 = new TH1D("", ";|t|   (GeV^2)", v_t_bin_edges.size() - 1, t_bin_edges);
        h_t_xi_range2 = new TH1D("", ";|t|   (GeV^2)", v_t_bin_edges.size() - 1, t_bin_edges);
        h_t_xi_range3 = new TH1D("", ";|t|   (GeV^2)", v_t_bin_edges.size() - 1, t_bin_edges);

        h2_th_x_vs_xi = new TH2D("", ";#xi;#theta_{x}   (rad)", 100, 0., 0.25, 100, -500E-6, +500E-6);
        h2_th_y_vs_xi = new TH2D("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.25, 100, -500E-6, +500E-6);
        h2_vtx_y_vs_xi = new TH2D("", ";#xi;vtx_{y}   (mm)", 100, 0., 0.25, 100, -500E-3, +500E-3);
        h2_t_vs_xi = new TH2D("", ";#xi;|t|   (GeV^2)", 100, 0., 0.25, v_t_bin_edges.size() - 1, t_bin_edges);

        p_th_x_vs_xi = new TProfile("", ";#xi;#theta_{x}   (rad)", 100, 0., 0.25);
        p_th_y_vs_xi = new TProfile("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.25);
        p_vtx_y_vs_xi = new TProfile("", ";#xi;vtx_{y}   (mm)", 100, 0., 0.25);

        delete[] t_bin_edges;
      }

      void Fill(const reco::ProtonTrack &p)
      {
        if (!h_xi)
          Init();

        if (p.valid())
        {
          const double th_x = p.direction().x() / p.direction().mag();
          const double th_y = p.direction().y() / p.direction().mag();
          const double mt = - CalculateT(p.xi(), th_x, th_y);

          h_chi_sq->Fill(p.fitChiSq);
          if (p.fitNDF > 0)
            h_chi_sq_norm->Fill(p.fitChiSq / p.fitNDF);

          h_xi->Fill(p.xi());

          h_th_x->Fill(th_x);
          h_th_y->Fill(th_y);

          h_vtx_y->Fill(p.vertex().y());

          h_t->Fill(mt);
          if (p.xi() > 0.04 && p.xi() < 0.07) h_t_xi_range1->Fill(mt);
          if (p.xi() > 0.07 && p.xi() < 0.10) h_t_xi_range2->Fill(mt);
          if (p.xi() > 0.10 && p.xi() < 0.13) h_t_xi_range3->Fill(mt);

          h2_th_x_vs_xi->Fill(p.xi(), th_x);
          h2_th_y_vs_xi->Fill(p.xi(), th_y);
          h2_vtx_y_vs_xi->Fill(p.xi(), p.vertex().y());
          h2_t_vs_xi->Fill(p.xi(), mt);

          p_th_x_vs_xi->Fill(p.xi(), th_x);
          p_th_y_vs_xi->Fill(p.xi(), th_y);
          p_vtx_y_vs_xi->Fill(p.xi(), p.vertex().y());
        }
      }

      void Write() const
      {
        h_chi_sq->Write("h_chi_sq");
        h_chi_sq_norm->Write("h_chi_sq_norm");

        h_xi->Write("h_xi");

        h_th_x->Write("h_th_x");
        h2_th_x_vs_xi->Write("h2_th_x_vs_xi");
        p_th_x_vs_xi->Write("p_th_x_vs_xi");
        TGraphErrors *g_th_x_RMS_vs_xi = new TGraphErrors();
        ProfileToRMSGraph(p_th_x_vs_xi, g_th_x_RMS_vs_xi);
        g_th_x_RMS_vs_xi->Write("g_th_x_RMS_vs_xi");

        h_th_y->Write("h_th_y");
        h2_th_y_vs_xi->Write("h2_th_y_vs_xi");
        p_th_y_vs_xi->Write("p_th_y_vs_xi");
        TGraphErrors *g_th_y_RMS_vs_xi = new TGraphErrors();
        ProfileToRMSGraph(p_th_y_vs_xi, g_th_y_RMS_vs_xi);
        g_th_y_RMS_vs_xi->Write("g_th_y_RMS_vs_xi");

        h_vtx_y->Write("h_vtx_y");
        h2_vtx_y_vs_xi->Write("h2_vtx_y_vs_xi");
        p_vtx_y_vs_xi->Write("p_vtx_y_vs_xi");
        TGraphErrors *g_vtx_y_RMS_vs_xi = new TGraphErrors();
        ProfileToRMSGraph(p_vtx_y_vs_xi, g_vtx_y_RMS_vs_xi);
        g_vtx_y_RMS_vs_xi->Write("g_vtx_y_RMS_vs_xi");

        h_t->Write("h_t");
        h_t_xi_range1->Write("h_t_xi_range1");
        h_t_xi_range2->Write("h_t_xi_range2");
        h_t_xi_range3->Write("h_t_xi_range3");

        h2_t_vs_xi->Write("h2_t_vs_xi");
      }
    };

    std::map<unsigned int, MultiRPPlots> multiRPPlots;

    struct SingleMultiCorrelationPlots
    {
      TH2D *h2_xi_mu_vs_xi_si = NULL;
      TH1D *h_xi_diff_mu_si = NULL;
      TH1D *h_xi_diff_si_mu = NULL;
      TProfile *p_xi_diff_si_mu_vs_xi_mu = NULL;

      TH2D *h2_th_y_mu_vs_th_y_si = NULL;

      void Init()
      {
        h2_xi_mu_vs_xi_si = new TH2D("", ";#xi_{single};#xi_{multi}", 100, 0., 0.25, 100, 0., 0.25);
        h_xi_diff_mu_si = new TH1D("", ";#xi_{multi} - #xi_{single}", 100, -0.1, +0.1);
        h_xi_diff_si_mu = new TH1D("", ";#xi_{single} - #xi_{multi}", 100, -0.1, +0.1);
        p_xi_diff_si_mu_vs_xi_mu = new TProfile("", ";#xi_{multi};#xi_{single} - #xi_{multi}", 100, 0., 0.25);

        h2_th_y_mu_vs_th_y_si = new TH2D("", ";#theta^{*}_{y,si};#theta^{*}_{y,mu}", 100, -500E-6, +500E-6, 100, -500E-6, +500E-6);
      }

      void Fill(const reco::ProtonTrack &p_single, const reco::ProtonTrack &p_multi)
      {
        if (!h2_xi_mu_vs_xi_si)
          Init();

        if (p_single.valid() && p_multi.valid())
        {
          h2_xi_mu_vs_xi_si->Fill(p_single.xi(), p_multi.xi());
          h_xi_diff_mu_si->Fill(p_multi.xi() - p_single.xi());
          h_xi_diff_si_mu->Fill(p_single.xi() - p_multi.xi());
          p_xi_diff_si_mu_vs_xi_mu->Fill(p_multi.xi(), p_single.xi() - p_multi.xi());

          const double th_y_si = p_single.direction().y() / p_single.direction().mag();
          const double th_y_mu = p_multi.direction().y() / p_multi.direction().mag();

          h2_th_y_mu_vs_th_y_si->Fill(th_y_si, th_y_mu);
        }
      }

      void Write() const
      {
        h2_xi_mu_vs_xi_si->Write("h2_xi_mu_vs_xi_si");
        h_xi_diff_mu_si->Write("h_xi_diff_mu_si");
        h_xi_diff_si_mu->Write("h_xi_diff_si_mu");
        p_xi_diff_si_mu_vs_xi_mu->Write("p_xi_diff_si_mu_vs_xi_mu");

        h2_th_y_mu_vs_th_y_si->Write("h2_th_y_mu_vs_th_y_si");
      }
    };

    std::map<unsigned int, SingleMultiCorrelationPlots> singleMultiCorrelationPlots;

    struct ArmCorrelationPlots
    {
      TH1D *h_xi_si_diffNF = NULL;
      TProfile *p_xi_si_diffNF_vs_xi_mu = NULL;

      TH1D *h_th_y_si_diffNF = NULL;
      TProfile *p_th_y_si_diffNF_vs_xi_mu = NULL;

      void Init()
      {
        h_xi_si_diffNF = new TH1D("", ";#xi_{sF} - #xi_{sN}", 100, -0.02, +0.02);
        p_xi_si_diffNF_vs_xi_mu = new TProfile("", ";#xi_{m};#xi_{sF} - #xi_{sN}", 100, 0., 0.25);

        h_th_y_si_diffNF = new TH1D("", ";#theta_{y,sF} - #theta_{y,sN}", 100, -100E-6, +100E-6);
        p_th_y_si_diffNF_vs_xi_mu = new TProfile("", ";#xi_{m};#theta_{y,sF} - #theta_{y,sN}", 100, 0., 0.25);
      }

      void Fill(const reco::ProtonTrack &p_s_N, const reco::ProtonTrack &p_s_F, const reco::ProtonTrack &p_m)
      {
        if (!h_xi_si_diffNF)
          Init();

        if (p_s_N.valid() && p_s_F.valid() && p_m.valid())
        {
          const double th_y_s_N = p_s_N.direction().y() / p_s_N.direction().mag();
          const double th_y_s_F = p_s_F.direction().y() / p_s_F.direction().mag();

          h_xi_si_diffNF->Fill(p_s_F.xi() - p_s_N.xi());
          p_xi_si_diffNF_vs_xi_mu->Fill(p_m.xi(), p_s_F.xi() - p_s_N.xi());

          h_th_y_si_diffNF->Fill(th_y_s_F - th_y_s_N);
          p_th_y_si_diffNF_vs_xi_mu->Fill(p_m.xi(), th_y_s_F - th_y_s_N);
        }
      }

      void Write() const
      {
        h_xi_si_diffNF->Write("h_xi_si_diffNF");
        p_xi_si_diffNF_vs_xi_mu->Write("p_xi_si_diffNF_vs_xi_mu");

        h_th_y_si_diffNF->Write("h_th_y_si_diffNF");
        p_th_y_si_diffNF_vs_xi_mu->Write("p_th_y_si_diffNF_vs_xi_mu");
      }
    };

    std::map<unsigned int, ArmCorrelationPlots> armCorrelationPlots;

    TProfile *p_x_L_diffNF_vs_x_L_N, *p_x_R_diffNF_vs_x_R_N;
    TProfile *p_y_L_diffNF_vs_y_L_N, *p_y_R_diffNF_vs_y_R_N;

    signed int n_non_empty_events;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionPlotter::CTPPSProtonReconstructionPlotter(const edm::ParameterSet &ps) :
  tokenTracks(consumes< std::vector<CTPPSLocalTrackLite>>(ps.getParameter<edm::InputTag>("tagTracks"))),
  tokenRecoProtons(consumes<std::vector<reco::ProtonTrack>>(ps.getParameter<InputTag>("tagRecoProtons"))),

  rpId_45_N(ps.getParameter<unsigned int>("rpId_45_N")),
  rpId_45_F(ps.getParameter<unsigned int>("rpId_45_F")),
  rpId_56_N(ps.getParameter<unsigned int>("rpId_56_N")),
  rpId_56_F(ps.getParameter<unsigned int>("rpId_56_F")),

  outputFile(ps.getParameter<string>("outputFile")),
  maxNonEmptyEvents(ps.getUntrackedParameter<signed int>("maxNonEmptyEvents", -1))
{
  p_x_L_diffNF_vs_x_L_N = new TProfile("p_x_L_diffNF_vs_x_L_N", ";x_{LN};x_{LF} - x_{LN}", 100, 0., +20.);
  p_x_R_diffNF_vs_x_R_N = new TProfile("p_x_R_diffNF_vs_x_R_N", ";x_{RN};x_{RF} - x_{RN}", 100, 0., +20.);

  p_y_L_diffNF_vs_y_L_N = new TProfile("p_y_L_diffNF_vs_y_L_N", ";y_{LN};y_{LF} - y_{LN}", 100, -20., +20.);
  p_y_R_diffNF_vs_y_R_N = new TProfile("p_y_R_diffNF_vs_y_R_N", ";y_{RN};y_{RF} - y_{RN}", 100, -20., +20.);

  n_non_empty_events = 0;
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionPlotter::analyze(const edm::Event &event, const edm::EventSetup&)
{
  // get input
  edm::Handle< std::vector<CTPPSLocalTrackLite> > tracks;
  event.getByToken(tokenTracks, tracks);

  Handle<vector<reco::ProtonTrack>> recoProtons;
  event.getByToken(tokenRecoProtons, recoProtons);

  if (recoProtons->size() > 0)
    n_non_empty_events++;

  if (maxNonEmptyEvents > 0 && n_non_empty_events > maxNonEmptyEvents)
    throw cms::Exception("CTPPSProtonReconstructionPlotter") << "Number of non empty events reached maximum.";

  // track plots
  const CTPPSLocalTrackLite *tr_L_N = NULL;
  const CTPPSLocalTrackLite *tr_L_F = NULL;
  const CTPPSLocalTrackLite *tr_R_N = NULL;
  const CTPPSLocalTrackLite *tr_R_F = NULL;

  for (const auto &tr : *tracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    if (decRPId == rpId_45_N) tr_L_N = &tr;
    if (decRPId == rpId_45_F) tr_L_F = &tr;
    if (decRPId == rpId_56_N) tr_R_N = &tr;
    if (decRPId == rpId_56_F) tr_R_F = &tr;
  }

  if (tr_L_N && tr_L_F)
  {
    p_x_L_diffNF_vs_x_L_N->Fill(tr_L_N->getX(), tr_L_F->getX() - tr_L_N->getX());
    p_y_L_diffNF_vs_y_L_N->Fill(tr_L_N->getY(), tr_L_F->getY() - tr_L_N->getY());
  }

  if (tr_R_N && tr_R_F)
  {
    p_x_R_diffNF_vs_x_R_N->Fill(tr_R_N->getX(), tr_R_F->getX() - tr_R_N->getX());
    p_y_R_diffNF_vs_y_R_N->Fill(tr_R_N->getY(), tr_R_F->getY() - tr_R_N->getY());
  }

  // make single-RP-reco plots
  for (const auto & proton : *recoProtons)
  {
    if (proton.method == reco::ProtonTrack::rmSingleRP)
    {
      CTPPSDetId rpId(* proton.contributingRPIds.begin());
      unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
      singleRPPlots[decRPId].Fill(proton);
    }
  }

  // make multi-RP-reco plots
  for (const auto & proton : *recoProtons)
  {
    if (proton.method == reco::ProtonTrack::rmMultiRP)
    {
      CTPPSDetId rpId(* proton.contributingRPIds.begin());
      unsigned int armId = rpId.arm();
      multiRPPlots[armId].Fill(proton);
    }
  }

  // make correlation plots
  for (unsigned int i = 0; i < recoProtons->size(); ++i)
  {
    for (unsigned int j = 0; j < recoProtons->size(); ++j)
    {
      const reco::ProtonTrack &pi = (*recoProtons)[i];
      const reco::ProtonTrack &pj = (*recoProtons)[j];

      if (pi.method != reco::ProtonTrack::rmSingleRP || pj.method != reco::ProtonTrack::rmMultiRP)
        continue;

      // only compare object from the same arm
      CTPPSDetId i_rpId(* pi.contributingRPIds.begin());
      CTPPSDetId j_rpId(* pj.contributingRPIds.begin());

      if (i_rpId.arm() != j_rpId.arm())
        continue;

      // build index
      const unsigned int idx = i_rpId.arm()*1000 + i_rpId.station()*100 + i_rpId.rp()*10 + j_rpId.arm();

      // fill plots
      singleMultiCorrelationPlots[idx].Fill(pi, pj);
    }
  }

  // arm correlation plots
  const reco::ProtonTrack *p_arm0_s_N = NULL, *p_arm0_s_F = NULL, *p_arm0_m = NULL;
  const reco::ProtonTrack *p_arm1_s_N = NULL, *p_arm1_s_F = NULL, *p_arm1_m = NULL;

  for (const auto & proton : *recoProtons)
  {
    CTPPSDetId rpId(* proton.contributingRPIds.begin());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    if (proton.method == reco::ProtonTrack::rmMultiRP && proton.lhcSector == reco::ProtonTrack::sector45) p_arm0_m = &proton;
    if (proton.method == reco::ProtonTrack::rmMultiRP && proton.lhcSector == reco::ProtonTrack::sector56) p_arm1_m = &proton;

    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == rpId_45_N) p_arm0_s_N = &proton;
    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == rpId_45_F) p_arm0_s_F = &proton;

    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == rpId_56_N) p_arm1_s_N = &proton;
    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == rpId_56_F) p_arm1_s_F = &proton;
  }

  if (p_arm0_s_N && p_arm0_s_F && p_arm0_m)
    armCorrelationPlots[0].Fill(*p_arm0_s_N, *p_arm0_s_F, *p_arm0_m);

  if (p_arm1_s_N && p_arm1_s_F && p_arm1_m)
    armCorrelationPlots[1].Fill(*p_arm1_s_N, *p_arm1_s_F, *p_arm1_m);
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionPlotter::endJob()
{
  printf(">> CTPPSProtonReconstructionPlotter: n_non_empty_events = %u\n", n_non_empty_events);

  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  p_x_L_diffNF_vs_x_L_N->Write();
  p_x_R_diffNF_vs_x_R_N->Write();

  p_y_L_diffNF_vs_y_L_N->Write();
  p_y_R_diffNF_vs_y_R_N->Write();

  TDirectory *d_singleRPPlots = f_out->mkdir("singleRPPlots");
  for (const auto it : singleRPPlots)
  {
    char buf[100];
    sprintf(buf, "rp%u", it.first);
    gDirectory = d_singleRPPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_multiRPPlots = f_out->mkdir("multiRPPlots");
  for (const auto it : multiRPPlots)
  {
    char buf[100];
    sprintf(buf, "arm%u", it.first);
    gDirectory = d_multiRPPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_singleMultiCorrelationPlots = f_out->mkdir("singleMultiCorrelationPlots");
  for (const auto it : singleMultiCorrelationPlots)
  {
    unsigned int si_rp = it.first / 10;
    unsigned int mu_arm = it.first % 10;

    char buf[100];
    sprintf(buf, "si_rp%u_mu_arm%u", si_rp, mu_arm);
    gDirectory = d_singleMultiCorrelationPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_armCorrelationPlots = f_out->mkdir("armCorrelationPlots");
  for (const auto it : armCorrelationPlots)
  {
    unsigned int arm = it.first;

    char buf[100];
    sprintf(buf, "arm%u", arm);
    gDirectory = d_armCorrelationPlots->mkdir(buf);
    it.second.Write();
  }

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstructionPlotter);
