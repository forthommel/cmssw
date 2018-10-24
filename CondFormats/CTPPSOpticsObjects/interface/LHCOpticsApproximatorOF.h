#ifndef _LHCOpticsApproximatorOF_h_ 
#define _LHCOpticsApproximatorOF_h_

#include "TNamed.h"
#include "TSpline.h"

#include <string>

class LHCOpticsApproximatorOF : public TNamed
{
  public:
    LHCOpticsApproximatorOF() {}

    LHCOpticsApproximatorOF(const std::string &fn, const std::string &dn);

    virtual ~LHCOpticsApproximatorOF()
    {
      if (s_x0_vs_xi)
      {
        delete s_x0_vs_xi;
        delete s_y0_vs_xi;
        delete s_v_x_vs_xi;
        delete s_v_y_vs_xi;
        delete s_L_x_vs_xi;
        delete s_L_y_vs_xi;
      }
    }

    void Transport(double *k_in, double *k_out) const;

    double GetBeamMomentum() const
    {
      return 6500.;
    }

  private:
    TSpline3 *s_x0_vs_xi = NULL;
    TSpline3 *s_y0_vs_xi = NULL;
    TSpline3 *s_v_x_vs_xi = NULL;
    TSpline3 *s_v_y_vs_xi = NULL;
    TSpline3 *s_L_x_vs_xi = NULL;
    TSpline3 *s_L_y_vs_xi = NULL;

    ClassDef(LHCOpticsApproximatorOF,1)
};

#endif
