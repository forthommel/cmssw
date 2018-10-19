/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Nicola Turini, Francesco Turini, Lorenzo Marini, Jan Kašpar
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//#include "HepPDT/ParticleDataTable.hh"
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandExponential.h"
#include "CLHEP/Random/RandBreitWigner.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "FWCore/Framework/interface/one/EDProducer.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TH1D.h"
#include "TFile.h"

#include "SimCTPPS/Generators/plugins/particle_ids.h"

//----------------------------------------------------------------------------------------------------
  
class PPXZGenerator : public edm::one::EDProducer<>
{

  public:
    PPXZGenerator(const edm::ParameterSet &);

    virtual ~PPXZGenerator();

  private:
    virtual void produce(edm::Event & e, const edm::EventSetup& es) override;

    // input parameters
    unsigned int verbosity;
    unsigned int debug;

    bool decayZToElectrons;
    bool decayZToMuons;

    const double m_X;       // mass of the X particle, GeV
    const double m_Z_mean;  // mass of the Z particle, mean, GeV
    const double m_Z_gamma; // mass of the Z particle, gamma, GeV

    const double m_e;       // mass of the X electron, GeV
    const double m_mu;      // mass of the X electron, GeV

    const double p_beam;    // beam momentum, GeV

    const double m_XZ_min;  // minimal value of invariant mass of the X-Z system, GeV
    const double c_XZ;      // parameter of the exponential distribution for the invariant mass of the X-Z system, GeV

    const double p_z_LAB_2p_mean; // mean p_z of the 2-proton system in the LAB frame, GeV
    const double p_z_LAB_2p_sigma; // RMS p_z of the 2-proton system in the LAB frame, GeV

    // debug histograms
    TH1D *h_m_Z;
    TH1D *h_m_XZ;
    TH1D *h_p_z_LAB_2p;

    TH1D *h_p_T_X;
    TH1D *h_p_z_X;
    TH1D *h_p_tot_X;
    TH1D *h_theta_X;
    TH1D *h_eta_X;

    TH1D *h_p_T_Z;
    TH1D *h_p_z_Z;
    TH1D *h_p_tot_Z;
    TH1D *h_theta_Z;
    TH1D *h_eta_Z;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

using namespace edm;
using namespace std;

//----------------------------------------------------------------------------------------------------

PPXZGenerator::PPXZGenerator(const edm::ParameterSet& pset) :
  verbosity(pset.getUntrackedParameter<unsigned int>("verbosity", 0)),
  debug(pset.getUntrackedParameter<unsigned int>("debug", 0)),

  decayZToElectrons(pset.getParameter<bool>("decayZToElectrons")),
  decayZToMuons(pset.getParameter<bool>("decayZToMuons")),

  m_X(pset.getParameter<double>("m_X")),
  m_Z_mean(pset.getParameter<double>("m_Z_mean")),
  m_Z_gamma(pset.getParameter<double>("m_Z_gamma")),
  m_e(pset.getParameter<double>("m_e")),
  m_mu(pset.getParameter<double>("m_mu")),

  p_beam(pset.getParameter<double>("p_beam")),
  m_XZ_min(pset.getParameter<double>("m_XZ_min")),
  c_XZ(pset.getParameter<double>("c_XZ")),
  p_z_LAB_2p_mean(pset.getParameter<double>("p_z_LAB_2p_mean")),
  p_z_LAB_2p_sigma(pset.getParameter<double>("p_z_LAB_2p_sigma"))
{
  if (debug)
  {
    h_m_Z = new TH1D("h_m_Z", ";m_{Z}   (GeV)", 100, 80., 100.);
    h_m_XZ = new TH1D("h_m_XZ", ";m_{XZ}   (GeV)", 100, 1200., 1500.);
    h_p_z_LAB_2p = new TH1D("h_p_z_LAB_2p", ";p_{z}(2 protons)   (GeV)", 100, -2000., +2000.);

    h_p_T_X = new TH1D("h_p_T_X", "p_T_X distribution; [MeV/c]; # counts", 100, -10., 170.);
    h_p_z_X = new TH1D("h_p_z_X", "p_z_X distribution; [MeV/c]; # counts", 100, -1500., 1500.);
    h_p_tot_X = new TH1D("h_p_tot_X", "p_tot_X distribution; [MeV/c]; # counts", 100, -300., 1450.);
    h_theta_X = new TH1D("h_theta_X", "theta_X distribution; [rad]; # counts", 100, -0.4, 4.5);
    h_eta_X = new TH1D("h_eta_X", "eta_X distribution; ; # counts", 100, -8., 8.);

    h_p_T_Z = new TH1D("h_p_T_Z", "p_T_Z distribution; [MeV/c]; # counts", 100, -10., 180.);
    h_p_z_Z = new TH1D("h_p_z_Z", "p_z_Z distribution; [MeV/c]; # counts", 100, -300., 300.);
    h_p_tot_Z = new TH1D("h_p_tot_Z", "p_tot_Z distribution; [MeV/c]; # counts", 100, -50., 300.);
    h_theta_Z = new TH1D("h_theta_Z", "theta_Z distribution; [rad]; # counts", 100, -0.4, 4.5);
    h_eta_Z = new TH1D("h_eta_Z", "eta_Z distribution; ; # counts", 100, -5., 5.);
  }

  produces<HepMCProduct>("unsmeared");
}

//----------------------------------------------------------------------------------------------------

void PPXZGenerator::produce(edm::Event &e, const edm::EventSetup& es)
{
  if (verbosity)
    printf("\n>> PPXZGenerator::produce > event %llu\n", e.id().event());

  // get conditions
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* engine = &rng->getEngine(e.streamID());

  //ESHandle<HepPDT::ParticleDataTable> pdgTable;
  //es.getData(pdgTable);

  // prepare HepMC event
  HepMC::GenEvent *fEvt = new HepMC::GenEvent();
  fEvt->set_event_number(e.id().event());

  // generate vertex position
  HepMC::GenVertex *vtx = new HepMC::GenVertex(HepMC::FourVector(0., 0., 0., 0.));
  fEvt->add_vertex(vtx);

  // TODO
  //const HepPDT::ParticleData *pData = pdgTable->particle(HepPDT::ParticleID(particleId));
  //double mass_1 = pData->mass().value();
  //double mass_2 = pData->mass().value();

  // generate mass of Z
  const double m_Z = CLHEP::RandBreitWigner::shoot(engine, m_Z_mean, m_Z_gamma);

  // generate invariant mass of the X-Z system
  const double c_XZ_mean = 1. / c_XZ;
  const double m_XZ = m_XZ_min + CLHEP::RandExponential::shoot(engine, c_XZ_mean);

  // generate p_z of the 2-proton system in the LAB frame
  const double p_z_LAB_2p = CLHEP::RandGauss::shoot(engine, p_z_LAB_2p_mean, p_z_LAB_2p_sigma);

  // generate spherical angles in the CMS frame of the X-Z system
  const double theta_c = CLHEP::RandFlat::shoot(engine) * M_PI;
  const double phi_c = CLHEP::RandFlat::shoot(engine) * 2. * M_PI;

  // determine xi's of the protons
  // proton 1: positive z momentum component
  const double xi2 = (p_z_LAB_2p + sqrt(p_z_LAB_2p*p_z_LAB_2p + m_XZ*m_XZ)) / (2. * p_beam);
  const double xi1 = m_XZ * m_XZ / (4. * p_beam * p_beam * xi2);

  if (verbosity)
  {
    printf("  m_Z = %.1f\n", m_Z);
    printf("  m_XZ = %.1f\n", m_XZ);
    printf("  p_z_LAB_2p = %.1f\n", p_z_LAB_2p);
    printf("  xi1 = %.3f, xi2 = %.3f\n", xi1, xi2);
    printf("  p_beam * (xi2 - xi1) = %.1f\n", p_beam * (xi2 - xi1));
  }

  // determine momenta of the X and Z particles in the CMS frame of the X-Z system
  const double p_c = sqrt( pow(m_XZ*m_XZ - m_X*m_X - m_Z*m_Z, 2.) / (4. * m_XZ * m_XZ) - m_X*m_X * m_Z*m_Z / (m_XZ*m_XZ) );

  CLHEP::HepLorentzVector momentum_X_CMS(
    + p_c * sin(theta_c) * cos(phi_c),
    + p_c * sin(theta_c) * sin(phi_c),
    + p_c * cos(theta_c),
    sqrt(p_c*p_c + m_X*m_X)
  );

  CLHEP::HepLorentzVector momentum_Z_CMS(
    - p_c * sin(theta_c) * cos(phi_c),
    - p_c * sin(theta_c) * sin(phi_c),
    - p_c * cos(theta_c),
    sqrt(p_c*p_c + m_Z*m_Z)
  );

  // determine boost from X-Z CMS frame to the LAB frame
  const double beta = (xi1 - xi2) / (xi1 + xi2);
  const CLHEP::Hep3Vector betaVector(0., 0., beta);

  if (verbosity)
    printf("  beta = %.3f\n", beta);

  // determine four-momenta of the outgoing particles in the LAB frame
  const CLHEP::HepLorentzVector momentum_Z = CLHEP::boostOf(momentum_Z_CMS, betaVector);
  const CLHEP::HepLorentzVector momentum_X = CLHEP::boostOf(momentum_X_CMS, betaVector);

  const CLHEP::HepLorentzVector momentum_p1(0., 0., +p_beam * (1. - xi1), p_beam * (1. - xi1));
  const CLHEP::HepLorentzVector momentum_p2(0., 0., -p_beam * (1. - xi2), p_beam * (1. - xi2));

  if (verbosity)
  {
    printf("  p_X_z = %.1f\n", momentum_X.z());
    printf("  p_Z_z = %.1f\n", momentum_Z.z());

    const CLHEP::HepLorentzVector m_tot = momentum_p1 + momentum_p2 + momentum_X + momentum_Z;
    printf("  four-momentum of p + p + X + Z: (%.1f, %.1f, %.1f | %.1f)\n", m_tot.x(), m_tot.y(), m_tot.z(), m_tot.t());
  }

  // fill in the HepMC record
  unsigned int barcode = 0;

  // status codes
  //const int statusInitial = 3;
  const int statusFinal = 1;
  const int statusDecayed = 2;

  int status_Z = (decayZToElectrons || decayZToMuons) ? statusDecayed : statusFinal;

  HepMC::GenParticle* particle_Z = new HepMC::GenParticle(momentum_Z, particleId_Z, status_Z);
  particle_Z->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_Z);

  HepMC::GenParticle* particle_X = new HepMC::GenParticle(momentum_X, particleId_X, statusFinal);
  particle_X->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_X);

  HepMC::GenParticle* particle_p1 = new HepMC::GenParticle(momentum_p1, particleId_p, statusFinal);
  particle_p1->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_p1);

  HepMC::GenParticle* particle_p2 = new HepMC::GenParticle(momentum_p2, particleId_p, statusFinal);
  particle_p2->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_p2);

  // decay Z if desired
  if (decayZToElectrons || decayZToMuons)
  {
    double m_l = 0.;
    signed int particleId_l_mi=0, particleId_l_pl=0;

    if (decayZToElectrons) m_l = m_e, particleId_l_mi = particleId_e_mi, particleId_l_pl = particleId_e_pl;
    if (decayZToMuons) m_l = m_mu, particleId_l_mi = particleId_mu_mi, particleId_l_pl = particleId_mu_pl;

    // generate decay angles in Z's rest frame;
    const double theta_d = CLHEP::RandFlat::shoot(engine) * M_PI;
    const double phi_d = CLHEP::RandFlat::shoot(engine) * 2. * M_PI;

    // lepton momentum and energy in Z's rest frame
    const double E_l = m_Z / 2.;
    const double p_l = sqrt(E_l*E_l - m_l*m_l);

    // lepton four-momenta in Z's rest frame
    CLHEP::HepLorentzVector momentum_l_mi(
      p_l * sin(theta_d) * cos(phi_d),
      p_l * sin(theta_d) * sin(phi_d),
      p_l * cos(theta_d),
      E_l
    );

    CLHEP::HepLorentzVector momentum_l_pl(
      -p_l * sin(theta_d) * cos(phi_d),
      -p_l * sin(theta_d) * sin(phi_d),
      -p_l * cos(theta_d),
      E_l
    );

    // apply boost
    double beta = momentum_Z.rho() / momentum_Z.t();
    CLHEP::Hep3Vector betaVector(momentum_Z.x(), momentum_Z.y(), momentum_Z.z());
    betaVector *= beta / betaVector.mag();
    momentum_l_mi = CLHEP::boostOf(momentum_l_mi, betaVector);
    momentum_l_pl = CLHEP::boostOf(momentum_l_pl, betaVector);

    if (verbosity)
    {
      const CLHEP::HepLorentzVector m_tot = momentum_p1 + momentum_p2 + momentum_X + momentum_l_mi + momentum_l_pl;
      printf("  four-momentum of p + p + X + l + l: (%.1f, %.1f, %.1f | %.1f)\n", m_tot.x(), m_tot.y(), m_tot.z(), m_tot.t());
    }

    // add particles to vertex
    HepMC::GenParticle* particle_l_mi = new HepMC::GenParticle(momentum_l_mi, particleId_l_mi, 1);
    particle_l_mi->suggest_barcode(++barcode);
    vtx->add_particle_out(particle_l_mi);

    HepMC::GenParticle* particle_l_pl = new HepMC::GenParticle(momentum_l_pl, particleId_l_pl, 1);
    particle_l_pl->suggest_barcode(++barcode);
    vtx->add_particle_out(particle_l_pl);
  }

  // validation
  if (debug)
  {
    const double p_T_X = sqrt(momentum_X.x()*momentum_X.x() + momentum_X.y()*momentum_X.y());
    const double p_z_X = momentum_X.z();
    const double p_tot_X = momentum_X.rho();
    const double theta_X = acos(p_z_X/p_tot_X);
    const double eta_X = -log(tan(theta_X/2.));

    const double p_T_Z = sqrt(momentum_Z.x()*momentum_Z.x() + momentum_Z.y()*momentum_Z.y());
    const double p_z_Z = momentum_Z.z();
    const double p_tot_Z = momentum_Z.rho();
    const double theta_Z = acos(p_z_Z/p_tot_Z);
    const double eta_Z = -log(tan(theta_Z/2.));

    h_m_Z->Fill(m_Z);
    h_m_XZ->Fill(m_XZ);
    h_p_z_LAB_2p->Fill(p_z_LAB_2p);

    h_p_T_X->Fill(p_T_X);
    h_p_z_X->Fill(p_z_X);
    h_p_tot_X->Fill(p_tot_X);
    h_theta_X->Fill(theta_X);
    h_eta_X->Fill(eta_X);

    h_p_T_Z->Fill(p_T_Z);
    h_p_z_Z->Fill(p_z_Z);
    h_p_tot_Z->Fill(p_tot_Z);
    h_theta_Z->Fill(theta_Z);
    h_eta_Z->Fill(eta_Z);
  }

  // save output
  std::unique_ptr<HepMCProduct> output(new HepMCProduct()) ;
  output->addHepMCData(fEvt);
  e.put(std::move(output), "unsmeared");
}

//----------------------------------------------------------------------------------------------------

PPXZGenerator::~PPXZGenerator()
{
  if (debug)
  {
    TFile *f_out = TFile::Open("PPXZGenerator_debug.root", "recreate");

    h_m_Z->Write();
    h_m_XZ->Write();
    h_p_z_LAB_2p->Write();

    h_p_T_X->Write();
    h_p_z_X->Write();
    h_p_tot_X->Write();
    h_theta_X->Write();
    h_eta_X->Write();

    h_p_T_Z->Write();
    h_p_z_Z->Write();
    h_p_tot_Z->Write();
    h_theta_Z->Write();
    h_eta_Z->Write();

    delete f_out;
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PPXZGenerator);
