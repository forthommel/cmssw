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

const CTPPSDiamondChannel*
CTPPSDiamondPlane::channel( CTPPSDiamondDetId detid ) const
{
  if ( detid.plane() != detid() ) return 0;
  return channel( detid.channel() );
}

const CTPPSDiamondChannel*
CTPPSDiamondPlane::channel( int id ) const
{
  for ( ChannelRefs::const_iterator it = channels_.begin(); it != channels_.end(); ++it ) {
    const CTPPSDiamondChannel* channel = *it;
    if ( channel->id().plane() == static_cast<unsigned int>( id ) ) return channel;
  }
  return 0;
}
