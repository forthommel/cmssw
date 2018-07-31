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

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TFile.h"
#include "TH1D.h"

#include <map>

//----------------------------------------------------------------------------------------------------

class SmearingValidator : public edm::one::EDAnalyzer<>
{
  public:
    explicit SmearingValidator( const edm::ParameterSet& );
    ~SmearingValidator();

  private:
    virtual void beginJob() override;

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    edm::EDGetTokenT<edm::HepMCProduct> tokenBeforeSmearing;
    edm::EDGetTokenT<edm::HepMCProduct> tokenAfterSmearing;

    std::string outputFile;

    TH1D *h_de_vtx_x, *h_de_vtx_y, *h_de_vtx_z;

    struct SectorPlots
    {
      TH1D *h_de_th_x = NULL;
      TH1D *h_de_th_y = NULL;
      TH1D *h_de_p = NULL;

      void Init()
      {
        h_de_th_x = new TH1D("", ";#Delta#theta_{x}   (#mum)", 100, 0., 0.);
        h_de_th_y = new TH1D("", ";#Delta#theta_{y}   (#mum)", 100, 0., 0.);
        h_de_p = new TH1D("", ";#Deltap   (GeV)", 100, 0., 0.);
      }

      void Write() const
      {
        h_de_th_x->Write("h_de_th_x");
        h_de_th_y->Write("h_de_th_y");
        h_de_p->Write("h_de_p");
      }
    };

    std::map<unsigned int, SectorPlots> sectorPlots;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;
using namespace HepMC;

//----------------------------------------------------------------------------------------------------

SmearingValidator::SmearingValidator(const edm::ParameterSet& iConfig) :
  tokenBeforeSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagBeforeSmearing")) ),
  tokenAfterSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagAfterSmearing")) ),
  outputFile(iConfig.getParameter<string>("outputFile"))
{
  h_de_vtx_x = new TH1D("h_de_vtx_x", ";#Delta vtx_{x}   (mm)", 100, 0., 0.);
  h_de_vtx_y = new TH1D("h_de_vtx_y", ";#Delta vtx_{y}   (mm)", 100, 0., 0.);
  h_de_vtx_z = new TH1D("h_de_vtx_z", ";#Delta vtx_{z}   (mm)", 100, 0., 0.);

  sectorPlots[0].Init();
  sectorPlots[1].Init();
}

//----------------------------------------------------------------------------------------------------

SmearingValidator::~SmearingValidator()
{
}

//----------------------------------------------------------------------------------------------------

void SmearingValidator::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  // get input
  edm::Handle<edm::HepMCProduct> hBeforeSmearing;
  iEvent.getByToken(tokenBeforeSmearing, hBeforeSmearing);
  HepMC::GenEvent *orig = (HepMC::GenEvent *) hBeforeSmearing->GetEvent();

  edm::Handle<edm::HepMCProduct> hAfterSmearing;
  iEvent.getByToken(tokenAfterSmearing, hAfterSmearing);
  HepMC::GenEvent *smear = (HepMC::GenEvent *) hAfterSmearing->GetEvent();

  // vertices
  GenEvent::vertex_const_iterator vold, vnew;
  for (vold = orig->vertices_begin(), vnew = smear->vertices_begin(); 
      vold != orig->vertices_end() && vnew != smear->vertices_end(); ++vold, ++vnew)
  {
	const FourVector &vo = (*vold)->position();
	const FourVector &vn = (*vnew)->position();

    /*
    {
	  cout << "Vertex\n\told: [" << vo.x() << ", " << vo.y() << ", " << vo.z() << ", " << vo.t() << "]\n"
		   << "\tnew: [" << vn.x() << ", " << vn.y() << ", " << vn.z() << ", " << vn.t() << "]\n";
    }
    */
    
    // HepMC gives vertex in mm
    h_de_vtx_x->Fill(vn.x() - vo.x());
    h_de_vtx_y->Fill(vn.y() - vo.y());
    h_de_vtx_z->Fill(vn.z() - vo.z());
  }

  // particles
  GenEvent::particle_const_iterator pold, pnew;
  for (pold = orig->particles_begin(), pnew = smear->particles_begin();
      pold != orig->particles_end() && pnew != smear->particles_end(); ++pold, ++pnew)
  {
    FourVector o = (*pold)->momentum(), n = (*pnew)->momentum();
    
    // determine direction region
    signed int idx = -1;
    const double thetaLim = 0.01; // rad
    double th = o.theta();

    if (th < thetaLim)
      idx = 0;
    if (th > (M_PI - thetaLim))
      idx = 1;

    if (idx < 0)
      continue;
  
    /*
        cout << "particle\n\told: [" << o.x() << ", " << o.y() << ", " << o.z() << ", " << o.t()
        << "]\n\tnew: [" << n.x() << ", " << n.y() << ", " << n.z() << ", " << n.t()
        << "]\n\tregion: " << idx << endl;
    */

    // fill histograms
    auto &sp = sectorPlots[idx];

    double othx = o.x() / o.z(), othy = o.y() / o.z();
    double nthx = n.x() / n.z(), nthy = n.y() / n.z();

    sp.h_de_p->Fill(n.rho() - o.rho());

    sp.h_de_th_x->Fill(nthx - othx);
    sp.h_de_th_y->Fill(nthy - othy);
  }
}

//----------------------------------------------------------------------------------------------------

void SmearingValidator::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void SmearingValidator::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  h_de_vtx_x->Write();
  h_de_vtx_y->Write();
  h_de_vtx_z->Write();
  
  gDirectory = f_out->mkdir("sector 45");
  sectorPlots[0].Write();

  gDirectory = f_out->mkdir("sector 56");
  sectorPlots[1].Write();

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(SmearingValidator);
