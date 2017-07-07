#ifndef Validation_CTPPS_ProtonReconstruction_h
#define Validation_CTPPS_ProtonReconstruction_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSpline.h"

namespace ctpps
{
  class ProtonReconstruction
  {
    public:
      ProtonReconstruction( const edm::ParameterSet& iConfig ) :
        si_x_alignment_( 150.e-6 ), si_x_neglected_angle_( 150.e-6 ), si_rel_D_( 0.055 )
      {
        const std::string filename = iConfig.getParameter<edm::FileInPath>( "filename" ).fullPath();
        auto file = std::make_unique<TFile>( filename.c_str() );
        for ( const auto& pot : iConfig.getParameter< std::vector<edm::ParameterSet> >( "interpolationCurves" ) ) {
          splines_[pot.getParameter<unsigned int>( "potId" )] = std::make_shared<TSpline>( dynamic_cast<TSpline*>( file->Get( iConfig.getParameter<std::string>( "splineName" ).c_str() )->Clone() ) );
        }
      }

      void reconstruct( const unsigned int& potid, double x, double& xi, double& xi_err ) const {
        xi = xi_err = 0.;

        //----- retrieve the spline associated to this pot
        if ( splines_.count( potid )==0 ) return;
        const auto& spline = splines_.at( potid ).get();

        //----- first compute xi(x)
        xi = spline->Eval( x );

        //----- then determine uncertainty on xi
        const double si_x = sqrt( si_x_alignment_*si_x_alignment_ + si_x_neglected_angle_*si_x_neglected_angle_ );
        const double si_xi_from_x = spline->Eval( x+si_x )-xi;
        const double si_xi_from_D_x = si_rel_D_ * xi;
        xi_err = sqrt( si_xi_from_x*si_xi_from_x + si_xi_from_D_x*si_xi_from_D_x );
      }

    private:
      std::map<unsigned int,std::shared_ptr<TSpline> > splines_;
      const double si_x_alignment_; // in m, alignment uncertainty
      const double si_x_neglected_angle_; // in m, to (approximately) account for the neglected angular term in proton transport
      const double si_rel_D_; // 1, relative uncertainty of dispersion
  };
}

#endif
