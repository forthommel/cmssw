/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithm_h
#define RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrackFwd.h"
#include "DataFormats/ProtonReco/interface/ProtonTrackExtraFwd.h"

#include "CondFormats/CTPPSOpticsObjects/interface/LHCOpticsApproximator.h"

#include "TFile.h"
#include "TSpline.h"
#include "Fit/Fitter.h"

#include <map>
#include <string>

//----------------------------------------------------------------------------------------------------

class ProtonReconstructionAlgorithm
{
  public:
    ProtonReconstructionAlgorithm(const std::string &optics_file_beam1, const std::string &optics_file_beam2,
      const edm::ParameterSet &beam_conditions, const std::vector<edm::ParameterSet> &detector_packages, unsigned int _verbosity);

    ~ProtonReconstructionAlgorithm();

    void clear();
    /// run proton reconstruction using single-RP strategy
    void reconstructFromSingleRP(const std::vector<const CTPPSLocalTrackLite*> &input, std::vector<reco::ProtonTrack> &output, reco::ProtonTrackExtraCollection& extras, reco::ProtonTrackExtraRefProd& r_extra) const;
    /// run proton reconstruction using multiple-RP strategy
    void reconstructFromMultiRP(const std::vector<const CTPPSLocalTrackLite*> &input, std::vector<reco::ProtonTrack> &output, reco::ProtonTrackExtraCollection& extras, reco::ProtonTrackExtraRefProd& r_extra) const;

  private:
    unsigned int verbosity;

    edm::ParameterSet beamConditions_;
    double halfCrossingAngleSector45_, halfCrossingAngleSector56_;
    double yOffsetSector45_, yOffsetSector56_;

    /// optics data associated with 1 RP
    struct RPOpticsData
    {
      std::shared_ptr<LHCOpticsApproximator> optics;
      std::shared_ptr<TSpline3> s_xi_vs_x, s_y0_vs_xi, s_v_y_vs_xi, s_L_y_vs_xi;
      double x0, y0;  // beam position, m
      double ch0, ch1;  // linear approximation (intercept and slope) of x(xi)
      double la0, la1;  // linear approximation (intercept and slope) of L_x(xi)
    };

    /// map: RP id --> optics data
    std::map<unsigned int, RPOpticsData> m_rp_optics_;

    /// class for calculation of chi^2
    class ChiSquareCalculator
    {
      public:
        ChiSquareCalculator( const edm::ParameterSet& bc) :
          beamConditions_( bc ),
          halfCrossingAngleSector45_( bc.getParameter<double>( "halfCrossingAngleSector45" ) ),
          halfCrossingAngleSector56_( bc.getParameter<double>( "halfCrossingAngleSector56" ) ),
          yOffsetSector45_( bc.getParameter<double>( "yOffsetSector45" ) ),
          yOffsetSector56_( bc.getParameter<double>( "yOffsetSector56" ) )
        {
        }

        double operator() (const double *parameters) const;

        const std::vector<const CTPPSLocalTrackLite*> *tracks;
        const std::map<unsigned int, RPOpticsData> *m_rp_optics;

      private:
        edm::ParameterSet beamConditions_;
        double halfCrossingAngleSector45_, halfCrossingAngleSector56_;
        double yOffsetSector45_, yOffsetSector56_;
    };

    /// fitter object
    std::unique_ptr<ROOT::Fit::Fitter> fitter_;

    /// object to calculate chi^2
    std::unique_ptr<ChiSquareCalculator> chiSquareCalculator_;
    mutable edm::Ref<reco::ProtonTrackExtraCollection>::key_type idx_pte_;
};

#endif
