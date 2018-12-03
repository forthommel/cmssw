/****************************************************************************
*
* Authors:
*  Jan Kašpar (jan.kaspar@gmail.com) 
*    
****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/CTPPSOpticsObjects/interface/LHCOpticsApproximator.h"

#include <map>

#include "TProfile.h"
#include "TRandom.h"

//----------------------------------------------------------------------------------------------------

class CTPPSOpticalFunctionsTester : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSOpticalFunctionsTester(const edm::ParameterSet&);
    ~CTPPSOpticalFunctionsTester();

  private: 
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) {}

    unsigned int verbosity;

    bool useExtraFunctions_;

    edm::FileInPath opticsFile_;
    std::vector<std::string> opticsObjects_;
    edm::ParameterSet beamConditions_;

    double vtx0_x_45_, vtx0_x_56_;
    double vtx0_y_45_, vtx0_y_56_;
    double half_crossing_angle_45_, half_crossing_angle_56_;

    unsigned int n_events;

    std::string outputFile_;
};

//----------------------------------------------------------------------------------------------------

CTPPSOpticalFunctionsTester::CTPPSOpticalFunctionsTester( const edm::ParameterSet& iConfig ) :
  verbosity( iConfig.getParameter<unsigned int>("verbosity")),
  useExtraFunctions_( iConfig.getParameter<bool>("useExtraFunctions")),
  opticsFile_            ( iConfig.getParameter<edm::FileInPath>( "opticsFile" ) ),
  opticsObjects_         ( iConfig.getParameter< std::vector<std::string> >( "opticsObjects" ) ),
  beamConditions_        ( iConfig.getParameter<edm::ParameterSet>( "beamConditions" ) ),
  vtx0_x_45_             ( beamConditions_.getParameter<double>( "xOffsetSector45" ) ), // in m
  vtx0_x_56_             ( beamConditions_.getParameter<double>( "xOffsetSector56" ) ), // in m
  vtx0_y_45_             ( beamConditions_.getParameter<double>( "yOffsetSector45" ) ), // in m
  vtx0_y_56_             ( beamConditions_.getParameter<double>( "yOffsetSector56" ) ), // in m
  half_crossing_angle_45_( beamConditions_.getParameter<double>( "halfCrossingAngleSector45" ) ), // in rad
  half_crossing_angle_56_( beamConditions_.getParameter<double>( "halfCrossingAngleSector56" ) ), // in rad
  n_events( iConfig.getParameter<unsigned int>("n_events")),
  outputFile_            ( iConfig.getParameter<std::string>( "outputFile" ) )
{
}

//----------------------------------------------------------------------------------------------------

CTPPSOpticalFunctionsTester::~CTPPSOpticalFunctionsTester()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSOpticalFunctionsTester::beginJob()
{
  printf(">> CTPPSOpticalFunctionsTester::beginJob\n");
  printf("    vtx0_x_45 = %.3E m, vtx0_x_56 = %.3E m\n", vtx0_x_45_, vtx0_x_56_);
  printf("    vtx0_y_45 = %.3E m, vtx0_y_56 = %.3E m\n", vtx0_y_45_, vtx0_y_56_);
  printf("    xangle_45 = %.1f urad, xangle_56 = %.1f urad\n", half_crossing_angle_45_ * 1E6, half_crossing_angle_56_ * 1E6);
  printf("    file = %s\n", opticsFile_.fullPath().c_str());

  // open input file
  auto f_in = std::make_unique<TFile>( opticsFile_.fullPath().c_str() );
  if ( !f_in )
    throw cms::Exception("CTPPSOpticalFunctionsTester") << "Cannot open file '" << opticsFile_ << "'.";

  // open output file
  TFile *f_out = TFile::Open(outputFile_.c_str(), "recreate");

  // go through all optics objects
  for ( const auto& objName : opticsObjects_ )
  {
    printf("    %s\n", objName.c_str());

    const auto optApp = dynamic_cast<LHCOpticsApproximator*>( f_in->Get( objName.c_str() ) );
    if (!optApp)
      throw cms::Exception("CTPPSOpticalFunctionsTester") << "Cannot load object '" << objName << "'.";

    // determine crossing angle, vertex offset
    double crossing_angle = 0.0;
    double vtx0_x = 0.0;
    double vtx0_y = 0.0;

    if ( optApp->GetBeamType()==LHCOpticsApproximator::lhcb2 )
    {
      crossing_angle = half_crossing_angle_45_;
      vtx0_x = vtx0_x_45_;
      vtx0_y = vtx0_y_45_;
    }

    if ( optApp->GetBeamType()==LHCOpticsApproximator::lhcb1 )
    {
      crossing_angle = half_crossing_angle_56_;
      vtx0_x = vtx0_x_56_;
      vtx0_y = vtx0_y_56_;
    }

    const bool check_apertures = false;
    const bool invert_beam_coord_systems = true;

    // book plots
    TProfile *p_de_x_vs_xi = new TProfile("", ";xi;De x", 100, 0., 0.2, "s");
    TProfile *p_de_x_vs_th_x = new TProfile("", ";th_x;De x", 100, -200E-6, +200E-6, "s");
    TProfile *p_de_x_vs_th_y = new TProfile("", ";th_y;De x", 100, -200E-6, +200E-6, "s");

    TProfile *p_de_y_vs_xi = new TProfile("", ";xi;De y", 100, 0., 0.2, "s");
    TProfile *p_de_y_vs_th_x = new TProfile("", ";th_x;De y", 100, -200E-6, +200E-6, "s");
    TProfile *p_de_y_vs_th_y = new TProfile("", ";th_y;De y", 100, -200E-6, +200E-6, "s");

    TProfile *p_xD_vs_xi = new TProfile("", ";xi;x_D", 500, 0., 0.2);
    TProfile *p_Lx_vs_xi = new TProfile("", ";xi;Lx", 500, 0., 0.2);
    TProfile *p_vx_vs_xi = new TProfile("", ";xi;vx", 500, 0., 0.2);
    TProfile *p_E14_vs_xi = new TProfile("", ";xi;E14", 500, 0., 0.2);

    TProfile *p_yD_vs_xi = new TProfile("", ";xi;y_D", 500, 0., 0.2);
    TProfile *p_Ly_vs_xi = new TProfile("", ";xi;Ly", 500, 0., 0.2);
    TProfile *p_vy_vs_xi = new TProfile("", ";xi;vy", 500, 0., 0.2);
    TProfile *p_E32_vs_xi = new TProfile("", ";xi;E32", 500, 0., 0.2);

    // make plots
    for (unsigned int event = 0; event < n_events; ++event)
    {
      const double xi = gRandom->Rndm() * 0.2;

      const double th_x = gRandom->Gaus() * 50E-6;  // rad
      const double th_y = gRandom->Gaus() * 50E-6;

      const double vtx_x = gRandom->Gaus() * 10E-6;  // m
      const double vtx_y = gRandom->Gaus() * 10E-6;

      const double ep_th_x = 10E-6;  // rad
      const double ep_th_y = 10E-6;

      const double ep_vtx_x = 10E-6; // m
      const double ep_vtx_y = 10E-6;

      std::array<double,5> k_in_all = { { vtx0_x + vtx_x, (crossing_angle + th_x) * ( 1.-xi ), vtx0_y + vtx_y, th_y * ( 1.-xi ), -xi } }, k_out_all;
      optApp->Transport( k_in_all.data(), k_out_all.data(), check_apertures, invert_beam_coord_systems );

      std::array<double,5> k_in_xi = { { vtx0_x, crossing_angle * ( 1.-xi ), vtx0_y, 0., -xi } }, k_out_xi;
      optApp->Transport( k_in_xi.data(), k_out_xi.data(), check_apertures, invert_beam_coord_systems );

      std::array<double,5> k_in_xi_th_x = { { vtx0_x, (crossing_angle + ep_th_x) * ( 1.-xi ), vtx0_y, 0., -xi } }, k_out_xi_th_x;
      optApp->Transport( k_in_xi_th_x.data(), k_out_xi_th_x.data(), check_apertures, invert_beam_coord_systems );

      std::array<double,5> k_in_xi_vtx_x = { { vtx0_x + ep_vtx_x, crossing_angle * ( 1.-xi ), vtx0_y, 0., -xi } }, k_out_xi_vtx_x;
      optApp->Transport( k_in_xi_vtx_x.data(), k_out_xi_vtx_x.data(), check_apertures, invert_beam_coord_systems );

      std::array<double,5> k_in_xi_th_y = { { vtx0_x, crossing_angle * ( 1.-xi ), vtx0_y, ep_th_y * ( 1.-xi ), -xi } }, k_out_xi_th_y;
      optApp->Transport( k_in_xi_th_y.data(), k_out_xi_th_y.data(), check_apertures, invert_beam_coord_systems );

      std::array<double,5> k_in_xi_vtx_y = { { vtx0_x, crossing_angle * ( 1.-xi ), vtx0_y + ep_vtx_y, 0., -xi } }, k_out_xi_vtx_y;
      optApp->Transport( k_in_xi_vtx_y.data(), k_out_xi_vtx_y.data(), check_apertures, invert_beam_coord_systems );

      const double xD = k_out_xi[0];
      const double Lx = (k_out_xi_th_x[0] - k_out_xi[0]) / ep_th_x;
      const double vx = (k_out_xi_vtx_x[0] - k_out_xi[0]) / ep_vtx_x;
      const double E14 = (k_out_xi_th_y[0] - k_out_xi[0]) / ep_th_y;

      const double yD = k_out_xi[2];
      const double Ly = (k_out_xi_th_y[2] - k_out_xi[2]) / ep_th_y;
      const double vy = (k_out_xi_vtx_y[2] - k_out_xi[2]) / ep_vtx_y;
      const double E32 = (k_out_xi_th_x[2] - k_out_xi[2]) / ep_th_x;

      const double x_par = k_out_all[0];
      const double y_par = k_out_all[2];

      double x_of = xD + Lx * th_x + vx * vtx_x;
      double y_of = yD + Ly * th_y + vy * vtx_y;

      if (useExtraFunctions_)
      {
        x_of += E14 * th_y;
        y_of += E32 * th_x;
      }

      const double de_x = x_of - x_par;
      const double de_y = y_of - y_par;

      if (verbosity)
      {
        printf("----------------------\n");
        printf("xi = %.3f, th_x = %.3E, th_y = %.3E, vtx_x = %.3E, vtx_y = %.3E\n", xi, th_x, th_y, vtx_x, vtx_y);
        printf("xD = %.3E, Lx = %.3f, vx = %.3f, E14 = %.3f\n", xD, Lx, vx, E14);
        printf("yD = %.3E, Ly = %.3f, vy = %.3f, E32 = %.3f\n", yD, Ly, vy, E32);
        printf("x_par = %.2f um, x_of = %.2f um, de x = %.2f um\n", x_par*1E6, x_of*1E6, de_x*1E6);
        printf("y_par = %.2f um, y_of = %.2f um, de y = %.2f um\n", y_par*1E6, y_of*1E6, de_y*1E6);
      }

      p_de_x_vs_xi->Fill(xi, de_x);
      p_de_x_vs_th_x->Fill(th_x, de_x);
      p_de_x_vs_th_y->Fill(th_y, de_x);

      p_de_y_vs_xi->Fill(xi, de_y);
      p_de_y_vs_th_x->Fill(th_x, de_y);
      p_de_y_vs_th_y->Fill(th_y, de_y);

      p_xD_vs_xi->Fill(xi, xD);
      p_Lx_vs_xi->Fill(xi, Lx);
      p_vx_vs_xi->Fill(xi, vx);
      p_E14_vs_xi->Fill(xi, E14);

      p_yD_vs_xi->Fill(xi, yD);
      p_Ly_vs_xi->Fill(xi, Ly);
      p_vy_vs_xi->Fill(xi, vy);
      p_E32_vs_xi->Fill(xi, E32);
    }

    // save plots
    gDirectory = f_out->mkdir(objName.c_str());

    p_de_x_vs_xi->Write("p_de_x_vs_xi");
    p_de_x_vs_th_x->Write("p_de_x_vs_th_x");
    p_de_x_vs_th_y->Write("p_de_x_vs_th_y");

    p_de_y_vs_xi->Write("p_de_y_vs_xi");
    p_de_y_vs_th_x->Write("p_de_y_vs_th_x");
    p_de_y_vs_th_y->Write("p_de_y_vs_th_y");

    p_xD_vs_xi->Write("p_x0_vs_xi");
    p_Lx_vs_xi->Write("p_L_x_vs_xi");
    p_vx_vs_xi->Write("p_v_x_vs_xi");
    p_E14_vs_xi->Write("p_E_14_vs_xi");

    p_yD_vs_xi->Write("p_y0_vs_xi");
    p_Ly_vs_xi->Write("p_L_y_vs_xi");
    p_vy_vs_xi->Write("p_v_y_vs_xi");
    p_E32_vs_xi->Write("p_E_32_vs_xi");
  }

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSOpticalFunctionsTester );
