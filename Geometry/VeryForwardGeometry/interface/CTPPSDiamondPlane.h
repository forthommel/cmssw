/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondPlane_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondPlane_h

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

class CTPPSDiamondChannel;
class CTPPSDiamondPlane : public GeomDet
{
  public:
    typedef std::vector<const CTPPSDiamondChannel*> ChannelRefs;

  public:
    CTPPSDiamondPlane( const BoundPlane::BoundPlanePointer bp, CTPPSDiamondDetId detid ) :
      GeomDet( bp ) {
      setDetId( detid );
    }
    ~CTPPSDiamondPlane();

    void add( CTPPSDiamondChannel* );

    const CTPPSDiamondDetId id() const { return CTPPSDiamondDetId( geographicalId().rawId() ); }

    const ChannelRefs& channels() const { return ChannelRefs( channels_.begin(), channels_.end() ); }
    const CTPPSDiamondChannel* channel( CTPPSDiamondDetId ) const;
    const CTPPSDiamondChannel* channel( int ) const;

  private:
    ChannelRefs channels_;
};

#endif
