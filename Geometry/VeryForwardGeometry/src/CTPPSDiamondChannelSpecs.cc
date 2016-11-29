/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondChannelSpecs.h"

CTPPSDiamondChannelSpecs::CTPPSDiamondChannelSpecs( SubDetector subdet, const std::string& name ) :
  GeomDetType( name, subdet )
{
  px_topo_ = new RectangularPixelTopology(  );
}

CTPPSDiamondChannelSpecs::~CTPPSDiamondChannelSpecs()
{
  if ( px_topo_ ) delete px_topo_;
}
