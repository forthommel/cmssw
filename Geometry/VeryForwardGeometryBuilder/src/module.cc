/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors: 
*	Jan Kaspar (jan.kaspar@gmail.com) 
*	Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#include "FWCore/Utilities/interface/typelookup.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"

TYPELOOKUP_DATA_REG(TotemRPGeometry);
TYPELOOKUP_DATA_REG(CTPPSDiamondGeometry);
TYPELOOKUP_DATA_REG(DetGeomDesc);
