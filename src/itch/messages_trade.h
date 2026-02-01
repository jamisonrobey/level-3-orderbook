#ifndef ITCH_MESSAGES_TRADE_H_
#define ITCH_MESSAGES_TRADE_H_

#include "types.h"
namespace itch
{
struct TradeMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    Side side;
    std::uint32_t shares;
    Symbol symbol;
    std::uint32_t price;
    std::uint64_t match_number;
};

struct CrossTradeMessage
{
    MessageHeader header;
    std::uint64_t shares;
    Symbol symbol;
    std::uint32_t cross_price;
    std::uint64_t match_number;
    CrossType type;
};

struct BrokenTradeMessage
{
    MessageHeader header;
    std::uint64_t match_number;
};

}
#endif
