/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Nicola Turini, Francesco Turini, Lorenzo Marini, Jan Kašpar
 *
 ****************************************************************************/

#include "IOMC/ParticleGuns/interface/PPXZGenerator.h" 

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "HepPDT/ParticleDataTable.hh"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include <CLHEP/Random/RandExponential.h>

using namespace edm;
using namespace std;

//----------------------------------------------------------------------------------------------------

PPXZGenerator::PPXZGenerator(const edm::ParameterSet& pset) : 
  debug(pset.getUntrackedParameter<unsigned int>("debug", 0))
  //parameterExample(pset.getParameter<double>("parameterExample"))
{
  if (debug)
  {
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
  printf("\n>> PPXZGenerator::produce > event %llu\n", e.id().event());

  // get conditions
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* engine = &rng->getEngine(e.streamID());

  ESHandle<HepPDT::ParticleDataTable> pdgTable;
  es.getData(pdgTable);

  // prepare HepMC event
  HepMC::GenEvent *fEvt = new HepMC::GenEvent();
  fEvt->set_event_number(e.id().event());
   
  // generate vertex position
  HepMC::GenVertex *vtx = new HepMC::GenVertex(HepMC::FourVector(0., 0., 0., 0.));
  fEvt->add_vertex(vtx);

  // particle ids
  const unsigned int particleId_Z = 23;
  const unsigned int particleId_X = 999999;
  const unsigned int particleId_p = 2212;

  // particle masses
  const double m_X = 1200.; // GeV
  const double m_Z = 91.;   // GeV

  //const HepPDT::ParticleData *pData = pdgTable->particle(HepPDT::ParticleID(particleId));
  //double mass_1 = pData->mass().value();
  //double mass_2 = pData->mass().value();

  // beam momentum
  const double p_beam = 6500.; // GeV

  // generate invariant mass of the X-Z system
  const double c = 0.04;
  const double c_mean = 1. / c;
  const double m_XZ = 1300. + CLHEP::RandExponential::shoot(engine, c_mean);

  // generate p_z of the 2-proton system in the LAB frame
  const double medlab = 180.0, sigmalab = 450.0;
  const double p_z_LAB_2p = CLHEP::RandGauss::shoot(engine, medlab, sigmalab);

  // generate spherical angles in the CMS frame of the X-Z system
  const double theta_c = CLHEP::RandFlat::shoot(engine) * M_PI;
  const double phi_c = CLHEP::RandFlat::shoot(engine) * 2. * M_PI;

  // determine xi's of the protons
  // proton 1: positive z momentum component
  const double xi2 = (p_z_LAB_2p + sqrt(p_z_LAB_2p*p_z_LAB_2p + m_XZ*m_XZ)) / (2. * p_beam);
  const double xi1 = m_XZ * m_XZ / (4. * p_beam * p_beam * xi2);

  printf("  m_XZ = %.1f\n", m_XZ);
  printf("  p_z_LAB_2p = %.1f\n", p_z_LAB_2p);
  printf("  xi1 = %.3f, xi2 = %.3f\n", xi1, xi2);
  printf("  p_beam * (xi2 - xi1) = %.1f\n", p_beam * (xi2 - xi1));

  // determine momenta of the X and Z particles in the CMS frame of the X-Z system
  const double p_c = sqrt( pow(m_XZ*m_XZ - m_X*m_X - m_Z*m_Z, 2.) / (4. * m_XZ * m_XZ) - m_X*m_X * m_Z*m_Z / (m_XZ*m_XZ) );

  const double E_X_CMS = sqrt(p_c*p_c + m_X*m_X);
  const double p_X_x_CMS = p_c * sin(theta_c) * cos(phi_c);
  const double p_X_y_CMS = p_c * sin(theta_c) * sin(phi_c);
  const double p_X_z_CMS = p_c * sin(theta_c);

  const double E_Z_CMS = sqrt(p_c*p_c + m_Z*m_Z);
  const double p_Z_x_CMS = - p_c * sin(theta_c) * cos(phi_c);
  const double p_Z_y_CMS = - p_c * sin(theta_c) * sin(phi_c);
  const double p_Z_z_CMS = - p_c * sin(theta_c);

  //const double E_XZ_CMS = E_X_CMS + E_Z_CMS;

  // determine boost from X-Z CMS frame to the LAB frame
  //const double E_XZ_LAB = p_beam * (xi1 + xi2);
  const double beta = - (xi1 - xi2) / (xi1 + xi2);
  const double gamma = 1. / sqrt(1. - beta*beta);

  printf("  beta = %.3f\n", beta);

  // determine four-momenta of the outgoing particles in the LAB frame

  HepMC::FourVector momentum_X(
    p_X_x_CMS,
    p_X_y_CMS,
    gamma * (p_X_z_CMS - beta * E_X_CMS),
    gamma * (E_X_CMS - beta * p_X_z_CMS)
  );
  
  HepMC::FourVector momentum_Z(
    p_Z_x_CMS,
    p_Z_y_CMS,
    gamma * (p_Z_z_CMS - beta * E_Z_CMS),
    gamma * (E_Z_CMS - beta * p_Z_z_CMS)
  );

  HepMC::FourVector momentum_p1(0., 0., +p_beam * (1. - xi1), p_beam * (1. - xi1));

  HepMC::FourVector momentum_p2(0., 0., -p_beam * (1. - xi2), p_beam * (1. - xi2));

  printf("  p_X_z = %.1f\n", momentum_X.z());
  printf("  p_Z_z = %.1f\n", momentum_Z.z());

  printf("  p_x lab sum = %.1f\n", momentum_X.x() + momentum_Z.x());
  printf("  p_y lab sum = %.1f\n", momentum_X.y() + momentum_Z.y());
  printf("  p_z lab sum = %.1f\n", momentum_X.z() + momentum_Z.z() + p_z_LAB_2p);

  // fill in the HepMC record
  unsigned int barcode = 0;

  HepMC::GenParticle* particle_Z = new HepMC::GenParticle(momentum_Z, particleId_Z, 1);
  particle_Z->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_Z);

  HepMC::GenParticle* particle_X = new HepMC::GenParticle(momentum_X, particleId_X, 1);
  particle_X->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_X);

  HepMC::GenParticle* particle_p1 = new HepMC::GenParticle(momentum_p1, particleId_p, 1);
  particle_p1->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_p1);

  HepMC::GenParticle* particle_p2 = new HepMC::GenParticle(momentum_p2, particleId_p, 1);
  particle_p2->suggest_barcode(++barcode);
  vtx->add_particle_out(particle_p2);

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
