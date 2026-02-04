#ifndef ITCH_TYPES_H_
#define ITCH_TYPES_H_

#include <cstdint>
#include <array>
#include <format>

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

// debug print
template <>
struct std::formatter<itch::MessageType> : std::formatter<std::string_view>
{
    auto format(itch::MessageType msg_type, std::format_context& ctx) const
    {
        std::string_view name{"Unknown"};
        switch (msg_type)
        {
        case itch::MessageType::SystemEvent:
            name = "SystemEvent";
            break;
        case itch::MessageType::StockDirectory:
            name = "StockDirectory";
            break;
        case itch::MessageType::StockTradingAction:
            name = "StockTradingAction";
            break;
        case itch::MessageType::RegSHORestriction:
            name = "RegSHORestriction";
            break;
        case itch::MessageType::MarketParticipantPosition:
            name = "MarketParticipantPosition";
            break;
        case itch::MessageType::MWCBDeclineLevel:
            name = "MWCBDeclineLevel";
            break;
        case itch::MessageType::MWCBStatus:
            name = "MWCBStatus";
            break;
        case itch::MessageType::IPOQuotingPeriodUpdate:
            name = "IPOQuotingPeriodUpdate";
            break;
        case itch::MessageType::LULDAuctionCollar:
            name = "LULDAuctionCollar";
            break;
        case itch::MessageType::OperationalHalt:
            name = "OperationalHalt";
            break;
        case itch::MessageType::AddOrder:
            name = "AddOrder";
            break;
        case itch::MessageType::AddOrderMPID:
            name = "AddOrderMPID";
            break;
        case itch::MessageType::OrderExecuted:
            name = "OrderExecuted";
            break;
        case itch::MessageType::OrderExecutedWithPrice:
            name = "OrderExecutedWithPrice";
            break;
        case itch::MessageType::OrderCancel:
            name = "OrderCancel";
            break;
        case itch::MessageType::OrderDelete:
            name = "OrderDelete";
            break;
        case itch::MessageType::OrderReplace:
            name = "OrderReplace";
            break;
        case itch::MessageType::Trade:
            name = "Trade";
            break;
        case itch::MessageType::CrossTrade:
            name = "CrossTrade";
            break;
        case itch::MessageType::BrokenTrade:
            name = "BrokenTrade";
            break;
        case itch::MessageType::NOII:
            name = "NOII";
            break;
        case itch::MessageType::RPII:
            name = "RPII";
            break;
        case itch::MessageType::DirectListingPriceDiscovery:
            name = "DirectListingPriceDiscovery";
            break;
        }
        return std::formatter<std::string_view>::format(name, ctx);
    }
};

#endif
