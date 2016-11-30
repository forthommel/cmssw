/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondChannel_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondChannel_h

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

class CTPPSDiamondPlane;
class CTPPSDiamondChannel : public GeomDetUnit
{
  public:
    CTPPSDiamondChannel( const BoundPlane::BoundPlanePointer bp, CTPPSDiamondDetId detid ) :
      GeomDet( bp ) {
      setDetId( detid );
    }
    ~CTPPSDiamondChannel() {}

    const CTPPSDiamondDetId id() const { return CTPPSDiamondDetId( geographicalId().rawId() ); }

    /// Plane this pixel belongs to
    const CTPPSDiamondPlane* plane() const { return plane_; }

  private:
    /// Pointer to parent plane
    const CTPPSDiamondPlane* plane_; // NOT owned
};

#endif
