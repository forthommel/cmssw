/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Nicola Turini, Francesco Turini, Lorenzo Marini, Jan Kašpar
 *
 ****************************************************************************/

#ifndef PPXZGenerator_H
#define PPXZGenerator_H

#include "FWCore/Framework/interface/one/EDProducer.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "TH1D.h"
#include "TFile.h"

namespace edm {
  
class PPXZGenerator : public one::EDProducer<>
{

  public:
    PPXZGenerator(const ParameterSet &);

    virtual ~PPXZGenerator();

  private:
    virtual void produce(Event & e, const EventSetup& es) override;

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

} 

#endif
