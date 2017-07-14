/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors: 
*  Jan Kaspar (jan.kaspar@gmail.com) 
*
****************************************************************************/

#include "Geometry/VeryForwardGeometryBuilder/interface/DDDTotemRPConstruction.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

// this might be useful one day
//.#include "Geometry/TrackerNumberingBuilder/interface/ExtractStringFromDDD.h"
//.#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerBuilder.h"
//.#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerDetIdBuilder.h"

#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------------------------

DDDTotemRPContruction::DDDTotemRPContruction()
{
}

//----------------------------------------------------------------------------------------------------

const DetGeomDesc* DDDTotemRPContruction::construct(const DDCompactView* cpv)
{
  // create DDFilteredView and apply the filter
  DDPassAllFilter filter;
  DDFilteredView fv(*cpv, filter);

  // conversion to DetGeomDesc structure
  // create the root node and recursively propagates through the tree
  // adds IDs
  DetGeomDesc* tracker = new DetGeomDesc(&fv);
  buildDetGeomDesc(&fv, tracker);

  // return the root of the structure
  return tracker;
}

//----------------------------------------------------------------------------------------------------

void DDDTotemRPContruction::buildDetGeomDesc(DDFilteredView *fv, DetGeomDesc *gd)
{
  // try to dive into next level
  if (! fv->firstChild())
    return;

  // loop over siblings in the level
  do {
    // create new DetGeomDesc node and add it to the parent's (gd) list
    DetGeomDesc* newGD = new DetGeomDesc(fv);

    // strip sensors
    if (fv->logicalPart().name().name().compare(DDD_TOTEM_RP_SENSOR_NAME) == 0)
    {
      const vector<int> &cN = fv->copyNumbers();
      // check size of copy numubers array
      if (cN.size() < 3)
        throw cms::Exception("DDDTotemRPContruction") << "size of copyNumbers for strip sensor is "
          << cN.size() << ". It must be >= 3." << endl;

      // extract information
      const unsigned int decRPId = cN[cN.size() - 3];
      const unsigned int arm = decRPId / 100;
      const unsigned int station = (decRPId % 100) / 10;
      const unsigned int rp = decRPId % 10;
      const unsigned int detector = cN[cN.size() - 1];
      newGD->setGeographicalID(TotemRPDetId(arm, station, rp, detector));
    }

    // strip RPs
    if (fv->logicalPart().name().name().compare(DDD_TOTEM_RP_RP_NAME) == 0)
    {
      const unsigned int decRPId = fv->copyno();

      // check it is a strip RP
      if (decRPId < 10000)
      {
        const unsigned int armIdx = (decRPId / 100) % 10;
        const unsigned int stIdx = (decRPId / 10) % 10;
        const unsigned int rpIdx = decRPId % 10;
        newGD->setGeographicalID(TotemRPDetId(armIdx, stIdx, rpIdx));
      }
    }

    // pixel sensors
    if (fv->logicalPart().name().name().compare(DDD_CTPPS_PIXELS_SENSOR_NAME) == 0)
    {
      const vector<int> &cN = fv->copyNumbers();
      // check size of copy numubers array
      if (cN.size() < 4)
        throw cms::Exception("DDDTotemRPContruction") << "size of copyNumbers for pixel sensor is "
          << cN.size() << ". It must be >= 4." << endl;

      // extract information
      const unsigned int decRPId = cN[cN.size() - 4] % 10000;
      const unsigned int arm = decRPId / 100;
      const unsigned int station = (decRPId % 100) / 10;
      const unsigned int rp = decRPId % 10;
      const unsigned int detector = cN[cN.size() - 2] - 1;
      newGD->setGeographicalID(CTPPSPixelDetId(arm, station, rp, detector));
    }

    // pixel RPs
    if (fv->logicalPart().name().name().compare(DDD_CTPPS_PIXELS_RP_NAME) == 0)
    {
      uint32_t decRPId = fv->copyno();
    
      // check it is a pixel RP
      if (decRPId >= 10000)
      {
        decRPId = decRPId % 10000;
        const uint32_t armIdx = (decRPId / 100) % 10;
        const uint32_t stIdx = (decRPId / 10) % 10;
        const uint32_t rpIdx = decRPId % 10;
        newGD->setGeographicalID(CTPPSPixelDetId(armIdx, stIdx, rpIdx));
      }
    }

    // diamond sensors
    if (fv->logicalPart().name().name().compare(DDD_CTPPS_DIAMONDS_SEGMENT_NAME) == 0)
    {
      const vector<int>& copy_num = fv->copyNumbers();

      const unsigned int id = copy_num[copy_num.size()-1],
                         arm = copy_num[1]-1,
                         station = 1,
                         rp = 6,
                         plane = ( id / 100 ),
                         channel = id % 100;
      newGD->setGeographicalID( CTPPSDiamondDetId( arm, station, rp, plane, channel ) );
    }

    // diamond RPs
    if (fv->logicalPart().name().name().compare(DDD_CTPPS_DIAMONDS_RP_NAME) == 0)
    {
      const vector<int>& copy_num = fv->copyNumbers();

      // check size of copy numubers array
      if (copy_num.size() < 2)
        throw cms::Exception("DDDTotemRPContruction") << "size of copyNumbers for diamond RP is "
          << copy_num.size() << ". It must be >= 2." << endl;

      const unsigned int arm = copy_num[1] - 1;
      const unsigned int station = 1;
      const unsigned int rp = 6;

      newGD->setGeographicalID(CTPPSDiamondDetId(arm, station, rp));
    }

    // add component
    gd->addComponent(newGD);

    // recursion
    buildDetGeomDesc(fv, newGD);
  } while (fv->nextSibling());

  // go a level up
  fv->parent();
}
