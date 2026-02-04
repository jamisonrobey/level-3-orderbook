#ifndef MARKET_H_
#define MARKET_H_

#include <vector>
#include "book.h"
namespace book
{
class Market
{
  public:
    Market();
    Book& get_book(std::uint16_t stock_locate);

  private:
    std::vector<Book> books_;
};
}

#endif // MARKET_H_
