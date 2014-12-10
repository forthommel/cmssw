#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "IOMC/RandomEngine/src/TRandomAdaptor.h"

#include "SimTransport/HectorProducer/interface/Hector.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
//#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "HepMC/SimpleVector.h"

#include "H_Parameters.h"

#include <math.h>

Hector::Hector(const edm::ParameterSet & param, bool verbosity) :
  m_verbosity(verbosity), rootEngine_(0)
{

  // Create LHC beam line
  edm::ParameterSet hector_par = param.getParameter<edm::ParameterSet>("Hector");

  // User definitons
  m_BeamLineLength      = (float)hector_par.getParameter<double>("BeamLineLength");
  posTrkFirstStation_F  = (float) hector_par.getParameter<double>("TrackerFirstStation_F");
  posTrkFirstStation_B  = (float) hector_par.getParameter<double>("TrackerFirstStation_B");
  posToFStation_F       = (float) hector_par.getParameter<double>("ToFStation_F");
  posToFStation_B       = (float) hector_par.getParameter<double>("ToFStation_B");
  posTCL4               = (float) hector_par.getParameter<double>("TCL4Position");
  posTCL5               = (float) hector_par.getParameter<double>("TCL5Position");
  posTCL6               = (float) hector_par.getParameter<double>("TCL6Position");

  beam1filename  = hector_par.getParameter<string>("Beam1");
  beam2filename  = hector_par.getParameter<string>("Beam2");
  m_smearAng     = hector_par.getParameter<bool>("smearAng");
  m_sigmaSTX     = hector_par.getParameter<double>("sigmaSTX" );
  m_sigmaSTY     = hector_par.getParameter<double>("sigmaSTY" );
  m_smearE       = hector_par.getParameter<bool>("smearEnergy");
  m_sig_e        = hector_par.getParameter<double>("sigmaEnergy");
  etacut         = hector_par.getParameter<double>("EtaCutForHector" );

  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable() ) {
    throw cms::Exception("Configuration")
      << "LHCTransport (Hector) requires the RandomNumberGeneratorService\n"
      "which is not present in the configuration file.  You must add the service\n"
      "in the configuration file or remove the modules that require it.";
  }
  if ( (rng->getEngine()).name() == "TRandom3" ) {
    rootEngine_ = ( (edm::TRandomAdaptor*) &(rng->getEngine()) )->getRootEngine();
    if ( m_verbosity ) LogDebug("HectorSetup") << "LHCTransport seed = " << rootEngine_->GetSeed();
  }
  else {
    edm::LogError("WrongRandomNumberGenerator") << "The TRandom3 engine must be used, Random Number Generator Service not correctly initialized!";
    rootEngine_ = new TRandom3();
  }

  theCorrespondenceMap.clear();

  if(m_verbosity) {
    edm::LogInfo("HectorSetup") << "===================================================================\n"
                                << " * * * * * * * * * * * * * * * * * * * * * * * * * * * *           \n"
                                << " *                                                         *       \n"
                                << " *                   --<--<--  A fast simulator --<--<--     *     \n"
                                << " *                 | --<--<--     of particle   --<--<--     *     \n"
                                << " *  ----HECTOR----<                                          *     \n"
                                << " *                 | -->-->-- transport through-->-->--      *     \n"
                                << " *                   -->-->-- generic beamlines -->-->--     *     \n"
                                << " *                                                           *     \n"
                                << " * JINST 2:P09005 (2007)                                     *     \n"
                                << " *      X Rouby, J de Favereau, K Piotrzkowski (CP3)         *     \n"
                                << " *       http://www.fynu.ucl.ac.be/hector.html               *     \n"
                                << " *                                                           *     \n"
                                << " * Center for Cosmology, Particle Physics and Phenomenology  *     \n"
                                << " *              Universite catholique de Louvain             *     \n"
                                << " *                 Louvain-la-Neuve, Belgium                 *     \n"
                                << " *                                                         *       \n"
                                << " * * * * * * * * * * * * * * * * * * * * * * * * * * * *           \n"
                                << " Hector configuration: \n"
                                << " BeamLine length        = " << m_BeamLineLength << "\n"
                                << " TCL4 Z position        = " << posTCL4 << "\n"
                                << " TCL5 Z position        = " << posTCL5 << "\n"
                                << " TCL6 Z position        = " << posTCL6 << "\n"
                                << " Tracker 1st position F    = " << posTrkFirstStation_F << "\n"
                                << " ToF         position F    = " << posToFStation_F << "\n"
                                << " Tracker 1st position B    = " << posTrkFirstStation_B << "\n"
                                << " ToF         position B    = " << posToFStation_B << "\n"
                                << "===================================================================\n";
  }
  edm::FileInPath b1(beam1filename.c_str());
  edm::FileInPath b2(beam2filename.c_str());

  // construct beam line for FP420:                                                                                           .
  H_beamlineF = new H_BeamLine( -1, m_BeamLineLength + 0.1 ); // (direction, length)
  H_beamlineB = new H_BeamLine(  1, m_BeamLineLength + 0.1 ); //
  H_beamlineF->fill( b2.fullPath(),  1, "IP5" );
  H_beamlineB->fill( b1.fullPath(),  1, "IP5" );
  H_beamlineF->offsetElements( 120, +0.097 );
  H_beamlineB->offsetElements( 120, +0.097 );
  H_beamlineF->calcMatrix();
  H_beamlineB->calcMatrix();
  cout << "Beam line elements" << endl;
  cout << H_beamlineB->getNumberOfElements() <<endl;
  cout << H_beamlineF->getNumberOfElements() <<endl;

  // Create a particle to get the beam energy from the beam file
  H_BeamParticle *beam = new H_BeamParticle(ProtonMass,1);
  fBeamEnergy = beam->getE(); // redefine globally the beam energy
  fBeamMomentum = sqrt(fBeamEnergy*fBeamEnergy - ProtonMassSQ);
  delete beam;
}

Hector::~Hector(){

  for (std::map<unsigned int,H_BeamParticle*>::iterator it = m_beamPart.begin(); it != m_beamPart.end(); ++it ) {
    delete (*it).second;
  }

  delete H_beamlineF;
  delete H_beamlineB;
}

void Hector::clearApertureFlags(){
  m_isStopped.clear();
}

void Hector::clear(){
  for ( std::map<unsigned int,H_BeamParticle*>::iterator it = m_beamPart.begin(); it != m_beamPart.end(); ++it ) {
    delete (*it).second;
  };
  m_beamPart.clear();
  m_direct.clear();
  m_eta.clear();
  m_pdg.clear();
  m_pz.clear();
  m_isCharged.clear();
}

void Hector::add( const HepMC::GenEvent * evt ,const edm::EventSetup & iSetup) {

  H_BeamParticle * h_p;
  unsigned int line;
  int direction;

  for(HepMC::GenEvent::particle_const_iterator eventParticle =evt->particles_begin();
                                               eventParticle != evt->particles_end();
                                             ++eventParticle ) {
     if ( (*eventParticle)->status() != 1 ) continue;
     if ( abs( (*eventParticle)->momentum().eta())<etacut) continue;

     line = (*eventParticle)->barcode();
     if ( m_beamPart.find(line) != m_beamPart.end() ) continue;
     if (m_verbosity) (*eventParticle)->print();

     double charge=1.;
     m_isCharged[line] = false;// neutrals
     HepMC::GenParticle * g = (*eventParticle);
     iSetup.getData( pdt );
     const ParticleData * part = pdt->particle( g->pdg_id() );
     if (part) charge = part->charge();
     if (charge !=0) m_isCharged[line] = true;//charged
     double mass = (*eventParticle)->generatedMass();

     h_p = new H_BeamParticle(mass,charge);

     double px,py,pz;
     px = (*eventParticle)->momentum().px();
     py = (*eventParticle)->momentum().py();
     pz = (*eventParticle)->momentum().pz();
     direction = (pz>0)?1:-1;

     // from mm to um
     double XforPosition = (*eventParticle)->production_vertex()->position().x()*cm/um;//um
     double YforPosition = (*eventParticle)->production_vertex()->position().y()*cm/um;//um
     double ZforPosition = (*eventParticle)->production_vertex()->position().z()*cm/m;//m
     double theta = (*eventParticle)->momentum().theta();
     double phi   = (*eventParticle)->momentum().phi();
     // crossing angle (beam tilt) is not known a priory; keep now 0.0 but, in principle, can be entered as parameters
     double TXforPosition=0.0, TYforPosition=0.0;//urad
//
     if (m_verbosity) {
        double beamP = sqrt(pow(fBeamEnergy,2)-mass*mass);
        double t = -2.*(beamP*sqrt(px*px+py*py+pz*pz)*cos(theta)*direction-(*eventParticle)->momentum().e()*fBeamEnergy+mass*mass);
        double xi=1.-(*eventParticle)->momentum().e()/fBeamEnergy;
        LogDebug("HectorEventProcessing") << "Particle initial parameters (from HepMC::Event): \n"
                                          << "X: " << XforPosition << " Y: " << YforPosition << " Z: " << ZforPosition
                                          << " TX: " << TXforPosition << " TY: " << TYforPosition << "\n"
                                          << "theta: " << theta  << " " << "phi: " << phi  << " "
                                          << "  t: " << t << " Xi: " << xi;
     }

     phi = atan2(py,px); // at CMS

// Clears H_BeamParticle::positions and sets the initial one
// the position must be set before the 4 momentum, since this last one will calculate the beam tilt according to the momentum
// Hector uses X = -X and Z = -Z in relation to CMSSW

     h_p->setPosition(-XforPosition, YforPosition, -TXforPosition, TYforPosition, -ZforPosition );
     h_p->set4Momentum( -px, py, -pz, (*eventParticle)->momentum().e() );
     if (m_verbosity) {
        double beamP = sqrt(pow(fBeamEnergy,2)-mass*mass);
        double partP = sqrt(pow(h_p->getE(),2)-pow(h_p->getM(),2));
        double theta = sqrt(pow(h_p->getTX(),2)+pow(h_p->getTY(),2))/URAD;
        double phi   = atan2(h_p->getTY()/URAD,h_p->getTX()/URAD);
        double t = -2.*(beamP*partP*cos(theta)-h_p->getE()*fBeamEnergy+mass*mass);
        double xi=1.-(h_p->getE()/fBeamEnergy);
        LogDebug("HectorEventProcessing") << "Particle initial parameters (from HECTOR): \n"
                                          << "X: " << h_p->getX() << " Y: " << h_p->getY() << " Z: " << h_p->getS()
                                          << " TX: " << h_p->getTX() << " TY: " << h_p->getTY() << "\n"
                                          << "theta: " << theta  << " "
                                          << "phi: " << phi << " "
                                          << "  t: " << t << " Xi: " << xi;
     }

     m_beamPart[line] = h_p;
     m_direct[line] = direction;
     m_eta[line] = (*eventParticle)->momentum().eta();
     m_pdg[line] = (*eventParticle)->pdg_id();
     m_pz[line]  = pz;

     if (m_verbosity) {
        LogDebug("HectorEventProcessing") << "Hector:add: barcode = " << line
                                          << " status = " << g->status()
                                          << " PDG Id = " << g->pdg_id()
                                          << " mass = " << mass
                                          << " pz = " << pz
                                          << " charge = " << charge
                                          << " m_isCharged[line] = " << m_isCharged[line];
     }
  }// for loop
}

void Hector::propagateToPPS(){
  unsigned int line;
  std::map< unsigned int, H_BeamParticle* >::iterator it;

  bool is_stop;
  int direction;

  long double hitX;
  long double hitY;
  long double hitThX;
  long double hitThY;

  if ( !(m_beamPart.size() && m_BeamLineLength>0.) ) return;

  if (! H_beamlineF || ! H_beamlineB) {
     edm::LogInfo("HectorEventProcessing") << "Hector:propagateToPPS: NULL beamline pointer.";
     return;
  }

  for (it = m_beamPart.begin(); it != m_beamPart.end(); ++it ) {
      line = (*it).first;
      H_BeamParticle * part = (*it).second;

      if(m_verbosity) LogDebug("HectorEventProcessing") << "Hector:propagatToPPS: barcode = " << line;

      m_isStopped[line] = 500.;// assume that the particle has stopped in the beam line

      if (! (*m_isCharged.find( line )).second ) continue;
      if (abs( (*m_pdg.find( line )).second) != 2212 ||            // propagate only protons
          abs( (*m_pz.find( line )).second) < 3000. ) continue;    // with Pz > 3000 GeV

      direction = (*m_direct.find( line )).second;
      H_BeamLine *beamline = (direction>0)?H_beamlineF:H_beamlineB;

      if (!beamline) continue;  // should never happen

      part->computePath( beamline );
      is_stop = part->stopped( beamline );

      if (is_stop) {
         m_isStopped[line] = part->getStoppingElement()->getS();
         continue;
      }
      //propagating
      if(m_verbosity) {
         double theta = sqrt(pow(part->getTX(),2)+pow(part->getTY(),2))/URAD;
         double phi   = atan2(part->getTY()/URAD,part->getTX()/URAD);

         LogDebug("HectorEventProcessing") << "Hector:propagateToPPS: barcode = " << line << " isStopped=" << is_stop;
         LogDebug("HectorEventProcessing") << "propatagetToPPS: Particle initial parameters (before propagation): \n"
                                           << "X: " << part->getX() << " Y: " << part->getY() << " Z: " << part->getS()
                                           << " TX: " << part->getTX() << " TY: " << part->getTY() << "\n"
                                           << "theta: " << theta  << " "
                                           << "phi: " << phi;
      }

      double detPos = (direction>0)?posTrkFirstStation_F:posTrkFirstStation_B;

      part->propagate( detPos );
      hitX = (double)-part->getX(); // in micrometer (X axis as in CMS)
      hitY = (double)part->getY(); // in micrometer
      hitThX= (double)-part->getTX();
      hitThY= (double) part->getTY();

      if(m_verbosity) LogDebug("HectorEventProcessing") << "Hector:propagateToPPS: barcode = "
                                                        << line << " x=  "<< hitX <<" y= " << hitY
                                                        << "Tx=  "<< hitThX  <<"Ty =  " << hitThY ;

/* At this point, the units are the same as in Hector, which means:
   X,Y     in micrometer
   Z       in meter
   Tx,Ty   in microradians
   energy  in GeV
*/

      m_VtxAtPPSTracker[line].x   = hitX;
      m_VtxAtPPSTracker[line].y   = hitY;
      m_VtxAtPPSTracker[line].Thx = hitThX; // in microrad
      m_VtxAtPPSTracker[line].Thy = hitThY; // in microrad
      m_VtxAtPPSTracker[line].e   = (long double)part->getE();
      m_VtxAtPPSTracker[line].direction = direction;

   } // loop over particle collection
}

/*int  Hector::getDirect( unsigned int part_n ) const {
  std::map<unsigned int, int>::const_iterator it = m_direct.find( part_n );
  if ( it != m_direct.end() ){
    return (*it).second;
  }
  return 0;
}
*/
void Hector::print() const {
  for (std::map<unsigned int,H_BeamParticle*>::const_iterator it = m_beamPart.begin(); it != m_beamPart.end(); ++it ) {
    (*it).second->printProperties();
  };
}


HepMC::GenEvent * Hector::addPartToHepMC( HepMC::GenEvent * evt ){

  theCorrespondenceMap.clear();

  unsigned int line;

  HepMC::GenParticle * gpart;
  long double tx,ty,theta,fi,energy,time = 0;
  long double vtx_x, vtx_y, vtx_z;

  std::map< unsigned int, H_BeamParticle* >::iterator it;

  for(it = m_beamPart.begin(); it != m_beamPart.end(); ++it ) {
     line = (*it).first;
     gpart = evt->barcode_to_particle( line );
     if (! gpart ) continue;

// get the propagated particle parameters; return false if particle did not get in any detector (ZDC, HPS240 nor FP420)
     if (!(getTransportedParameters(line,tx,ty,vtx_x,vtx_y,vtx_z,energy))) continue;

     tx/=URAD; ty/=URAD; // converts from micro rad to radians
     //energy;        // convert from GeV to MeV
     vtx_z*=meter;       // meter to mm
     vtx_x*=micrometer;  // micrometer mm
     vtx_y*=micrometer;  // micrometer mm

     
     //int direction = 1;
     theta  = sqrt((tx*tx) + (ty*ty));
     if ((*m_VtxAtPPSTracker.find( line )).second.direction <0 ) theta= TMath::Pi()-theta;

     fi     = std::atan2(ty,tx); // tx, ty never == 0?
     std::cout << "Phi : " << fi << " theta: "  << theta << endl;

     time = fabs(( vtx_z - gpart->production_vertex()->position().z()*cm )); // GenVertex is given in cm instead of mm

     if (m_verbosity) {
        LogDebug("HectorEventProcessing") <<"Hector:addParticle: x= "<< vtx_x << "\n"
                                          <<"Hector:addParticle: y= "<< vtx_y << "\n"
                                          <<"Hector:addParticle: z= "<< vtx_z << "\n"
                                          <<"Hector:addParticle: t= "<< time;
     }
// GenVertex uses  mm 
     HepMC::GenVertex * vert = new HepMC::GenVertex( HepMC::FourVector(vtx_x, vtx_y, vtx_z, time + .001*time ) );

     gpart->set_status( 2 );
     vert->add_particle_in( gpart );

     if (m_verbosity) {
        LogDebug("HectorEventProcessing") <<"Hector:addParticle: Px= "<< energy*std::sin(theta)*std::cos(fi) << "\n"
                                          <<"Hector:addParticle: Py= "<< energy*std::sin(theta)*std::sin(fi) << "\n"
                                          <<"Hector:addParticle: Pz= "<< energy*std::cos(theta)  << "\n"
                                          <<"Hector:addParticle: E = "<< energy << "\n"
                                          <<"Hector:addParticle: Id= "<< gpart->pdg_id() << "\n"
                                          <<"Hector:addParticle: Flow="<< gpart->flow();
     }
     double partP = sqrt(energy*energy+ProtonMassSQ);
     vert->add_particle_out( new HepMC::GenParticle( HepMC::FourVector(partP*std::sin(theta)*std::cos(fi),
                                                                       partP*std::sin(theta)*std::sin(fi),
                                                                       partP*std::cos(theta),
                                                                       energy ),
                                              gpart->pdg_id(), 1, gpart->flow() ) );

     evt->add_vertex( vert );

     int ingoing = (*vert->particles_in_const_begin())->barcode();
     int outgoing = (*vert->particles_out_const_begin())->barcode();
     LHCTransportLink theLink(ingoing,outgoing);

     if (m_verbosity) LogDebug("HectorEventProcessing") << "Hector:addParticle  addPartToHepMC: LHCTransportLink " << theLink;

     theCorrespondenceMap.push_back(theLink);

     if (m_verbosity) LogDebug("HectorEventProcessing") << "Hector:addParticle :TRANSPORTED pz= " << gpart->momentum().pz()
                                                        << " eta= "<< gpart->momentum().eta()
                                                        << " status= "<< gpart->status();
  }//for

  return evt;
}
bool Hector::getTransportedParameters(const unsigned int line, long double& tx, long double& ty, long double& vtx_x, long double& vtx_y, long double& vtx_z, long double& energy)
{
/* At this point, the units are the same as in Hector, which means:
   X,Y     in micrometer
   Z       in meter
   Tx,Ty   in microradians
   energy  in GeV
*/
     if (!((*m_isStopped.find(line)).second<posTrkFirstStation_F ||
           (*m_isStopped.find(line)).second<posTrkFirstStation_B)) {     //!((*m_isStoppedfp420
        tx     = (*m_VtxAtPPSTracker.find(line)).second.Thx;
        ty     = (*m_VtxAtPPSTracker.find(line)).second.Thy;
        vtx_x  = (*m_VtxAtPPSTracker.find(line)).second.x;
        vtx_y  = (*m_VtxAtPPSTracker.find(line)).second.y;
        vtx_z  =((*m_VtxAtPPSTracker.find(line)).second.direction<0)?-posTrkFirstStation_B:posTrkFirstStation_F;
        energy = (*m_VtxAtPPSTracker.find(line)).second.e;
     }
     else {return false;}
     if (vtx_z==0.) return false;
     return true;
}
