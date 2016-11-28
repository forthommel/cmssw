/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondGeometry_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondGeometry_h

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondPlane.h"

#include <map>

//----------------------------------------------------------------------------------------------------

class CTPPSDiamondGeometry : public TrackingGeometry
{
  public:
    CTPPSDiamondGeometry() {}
    virtual ~CTPPSDiamondGeometry() {}

    const GeomDet* idToDet( DetId ) const override;

    const CTPPSDiamondPlane* plane( const CTPPSDiamondDetId& );
    void add( const CTPPSDiamondPlane* );

  private:

    mapIdToDet mapping_;
};

#endif
