/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include <set>

/**
 * FIXME make use of the general reco::Candidate object, with appropriate set'ters and get'ters
 */

using namespace reco;

ProtonTrack::ProtonTrack() :
  xi_( 0. ), xi_unc_( 0. )
{}

ProtonTrack::ProtonTrack( double chi2, double ndof, const Point& vtx, const Vector& dir, float xi, float xi_unc ) :
  Track( chi2, ndof, vtx, dir, +1, CovarianceMatrix()/*FIXME*/ ),
  xi_( xi ), xi_unc_( xi_unc )
{}

