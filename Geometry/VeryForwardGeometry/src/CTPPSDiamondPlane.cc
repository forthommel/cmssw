/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondPlane.h"

CTPPSDiamondPlane::~CTPPSDiamondPlane()
{};

void
CTPPSDiamondPlane::add( CTPPSDiamondChannel* channel )
{
  channels_.push_back( channel );
}

const GeomDet*
CTPPSDiamondPlane::component( DetId id ) const
{
  return channel( CTPPSDiamondDetId( id.rawId() ) );
}

const CTPPSDiamondChannel*
CTPPSDiamondPlane::channel( CTPPSDiamondDetId detid )
{
  if ( detid.plane() != detid() ) return 0;
  return channel( detid.channel() );
}

const CTPPSDiamondChannel*
CTPPSDiamondPlane::channel( int id )
{
  for ( ChannelRefs::const_iterator it = channels_.begin(); it != channels_.end(); ++it ) {
    const CTPPSDiamondChannel* channel = *it;
    if ( channel->id().plane() == id ) return channel;
  }
  return 0;
}
