/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondTopology.h"

CTPPSDiamondTopology::CTPPSDiamondTopology( const CTPPSDiamondTopology::DiamondsDimensions& dim ) :
  dim_( dim )
{
  std::ostringstream os;
  for ( unsigned int i = 0; i < dim_.size(); ++i ) {
    if ( i > 0 ) os << std::endl;
    std::pair<float, std::vector<float> > y_elem = dim_.at( i );
    os << "* row " << i << " has vertical size: " << y_elem.first << std::endl;
    for ( unsigned int j = 0; j < y_elem.second.size(); ++j ) {
      os << std::endl << "\t* roc " << j << " has horizontal size: " << y_elem.second.at( j );
    }    
  }
  LogDebug("CTPPSDiamondTopology") << os.str();
}

LocalPoint
CTPPSDiamondTopology::localPosition( const MeasurementPoint& point ) const
{
  const float x = point.x(),
              y = point.y();

  float x_shift = 0.,
        y_shift = 0.;

  for ( CTPPSDiamondTopology::DiamondsDimensions::const_iterator it_y = dim_.begin(); it_y != dim_.end(); ++it_y ) {
    const float y_size = it_y->first;
    if ( y > y_size ) {
      y_shift += y_size;
      continue;
    }

    // at this stage we found the corresponding y-row
    const std::vector<float> x_sizes_at_y = it_y->second;

    for ( std::vector<float>::const_iterator it_x = x_sizes_at_y.begin(); it_x != x_sizes_at_y.end(); ++it_x ) {
      const float x_size = *it_x;
      if ( x > x_size ) {
        x_shift += x_size;
        continue;
      }
      // we found the corresponding x-row
      return LocalPoint( x-x_shift, y-y_shift );
    }
  }

  // if point coordinates exceeding the dimensions of the plane, return the global position
  return LocalPoint( x, y );
}

LocalError
CTPPSDiamondTopology::localError( const MeasurementPoint& point, const MeasurementError& error ) const
{
  return LocalError(); //FIXME
}

MeasurementPoint
CTPPSDiamondTopology::measurementPosition( const LocalPoint& ) const
{
  return MeasurementPoint(); //FIXME
}

MeasurementError
CTPPSDiamondTopology::measurementError( const LocalPoint&, const LocalError& ) const
{
  return MeasurementError(); //FIXME
}

int
CTPPSDiamondTopology::channel( const LocalPoint& ) const
{
  return 0;
}

