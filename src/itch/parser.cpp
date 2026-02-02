#include "parser.h"
#include "messages_auction.h"
#include "messages_orders.h"
#include "messages_stock.h"

#include <cstring>
#include <bit>

namespace
{

template <typename T>
T read(std::span<const std::byte> bytes, std::size_t& pos)
{
    T value{};
    std::memcpy(&value, &bytes[pos], sizeof(T));
    pos += sizeof(T);
    return value;
}

template <typename T>
T read_be(std::span<const std::byte> bytes, std::size_t& pos)
{
    auto value{read<T>(bytes, pos)};
    value = std::byteswap(value);
    return value;
}

template <typename T>
T read_enum(std::span<const std::byte> bytes, std::size_t& pos)
{
    return static_cast<T>(read<std::underlying_type_t<T>>(bytes, pos));
}

itch::MessageHeader read_header(std::span<const std::byte> bytes, std::size_t& pos)
{
    itch::MessageHeader header{};
    header.stock_locate = read_be<std::uint16_t>(bytes, pos);
    header.tracking_number = read_be<std::uint16_t>(bytes, pos);

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
        .header = read_header(bytes, pos),
        .event_code = read_enum<EventCode>(bytes, pos)};
}

StockDirectoryMessage parse_stock_directory_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .market_category = read_enum<MarketCategory>(bytes, pos),
        .financial_status = read_enum<FinancialStatus>(bytes, pos),
        .round_lot_size = read_be<std::uint32_t>(bytes, pos),
        .round_lots_only = read_enum<RoundLotsOnly>(bytes, pos),
        .issue_classification = read_enum<IssueClassification>(bytes, pos),
        .issue_sub_type = read_enum<IssueSubType>(bytes, pos),
        .authenticity = read_enum<Authenticity>(bytes, pos),
        .short_sale_threshold = read_enum<ShortSaleThresholdIndicator>(bytes, pos),
        .ipo_flag = read_enum<IPOFlag>(bytes, pos),
        .luld_reference_price_tier = read_enum<LULDReferencePriceTier>(bytes, pos),
        .etp_flag = read_enum<ETPFlag>(bytes, pos),
        .etp_leverage_factor = read_be<std::uint32_t>(bytes, pos),
        .inverse_indicator = read_enum<InverseIndicator>(bytes, pos)};
}

StockTradingActionMessage parse_stock_trading_action_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .trading_state = read_enum<TradingState>(bytes, pos),
        .reserved = read<std::byte>(bytes, pos),
        .reason = read_enum<TradingStateReason>(bytes, pos)};
}

RegSHORestrictionMessage parse_reg_sho_restriction_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .action = read_enum<RegSHOAction>(bytes, pos)};
}

MarketParticipantPositionMessage parse_market_participant_position_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .attribution = read<MPID>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .primary_market_maker = read_enum<PrimaryMarketMaker>(bytes, pos),
        .market_maker_mode = read_enum<MarketMakerMode>(bytes, pos),
        .participant_state = read_enum<MarketParticipantState>(bytes, pos)};
}

MWCBDeclineLevelMessage parse_mwcb_decline_level_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .level_1 = read_be<std::uint64_t>(bytes, pos),
        .level_2 = read_be<std::uint64_t>(bytes, pos),
        .level_3 = read_be<std::uint64_t>(bytes, pos)};
}

MWCBStatusMessage parse_mwcb_status_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .breached_level = read_enum<MWCBLevel>(bytes, pos)};
}

IPOQuotingPeriodUpdateMessage parse_ipo_quoting_period_update_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .quotation_release_time = read_be<std::uint32_t>(bytes, pos),
        .release_qualifier = read_enum<IPOQuotationReleaseQualifier>(bytes, pos),
        .ipo_price = read_be<std::uint32_t>(bytes, pos)};
}

LULDAuctionCollarMessage parse_luld_auction_collar_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .reference_price = read_be<std::uint32_t>(bytes, pos),
        .upper_price = read_be<std::uint32_t>(bytes, pos),
        .lower_price = read_be<std::uint32_t>(bytes, pos),
        .extension_number = read_be<std::uint32_t>(bytes, pos)};
}

OperationalHaltMessage parse_operational_halt_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .market_code = read_enum<MarketCode>(bytes, pos),
        .action = read_enum<OperationalHaltAction>(bytes, pos)};
}

AddOrderMessage parse_add_order_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .side = read_enum<Side>(bytes, pos),
        .shares = read_be<std::uint32_t>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .price = read_be<std::uint32_t>(bytes, pos)};
}

AddOrderMPIDMessage parse_add_order_mpid_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .side = read_enum<Side>(bytes, pos),
        .shares = read_be<std::uint32_t>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .price = read_be<std::uint32_t>(bytes, pos),
        .attribution = read<MPID>(bytes, pos)};
}

OrderExecutedMessage parse_order_executed_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .executed_shares = read_be<std::uint32_t>(bytes, pos),
        .match_number = read_be<std::uint64_t>(bytes, pos)};
}

OrderExecutedWithPriceMessage parse_order_executed_with_price_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .executed_shares = read_be<std::uint32_t>(bytes, pos),
        .match_number = read_be<std::uint64_t>(bytes, pos),
        .printable = read_enum<Printable>(bytes, pos),
        .execution_price = read_be<std::uint32_t>(bytes, pos)};
}

OrderCancelMessage parse_order_cancel_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .canceled_shares = read_be<std::uint32_t>(bytes, pos)};
}

OrderDeleteMessage parse_order_delete_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos)};
}

OrderReplaceMessage parse_order_replace_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .original_order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .new_order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .shares = read_be<std::uint32_t>(bytes, pos),
        .price = read_be<std::uint32_t>(bytes, pos)};
}

TradeMessage parse_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .order_reference_number = read_be<std::uint64_t>(bytes, pos),
        .side = read_enum<Side>(bytes, pos),
        .shares = read_be<std::uint32_t>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .price = read_be<std::uint32_t>(bytes, pos),
        .match_number = read_be<std::uint64_t>(bytes, pos)};
}

CrossTradeMessage parse_cross_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .shares = read_be<std::uint64_t>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .cross_price = read_be<std::uint32_t>(bytes, pos),
        .match_number = read_be<std::uint64_t>(bytes, pos),
        .type = read_enum<CrossType>(bytes, pos)};
}

BrokenTradeMessage parse_broken_trade_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .match_number = read_be<std::uint64_t>(bytes, pos)};
}

NOIIMessage parse_noii_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .paired_shares = read_be<std::uint64_t>(bytes, pos),
        .imbalance_shares = read_be<std::uint64_t>(bytes, pos),
        .imbalance_direction = read_enum<ImbalanceDirection>(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .far_price = read_be<std::uint32_t>(bytes, pos),
        .near_price = read_be<std::uint32_t>(bytes, pos),
        .current_reference_price = read_be<std::uint32_t>(bytes, pos),
        .cross_type = read_enum<CrossType>(bytes, pos),
        .price_variation_indicator = read_enum<PriceVariationIndicator>(bytes, pos)};
}

RPIIMessage parse_rpii_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .interest_flag = read_enum<InterestFlag>(bytes, pos)};
}

DirectListingPriceDiscoveryMessage parse_direct_listing_price_discovery_message(std::span<const std::byte> bytes)
{
    std::size_t pos{0};
    return {
        .header = read_header(bytes, pos),
        .symbol = read<Symbol>(bytes, pos),
        .open_eligibility = read_enum<OpenEligibility>(bytes, pos),
        .min_allowed_price = read_be<std::uint32_t>(bytes, pos),
        .max_allowed_price = read_be<std::uint32_t>(bytes, pos),
        .near_execution_price = read_be<std::uint32_t>(bytes, pos),
        .near_execution_time = read_be<std::uint64_t>(bytes, pos),
        .lower_price_range_collar = read_be<std::uint32_t>(bytes, pos),
        .upper_price_range_collar = read_be<std::uint32_t>(bytes, pos)};
}

}
