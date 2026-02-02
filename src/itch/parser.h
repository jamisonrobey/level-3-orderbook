#ifndef ITCH_PARSER_H_
#define ITCH_PARSER_H_

#include "messages_system.h"
#include "messages_stock.h"
#include "messages_orders.h"
#include "messages_trade.h"
#include "messages_auction.h"
#include <span>

namespace itch
{

SystemEventMessage parse_system_event_message(std::span<const std::byte> bytes);

StockDirectoryMessage parse_stock_directory_message(std::span<const std::byte> bytes);
StockTradingActionMessage parse_stock_trading_action_message(std::span<const std::byte> bytes);
RegSHORestrictionMessage parse_reg_sho_restriction_message(std::span<const std::byte> bytes);
MarketParticipantPositionMessage parse_market_participant_position_message(std::span<const std::byte> bytes);
MWCBDeclineLevelMessage parse_mwcb_decline_level_message(std::span<const std::byte> bytes);
MWCBStatusMessage parse_mwcb_status_message(std::span<const std::byte> bytes);
IPOQuotingPeriodUpdateMessage parse_ipo_quoting_period_update_message(std::span<const std::byte> bytes);
LULDAuctionCollarMessage parse_luld_auction_collar_message(std::span<const std::byte> bytes);
OperationalHaltMessage parse_operational_halt_message(std::span<const std::byte> bytes);

AddOrderMessage parse_add_order_message(std::span<const std::byte> bytes);
AddOrderMPIDMessage parse_add_order_mpid_message(std::span<const std::byte> bytes);
OrderExecutedMessage parse_order_executed_message(std::span<const std::byte> bytes);
OrderExecutedWithPriceMessage parse_order_executed_with_price_message(std::span<const std::byte> bytes);
OrderCancelMessage parse_order_cancel_message(std::span<const std::byte> bytes);
OrderDeleteMessage parse_order_delete_message(std::span<const std::byte> bytes);
OrderReplaceMessage parse_order_replace_message(std::span<const std::byte> bytes);

TradeMessage parse_trade_message(std::span<const std::byte> bytes);
CrossTradeMessage parse_cross_trade_message(std::span<const std::byte> bytes);
BrokenTradeMessage parse_broken_trade_message(std::span<const std::byte> bytes);

NOIIMessage parse_noii_message(std::span<const std::byte> bytes);
RPIIMessage parse_rpii_message(std::span<const std::byte> bytes);
DirectListingPriceDiscoveryMessage parse_direct_listing_price_discovery_message(std::span<const std::byte> bytes);
}

#endif
