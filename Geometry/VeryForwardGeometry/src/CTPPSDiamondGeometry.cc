/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondGeometry.h"

//----------------------------------------------------------------------------------------------------

/*CTPPSDiamondGeometry::CTPPSDiamondGeometry()
{}

CTPPSDiamondGeometry::~CTPPSDiamondGeometry()
{}*/

const CTPPSDiamondPixel*
CTPPSDiamondGeometry::pixel( const CTPPSDiamondDetId& id )
{
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

#endif
