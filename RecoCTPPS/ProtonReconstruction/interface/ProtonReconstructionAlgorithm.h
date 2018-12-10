/****************************************************************************
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 ****************************************************************************/

#ifndef RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithm_h
#define RecoCTPPS_ProtonReconstruction_ProtonReconstructionAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLiteFwd.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/CTPPSReadoutObjects/interface/LHCOpticalFunctionsSet.h"

#include "TSpline.h"
#include "Fit/Fitter.h"

#include <unordered_map>

//----------------------------------------------------------------------------------------------------

class ProtonReconstructionAlgorithm
{
  public:
    ProtonReconstructionAlgorithm(bool fit_vtx_y, unsigned int verbosity);
    ~ProtonReconstructionAlgorithm() = default;

    typedef std::vector<CTPPSLocalTrackLiteRef> TracksCollection;

    /// runs proton reconstruction using single-RP strategy
    void reconstructFromSingleRP(const TracksCollection& input, std::vector<reco::ProtonTrack> &output, const LHCInfo &lhcInfo) const;

    /// runs proton reconstruction using multiple-RP strategy
    void reconstructFromMultiRP(const TracksCollection& input, std::vector<reco::ProtonTrack> &output, const LHCInfo &lhcInfo) const;

    void init(const std::unordered_map<unsigned int, LHCOpticalFunctionsSet> &opticalFunctions);

    void release();

  private:
    unsigned int verbosity_;

    bool fitVtxY_;

    bool initialized_;

    /// optics data associated with 1 RP
    struct RPOpticsData
    {
      const LHCOpticalFunctionsSet *optics;
      std::shared_ptr<TSpline3> s_xi_vs_x_d, s_y_d_vs_xi, s_v_y_vs_xi, s_L_y_vs_xi;
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
        ChiSquareCalculator() {}

        double operator() (const double *parameters) const;

        const TracksCollection* tracks_;
        const std::map<unsigned int, RPOpticsData> *m_rp_optics_;
    };

    /// fitter object
    std::unique_ptr<ROOT::Fit::Fitter> fitter_;

    /// object to calculate chi^2
    std::unique_ptr<ChiSquareCalculator> chiSquareCalculator_;
};

#endif
