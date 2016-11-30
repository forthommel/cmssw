/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometryBuilder_CTPPSGeometryBuilder_h
#define Geometry_VeryForwardGeometryBuilder_CTPPSGeometryBuilder_h

#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondGeometry.h"

class DDCompactView;
class CTPPSGeometryBuilder
{
  public:
    CTPPSGeometryBuilder();
    ~CTPPSGeometryBuilder();

    CTPPSDiamondGeometry* buildDiamonds( const DDCompactView* );

  private:
    CTPPSDiamondGeometry* parseDiamonds( DDFilteredView& );

};

#endif
