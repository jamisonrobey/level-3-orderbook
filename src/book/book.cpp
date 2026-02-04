#include "book.h"

namespace book
{
void Book::add(std::uint64_t ref_num, std::uint32_t shares, std::uint32_t price, itch::Side side)
{
    orders_.insert({ref_num, Order{.shares = shares, .price = price, .side = side}});
}

void Book::reduce(std::uint64_t ref_num, std::uint32_t shares)
{
    if (const auto it{orders_.find(ref_num)}; it != orders_.end())
    {
        if (it->second.shares <= shares)
        {
            orders_.erase(it);
        }
        else
        {
            it->second.shares -= shares;
        }
    }
}

void Book::remove(std::uint64_t ref_num)
{
    orders_.erase(ref_num);
}

void Book::replace(const itch::OrderReplaceMessage& msg)
{
    if (const auto it{orders_.find(msg.original_order_reference_number)}; it != orders_.end())
    {
        itch::Side old_side = it->second.side;
        orders_.erase(it);
        add(msg.new_order_reference_number, msg.shares, msg.price, old_side);
    }
}

}
