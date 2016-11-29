/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometryBuilder_CTPPSGeometryBuilder_h
#define Geometry_VeryForwardGeometryBuilder_CTPPSGeometryBuilder_h

#include "DetectorDescription/Core/interface/DDSolidShapes.h"

class CTPPSGeometryBuilder
{
  public:
    CTPPSGeometryBuilder();
    ~CTPPSGeometryBuilder();

    const DetGeomDesc* construct( const DDCompactView* );
};

#endif
