#ifndef ITCH_MESSAGES_ORDERS_H_
#define ITCH_MESSAGES_ORDERS_H_

#include "types.h"

namespace itch
{

struct AddOrderMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    Side side;
    std::uint32_t shares;
    Symbol symbol;
    std::uint32_t price;
};

struct AddOrderMPIDMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    Side side;
    std::uint32_t shares;
    Symbol symbol;
    std::uint32_t price;
    MPID attribution;
};

struct OrderExecutedMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    std::uint32_t executed_shares;
    std::uint64_t match_number;
};

enum class Printable : char
{
    Yes = 'Y',
    No = 'N'
};

struct OrderExecutedWithPriceMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    std::uint32_t executed_shares;
    std::uint64_t match_number;
    Printable printable;
    std::uint32_t execution_price;
};

struct OrderCancelMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
    std::uint32_t canceled_shares;
};

struct OrderDeleteMessage
{
    MessageHeader header;
    std::uint64_t order_reference_number;
};

struct OrderReplaceMessage
{
    MessageHeader header;
    std::uint64_t original_order_reference_number;
    std::uint64_t new_order_reference_number;
    std::uint32_t shares;
    std::uint32_t price;
};

}

#endif
