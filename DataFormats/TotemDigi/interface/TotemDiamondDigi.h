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
    TotemDiamondDigi(unsigned short* payload=0) : payload_(payload)
    {
    };

    struct HPTDCErrorFlags {
      HPTDCErrorFlags(unsigned short flags): error_flags(flags) {;}

      bool InternalFatalChipError() const      { return error_flags&0x1; }
      bool EventLost() const                   { return (error_flags>> 1)&0x1; }
      bool HitRejectedByEventSizeLimit() const { return (error_flags>> 2)&0x1; }
      bool HitErrorGroup3() const              { return (error_flags>> 3)&0x1; }
      bool HitLostL1OverflowGroup3() const     { return (error_flags>> 4)&0x1; }
      bool HitLostROFifoOverflowGroup3() const { return (error_flags>> 5)&0x1; }
      bool HitErrorGroup2() const              { return (error_flags>> 6)&0x1; }
      bool HitLostL1OverflowGroup2() const     { return (error_flags>> 7)&0x1; }
      bool HitLostROFifoOverflowGroup2() const { return (error_flags>> 8)&0x1; }
      bool HitErrorGroup1() const              { return (error_flags>> 9)&0x1; }
      bool HitLostL1OverflowGroup1() const     { return (error_flags>>10)&0x1; }
      bool HitLostROFifoOverflowGroup1() const { return (error_flags>>11)&0x1; }
      bool HitErrorGroup0() const              { return (error_flags>>12)&0x1; }
      bool HitLostL1OverflowGroup0() const     { return (error_flags>>13)&0x1; }
      bool HitLostROFifoOverflowGroup0() const { return (error_flags>>14)&0x1; }

      unsigned short error_flags;
    };

    HPTDCErrorFlags getHPTDCErrorFlags() const
    {
      return HPTDCErrorFlags(payload_[0]);
    }

    bool hasMultipleHits() const
    {
      return payload_[1]&0x1;
    }

    unsigned int getThresholdVoltage() const
    {
      return ((payload_[2]&0x3ff)<<16)+payload_[3];
    }

    unsigned int getTrailingEdge() const
    {
      return ((payload_[4]&0x1f)<<16)+payload_[5];
    }
  
    unsigned int getLeadingEdge() const
    {
      return ((payload_[6]&0x1f)<<16)+payload_[7];
    }
  
  private:
    /// index of the activated strip
    unsigned short* payload_;
};


inline bool operator< (const TotemDiamondDigi& one, const TotemDiamondDigi& other)
{
  return one.getLeadingEdge() < other.getTrailingEdge();
}

#endif
