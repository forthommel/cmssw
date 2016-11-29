/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondChannelSpecs_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondChannelSpecs_h

#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"

class CTPPSDiamondChannelSpecs : public GeomDetType
{
  public:
    CTPPSDiamondChannelSpecs( SubDetector, const std::string& );
    ~CTPPSDiamondChannelSpecs();

    const Topology& topology() const { return ( *px_topo_ ); }
    const PixelTopology& specificTopology() const { return ( *px_topo_ ); }

  private:
    PixelTopology* px_topo_;
};

#endif
