#ifndef ITCH_MESSAGES_AUCTION_H_
#define ITCH_MESSAGES_AUCTION_H_

#include "types.h"
namespace itch
{

enum class ImbalanceDirection : char
{
    BuyImbalance = 'B',
    SellImbalance = 'S',
    NoImbalance = 'N',
    InsufficientOrders = 'O',
    Paused = 'P'
};

enum class PriceVariationIndicator : char
{
    LessThan1Percent = 'L',
    From1To2Percent = '1',
    From2To3Percent = '2',
    From3To4Percent = '3',
    From4To5Percent = '4',
    From5To6Percent = '5',
    From6To7Percent = '6',
    From7To8Percent = '7',
    From8To9Percent = '8',
    From9To10Percent = '9',
    From10To20Percent = 'A',
    From20To30Percent = 'B',
    GreaterThan30Percent = 'C',
    CannotCalculate = ' '
};

struct NOIIMessage
{
    MessageHeader header;
    std::uint64_t paired_shares;
    std::uint64_t imbalance_shares;
    ImbalanceDirection imbalance_direction;
    Symbol stock;
    std::uint32_t far_price;
    std::uint32_t near_price;
    std::uint32_t current_reference_price;
    CrossType cross_type;
    PriceVariationIndicator price_variation_indicator;
};

enum class InterestFlag : char
{
    BuySide = 'B',
    SellSide = 'S',
    BothSides = 'A',
    NoRPIOrders = 'N'
};

struct RPIIMessage
{
    MessageHeader header;
    Symbol symbol;
    InterestFlag interest_flag;
};

enum class OpenEligibility : char
{
    Eligible = 'Y',
    NotEligible = 'N'
};

struct DirectListingPriceDiscoveryMessage
{
    MessageHeader header;
    Symbol symbol;
    OpenEligibility open_eligibility;
    std::uint32_t min_allowed_price;
    std::uint32_t max_allowed_price;
    std::uint32_t near_execution_price;
    std::uint64_t near_execution_time;
    std::uint32_t lower_price_range_collar;
    std::uint32_t upper_price_range_collar;
};

}
#endif
