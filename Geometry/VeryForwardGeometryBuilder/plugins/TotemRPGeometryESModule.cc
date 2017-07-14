/****************************************************************************
*
* Authors:
*  Jan Kaspar (jan.kaspar@gmail.com) 
*  Dominik Mierzejewski <dmierzej@cern.ch>
*    
****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
 
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/graphwalker.h"
#include "DetectorDescription/Core/interface/DDMaterial.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDSpecifics.h"
#include "DetectorDescription/Core/interface/DDRotationMatrix.h"

// TODO: remove ?
/*
#include "DetectorDescription/Core/src/Material.h"
#include "DetectorDescription/Core/src/Solid.h"
#include "DetectorDescription/Core/src/LogicalPart.h"
#include "DetectorDescription/Core/src/Specific.h"
*/

#include "DataFormats/CTPPSAlignment/interface/RPAlignmentCorrectionsData.h"

#include "CondFormats/AlignmentRecord/interface/RPRealAlignmentRecord.h"
#include "CondFormats/AlignmentRecord/interface/RPMisalignedAlignmentRecord.h"

#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSDDDNames.h"

/**
 * \brief Builds ideal, real and misaligned geometries.
 *
 * First, it creates a tree of DetGeomDesc from DDCompView. For real and misaligned geometries,
 * it applies alignment corrections (RPAlignmentCorrections) found in corresponding ...GeometryRecord.
 *
 * Second, it creates CTPPSGeometry from DetGeoDesc tree.
 **/
class  TotemRPGeometryESModule : public edm::ESProducer
{
  public:
    TotemRPGeometryESModule(const edm::ParameterSet &p);
    virtual ~TotemRPGeometryESModule(); 

    std::unique_ptr<DetGeomDesc> produceIdealGD(const IdealGeometryRecord &);

    std::unique_ptr<DetGeomDesc> produceRealGD(const VeryForwardRealGeometryRecord &);
    std::unique_ptr<TotemRPGeometry> produceRealTG(const VeryForwardRealGeometryRecord &);

    std::unique_ptr<DetGeomDesc> produceMisalignedGD(const VeryForwardMisalignedGeometryRecord &);
    std::unique_ptr<TotemRPGeometry> produceMisalignedTG(const VeryForwardMisalignedGeometryRecord &);

  protected:
    unsigned int verbosity;

    static void applyAlignments(const edm::ESHandle<DetGeomDesc> &idealGD,
      const edm::ESHandle<RPAlignmentCorrectionsData> &alignments, DetGeomDesc* &newGD);

    static void buildDetGeomDesc(DDFilteredView *fv, DetGeomDesc *gd);
};


using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

TotemRPGeometryESModule::TotemRPGeometryESModule(const edm::ParameterSet &p)
{
  verbosity = p.getUntrackedParameter<unsigned int>("verbosity", 1);  

  setWhatProduced(this, &TotemRPGeometryESModule::produceIdealGD);

  setWhatProduced(this, &TotemRPGeometryESModule::produceRealGD);
  setWhatProduced(this, &TotemRPGeometryESModule::produceRealTG);

  setWhatProduced(this, &TotemRPGeometryESModule::produceMisalignedGD);
  setWhatProduced(this, &TotemRPGeometryESModule::produceMisalignedTG);
}

//----------------------------------------------------------------------------------------------------

TotemRPGeometryESModule::~TotemRPGeometryESModule()
{
}

//----------------------------------------------------------------------------------------------------

void TotemRPGeometryESModule::applyAlignments(const ESHandle<DetGeomDesc> &idealGD, 
    const ESHandle<RPAlignmentCorrectionsData> &alignments, DetGeomDesc* &newGD)
{
  newGD = new DetGeomDesc( *(idealGD.product()) );
  deque<const DetGeomDesc *> buffer;
  deque<DetGeomDesc *> bufferNew;
  buffer.push_back(idealGD.product());
  bufferNew.push_back(newGD);

  while (buffer.size() > 0)
  {
    const DetGeomDesc *sD = buffer.front();
    DetGeomDesc *pD = bufferNew.front();
    buffer.pop_front();
    bufferNew.pop_front();

    // Is it sensor? If yes, apply full sensor alignments
    if ( pD->name().name().compare(DDD_TOTEM_RP_SENSOR_NAME) == 0
      || pD->name().name().compare(DDD_CTPPS_DIAMONDS_SEGMENT_NAME) == 0
	  || pD->name().name().compare(DDD_CTPPS_PIXELS_SENSOR_NAME)==0 )
    {
      unsigned int plId = pD->geographicalID();

      if (alignments.isValid())
      {
        const RPAlignmentCorrectionData& ac = alignments->GetFullSensorCorrection(plId);
        pD->ApplyAlignment(ac);
      }
    }

    // Is it RP box? If yes, apply RP alignments
    if (pD->name().name().compare(DDD_TOTEM_RP_RP_NAME) == 0
        || pD->name().name().compare(DDD_CTPPS_PIXELS_RP_NAME) == 0
        || pD->name().name().compare(DDD_CTPPS_DIAMONDS_RP_NAME) == 0
      )
    {
      unsigned int rpId = pD->geographicalID();
      
      if (alignments.isValid())
      {
        const RPAlignmentCorrectionData& ac = alignments->GetRPCorrection(rpId);
        pD->ApplyAlignment(ac);
      }
    }

    // create and add children
    for (unsigned int i = 0; i < sD->components().size(); i++)
    {
      const DetGeomDesc *sDC = sD->components()[i];
      buffer.push_back(sDC);
    
      // create new node with the same information as in sDC and add it as a child of pD
      DetGeomDesc * cD = new DetGeomDesc(*sDC);
      pD->addComponent(cD);

      bufferNew.push_back(cD);
    }
  }
}

//----------------------------------------------------------------------------------------------------

void TotemRPGeometryESModule::buildDetGeomDesc(DDFilteredView *fv, DetGeomDesc *gd)
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

//----------------------------------------------------------------------------------------------------

std::unique_ptr<DetGeomDesc> TotemRPGeometryESModule::produceIdealGD(const IdealGeometryRecord &iRecord)
{
  // get the DDCompactView from EventSetup
  edm::ESHandle<DDCompactView> cpv;
  iRecord.get(cpv);

  // create DDFilteredView and apply the filter
  DDPassAllFilter filter;
  DDFilteredView fv(*(cpv.product()), filter);

  // conversion to DetGeomDesc structure
  DetGeomDesc* root = new DetGeomDesc(&fv);
  buildDetGeomDesc(&fv, root);
  
  // construct the tree of DetGeomDesc
  return std::unique_ptr<DetGeomDesc>( const_cast<DetGeomDesc*>(root) );
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<DetGeomDesc> TotemRPGeometryESModule::produceRealGD(const VeryForwardRealGeometryRecord &iRecord)
{
  // get the input GeometricalDet
  edm::ESHandle<DetGeomDesc> idealGD;
  iRecord.getRecord<IdealGeometryRecord>().get(idealGD);

  // load alignments
  edm::ESHandle<RPAlignmentCorrectionsData> alignments;
  try { iRecord.getRecord<RPRealAlignmentRecord>().get(alignments); }
  catch (...) {}

  if (alignments.isValid())
  {
    if (verbosity)
      LogVerbatim("TotemRPGeometryESModule::produceRealGD")
        << ">> TotemRPGeometryESModule::produceRealGD > Real geometry: "
        << alignments->GetRPMap().size() << " RP and "
        << alignments->GetSensorMap().size() << " sensor alignments applied.";
  } else {
    if (verbosity)
      LogVerbatim("TotemRPGeometryESModule::produceRealGD")
        << ">> TotemRPGeometryESModule::produceRealGD > Real geometry: No alignments applied.";
  }

  DetGeomDesc* newGD = NULL;
  applyAlignments(idealGD, alignments, newGD);
  return std::unique_ptr<DetGeomDesc>(newGD);
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<DetGeomDesc> TotemRPGeometryESModule::produceMisalignedGD(const VeryForwardMisalignedGeometryRecord &iRecord)
{
  // get the input GeometricalDet
  edm::ESHandle<DetGeomDesc> idealGD;
  iRecord.getRecord<IdealGeometryRecord>().get(idealGD);

  // load alignments
  edm::ESHandle<RPAlignmentCorrectionsData> alignments;
  try { iRecord.getRecord<RPMisalignedAlignmentRecord>().get(alignments); }
  catch (...) {}

  if (alignments.isValid())
  {
    if (verbosity)
      LogVerbatim("TotemRPGeometryESModule::produceMisalignedGD")
        << ">> TotemRPGeometryESModule::produceMisalignedGD > Misaligned geometry: "
        << alignments->GetRPMap().size() << " RP and "
        << alignments->GetSensorMap().size() << " sensor alignments applied.";
  } else {
    if (verbosity)
      LogVerbatim("TotemRPGeometryESModule::produceMisalignedGD")
        << ">> TotemRPGeometryESModule::produceMisalignedGD > Misaligned geometry: No alignments applied.";
  }

  DetGeomDesc* newGD = NULL;
  applyAlignments(idealGD, alignments, newGD);
  return std::unique_ptr<DetGeomDesc>(newGD);
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<TotemRPGeometry> TotemRPGeometryESModule::produceRealTG(const VeryForwardRealGeometryRecord &iRecord)
{
  edm::ESHandle<DetGeomDesc> gD;
  iRecord.get(gD);

  return std::make_unique<TotemRPGeometry>( gD.product());
}

//----------------------------------------------------------------------------------------------------

std::unique_ptr<TotemRPGeometry> TotemRPGeometryESModule::produceMisalignedTG(const VeryForwardMisalignedGeometryRecord &iRecord)
{
  edm::ESHandle<DetGeomDesc> gD;
  iRecord.get(gD);

  return std::make_unique<TotemRPGeometry>( gD.product());
}

DEFINE_FWK_EVENTSETUP_MODULE(TotemRPGeometryESModule);
