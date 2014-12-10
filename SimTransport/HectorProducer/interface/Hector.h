#ifndef SimTransport_Hector_h
#define SimTransport_Hector_h

// user include files
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
/*
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
*/
// HepMC headers
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "SimDataFormats/Forward/interface/LHCTransportLink.h"
#include <vector>

// SimpleConfigurable replacement
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//Hector headers
#include "H_BeamLine.h"
#include "H_BeamParticle.h"
#include <string>
#include <map>
#include "TRandom3.h"
#include "TMath.h"
#include <math.h>

// some useful definitions

const double   ProtonMass = 0.93827;
const double   ProtonMassSQ = pow(ProtonMass,2);
const double   c_light_s = 2.99792458e+8;// m/s;
const double   s_to_ns  = 1.e-9;
const double   c_light_ns = c_light_s*s_to_ns;
const double   urad     = 1./1000000.; //micro rad
const double   um_to_mm = 0.001;
const double   um_to_cm = 0.0001;
const double   mm_to_um = 1000.;
const double   mm_to_cm = 0.1;
const double   mm_to_m  = 0.001;
const double   cm_to_um = 10000.;
const double   cm_to_mm = 10.;
const double   cm_to_m  = 0.01;
const double   m_to_cm  = 100.;
const double   m_to_mm  = 1000.;
const double   m_to_um  = 1000000.;

class Hector {
  
 public:
  //  Hector(const edm::ParameterSet & ps);
  Hector(const edm::ParameterSet & ps, bool verbosity);
  //  Hector();
  virtual ~Hector();
  
  /*!Clears ApertureFlags, prepares Hector for a next event*/
  void clearApertureFlags();
  /*!Clears BeamParticle, prepares Hector for a next Aperture check or/and a next event*/
  void clear();
  /*!Adds the stable protons from the event \a ev to a beamline*/
  void add( const HepMC::GenEvent * ev , const edm::EventSetup & es);
  /*!propagate the particles through a beamline to PPS*/
  void propagateToPPS();

  //int getDirect( unsigned int part_n ) const;

  /*!Prints properties of all particles in a beamline*/
    void print() const;

    HepMC::GenEvent * addPartToHepMC( HepMC::GenEvent * event );

    std::vector<LHCTransportLink> & getCorrespondenceMap() { return theCorrespondenceMap; }
    
    /* returns the angle, position and energy of the transported particle and returns true; 
       returns false if particle has stopped prior to reach any detector (ZDC, HPS240 or FP420) */
    bool getTransportedParameters(const unsigned int line  /* input parameter = index to the propagated particle*/, 
                                  long double& tx, long double& ty, 
                                  long double& vtx_x, long double& vtx_y, long double& vtx_z,
                                  long double& energy /* output parameters*/
                                 );
 private:
    
    struct PPSGenVertex {
           double x;
           double y;
           double Thx;
           double Thy;
           double e;
           int    direction;
    }; 
    // Defaults
    double m_BeamLineLength;
    
    double etacut;
    bool   m_smearAng;
    double m_sig_e;
    bool   m_smearE;
    double m_sigmaSTX;
    double m_sigmaSTY;
    double fBeamEnergy;
    double fBeamMomentum;
   
    float posTrkFirstStation_F;   // Position in Z of first forward tracker station (+Z)
    float posTrkSecondStation_F;  // Position in Z of second forward tracker station
    float posTrkFirstStation_B;   // Position in Z of first backward tracker station (-Z)
    float posTrkSecondStation_B;  // Position in Z of second backward tracker station
    float posToFStation_F;        // Position in Z of forward ToF station (+Z)
    float posToFStation_B;        // Position in Z of backward ToF station (-Z)
    float posPPSRegionStart_F;    // Position in Z of the first point in PPS (+Z)
    float posPPSRegionStart_B;    // Position in Z of the first point in PPS (-Z)
    float posTCL4;                // Position in Z of Collimator TCL4
    float posTCL5;                // Position in Z of Collimator TCL5
    float posTCL6;                // Position in Z of Collimator TCL6
    
    edm::ESHandle < ParticleDataTable > pdt;
    
    // Hector
    H_BeamLine * H_beamlineF;    // foreard beam line (+Z) - Beam 2 of LHC - counterclockwise
    H_BeamLine * H_beamlineB;    // backward bean line(-Z) - Beam 1 of LHC - clockwise

    std::map<unsigned int, int> m_direct;                       // particle direction
    //    
    std::map<unsigned int, H_BeamParticle*> m_beamPart;         // particles from primary vertex

// stopping position of the particles, 0 means it has not stopped before the last propagated Z position
    std::map<unsigned int, float> m_isStopped;
    //
    std::map<unsigned int, double> m_eta;
    std::map<unsigned int, int>    m_pdg;
    std::map<unsigned int, double> m_pz;
    std::map<unsigned int, bool>   m_isCharged;
    

// Coordinate and angles at the corresponding positions 
// used in the vertex creating after propagation
    //PPS
    //std::map<unsigned int, double> m_xAtPPSTracker;
    //std::map<unsigned int, double> m_yAtPPSTracker;
    //std::map<unsigned int, double> m_TxAtPPSTracker;
    //std::map<unsigned int, double> m_TyAtPPSTracker;
    //std::map<unsigned int, double> m_eAtPPSTracker;
    //std::map<unsigned int, double> m_xAtTCL4;
    //std::map<unsigned int, double> m_xAtPPSTracker;
    //std::map<unsigned int, double> m_xAtPPSTracker;
    std::map<unsigned int,PPSGenVertex> m_VtxAtPPSTracker;
    std::map<unsigned int,PPSGenVertex> m_VtxAtToF;
    std::map<unsigned int,PPSGenVertex> m_VtxAtTCL4;
    std::map<unsigned int,PPSGenVertex> m_VtxAtTCL5;
    std::map<unsigned int,PPSGenVertex> m_VtxAtTCL6;


    string beam1filename;
    string beam2filename;
    
    bool m_verbosity;

    std::vector<LHCTransportLink> theCorrespondenceMap;

    TRandom3* rootEngine_;
};
#endif
