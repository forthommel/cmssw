#include "CondFormats/CTPPSOpticsObjects/interface/LHCOpticsApproximatorOF.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TFile.h"

using namespace std;

ClassImp(LHCOpticsApproximatorOF);

//----------------------------------------------------------------------------------------------------
  
LHCOpticsApproximatorOF::LHCOpticsApproximatorOF(const std::string &fn, const std::string &dn)
{
  TFile *f_in = TFile::Open(fn.c_str());
  if (f_in == NULL)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot open file " << fn << ".";

  TGraph *g;

  g = (TGraph *) f_in->Get((dn + "/g_x0_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_x0_vs_xi.";
  s_x0_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  g = (TGraph *) f_in->Get((dn + "/g_y0_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_y0_vs_xi.";
  s_y0_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  g = (TGraph *) f_in->Get((dn + "/g_v_x_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_v_x_vs_xi.";
  s_v_x_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  g = (TGraph *) f_in->Get((dn + "/g_v_y_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_v_y_vs_xi.";
  s_v_y_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  g = (TGraph *) f_in->Get((dn + "/g_L_x_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_L_x_vs_xi.";
  s_L_x_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  g = (TGraph *) f_in->Get((dn + "/g_L_y_vs_xi").c_str());
  if (!g)
    throw cms::Exception("LHCOpticsApproximatorOF") << "Cannot load object " << dn << "/g_L_y_vs_xi.";
  s_L_y_vs_xi = new TSpline3("", g->GetX(), g->GetY(), g->GetN());

  delete f_in;
}

//----------------------------------------------------------------------------------------------------

void LHCOpticsApproximatorOF::Transport(double *k_in, double *k_out) const
{
  const double &vtx_x = k_in[0];
  const double &th_x = k_in[1];
  const double &vtx_y = k_in[2];
  const double &th_y = k_in[3];
  const double &xi = k_in[4];

  k_out[0] = s_x0_vs_xi->Eval(xi) + s_v_x_vs_xi->Eval(xi) * vtx_x + s_L_x_vs_xi->Eval(xi) * th_x;
  k_out[1] = 0.;
  k_out[2] = s_y0_vs_xi->Eval(xi) + s_v_y_vs_xi->Eval(xi) * vtx_y + s_L_y_vs_xi->Eval(xi) * th_y;
  k_out[3] = 0.;
  k_out[4] = xi;
}
