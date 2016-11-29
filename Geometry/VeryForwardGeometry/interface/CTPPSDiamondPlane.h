/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondPlane_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondPlane_h

#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondChannel.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

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

    const CTPPSDiamondDetId id() const { return CTPPSDiamondDetId( geographicalId().rawId() ); }

    virtual std::vector<const GeomDet*> components() const { return std::vector<const GeomDet*>( channels_.begin(), channels_.end() ); }
    virtual const GeomDet* component( DetId detid ) const { return channel( CTPPSDiamondDetId( detid.rawId() ) ); }

    void add( CTPPSDiamondChannel* );

    const ChannelRefs& channels() const { return channels_; }
    const CTPPSDiamondChannel* channel( CTPPSDiamondDetId ) const;
    const CTPPSDiamondChannel* channel( int ) const;

  private:
    ChannelRefs channels_;
};

#endif
