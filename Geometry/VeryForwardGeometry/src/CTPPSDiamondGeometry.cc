/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondGeometry.h"

//---------------------------------------------------------------------------

CTPPSDiamondGeometry::CTPPSDiamondGeometry()
{}

CTPPSDiamondGeometry::~CTPPSDiamondGeometry()
{}

//---------------------------------------------------------------------------

void
CTPPSDiamondGeometry::add( CTPPSDiamondPlane* plane )
{
  all_planes_.push_back( plane );
  dets_.push_back( plane );
  dets_ids_.push_back( plane->geographicalId() );
  mapping_.insert( std::pair<DetId,GeomDet*>( plane->geographicalId(), plane ) );
}

const CTPPSDiamondPlane*
CTPPSDiamondGeometry::plane( const CTPPSDiamondDetId& id ) const
{
  return dynamic_cast<const CTPPSDiamondPlane*>( idToDet( id.plane() ) );
}

//---------------------------------------------------------------------------

void
CTPPSDiamondGeometry::add( CTPPSDiamondChannel* channel )
{
  all_channels_.push_back( channel );
  dets_.push_back( channel );
  dets_ids_.push_back( channel->geographicalId() );
  channels_.push_back( channel );
  channels_ids_.push_back( channel->geographicalId() );
  channels_types_.push_back( const_cast<GeomDetType*>( &( channel->type() ) ) );
  mapping_.insert( std::pair<DetId,GeomDetUnit*>( channel->geographicalId(), channel ) );
}

const CTPPSDiamondChannel*
CTPPSDiamondGeometry::channel( const CTPPSDiamondDetId& id ) const
{
  return dynamic_cast<const CTPPSDiamondChannel*>( idToDet( id.channel() ) );
}

//---------------------------------------------------------------------------

const CTPPSDiamondGeometry::DetTypeContainer&
CTPPSDiamondGeometry::detTypes() const
{
  return channels_types_;
}

const CTPPSDiamondGeometry::DetUnitContainer&
CTPPSDiamondGeometry::detUnits() const
{
  return channels_;
}

const CTPPSDiamondGeometry::DetContainer&
CTPPSDiamondGeometry::dets() const
{
  return dets_;
}

const CTPPSDiamondGeometry::DetIdContainer&
CTPPSDiamondGeometry::detUnitIds() const
{
  return channels_ids_;
}

const CTPPSDiamondGeometry::DetIdContainer&
CTPPSDiamondGeometry::detIds() const
{
  return dets_ids_;
}

const GeomDetUnit*
CTPPSDiamondGeometry::idToDetUnit( DetId id ) const
{
  return dynamic_cast<const GeomDetUnit*>( idToDet( id ) );
}

const GeomDet*
CTPPSDiamondGeometry::idToDet( DetId id ) const
{
  mapIdToDet::const_iterator i = mapping_.find( id );
  if ( i != mapping_.end() )
    return i->second;

  LogDebug("CTPPSDiamondGeometry") << "Invalid DetID: no associated GeomDet found for " << id;
  return 0;
}
