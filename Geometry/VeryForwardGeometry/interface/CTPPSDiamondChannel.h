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
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

class CTPPSDiamondPlane;
class CTPPSDiamondChannel : public GeomDet
{
  public:
    CTPPSDiamondChannel( const BoundPlane::BoundPlanePointer bp, CTPPSDiamondDetId detid ) :
      GeomDet( bp ) {
      setDetId( detid );
    }
    ~CTPPSDiamondChannel()

    const CTPPSDiamondDetId id() const { return CTPPSDiamondDetId( geographicalId().rawId() ); }

    /// Plane this pixel belongs to
    const CTPPSDiamondPlane* plane() const { return plane_; }
    const Topology& topology() const { return ch_spec_->topology(); }
    const PixelTopology& specificTopology() const { return ch_spec_->specificTopology(); }

  private:
    /// Pointer to parent plane
    const CTPPSDiamondPlane* plane_; // NOT owned
};

#endif
