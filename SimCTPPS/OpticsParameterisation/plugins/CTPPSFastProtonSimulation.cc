/****************************************************************************
 *
 * This is a part of CTPPS offline software
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "CondFormats/CTPPSOpticsObjects/interface/LHCOpticsApproximator.h"

#include "Geometry/VeryForwardRPTopology/interface/RPTopology.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"

#include <unordered_map>

#include "TMath.h"
#include "TMatrixD.h"
#include "TVectorD.h"

//----------------------------------------------------------------------------------------------------

class CTPPSFastProtonSimulation : public edm::stream::EDProducer<>
{
  public:
    explicit CTPPSFastProtonSimulation( const edm::ParameterSet& );
    ~CTPPSFastProtonSimulation();

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    struct CTPPSPotInfo
    {
      CTPPSPotInfo() : detid( 0 ), z_position( 0.0 ), approximator( 0 ), x_max(0.) {}

      CTPPSPotInfo( const CTPPSDetId& det_id, double z_position, LHCOpticsApproximator* approx, double _x_max ) :
        detid( det_id ), z_position( z_position ), approximator( approx ), x_max(_x_max) {}

      CTPPSDetId detid;
      double z_position;
      LHCOpticsApproximator* approximator;
      double x_max;
    };

    virtual void beginRun( const edm::Run&, const edm::EventSetup& ) override;

    virtual void produce( edm::Event&, const edm::EventSetup& ) override;

    void processProton(const HepMC::GenVertex* in_vtx, const HepMC::GenParticle* in_trk,
      const CTPPSGeometry &geometry, std::vector<CTPPSLocalTrackLite> &out_tracks, edm::DetSetVector<TotemRPRecHit>& out_strip_hits,
      edm::DetSetVector<CTPPSPixelRecHit> &out_pixel_hits, edm::DetSetVector<CTPPSDiamondRecHit> &out_diamond_hits) const;

    static bool isPixelHit(float xLocalCoordinate, float yLocalCoordinate, bool is3x2 = true)
    {
      float tmpXlocalCoordinate = xLocalCoordinate + (79*0.1 + 0.2);
      float tmpYlocalCoordinate = yLocalCoordinate + (0.15*51 + 0.3*2 + 0.15*25);

      if(tmpXlocalCoordinate<0) return false;
      if(tmpYlocalCoordinate<0) return false;
      int xModuleSize = 0.1*79 + 0.2*2 + 0.1*79; // mm - 100 um pitch direction
      int yModuleSize; // mm - 150 um pitch direction
      if (is3x2) yModuleSize = 0.15*51 + 0.3*2 + 0.15*50 + 0.3*2 + 0.15*51;
      else       yModuleSize = 0.15*51 + 0.3*2 + 0.15*51;
      if(tmpXlocalCoordinate>xModuleSize) return false;
      if(tmpYlocalCoordinate>yModuleSize) return false;
      return true;
    }

    // ------------ config file parameters ------------

    /// input tag
    edm::EDGetTokenT<edm::HepMCProduct> hepMCToken_;

    /// beam conditions
    edm::ParameterSet beamConditions_;
    double halfCrossingAngleSector45_, halfCrossingAngleSector56_;
    double yOffsetSector45_, yOffsetSector56_;

    /// optics input
    edm::FileInPath opticsFileBeam1_, opticsFileBeam2_;

    /// detector information
    std::vector<edm::ParameterSet> detectorPackages_;

    /// flags what output to be produced
    bool produceScoringPlaneHits_;
    bool produceRecHits_;

    /// simulation parameters
    bool checkApertures_;
    bool produceHitsRelativeToBeam_;
    bool roundToPitch_;
    bool checkIsHit_;
    double pitchStrips_; ///< strip pitch in mm
    double pitchPixelsHor_;
    double pitchPixelsVer_;
    double insensitiveMarginStrips_; ///< size of insensitive margin at sensor's edge facing the beam, in mm


    // ------------ internal parameters ------------

    unsigned int verbosity_;

    /// collection of RP information
    std::vector<CTPPSPotInfo> pots_;

    /// map: RP id -> vector of sensor ids
    std::unordered_map<unsigned int, std::vector<CTPPSDetId> > strips_list_;

    /// internal variable: v position of strip 0, in mm
    double stripZeroPosition_;

    static const bool invertBeamCoordinatesSystem_;
};

//----------------------------------------------------------------------------------------------------

const bool CTPPSFastProtonSimulation::invertBeamCoordinatesSystem_ = true;

//----------------------------------------------------------------------------------------------------

CTPPSFastProtonSimulation::CTPPSFastProtonSimulation( const edm::ParameterSet& iConfig ) :
  hepMCToken_( consumes<edm::HepMCProduct>( iConfig.getParameter<edm::InputTag>( "hepMCTag" ) ) ),

  beamConditions_             ( iConfig.getParameter<edm::ParameterSet>( "beamConditions" ) ),
  halfCrossingAngleSector45_  ( beamConditions_.getParameter<double>( "halfCrossingAngleSector45" ) ),
  halfCrossingAngleSector56_  ( beamConditions_.getParameter<double>( "halfCrossingAngleSector56" ) ),
  yOffsetSector45_            ( beamConditions_.getParameter<double>( "yOffsetSector45" ) ),
  yOffsetSector56_            ( beamConditions_.getParameter<double>( "yOffsetSector56" ) ),

  opticsFileBeam1_            ( iConfig.getParameter<edm::FileInPath>( "opticsFileBeam1" ) ),
  opticsFileBeam2_            ( iConfig.getParameter<edm::FileInPath>( "opticsFileBeam2" ) ),
  detectorPackages_           ( iConfig.getParameter< std::vector<edm::ParameterSet> >( "detectorPackages" ) ),

  produceScoringPlaneHits_    ( iConfig.getParameter<bool>( "produceScoringPlaneHits" ) ),
  produceRecHits_             ( iConfig.getParameter<bool>( "produceRecHits" ) ),

  checkApertures_             ( iConfig.getParameter<bool>( "checkApertures" ) ),
  produceHitsRelativeToBeam_  ( iConfig.getParameter<bool>( "produceHitsRelativeToBeam" ) ),
  roundToPitch_               ( iConfig.getParameter<bool>( "roundToPitch" ) ),
  checkIsHit_                 ( iConfig.getParameter<bool>( "checkIsHit" ) ),
  pitchStrips_                ( iConfig.getParameter<double>( "pitchStrips" ) ),
  pitchPixelsHor_             ( iConfig.getParameter<double>( "pitchPixelsHor" ) ),
  pitchPixelsVer_             ( iConfig.getParameter<double>( "pitchPixelsVer" ) ),
  insensitiveMarginStrips_    ( iConfig.getParameter<double>( "insensitiveMarginStrips" ) ),

  verbosity_                  ( iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) )
{
  if (produceScoringPlaneHits_)
    produces<std::vector<CTPPSLocalTrackLite>>();

  if (produceRecHits_)
  {
    produces<edm::DetSetVector<TotemRPRecHit>>();
    produces<edm::DetSetVector<CTPPSDiamondRecHit>>();
    produces<edm::DetSetVector<CTPPSPixelRecHit>>();
  }

  // v position of strip 0
  stripZeroPosition_ = RPTopology::last_strip_to_border_dist_ + (RPTopology::no_of_strips_-1)*RPTopology::pitch_ - RPTopology::y_width_/2.;

  auto f_in_optics_beam1 = std::make_unique<TFile>( opticsFileBeam1_.fullPath().c_str() );
  auto f_in_optics_beam2 = std::make_unique<TFile>( opticsFileBeam2_.fullPath().c_str() );

  // load optics and interpolators
  for ( const auto& rp : detectorPackages_ )
  {
    const std::string interp_name = rp.getParameter<std::string>( "interpolatorName" );
    const unsigned int raw_detid = rp.getParameter<unsigned int>( "potId" );
    const double z_position = rp.getParameter<double>( "zPosition" );
    const double x_max = rp.getParameter<double>( "maxX" );

    CTPPSDetId detid( raw_detid );

    if ( detid.arm()==0 ) // sector 45 -- beam 2
      pots_.emplace_back( detid, z_position, dynamic_cast<LHCOpticsApproximator*>( f_in_optics_beam2->Get( interp_name.c_str() ) ), x_max );
    if ( detid.arm()==1 ) // sector 56 -- beam 1
      pots_.emplace_back( detid, z_position, dynamic_cast<LHCOpticsApproximator*>( f_in_optics_beam1->Get( interp_name.c_str() ) ), x_max );
  }
}

//----------------------------------------------------------------------------------------------------

CTPPSFastProtonSimulation::~CTPPSFastProtonSimulation()
{}


//----------------------------------------------------------------------------------------------------

void CTPPSFastProtonSimulation::beginRun( const edm::Run&, const edm::EventSetup& iSetup )
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSFastProtonSimulation::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  // get input
  edm::Handle<edm::HepMCProduct> hepmc_prod;
  iEvent.getByToken(hepMCToken_, hepmc_prod);

  // get geometry
  edm::ESHandle<CTPPSGeometry> geometry;
  iSetup.get<VeryForwardMisalignedGeometryRecord>().get(geometry);

  // prepare outputs
  std::unique_ptr<edm::DetSetVector<TotemRPRecHit>> pStripRecHits(new edm::DetSetVector<TotemRPRecHit>());
  std::unique_ptr<edm::DetSetVector<CTPPSDiamondRecHit>> pDiamondRecHits(new edm::DetSetVector<CTPPSDiamondRecHit>());
  std::unique_ptr<edm::DetSetVector<CTPPSPixelRecHit>> pPixelRecHits(new edm::DetSetVector<CTPPSPixelRecHit>());

  std::unique_ptr<std::vector<CTPPSLocalTrackLite>> pTracks(new std::vector<CTPPSLocalTrackLite>());

  // loop over event vertices
  auto evt = new HepMC::GenEvent( *hepmc_prod->GetEvent() );
  for ( auto it_vtx = evt->vertices_begin(); it_vtx != evt->vertices_end(); ++it_vtx )
  {
    auto vtx = *( it_vtx );

    // loop over outgoing particles
    for ( auto it_part = vtx->particles_out_const_begin(); it_part != vtx->particles_out_const_end(); ++it_part )
    {
      auto part = *( it_part );

      // accept only stable protons
      if ( part->pdg_id() != 2212 )
        continue;

      if ( part->status() != 1 && part->status() < 83 )
        continue;

      processProton(vtx, part, *geometry, *pTracks, *pStripRecHits, *pPixelRecHits, *pDiamondRecHits);
    }
  }

  if (produceScoringPlaneHits_)
    iEvent.put(std::move(pTracks));

  if (produceRecHits_)
  {
    iEvent.put(std::move(pStripRecHits));
    iEvent.put(std::move(pPixelRecHits));
    iEvent.put(std::move(pDiamondRecHits));
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSFastProtonSimulation::processProton(const HepMC::GenVertex* in_vtx, const HepMC::GenParticle* in_trk,
  const CTPPSGeometry &geometry, std::vector<CTPPSLocalTrackLite> &out_tracks, edm::DetSetVector<TotemRPRecHit>& out_strip_hits,
  edm::DetSetVector<CTPPSPixelRecHit> &out_pixel_hits, edm::DetSetVector<CTPPSDiamondRecHit> &out_diamond_hits) const
{
  /// xi is positive for diffractive protons, thus proton momentum p = (1-xi) * p_nom
  /// horizontal component of proton momentum: p_x = th_x * (1-xi) * p_nom
  
  // vectors in CMS convention
  const HepMC::FourVector vtx_cms = in_vtx->position(); // in mm
  const HepMC::FourVector mom_cms = in_trk->momentum();

  // transformation to LHC/TOTEM convention
  HepMC::FourVector vtx_lhc(-vtx_cms.x(), vtx_cms.y(), -vtx_cms.z(), vtx_cms.t());
  HepMC::FourVector mom_lhc(-mom_cms.x(), mom_cms.y(), -mom_cms.z(), mom_cms.t());

  // determine the LHC arm and related parameters
  unsigned int arm = 3;
  double half_cr_angle = 0.0, vtx_y_offset = 0.0;
  double z_sign;

  if (mom_lhc.z() < 0)  // sector 45
  {
    arm = 0;
    z_sign = -1;
    vtx_y_offset = yOffsetSector45_;
    half_cr_angle = halfCrossingAngleSector45_;
  } else {  // sector 56
    arm = 1;
    z_sign = +1;
    vtx_y_offset = yOffsetSector56_;
    half_cr_angle = halfCrossingAngleSector56_;
  }
  
  // transport the proton into each pot
  for (const auto& rp : pots_)
  {
    // first check the arm
    if (rp.detid.arm() != arm)
      continue;

    // calculate kinematics for optics parametrisation
    const double p0 = rp.approximator->GetBeamMomentum();
    const double p = mom_lhc.rho();
    const double xi = 1. - p / p0;
    const double th_x_phys = mom_lhc.x() / p;
    const double th_y_phys = mom_lhc.y() / p;
    const double vtx_lhc_eff_x = vtx_lhc.x() - vtx_lhc.z() * (mom_lhc.x() / mom_lhc.z() + half_cr_angle);
    const double vtx_lhc_eff_y = vtx_lhc.y() - vtx_lhc.z() * (mom_lhc.y() / mom_lhc.z());

    double kin_tr_in[5] = {
      vtx_lhc_eff_x * 1E-3, // conversion mm -> m
      (th_x_phys + half_cr_angle) * (1.-xi),
      vtx_lhc_eff_y * 1E-3 + vtx_y_offset,
      th_y_phys * (1.-xi),
      -xi
    };

    if (verbosity_)
    {
      const unsigned int rpDecId = rp.detid.arm()*100 + rp.detid.station()*10 + rp.detid.rp();
      printf("simu: RP=%u, xi=%.4f, th_x=%.3E, %.3E, vtx_lhc_eff_x=%.3E, vtx_lhc_eff_y=%.3E\n", rpDecId,
        xi, th_x_phys, th_y_phys, vtx_lhc_eff_x, vtx_lhc_eff_y);
    }

    // transport proton
    double kin_tr_out[5];
    bool proton_transported = rp.approximator->Transport(kin_tr_in, kin_tr_out, checkApertures_, invertBeamCoordinatesSystem_);
    const double b_x_tr = kin_tr_out[0], b_y_tr = kin_tr_out[2];
    const double a_x_tr = kin_tr_out[1]/(1.-xi), a_y_tr = kin_tr_out[3]/(1.-xi);

    // determine beam position
    double kin_be_in[5] = { 0., half_cr_angle, vtx_y_offset, 0., 0. };
    double kin_be_out[5];
    rp.approximator->Transport(kin_be_in, kin_be_out, false, invertBeamCoordinatesSystem_);
    const double b_x_be = kin_be_out[0], b_y_be = kin_be_out[2];
    const double a_x_be = kin_be_out[1], a_y_be = kin_be_out[3];

    // stop if proton not transported
    if (!proton_transported)
      continue;

    // track parameters in the chosen reference frame, in rad and mm
    double a_x = a_x_tr, a_y = a_y_tr;
    double b_x = b_x_tr * 1E3, b_y = b_y_tr * 1E3;
    if (produceHitsRelativeToBeam_)
    {
      a_x -= a_x_be; a_y -= a_y_be;
      b_x -= b_x_be * 1E3; b_y -= b_y_be * 1E3;
    }

    // get z position of the approximator scoring plane
    const double approximator_z = rp.z_position * 1E3; // in mm

    if (verbosity_)
    {
      printf("    proton transported: a_x = %.3E rad, a_y = %.3E rad, b_x = %.3f mm, b_y = %.3f mm, z = %.3f mm\n",
        a_x, a_y, b_x, b_y, approximator_z);
    }

    // check whether within the x_max limit
    if (b_x > rp.x_max)
      continue;
  
    // save scoring plane hit
    if (produceScoringPlaneHits_)
        out_tracks.emplace_back(rp.detid, b_x, 0., b_y, 0.);

    // stop if rec hits are not to be produced
    if (!produceRecHits_)
      continue;

    // loop over all sensors in the RP
    for (const auto& detIdInt : geometry.getSensorsInRP(rp.detid))
    {
      CTPPSDetId detId(detIdInt);

      // determine the track impact point (in global coordinates)
      // !! this assumes that local axes (1, 0, 0) and (0, 1, 0) describe the sensor surface
      CLHEP::Hep3Vector gl_o = geometry.localToGlobal(detId, CLHEP::Hep3Vector(0, 0, 0));
      CLHEP::Hep3Vector gl_a1 = geometry.localToGlobal(detId, CLHEP::Hep3Vector(1, 0, 0)) - gl_o;
      CLHEP::Hep3Vector gl_a2 = geometry.localToGlobal(detId, CLHEP::Hep3Vector(0, 1, 0)) - gl_o;

      TMatrixD A(3, 3);
      TVectorD B(3);
      A(0, 0) = a_x;    A(0, 1) = -gl_a1.x(); A(0, 2) = -gl_a2.x(); B(0) = gl_o.x() - b_x;
      A(1, 0) = a_y;    A(1, 1) = -gl_a1.y(); A(1, 2) = -gl_a2.y(); B(1) = gl_o.y() - b_y;
      A(2, 0) = z_sign; A(2, 1) = -gl_a1.z(); A(2, 2) = -gl_a2.z(); B(2) = gl_o.z() - approximator_z;
      TMatrixD Ai(3, 3);
      Ai = A.Invert();
      TVectorD P(3);
      P = Ai * B;

      double ze = P(0);
      CLHEP::Hep3Vector h_glo(a_x * ze + b_x, a_y * ze + b_y, z_sign*ze + approximator_z);

      // hit in local coordinates
      CLHEP::Hep3Vector h_loc = geometry.globalToLocal(detId, h_glo);

      if (verbosity_)
      {
        printf("\n");
        printf("    de z = %f mm, p1 = %f mm, p2 = %f mm\n", P(0), P(1), P(2));
        printf("    h_glo: x = %f mm, y = %f mm, z = %f mm\n", h_glo.x(), h_glo.y(), h_glo.z());
        printf("    h_loc: c1 = %f mm, c2 = %f mm, c3 = %f mm\n", h_loc.x(), h_loc.y(), h_loc.z());
      }

      // strips
      if (detId.subdetId() == CTPPSDetId::sdTrackingStrip)
      {
        double u = h_loc.x();
        double v = h_loc.y();

        if (verbosity_ > 5)
          printf("            u=%+8.4f, v=%+8.4f", u, v);

        // is it within detector?
        if (checkIsHit_  && !RPTopology::IsHit(u, v, insensitiveMarginStrips_))
        {
          if (verbosity_ > 5)
            printf(" | no hit\n");
          continue;
        } 

        // round the measurement
        if (roundToPitch_)
        {
          double m = stripZeroPosition_ - v;
          signed int strip = (int) floor(m / pitchStrips_ + 0.5);

          v = stripZeroPosition_ - pitchStrips_ * strip;

          if (verbosity_ > 5)
            printf(" | strip=%+4i", strip);
        }

        double sigma = pitchStrips_ / sqrt(12.);

        if (verbosity_ > 5)
          printf(" | m=%+8.4f, sigma=%+8.4f\n", v, sigma);

        edm::DetSet<TotemRPRecHit> &hits = out_strip_hits.find_or_insert(detId);
        hits.push_back(TotemRPRecHit(v, sigma));
      }

      // diamonds
      if (detId.subdetId() == CTPPSDetId::sdTimingDiamond)
      {
        // TODO: proper implementation
        /*
        if (checkIsHit_)
        {
          if (verbosity_ > 5)
            printf(" | no hit\n");
          continue;
        } 
        if (roundToPitch_)
        {
          h_loc.setX( pitchDiamonds * floor(h_loc.x()/pitchDiamonds + 0.5) );
        }

        if (verbosity_ > 5)
          printf("            m = %.3f\n", h_loc.x());

        const double width = pitchDiamonds;

        DetSet<CTPPSDiamondRecHit> &hits = diamondHitColl->find_or_insert(detId);
        HPTDCErrorFlags flags;
        hits.push_back(CTPPSDiamondRecHit(h_loc.x(), width, 0., 0., 0., 0., 0., 0., 0., 0, flags, false));
        */
      }

      // pixels
      if (detId.subdetId() == CTPPSDetId::sdTrackingPixel)
      {
        if (verbosity_)
        {
          CTPPSPixelDetId pixelDetId(detIdInt);
          printf("    pixel plane %u: local hit x = %.3f mm, y = %.3f mm, z = %.1E mm\n", pixelDetId.plane(), h_loc.x(), h_loc.y(), h_loc.z());
        }

        if (checkIsHit_  && !isPixelHit(h_loc.x(), h_loc.y()))
          continue;

        if (roundToPitch_)
        {
          h_loc.setX( pitchPixelsHor_ * floor(h_loc.x()/pitchPixelsHor_ + 0.5) );
          h_loc.setY( pitchPixelsVer_ * floor(h_loc.y()/pitchPixelsVer_ + 0.5) );
        }

        if (verbosity_ > 5)
          printf("            hit accepted: m1 = %.3f mm, m2 = %.3f mm\n", h_loc.x(), h_loc.y());

        const double sigmaHor = pitchPixelsHor_ / sqrt(12.);
        const double sigmaVer = pitchPixelsVer_ / sqrt(12.);

        const LocalPoint lp(h_loc.x(), h_loc.y(), h_loc.z());
        const LocalError le(sigmaHor, 0., sigmaVer);

        edm::DetSet<CTPPSPixelRecHit> &hits = out_pixel_hits.find_or_insert(detId);
        hits.push_back(CTPPSPixelRecHit(lp, le));
      } 
    }
  }
}

//----------------------------------------------------------------------------------------------------

void
CTPPSFastProtonSimulation::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSFastProtonSimulation );
