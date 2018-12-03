/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithmOFDB_h
#define RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithmOFDB_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "CondFormats/CTPPSOpticsObjects/interface/LHCOpticsApproximatorOF.h"

#include "TFile.h"
#include "TSpline.h"
#include "Fit/Fitter.h"
#include "TH1D.h"

#include <map>
#include <string>

//----------------------------------------------------------------------------------------------------

class ProtonReconstructionAlgorithmOFDB
{
  public:
    ProtonReconstructionAlgorithmOFDB(double _xangle1, const std::string &optics_file_1, double _xangle2, const std::string &optics_file_2,
      const edm::ParameterSet &beam_conditions, const std::vector<edm::ParameterSet> &detector_packages, bool _fit_vtx_y, unsigned int _verbosity);

    ~ProtonReconstructionAlgorithmOFDB();

    /// runs proton reconstruction using single-RP strategy
    void reconstructFromSingleRP(const std::vector<const CTPPSLocalTrackLite*> &input, std::vector<reco::ProtonTrack> &output) const;

    /// runs proton reconstruction using multiple-RP strategy
    void reconstructFromMultiRP(const std::vector<const CTPPSLocalTrackLite*> &input, std::vector<reco::ProtonTrack> &output) const;

    void Init(double xangle);

    void Release();

  private:
    unsigned int verbosity;
    bool initialized;

    edm::ParameterSet beamConditions_;
    double halfCrossingAngleSector45_, halfCrossingAngleSector56_;
    double yOffsetSector45_, yOffsetSector56_;

    double xangle1, xangle2;
    std::string opticsFile1, opticsFile2;

    bool fitVtxY_;
  
    std::map<unsigned int, std::string> idNameMap;

    /// optics data associated with 1 RP
    struct RPOpticsData
    {
      LHCOpticsApproximatorOF *optics;
      TSpline3 *s_xi_vs_x, *s_y0_vs_xi, *s_v_y_vs_xi, *s_L_y_vs_xi;
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

    /// validation plots
    //std::map<unsigned int, std::pair<TH1D*, TH1D*>> m_rp_de_xy;
};

#endif
