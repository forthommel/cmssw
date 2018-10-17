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

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

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

    double parameterExample;

    // debug histograms
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
