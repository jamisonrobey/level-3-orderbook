#ifndef BOOK_H_
#define BOOK_H_

#include "../itch/types.h"
#include "../itch/messages_orders.h"
#include <unordered_map>
namespace book
{
struct Order
{
    std::uint32_t shares;
    std::uint32_t price;
    itch::Side side;
};

class Book
{
  public:
    void add(std::uint64_t ref_num, std::uint32_t shares, std::uint32_t price, itch::Side side);
    void reduce(std::uint64_t ref_num, std::uint32_t shares);
    void remove(std::uint64_t ref_num);
    void replace(const itch::OrderReplaceMessage& msg);

  private:
    std::unordered_map<std::uint64_t, Order> orders_;
};
}

#endif
