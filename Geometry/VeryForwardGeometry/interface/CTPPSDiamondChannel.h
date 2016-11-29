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

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondChannelSpecs.h"

class CTPPSDiamondPlane;
class CTPPSDiamondChannel : public GeomDetUnit
{
  public:
    CTPPSDiamondChannel( const BoundPlane::BoundPlanePointer bp, CTPPSDiamondDetId detid, CTPPSDiamondChannelSpecs* specs ) :
      GeomDet( bp ), ch_spec_( specs ) {
      setDetId( detid );
    }
    ~CTPPSDiamondChannel() {}

    const Topology& topology() const { return ch_spec_->topology(); }
    const PixelTopology& specificTopology() const { return ch_spec_->specificTopology(); }
    const GeomDetType& type() const { return ( *ch_spec_ ); }
    
    const CTPPSDiamondDetId id() const { return CTPPSDiamondDetId( geographicalId().rawId() ); }

    /// Plane this pixel belongs to
    const CTPPSDiamondPlane* plane() const { return plane_; }

  private:
    CTPPSDiamondChannelSpecs* ch_spec_;

    /// Pointer to parent plane
    const CTPPSDiamondPlane* plane_; // NOT owned
};

#endif
