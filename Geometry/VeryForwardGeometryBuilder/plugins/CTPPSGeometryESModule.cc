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

#include "Geometry/VeryForwardGeometryBuilder/interface/MeasuredGeometryProducer.h"

class CTPPSGeometryESModule : public edm::ESProducer
{
  public:
    CTPPSGeometryESModule( const edm::ParameterSet& );
    ~CTPPSGeometryESModule();

    std::unique_ptr<CTPPSDiamondGeometry> produceMeasuredDiamondGeometry( const VeryForwardMeasuredGeometryRecord& );
    std::unique_ptr<CTPPSDiamondGeometry> produceRealDiamondGeometry( const VeryForwardRealGeometryRecord& );
    std::unique_ptr<CTPPSDiamondGeometry> produceMisalignedDiamondGeometry( const VeryForwardMisalignedGeometryRecord& );

  private:

};

CTPPSGeometryESModule::CTPPSGeometryESModule( const edm::ParameterSet& iConfig )
{
  // tell the FW that data is being produced
  //setWhatProduced( this );
  setWhatProduced( this, &CTPPSGeometryESModule::produceMeasuredDiamondGeometry );
}

CTPPSGeometryESModule::~CTPPSGeometryESModule()
{
}

/*std::shared_ptr<CTPPSDiamondGeometry>
CTPPSGeometryESModule::produce( const MyRecord& iRecord )
{
  std::shared_ptr<MyType> pMyType;
  return products( pMyType );
}*/

std::unique_ptr<CTPPSDiamondGeometry>
CTPPSGeometryESModule::produceMeasuredDiamondGeometry( const VeryForwardMeasuredGeometryRecord& iRecord )
{
}

