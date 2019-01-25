#include "DataFormats/DateDigi/interface/DateEvent.h"

std::ostream&
operator<<( std::ostream& os, const date::Event::Header& hdr )
{
  return os << "DATE::event::header\n"
    << "  event size: " << hdr.eventSize << "\n"
    << "  magic number: " << hdr.eventMagic << "\n"
    << "  header size: " << hdr.eventHeadSize << "\n"
    << "  event version: " << hdr.eventVersion << "\n"
    << "  event type: " << hdr.eventType << "\n"
    << "  run number: " << hdr.eventRunNb << "\n"
    << "  event ID: " << hdr.eventId[0] << "/" << hdr.eventId[1] << "\n"
    << "  trigger pattern: " << hdr.eventTriggerPattern[0] << "/" << hdr.eventTriggerPattern[1] << "\n"
    << "  detector pattern: " << hdr.eventDetectorPattern[0] << "\n"
    << "  event type attribute: " << hdr.eventTypeAttribute[0] << "/" << hdr.eventTypeAttribute[1] << "/" << hdr.eventTypeAttribute[2] << "\n"
    << "  LCD id: " << hdr.eventLdcId << "\n"
    << "  GCD id: " << hdr.eventGdcId << "\n"
    << "  timestamp: " << hdr.eventTimestampSec << "/" << hdr.eventTimestamp << "\n"
    << "  timestamp[us]: " << hdr.eventTimestampUsec << "\n";
}

