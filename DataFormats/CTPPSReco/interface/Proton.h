#ifndef DataFormats_CTPPSReco_Proton_h
#define DataFormats_CTPPSReco_Proton_h

namespace reco
{
  class Proton
  {
    public:
      Proton() : xi_( 0. ), err_xi_( 0. ), pot_id_( 0 ) {}
      Proton( unsigned int pot, float xi, float err_xi ) : xi_( xi ), err_xi_( err_xi ), pot_id_( pot ) {}

      float xi() const { return xi_; }
      float xiError() const { return err_xi_; }
      unsigned int pot() const { return pot_id_; }

    private:
      float xi_;
      float err_xi_;
      unsigned int pot_id_;
  };
}

#endif
