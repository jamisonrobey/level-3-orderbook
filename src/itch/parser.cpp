#include "parser.h"
#include "messages_auction.h"
#include "messages_orders.h"
#include "messages_stock.h"

#include <cstring>
#include "../util/binary_io.h"

namespace
{

template <typename T>
T extract_enum(std::span<const std::byte> bytes, std::size_t& pos)
{
    return static_cast<T>(util::extract<std::underlying_type_t<T>>(bytes, pos));
}

itch::MessageHeader extract_header(std::span<const std::byte> bytes, std::size_t& pos)
{
    itch::MessageHeader header{};
    header.stock_locate = util::extract_be<std::uint16_t>(bytes, pos);
    header.tracking_number = util::extract_be<std::uint16_t>(bytes, pos);

    // 48-bit be timestamp into 64bit
    for (int i = 0; i < 6; ++i)
    {
        header.timestamp = (header.timestamp << 8) | static_cast<std::uint64_t>(bytes[pos++]);
    }

    return header;
}

}

namespace itch
{
SystemEventMessage parse_system_event_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .event_code = extract_enum<EventCode>(bytes, pos)};
}

StockDirectoryMessage parse_stock_directory_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .market_category = extract_enum<MarketCategory>(bytes, pos),
        .financial_status = extract_enum<FinancialStatus>(bytes, pos),
        .round_lot_size = util::extract_be<std::uint32_t>(bytes, pos),
        .round_lots_only = extract_enum<RoundLotsOnly>(bytes, pos),
        .issue_classification = extract_enum<IssueClassification>(bytes, pos),
        .issue_sub_type = extract_enum<IssueSubType>(bytes, pos),
        .authenticity = extract_enum<Authenticity>(bytes, pos),
        .short_sale_threshold = extract_enum<ShortSaleThresholdIndicator>(bytes, pos),
        .ipo_flag = extract_enum<IPOFlag>(bytes, pos),
        .luld_reference_price_tier = extract_enum<LULDReferencePriceTier>(bytes, pos),
        .etp_flag = extract_enum<ETPFlag>(bytes, pos),
        .etp_leverage_factor = util::extract_be<std::uint32_t>(bytes, pos),
        .inverse_indicator = extract_enum<InverseIndicator>(bytes, pos)};
}

StockTradingActionMessage parse_stock_trading_action_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .trading_state = extract_enum<TradingState>(bytes, pos),
        .reserved = util::extract<std::byte>(bytes, pos),
        .reason = extract_enum<TradingStateReason>(bytes, pos)};
}

RegSHORestrictionMessage parse_reg_sho_restriction_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .action = extract_enum<RegSHOAction>(bytes, pos)};
}

MarketParticipantPositionMessage parse_market_participant_position_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .attribution = util::extract<MPID>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .primary_market_maker = extract_enum<PrimaryMarketMaker>(bytes, pos),
        .market_maker_mode = extract_enum<MarketMakerMode>(bytes, pos),
        .participant_state = extract_enum<MarketParticipantState>(bytes, pos)};
}

MWCBDeclineLevelMessage parse_mwcb_decline_level_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .level_1 = util::extract_be<std::uint64_t>(bytes, pos),
        .level_2 = util::extract_be<std::uint64_t>(bytes, pos),
        .level_3 = util::extract_be<std::uint64_t>(bytes, pos)};
}

MWCBStatusMessage parse_mwcb_status_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .breached_level = extract_enum<MWCBLevel>(bytes, pos)};
}

IPOQuotingPeriodUpdateMessage parse_ipo_quoting_period_update_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .quotation_release_time = util::extract_be<std::uint32_t>(bytes, pos),
        .release_qualifier = extract_enum<IPOQuotationReleaseQualifier>(bytes, pos),
        .ipo_price = util::extract_be<std::uint32_t>(bytes, pos)};
}

LULDAuctionCollarMessage parse_luld_auction_collar_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .reference_price = util::extract_be<std::uint32_t>(bytes, pos),
        .upper_price = util::extract_be<std::uint32_t>(bytes, pos),
        .lower_price = util::extract_be<std::uint32_t>(bytes, pos),
        .extension_number = util::extract_be<std::uint32_t>(bytes, pos)};
}

OperationalHaltMessage parse_operational_halt_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .market_code = extract_enum<MarketCode>(bytes, pos),
        .action = extract_enum<OperationalHaltAction>(bytes, pos)};
}

AddOrderMessage parse_add_order_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .side = extract_enum<Side>(bytes, pos),
        .shares = util::extract_be<std::uint32_t>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .price = util::extract_be<std::uint32_t>(bytes, pos)};
}

AddOrderMPIDMessage parse_add_order_mpid_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .side = extract_enum<Side>(bytes, pos),
        .shares = util::extract_be<std::uint32_t>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .price = util::extract_be<std::uint32_t>(bytes, pos),
        .attribution = util::extract<MPID>(bytes, pos)};
}

OrderExecutedMessage parse_order_executed_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .executed_shares = util::extract_be<std::uint32_t>(bytes, pos),
        .match_number = util::extract_be<std::uint64_t>(bytes, pos)};
}

OrderExecutedWithPriceMessage parse_order_executed_with_price_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .executed_shares = util::extract_be<std::uint32_t>(bytes, pos),
        .match_number = util::extract_be<std::uint64_t>(bytes, pos),
        .printable = extract_enum<Printable>(bytes, pos),
        .execution_price = util::extract_be<std::uint32_t>(bytes, pos)};
}

OrderCancelMessage parse_order_cancel_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .canceled_shares = util::extract_be<std::uint32_t>(bytes, pos)};
}

OrderDeleteMessage parse_order_delete_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos)};
}

OrderReplaceMessage parse_order_replace_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .original_order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .new_order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .shares = util::extract_be<std::uint32_t>(bytes, pos),
        .price = util::extract_be<std::uint32_t>(bytes, pos)};
}

TradeMessage parse_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .order_reference_number = util::extract_be<std::uint64_t>(bytes, pos),
        .side = extract_enum<Side>(bytes, pos),
        .shares = util::extract_be<std::uint32_t>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .price = util::extract_be<std::uint32_t>(bytes, pos),
        .match_number = util::extract_be<std::uint64_t>(bytes, pos)};
}

CrossTradeMessage parse_cross_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .shares = util::extract_be<std::uint64_t>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .cross_price = util::extract_be<std::uint32_t>(bytes, pos),
        .match_number = util::extract_be<std::uint64_t>(bytes, pos),
        .type = extract_enum<CrossType>(bytes, pos)};
}

BrokenTradeMessage parse_broken_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .match_number = util::extract_be<std::uint64_t>(bytes, pos)};
}

NOIIMessage parse_noii_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .paired_shares = util::extract_be<std::uint64_t>(bytes, pos),
        .imbalance_shares = util::extract_be<std::uint64_t>(bytes, pos),
        .imbalance_direction = extract_enum<ImbalanceDirection>(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .far_price = util::extract_be<std::uint32_t>(bytes, pos),
        .near_price = util::extract_be<std::uint32_t>(bytes, pos),
        .current_reference_price = util::extract_be<std::uint32_t>(bytes, pos),
        .cross_type = extract_enum<CrossType>(bytes, pos),
        .price_variation_indicator = extract_enum<PriceVariationIndicator>(bytes, pos)};
}

RPIIMessage parse_rpii_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .interest_flag = extract_enum<InterestFlag>(bytes, pos)};
}

DirectListingPriceDiscoveryMessage parse_direct_listing_price_discovery_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = extract_header(bytes, pos),
        .symbol = util::extract<Symbol>(bytes, pos),
        .open_eligibility = extract_enum<OpenEligibility>(bytes, pos),
        .min_allowed_price = util::extract_be<std::uint32_t>(bytes, pos),
        .max_allowed_price = util::extract_be<std::uint32_t>(bytes, pos),
        .near_execution_price = util::extract_be<std::uint32_t>(bytes, pos),
        .near_execution_time = util::extract_be<std::uint64_t>(bytes, pos),
        .lower_price_range_collar = util::extract_be<std::uint32_t>(bytes, pos),
        .upper_price_range_collar = util::extract_be<std::uint32_t>(bytes, pos)};
}

}
