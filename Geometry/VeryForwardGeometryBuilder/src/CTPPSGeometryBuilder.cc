/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometryBuilder.h"

CTPPSGeometryBuilder::CTPPSGeometryBuilder()
{
}

CTPPSGeometryBuilder::~CTPPSGeometryBuilder()
{
}

CTPPSDiamondGeometry*
CTPPSGeometryBuilder::buildDiamonds( const DDCompactView* comp_view )
{
  // only look at the diamonds attributes
  const std::string attr = "ReadOutName",
                    value = "CTPPSDiamonds";
  DDValue val( attr, value, 0. );

  DDSpecificsFilter filter;
  const bool compare_str = true;
  filter.setCriteria( val, DDCompOp::matches, DDLogOp::AND, compare_str, true );

  DDFilteredView view( *comp_view );
  view.addFilter( filter );

  return parseDiamonds( view );
}

CTPPSDiamondGeometry*
CTPPSGeometryBuilder::parseDiamonds( DDFilteredView& view )
{
  CTPPSDiamondGeometry* geom = new CTPPSDiamondGeometry;

  bool do_subdet = view.firstChild();

  while ( do_subdet ) {

    DDValue nChannels( "nChannels" );

    std::vector<const DDsvalues_type*> specs( view.specifics() );

    int num_channels = 0;
    for ( std::vector<const DDsvalues_type*>::const_iterator is = specs.begin(); is != specs.end(); ++is ) {
      if ( !DDfetch( *is, nChannels ) ) continue;
      num_channels = int( nChannels.doubles()[0] );
    }
    LogDebug("CTPPSGeometryBuilder") << "number of channels: " << num_channels;

    //...
  }

  return geom;
}
