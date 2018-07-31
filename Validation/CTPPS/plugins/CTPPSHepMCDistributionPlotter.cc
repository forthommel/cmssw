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

class CTPPSHepMCDistributionPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSHepMCDistributionPlotter( const edm::ParameterSet& );
    ~CTPPSHepMCDistributionPlotter();

  private:
    virtual void beginJob() override;

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    edm::EDGetTokenT<edm::HepMCProduct> tokenHepMC;

    std::string outputFile;

    TH1D *h_xi;
    TH1D *h_th_x;
    TH1D *h_th_y;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;
using namespace HepMC;

//----------------------------------------------------------------------------------------------------

CTPPSHepMCDistributionPlotter::CTPPSHepMCDistributionPlotter(const edm::ParameterSet& iConfig) :
  tokenHepMC( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagHepMC")) ),
  outputFile(iConfig.getParameter<string>("outputFile"))
{
  h_xi = new TH1D("h_xi", ";#xi", 100, 0., 0.30);
  h_th_x = new TH1D("h_th_x", ";#theta^{*}_{x}", 100, -300E-6, +300E-6);
  h_th_y = new TH1D("h_th_y", ";#theta^{*}_{y}", 100, -300E-6, +300E-6);
}

//----------------------------------------------------------------------------------------------------

CTPPSHepMCDistributionPlotter::~CTPPSHepMCDistributionPlotter()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSHepMCDistributionPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  // get input
  edm::Handle<edm::HepMCProduct> hHepMC;
  iEvent.getByToken(tokenHepMC, hHepMC);
  HepMC::GenEvent *hepMCEvent = (HepMC::GenEvent *) hHepMC->GetEvent();

  /*
  // extract vertex position
  bool vertex_set = false;
  FourVector vtx;
  for (auto it = hepMCEventAfterSmearing->vertices_begin(); it != hepMCEventAfterSmearing->vertices_end(); ++it)
  {
    if (vertex_set)
    {
      // TODO
      //printf("ERROR: multiple vertices found.\n");
      //return;
    }

    vertex_set = true;
    vtx = (*it)->position();
  }
  */

  // extract protons
  for (auto it = hepMCEvent->particles_begin(); it != hepMCEvent->particles_end(); ++it)
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

    const double p_nom = 6500.;
    const double xi_simu = (p_nom - mom.e()) / p_nom;
    const double th_x_simu = mom.x() / mom.rho();
    const double th_y_simu = mom.y() / mom.rho();

    h_xi->Fill(xi_simu);
    h_th_x->Fill(th_x_simu);
    h_th_y->Fill(th_y_simu);
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSHepMCDistributionPlotter::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSHepMCDistributionPlotter::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  h_xi->Write();
  h_th_x->Write();
  h_th_y->Write();

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSHepMCDistributionPlotter);
