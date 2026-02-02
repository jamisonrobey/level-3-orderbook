#include <gtest/gtest.h>
#include <itch/parser.h>

#include <vector>
#include <string_view>
#include <span>

namespace
{

// Helper to create a message with a default header (Locate=1, Tracking=2, Time=3)
std::vector<std::byte> make_msg(std::initializer_list<unsigned char> payload)
{
    std::vector<std::byte> data = {
        std::byte{0}, std::byte{1}, // Stock Locate
        std::byte{0},
        std::byte{2}, // Tracking
        std::byte{0},
        std::byte{0},
        std::byte{0},
        std::byte{0},
        std::byte{0},
        std::byte{3} // Timestamp
    };
    data.reserve(data.size() + payload.size());
    for (auto b : payload) data.push_back(std::byte{b});
    return data;
}

} // namespace

TEST(ItchParser, SystemEventMessage)
{
    // clang-format off
    auto msg = itch::parse_system_event_message(make_msg({
        'O'
    }));
    // clang-format on

    EXPECT_EQ(msg.header.stock_locate, 1);
    EXPECT_EQ(msg.header.timestamp, 3);
    EXPECT_EQ(msg.event_code, itch::EventCode::StartOfMessages);
}

TEST(ItchParser, StockDirectoryMessage)
{
    // clang-format off
    auto msg = itch::parse_stock_directory_message(make_msg({
        'A', 'A', 'P', 'L', ' ', ' ', ' ', ' ',       // Symbol
        'Q', 'N',                                      // Market Cat, Fin Status
        0x00, 0x00, 0x00, 0x64,                        // Round Lot (100)
        'N', 'C', 'C', ' ', 'P', 'N', 'N', '1', 'N',   // Flags
        0x00, 0x00, 0x00, 0x01, 'N'                    // Leverage, Inverse
    }));
    // clang-format on

    EXPECT_EQ(std::string_view(msg.symbol.data(), msg.symbol.size()), "AAPL    ");
    EXPECT_EQ(msg.market_category, itch::MarketCategory::NasdaqGlobalSelect);
    EXPECT_EQ(msg.round_lot_size, 100);
    EXPECT_EQ(msg.etp_leverage_factor, 1);
}

TEST(ItchParser, StockTradingActionMessage)
{
    // clang-format off
    auto msg = itch::parse_stock_trading_action_message(make_msg({
        'G', 'O', 'O', 'G', ' ', ' ', ' ', ' ',
        'T', 0x00, 'I', 'P', 'O', '1'
    }));
    // clang-format on

    EXPECT_EQ(std::string_view(msg.symbol.data(), msg.symbol.size()), "GOOG    ");
    EXPECT_EQ(msg.trading_state, itch::TradingState::Trading);
    EXPECT_EQ(msg.reason, itch::TradingStateReason::IPONotYetTrading);
}

TEST(ItchParser, RegSHORestrictionMessage)
{
    // clang-format off
    auto msg = itch::parse_reg_sho_restriction_message(make_msg({
        'M', 'S', 'F', 'T', ' ', ' ', ' ', ' ', '1'
    }));
    // clang-format on

    EXPECT_EQ(msg.action, itch::RegSHOAction::RestrictedDueToIntradayDrop);
}

TEST(ItchParser, MarketParticipantPositionMessage)
{
    // clang-format off
    auto msg = itch::parse_market_participant_position_message(make_msg({
        'G', 'S', 'C', 'O',                     // MPID
        'A', 'A', 'P', 'L', ' ', ' ', ' ', ' ', // Symbol
        'Y', 'N', 'A'                           // PMM, Mode, State
    }));
    // clang-format on

    EXPECT_EQ(std::string_view(msg.attribution.data(), msg.attribution.size()), "GSCO");
    EXPECT_EQ(msg.primary_market_maker, itch::PrimaryMarketMaker::Yes);
}

TEST(ItchParser, MWCBDeclineLevelMessage)
{
    // clang-format off
    auto msg = itch::parse_mwcb_decline_level_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8, // L1: 1000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xD0, // L2: 2000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0xB8  // L3: 3000
    }));
    // clang-format on

    EXPECT_EQ(msg.level_1, 1000);
    EXPECT_EQ(msg.level_2, 2000);
    EXPECT_EQ(msg.level_3, 3000);
}

TEST(ItchParser, MWCBStatusMessage)
{
    // clang-format off
    auto msg = itch::parse_mwcb_status_message(make_msg({
        '1'
    }));
    // clang-format on

    EXPECT_EQ(msg.breached_level, itch::MWCBLevel::Level1);
}

TEST(ItchParser, IPOQuotingPeriodUpdateMessage)
{
    // clang-format off
    auto msg = itch::parse_ipo_quoting_period_update_message(make_msg({
        'I', 'P', 'O', ' ', ' ', ' ', ' ', ' ',
        0x00, 0x00, 0x00, 0x7B, // Time: 123
        'A',                    // Qualifier
        0x00, 0x00, 0x27, 0x10  // Price: 10000
    }));
    // clang-format on

    EXPECT_EQ(msg.quotation_release_time, 123);
    EXPECT_EQ(msg.release_qualifier, itch::IPOQuotationReleaseQualifier::Anticipated);
    EXPECT_EQ(msg.ipo_price, 10000);
}

TEST(ItchParser, LULDAuctionCollarMessage)
{
    // clang-format off
    auto msg = itch::parse_luld_auction_collar_message(make_msg({
        'S', 'Y', 'M', ' ', ' ', ' ', ' ', ' ',
        0x00, 0x00, 0x03, 0xE8, // Ref: 1000
        0x00, 0x00, 0x04, 0x4C, // Upper: 1100
        0x00, 0x00, 0x03, 0x84, // Lower: 900
        0x00, 0x00, 0x00, 0x01  // Ext: 1
    }));
    // clang-format on

    EXPECT_EQ(msg.reference_price, 1000);
    EXPECT_EQ(msg.upper_price, 1100);
}

TEST(ItchParser, OperationalHaltMessage)
{
    // clang-format off
    auto msg = itch::parse_operational_halt_message(make_msg({
        'H', 'A', 'L', 'T', ' ', ' ', ' ', ' ',
        'Q', 'H'
    }));
    // clang-format on

    EXPECT_EQ(msg.market_code, itch::MarketCode::Nasdaq);
    EXPECT_EQ(msg.action, itch::OperationalHaltAction::Halted);
}

TEST(ItchParser, AddOrderMessage)
{
    // clang-format off
    auto msg = itch::parse_add_order_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Ref: 10
        'B',                                            // Buy
        0x00, 0x00, 0x00, 0x64,                         // Shares: 100
        'A', 'B', 'C', ' ', ' ', ' ', ' ', ' ',         // Symbol
        0x00, 0x01, 0x86, 0xA0                          // Price: 100000
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 10);
    EXPECT_EQ(msg.side, itch::Side::Buy);
    EXPECT_EQ(msg.shares, 100);
    EXPECT_EQ(msg.price, 100000);
}

TEST(ItchParser, AddOrderMPIDMessage)
{
    // clang-format off
    auto msg = itch::parse_add_order_mpid_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, // Ref: 11
        'S',                                            // Sell
        0x00, 0x00, 0x00, 0x64,                         // Shares: 100
        'X', 'Y', 'Z', ' ', ' ', ' ', ' ', ' ',         // Symbol
        0x00, 0x00, 0x27, 0x10,                         // Price: 10000
        'M', 'P', 'I', 'D'                              // MPID
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 11);
    EXPECT_EQ(std::string_view(msg.attribution.data(), msg.attribution.size()), "MPID");
}

TEST(ItchParser, OrderExecutedMessage)
{
    // clang-format off
    auto msg = itch::parse_order_executed_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Ref: 10
        0x00, 0x00, 0x00, 0x32,                         // Exec: 50
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00  // Match: 256
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 10);
    EXPECT_EQ(msg.executed_shares, 50);
}

TEST(ItchParser, OrderExecutedWithPriceMessage)
{
    // clang-format off
    auto msg = itch::parse_order_executed_with_price_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Ref: 10
        0x00, 0x00, 0x00, 0x19,                         // Exec: 25
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, // Match: 257
        'Y',                                            // Printable
        0x00, 0x00, 0x27, 0x10                          // Price: 10000
    }));
    // clang-format on

    EXPECT_EQ(msg.executed_shares, 25);
    EXPECT_EQ(msg.printable, itch::Printable::Yes);
    EXPECT_EQ(msg.execution_price, 10000);
}

TEST(ItchParser, OrderCancelMessage)
{
    // clang-format off
    auto msg = itch::parse_order_cancel_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Ref: 10
        0x00, 0x00, 0x00, 0x05                          // Cancel: 5
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 10);
    EXPECT_EQ(msg.canceled_shares, 5);
}

TEST(ItchParser, OrderDeleteMessage)
{
    // clang-format off
    auto msg = itch::parse_order_delete_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A // Ref: 10
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 10);
}

TEST(ItchParser, OrderReplaceMessage)
{
    // clang-format off
    auto msg = itch::parse_order_replace_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Orig: 10
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, // New: 12
        0x00, 0x00, 0x00, 0x64,                         // Shares: 100
        0x00, 0x00, 0x27, 0x10                          // Price: 10000
    }));
    // clang-format on

    EXPECT_EQ(msg.original_order_reference_number, 10);
    EXPECT_EQ(msg.new_order_reference_number, 12);
}

TEST(ItchParser, TradeMessage)
{
    // clang-format off
    auto msg = itch::parse_trade_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, // Ref: 10
        'B',                                            // Side
        0x00, 0x00, 0x00, 0x64,                         // Shares: 100
        'T', 'R', 'A', 'D', ' ', ' ', ' ', ' ',         // Symbol
        0x00, 0x00, 0x27, 0x10,                         // Price: 10000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00  // Match: 256
    }));
    // clang-format on

    EXPECT_EQ(msg.order_reference_number, 10);
    EXPECT_EQ(msg.side, itch::Side::Buy);
    EXPECT_EQ(msg.shares, 100);
}

TEST(ItchParser, CrossTradeMessage)
{
    // clang-format off
    auto msg = itch::parse_cross_trade_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x10, // Shares: 10000
        'C', 'R', 'O', 'S', 'S', ' ', ' ', ' ',
        0x00, 0x00, 0x27, 0x10,                         // Price: 10000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // Match: 1
        'O'                                             // Type: Opening
    }));
    // clang-format on

    EXPECT_EQ(msg.shares, 10000);
    EXPECT_EQ(msg.cross_price, 10000);
    EXPECT_EQ(msg.type, itch::CrossType::Opening);
}

TEST(ItchParser, BrokenTradeMessage)
{
    // clang-format off
    auto msg = itch::parse_broken_trade_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00 // Match: 256
    }));
    // clang-format on

    EXPECT_EQ(msg.match_number, 256);
}

TEST(ItchParser, NOIIMessage)
{
    // clang-format off
    auto msg = itch::parse_noii_message(make_msg({
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8, // Paired: 1000
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF4, // Imbal: 500
        'B',                                            // Direction
        'N', 'O', 'I', 'I', ' ', ' ', ' ', ' ',
        0x00, 0x00, 0x27, 0x10,                         // Far
        0x00, 0x00, 0x27, 0x11,                         // Near
        0x00, 0x00, 0x27, 0x12,                         // Ref
        'C', '1'                                        // Type, Var
    }));
    // clang-format on

    EXPECT_EQ(msg.paired_shares, 1000);
    EXPECT_EQ(msg.imbalance_shares, 500);
    EXPECT_EQ(msg.near_price, 10001);
}

TEST(ItchParser, RPIIMessage)
{
    // clang-format off
    auto msg = itch::parse_rpii_message(make_msg({
        'R', 'P', 'I', ' ', ' ', ' ', ' ', ' ',
        'B'
    }));
    // clang-format on

    EXPECT_EQ(msg.interest_flag, itch::InterestFlag::BuySide);
}

TEST(ItchParser, DirectListingPriceDiscoveryMessage)
{
    // clang-format off
    auto msg = itch::parse_direct_listing_price_discovery_message(make_msg({
        'D', 'L', 'C', 'R', ' ', ' ', ' ', ' ',
        'Y',
        0x00, 0x00, 0x23, 0x28, // Min: 9000
        0x00, 0x00, 0x27, 0x10, // Max: 10000
        0x00, 0x00, 0x25, 0x1C, // Near: 9500
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64,
        0x00, 0x00, 0x21, 0x34, // Lower
        0x00, 0x00, 0x29, 0x04  // Upper
    }));
    // clang-format on

    EXPECT_EQ(msg.open_eligibility, itch::OpenEligibility::Eligible);
    EXPECT_EQ(msg.near_execution_price, 9500);
}
