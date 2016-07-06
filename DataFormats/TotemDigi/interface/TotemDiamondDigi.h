/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
*   Hubert Niewiadomski
*   Jan Kašpar (jan.kaspar@gmail.com)
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*
****************************************************************************/

#ifndef DataFormats_TotemDigi_TotemDiamondDigi
#define DataFormats_TotemDigi_TotemDiamondDigi

/**
 * Digi structure for CMS-TOTEM diamond detectors.
**/
class TotemDiamondDigi
{
  public:
    TotemDiamondDigi(unsigned short strip_no=0) : strip_no_(strip_no)
    {
    };

    unsigned short getStripNumber() const
    {
      return strip_no_;
    }
  
  private:
    /// index of the activated strip
    unsigned short strip_no_;
};


inline bool operator< (const TotemDiamondDigi& one, const TotemDiamondDigi& other)
{
  return one.getStripNumber() < other.getStripNumber();
}

#endif
