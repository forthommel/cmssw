/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/ESProducts.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"

#include "Geometry/Records/interface/VeryForwardMisalignedGeometryRecord.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"
#include "Geometry/Records/interface/VeryForwardMeasuredGeometryRecord.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/MeasuredGeometryProducer.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometryBuilder.h"

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondGeometry.h"

class CTPPSGeometryESModule : public edm::ESProducer
{
  public:
    CTPPSGeometryESModule( const edm::ParameterSet& );
    ~CTPPSGeometryESModule();

    std::shared_ptr<CTPPSDiamondGeometry> produceDiamondGeometry( const VeryForwardRealGeometryRecord& );

    std::shared_ptr<CTPPSDiamondGeometry> produceMeasuredDiamondGeometry( const VeryForwardMeasuredGeometryRecord& );
    //std::shared_ptr<CTPPSDiamondGeometry> produceRealDiamondGeometry( const VeryForwardRealGeometryRecord& );
    std::shared_ptr<CTPPSDiamondGeometry> produceMisalignedDiamondGeometry( const VeryForwardMisalignedGeometryRecord& );

  private:

};

CTPPSGeometryESModule::CTPPSGeometryESModule( const edm::ParameterSet& iConfig )
{
  // tell the FW that data is being produced
  //setWhatProduced( this );
  setWhatProduced( this, &CTPPSGeometryESModule::produceDiamondGeometry );
  /*setWhatProduced( this, &CTPPSGeometryESModule::produceMeasuredDiamondGeometry );
  setWhatProduced( this, &CTPPSGeometryESModule::produceRealDiamondGeometry );
  setWhatProduced( this, &CTPPSGeometryESModule::produceMisalignedDiamondGeometry );*/
}

CTPPSGeometryESModule::~CTPPSGeometryESModule()
{
}

std::shared_ptr<CTPPSDiamondGeometry>
CTPPSGeometryESModule::produceDiamondGeometry( const VeryForwardRealGeometryRecord& iRecord )
{
  CTPPSGeometryBuilder builder;

  edm::ESTransientHandle<DDCompactView> rcd;
  iRecord.getRecord<RPRealAlignmentRecord>().get( rcd );

  return std::shared_ptr<CTPPSDiamondGeometry>( builder.buildDiamonds( rcd.product() ) );

  /*std::shared_ptr<MyType> pMyType;
  return products( pMyType );*/
}

/*std::shared_ptr<CTPPSDiamondGeometry>
CTPPSGeometryESModule::produceMeasuredDiamondGeometry( const VeryForwardMeasuredGeometryRecord& iRecord )
{
  edm::ESHandle<DetGeomDesc> gGeomDesc;
  iRecord.get( gGeomDesc );

  return std::shared_ptr<CTPPSDiamondGeometry>( gGeomDesc.product() );
  //return std::make_unique<CTPPSDiamondGeometry>();
}*/
