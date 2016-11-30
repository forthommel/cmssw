/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondTopology_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondTopology_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/CommonTopologies/interface/Topology.h"
//#include "Geometry/CommonTopologies/interface/PixelTopology.h"

class CTPPSDiamondTopology : public Topology
{
  public:
    typedef std::vector< std::pair<float, std::vector<float> > > DiamondsDimensions;

  public:
    CTPPSDiamondTopology( const DiamondsDimensions& );

    LocalPoint localPosition( const MeasurementPoint& ) const;
    LocalError localError( const MeasurementPoint&, const MeasurementError& ) const;

    MeasurementPoint measurementPosition( const LocalPoint& ) const;
    MeasurementError measurementError( const LocalPoint&, const LocalError& ) const;

    /*typedef std::pair<float,float> Coordinate;
    Coordinate pixel( const LocalPoint& ) const;*/
    int channel( const LocalPoint& ) const;

  private:
    DiamondsDimensions dim_;
};

#endif
