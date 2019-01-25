#ifndef DataFormats_DateDigi_DateEvent_h
#define DataFormats_DateDigi_DateEvent_h

namespace date
{
  class Event
  {
    public:
      struct Header
      {
        unsigned int eventSize;
        unsigned int eventMagic;
        unsigned int eventHeadSize;
        unsigned int eventVersion;
        unsigned int eventType;
        unsigned int eventRunNb;
        unsigned int eventId[2];
        unsigned int eventTriggerPattern[2];
        unsigned int eventDetectorPattern[1];
        unsigned int eventTypeAttribute[3];
        unsigned int eventLdcId;
        unsigned int eventGdcId;
        union {
          unsigned int eventTimestampSec;
          /* This definition is only for backward compatibility with event.h < 3.11 */
          unsigned int eventTimestamp;
        };
        unsigned int eventTimestampUsec;
      };
    public:
      explicit Event() {}
  };
}

#endif

