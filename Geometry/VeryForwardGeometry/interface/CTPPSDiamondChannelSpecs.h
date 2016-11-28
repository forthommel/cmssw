/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondChannelSpecs_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondChannelSpecs_h

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"

class CTPPSDiamondChannelSpecs : public PixelGeomDetType
{
  public:
    CTPPSDiamondChannelSpecs( SubDetector, const std::string& );
    ~CTPPSDiamondChannelSpecs()
};

#endif
