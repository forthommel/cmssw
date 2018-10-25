#include "TFile.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TF1.h"

#include <string>
#include <map>

using namespace std;

//----------------------------------------------------------------------------------------------------

unsigned int missing_counter = 0;

TObject* Load(TFile *f, const string &path, bool &process)
{
  TObject *obj = f->Get(path.c_str());

  if (obj == NULL)
  {
    printf("ERROR: can't load object '%s'.\n", path.c_str());
    missing_counter++;
    process = false;
  }

  return obj;
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  // parse command line
  string dir = argv[1];

  // config
  struct Record
  {
    string dir;
    double min;
    double max;
  };

  vector<Record> mu_records = {
    { "arm0", 0.05, 0.10 },
    { "arm1", 0.06, 0.12 },
  };

  vector<Record> si_mu_records = {
    { "si_rp23_mu_arm0", 0.05, 0.10 },
    { "si_rp3_mu_arm0", 0.05, 0.10 },
    { "si_rp103_mu_arm1", 0.05, 0.10 },
    { "si_rp123_mu_arm1", 0.05, 0.10 },
  };

  vector<Record> arm_records = {
    { "arm0", 0.06, 0.08 },
    { "arm1", 0.06, 0.10 },
  };

  TF1 *ff = new TF1("ff", "[0]");
  TF1 *ff_pol1 = new TF1("ff_pol1", "[0] + [1]*x");

  // prepare input
  TFile *f_in = TFile::Open((dir + "/output.root").c_str());
  if (!f_in)
  {
    printf("ERROR: can't open input.\n");
    return 10;
  }

  // prepare output
  TFile *f_out = TFile::Open((dir + "/do_fits.root").c_str(), "recreate");

  // process mu
  TDirectory *mu_dir = f_out->mkdir("multiRPPlots");
  for (const auto &rec : mu_records)
  {
    bool process = true;

    TProfile *p_x = (TProfile *) Load(f_in, "multiRPPlots/" + rec.dir + "/p_th_x_vs_xi", process);
    TProfile *p_y = (TProfile *) Load(f_in, "multiRPPlots/" + rec.dir + "/p_th_y_vs_xi", process);

    TProfile *p_vtx_y_vs_xi = (TProfile *) Load(f_in, "multiRPPlots/" + rec.dir + "/p_vtx_y_vs_xi", process);

    if (process)
    {
      ff->SetParameter(0, 0.);
      p_x->Fit(ff, "Q", "", rec.min, rec.max);

      ff_pol1->SetParameters(0., 0.);
      p_x->Fit(ff_pol1, "Q+", "", rec.min, rec.max);

      double x_min = 0., x_max = 0.;
      if (rec.dir == "arm0") { x_min = 0.08; x_max = 0.11; }
      if (rec.dir == "arm1") { x_min = 0.07; x_max = 0.12; }

      ff->SetParameter(0, 0.);
      p_y->Fit(ff, "Q", "", x_min, x_max);

      ff_pol1->SetParameters(0., 0.);
      p_y->Fit(ff_pol1, "Q+", "", x_min, x_max);

      ff->SetParameter(0, 0.);
      p_vtx_y_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

      gDirectory = mu_dir->mkdir(rec.dir.c_str());
      p_x->Write("p_th_x_vs_xi");
      p_y->Write("p_th_y_vs_xi");
      p_vtx_y_vs_xi->Write("p_vtx_y_vs_xi");
    }

    //----------

    process = true;

    TGraphErrors *g_th_x_RMS_vs_xi = (TGraphErrors *) Load(f_in, "multiRPPlots/" + rec.dir + "/g_th_x_RMS_vs_xi", process);
    TGraphErrors *g_th_y_RMS_vs_xi = (TGraphErrors *) Load(f_in, "multiRPPlots/" + rec.dir + "/g_th_y_RMS_vs_xi", process);

    if (process)
    {
      ff->SetParameter(0, 0.);
      g_th_x_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

      ff->SetParameter(0, 0.);
      g_th_y_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

      g_th_x_RMS_vs_xi->Write("g_th_x_RMS_vs_xi");
      g_th_y_RMS_vs_xi->Write("g_th_y_RMS_vs_xi");
    }

    //----------

    process = true;

    TGraphErrors *g_vtx_y_RMS_vs_xi = (TGraphErrors *) Load(f_in, "multiRPPlots/" + rec.dir + "/g_vtx_y_RMS_vs_xi", process);

    if (process)
    {
      ff->SetParameter(0, 0.);
      g_vtx_y_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

      g_vtx_y_RMS_vs_xi->Write("g_vtx_y_RMS_vs_xi");
    }
  }

  // process si_mu
  TDirectory *si_mu_dir = f_out->mkdir("singleMultiCorrelationPlots");
  for (const auto &rec : si_mu_records)
  {
    bool process = true;

    TProfile *p = (TProfile *) Load(f_in, "singleMultiCorrelationPlots/" + rec.dir + "/p_xi_diff_si_mu_vs_xi_mu", process);

    if (process)
    {
      ff->SetParameter(0, 0.);

      p->Fit(ff, "Q", "", rec.min, rec.max);

      gDirectory = si_mu_dir->mkdir(rec.dir.c_str());
      p->Write("p_xi_diff_si_mu_vs_xi_mu");
    }
  }

  // process arm
  TDirectory *arm_dir = f_out->mkdir("armCorrelationPlots");
  for (const auto &rec : arm_records)
  {
    bool process = true;

    TProfile *p = (TProfile *) Load(f_in, "armCorrelationPlots/" + rec.dir + "/p_xi_si_diffNF_vs_xi_mu", process);

    if (process)
    {
      ff->SetParameter(0, 0.);
      p->Fit(ff, "Q", "", rec.min, rec.max);

      gDirectory = arm_dir->mkdir(rec.dir.c_str());
      p->Write("p_xi_si_diffNF_vs_xi_mu");
    }
  }

  // clean up
  delete f_in;
  delete f_out;

  if (missing_counter > 0)
  {
    printf("WARNING: %u plots missing.\n", missing_counter);
    return 1;
  }

  return 0;
}
