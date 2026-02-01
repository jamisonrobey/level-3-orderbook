#ifndef ITCH_MESSAGES_SYSTEM_H_
#define ITCH_MESSAGES_SYSTEM_H_

#include "types.h"

namespace itch
{

enum class EventCode : char
{
    StartOfMessages = 'O',
    StartOfSystemHours = 'S',
    StartOfMarketHours = 'Q',
    EndOfMarketHours = 'M',
    EndOfSystemHours = 'E',
    EndOfMessages = 'C'
};

struct SystemEventMessage
{
    MessageHeader header;
    EventCode event_code;
};

}

#endif
