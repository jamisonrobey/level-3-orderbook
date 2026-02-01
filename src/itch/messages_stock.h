#ifndef ITCH_MESSAGES_STOCK_H_
#define ITCH_MESSAGES_STOCK_H_

#include "types.h"
#include <cstdint>

namespace itch
{

enum class MarketCategory : char
{
    NasdaqGlobalSelect = 'Q',
    NasdaqGlobalMarket = 'G',
    NasdaqCapitalMarket = 'S',
    NYSE = 'N',
    NYSEAmerican = 'A',
    NYSEArca = 'P',
    BATS = 'Z',
    IEX = 'V',
    NotAvailable = ' '
};

enum class FinancialStatus : char
{
    Deficient = 'D',
    Delinquent = 'E',
    Bankrupt = 'Q',
    Suspended = 'S',
    DeficientAndBankrupt = 'G',
    DeficientAndDelinquent = 'H',
    DeficientDelinquentAndBankrupt = 'K',
    CreationsSuspended = 'C',
    Normal = 'N'
};

enum class IssueClassification : char
{
    AmericanDepositaryShare = 'A',
    Bond = 'B',
    CommonStock = 'C',
    DepositoryReceipt = 'F',
    Rule144A = 'I',
    LimitedPartnership = 'L',
    Notes = 'N',
    OrdinaryShare = 'O',
    PreferredStock = 'P',
    OtherSecurities = 'Q',
    Right = 'R',
    SharesOfBeneficialInterest = 'S',
    ConvertibleDebenture = 'T',
    Unit = 'U',
    UnitsBeneficialInterest = 'V',
    Warrant = 'W'
};

consteval std::uint16_t pack2(char c1, char c2 = ' ')
{
    return static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(c1) |
        (static_cast<std::uint16_t>(c2) << 8));
}

consteval std::uint32_t pack4(char c1, char c2 = ' ', char c3 = ' ', char c4 = ' ')
{
    return static_cast<std::uint32_t>(
        static_cast<std::uint32_t>(c1) |
        (static_cast<std::uint32_t>(c2) << 8) |
        (static_cast<std::uint32_t>(c3) << 16) |
        (static_cast<std::uint32_t>(c4) << 24));
}

enum class IssueSubType : std::uint16_t
{
    PreferredTrustSecurities = pack2('A'),
    IndexBasedDerivative = pack2('B'),
    CommonShares = pack2('C'),
    GlobalDepositaryShares = pack2('D'),
    ETFPortfolioDepositaryReceipt = pack2('E'),
    HOLDRS = pack2('F'),
    GlobalShares = pack2('G'),
    ETFIndexFundShares = pack2('I'),
    CorporateBackedTrustSecurity = pack2('J'),
    ContingentLitigationRight = pack2('L'),
    EquityBasedDerivative = pack2('M'),
    NYRegistryShares = pack2('N'),
    OpenEndMutualFund = pack2('O'),
    PrivatelyHeldSecurity = pack2('P'),
    ClosedEndFund = pack2('Q'),
    RegS = pack2('R'),
    SEED = pack2('S'),
    TrackingStock = pack2('T'),
    Portal = pack2('U'),
    ContingentValueRight = pack2('V'),
    TrustIssuedReceipts = pack2('W'),
    Trust = pack2('X'),
    Other = pack2('Y'),
    NotApplicable = pack2('Z'),
    AlphaIndexETN = pack2('A', 'I'),
    CommodityBasedTrustShares = pack2('C', 'B'),
    CommodityFuturesTrustShares = pack2('C', 'F'),
    CommodityLinkedSecurities = pack2('C', 'L'),
    CommodityIndexTrustShares = pack2('C', 'M'),
    CollateralizedMortgageObligation = pack2('C', 'O'),
    CurrencyTrustShares = pack2('C', 'T'),
    CommodityCurrencyLinkedSecurities = pack2('C', 'U'),
    CurrencyWarrants = pack2('C', 'W'),
    EquityGoldShares = pack2('E', 'G'),
    ETNEquityIndexLinked = pack2('E', 'I'),
    NextShares = pack2('E', 'M'),
    ExchangeTradedNotes = pack2('E', 'N'),
    EquityUnits = pack2('E', 'U'),
    ETNFixedIncomeLinked = pack2('F', 'I'),
    ETNFuturesLinked = pack2('F', 'L'),
    InterestRate = pack2('I', 'R'),
    IndexWarrant = pack2('I', 'W'),
    IndexLinkedExchangeableNotes = pack2('I', 'X'),
    LimitedLiabilityCompany = pack2('L', 'L'),
    ManagedFundShares = pack2('M', 'F'),
    ETNMultiFactorIndexLinked = pack2('M', 'L'),
    ManagedTrustSecurities = pack2('M', 'T'),
    PoisonPill = pack2('P', 'P'),
    PartnershipUnits = pack2('P', 'U'),
    CommodityRedeemableCommodityLinked = pack2('R', 'C'),
    ETNRedeemableFuturesLinked = pack2('R', 'F'),
    REIT = pack2('R', 'T'),
    CommodityRedeemableCurrencyLinked = pack2('R', 'U'),
    SpotRateClosing = pack2('S', 'C'),
    SpotRateIntraday = pack2('S', 'I'),
    TrustCertificates = pack2('T', 'C'),
    TrustUnits = pack2('T', 'U'),
    WorldCurrencyOption = pack2('W', 'C')
};

enum class Authenticity : char
{
    Production = 'P',
    Test = 'T'
};

enum class ShortSaleThresholdIndicator : char
{
    Restricted = 'Y',
    NotRestricted = 'N',
    NotAvailable = ' '
};

enum class IPOFlag : char
{
    IsNewIPO = 'Y',
    NotNewIPO = 'N',
    NotAvailable = ' '
};

enum class LULDReferencePriceTier : char
{
    Tier1 = '1',
    Tier2 = '2',
    NotAvailable = ' '
};

enum class ETPFlag : char
{
    IsETP = 'Y',
    NotETP = 'N',
    NotAvailable = ' '
};

enum class InverseIndicator : char
{
    IsInverse = 'Y',
    NotInverse = 'N'
};

enum class RoundLotsOnly : char
{
    Yes = 'Y',
    No = 'N'
};

struct StockDirectoryMessage
{
    MessageHeader header;
    Symbol symbol;
    MarketCategory market_category;
    FinancialStatus financial_status;
    std::uint32_t round_lot_size;
    RoundLotsOnly round_lots_only;
    IssueClassification issue_classification;
    IssueSubType issue_sub_type;
    Authenticity authenticity;
    ShortSaleThresholdIndicator short_sale_threshold;
    IPOFlag ipo_flag;
    LULDReferencePriceTier luld_reference_price_tier;
    ETPFlag etp_flag;
    std::uint32_t etp_leverage_factor;
    InverseIndicator inverse_indicator;
};

enum class TradingState : char
{
    Halted = 'H',
    Paused = 'P',
    QuotationOnly = 'Q',
    Trading = 'T'
};

enum class TradingStateReason : std::uint32_t
{
    HaltNewsPending = pack4('T', '1'),
    HaltNewsDisseminated = pack4('T', '2'),
    PauseSingleSecurity = pack4('T', '5'),
    HaltRegulatory = pack4('T', '6'),
    HaltETF = pack4('T', '8'),
    HaltTrading = pack4('T', '1', '2'),
    HaltNonCompliance = pack4('H', '4'),
    HaltFilingsNotCurrent = pack4('H', '9'),
    HaltSECSuspension = pack4('H', '1', '0'),
    HaltRegulatoryConcern = pack4('H', '1', '1'),
    HaltOperational = pack4('O', '1'),
    PauseLULD = pack4('L', 'U', 'D', 'P'),
    HaltMWCBLevel1 = pack4('M', 'W', 'C', '1'),
    HaltMWCBLevel2 = pack4('M', 'W', 'C', '2'),
    HaltMWCBLevel3 = pack4('M', 'W', 'C', '3'),
    HaltMWCBCarryover = pack4('M', 'W', 'C', '0'),
    IPONotYetTrading = pack4('I', 'P', 'O', '1'),
    QuoteCorporateAction = pack4('M', '1'),
    QuoteNotAvailable = pack4('M', '2'),
    ResumeNewsAndTime = pack4('T', '3'),
    QuoteOnlyPause = pack4('T', '7'),
    ResumeQualificationsReviewed = pack4('R', '4'),
    ResumeFilingsSatisfied = pack4('R', '9'),
    ResumeNewsNotForthcoming = pack4('C', '3'),
    ResumeQualificationsEnded = pack4('C', '4'),
    ResumeQualificationsAndFilings = pack4('C', '9'),
    ResumeOtherRegulator = pack4('C', '1', '1'),
    ResumeMWCB = pack4('M', 'W', 'C', 'Q'),
    ReleaseNewIssue = pack4('R', '1'),
    ReleaseIssue = pack4('R', '2'),
    ReleaseIPOQuotation = pack4('I', 'P', 'O', 'Q'),
    ExtendIPOPositioning = pack4('I', 'P', 'O', 'E'),
    ReasonNotAvailable = pack4(' ')
};

struct StockTradingActionMessage
{
    MessageHeader header;
    Symbol symbol;
    TradingState trading_state;
    std::byte reserved;
    TradingStateReason reason;
};

enum class RegSHOAction : char
{
    NoRestriction = '0',
    RestrictedDueToIntradayDrop = '1',
    RestrictedRemainsInEffect = '2'
};

struct RegSHORestrictionMessage
{
    MessageHeader header;
    Symbol symbol;
    RegSHOAction action;
};

enum class PrimaryMarketMaker : char
{
    Yes = 'Y',
    No = 'N'
};

enum class MarketMakerMode : char
{
    Normal = 'N',
    Passive = 'P',
    Syndicate = 'S',
    PreSyndicate = 'R',
    Penalty = 'L'
};

enum class MarketParticipantState : char
{
    Active = 'A',
    Excused = 'E',
    Withdrawn = 'W',
    Suspended = 'S',
    Deleted = 'D'
};

struct MarketParticipantPositionMessage
{
    MessageHeader header;
    MPID attribution;
    Symbol symbol;
    PrimaryMarketMaker primary_market_maker;
    MarketMakerMode market_maker_mode;
    MarketParticipantState participant_state;
};

struct MWCBDeclineLevelMessage
{
    MessageHeader header;
    std::uint64_t level_1;
    std::uint64_t level_2;
    std::uint64_t level_3;
};

enum class MWCBLevel : char
{
    Level1 = '1',
    Level2 = '2',
    Level3 = '3'
};

struct MWCBStatusMessage
{
    MessageHeader header;
    MWCBLevel breached_level;
};

enum class IPOQuotationReleaseQualifier : char
{
    Anticipated = 'A',
    CanceledOrPostponed = 'C'
};

struct IPOQuotingPeriodUpdateMessage
{
    MessageHeader header;
    Symbol symbol;
    std::uint32_t quotation_release_time;
    IPOQuotationReleaseQualifier release_qualifier;
    std::uint32_t ipo_price;
};

struct LULDAuctionCollarMessage
{
    MessageHeader header;
    Symbol symbol;
    std::uint32_t reference_price;
    std::uint32_t upper_price;
    std::uint32_t lower_price;
    std::uint32_t extension_number;
};

enum class MarketCode : char
{
    Nasdaq = 'Q',
    BX = 'B',
    PSX = 'X'
};

enum class OperationalHaltAction : char
{
    Halted = 'H',
    Resumed = 'T'
};

struct OperationalHaltMessage
{
    MessageHeader header;
    Symbol symbol;
    MarketCode market_code;
    OperationalHaltAction action;
};

}

#endif
