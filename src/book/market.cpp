#include "market.h"

namespace book
{
Market::Market()
    : books_(std::numeric_limits<std::uint16_t>::max())
{
}

Book& Market::get_book(std::uint16_t stock_locate)
{
    return books_[stock_locate];
}

}
