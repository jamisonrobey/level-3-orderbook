#ifndef ITCH_TYPES_H_
#define ITCH_TYPES_H_

#include <cstdint>
#include <array>

namespace itch
{

enum class MessageType : char
{
    SystemEvent = 'S',
    StockDirectory = 'R',
    StockTradingAction = 'H',
    RegSHORestriction = 'Y',
    MarketParticipantPosition = 'L',
    MWCBDeclineLevel = 'V',
    MWCBStatus = 'W',
    IPOQuotingPeriodUpdate = 'K',
    LULDAuctionCollar = 'J',
    OperationalHalt = 'h',
    AddOrder = 'A',
    AddOrderMPID = 'F',
    OrderExecuted = 'E',
    OrderExecutedWithPrice = 'C',
    OrderCancel = 'X',
    OrderDelete = 'D',
    OrderReplace = 'U',
    Trade = 'P',
    CrossTrade = 'Q',
    BrokenTrade = 'B',
    NOII = 'I',
    RPII = 'N',
    DirectListingPriceDiscovery = 'O'
};

struct MessageHeader
{
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    std::uint64_t timestamp;
};

enum class Side : char
{
    Buy = 'B',
    Sell = 'S'
};

enum class CrossType : char
{
    Opening = 'O',
    Closing = 'C',
    IPOHalt = 'H',
    ExtendedTradingClose = 'A'
};

using Symbol = std::array<char, 8>;
using MPID = std::array<char, 4>;

}

#endif
