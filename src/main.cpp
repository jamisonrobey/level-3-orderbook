#include <cstdio>
#include <print>
#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "book/market.h"
#include "fd/fd.h"
#include "itch/types.h"
#include "util/binary_io.h"
#include "itch/parser.h"

std::optional<FD> create_mcast_socket(std::string_view mcast_group, int port);
void process_packet(std::span<const std::byte> buffer, book::Market& market);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::println(std::cerr, "usage: {} <multicast_group> <port>", argv[0]);
        return 1;
    }

    const auto* mcast_group{argv[1]};
    const int port{std::atoi(argv[2])};

    if (port <= 0 || port > 65535)
    {
        std::println(std::cerr, "invalid port number {}", port);
        return 1;
    }

    const auto sock{create_mcast_socket(mcast_group, port)};
    if (!sock)
    {
        return 1;
    }

    book::Market market{};

    while (1)
    {
        std::byte msgbuf[1500];
        ssize_t nbytes = recvfrom(sock->fd(),
                                  msgbuf,
                                  1500,
                                  0,
                                  nullptr,
                                  nullptr);

        if (nbytes < 0)
        {
            perror("recvfrom");
            return 1;
        }

        process_packet(std::span{msgbuf, static_cast<size_t>(nbytes)}, market);
    }

    return 0;
}

std::optional<FD> create_mcast_socket(std::string_view mcast_group, int port)
{
    FD sock{socket(AF_INET, SOCK_DGRAM, 0)};

    const auto yes{1};
    if (setsockopt(sock.fd(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        std::perror("setsockopt REUSE_ADDR");
        return std::nullopt;
    }

    sockaddr_in addr{.sin_family = AF_INET,
                     .sin_port = htons(static_cast<std::uint16_t>(port)),
                     .sin_addr = {.s_addr = htonl(INADDR_ANY)},
                     .sin_zero = {}};

    if (bind(sock.fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        std::perror("bind");
        return std::nullopt;
    }

    ip_mreq mreq{.imr_multiaddr = {.s_addr = inet_addr(mcast_group.data())},
                 .imr_interface = {.s_addr = htonl(INADDR_ANY)}};

    if (setsockopt(sock.fd(), IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        std::perror("setsockopt IP_ADD_MEMBERSHIP");
        return std::nullopt;
    }

    return sock;
}

using Session = std::array<char, 10>;
struct MoldUDP64Header
{
    Session session;
    std::uint64_t sequence_number;
    std::uint16_t msg_count;
};

void process_packet(std::span<const std::byte> buffer, book::Market& market)
{

    MoldUDP64Header header{};
    std::size_t pos{0};

    header.session = util::extract<Session>(buffer, pos);
    header.sequence_number = util::extract_be<std::uint64_t>(buffer, pos);
    header.msg_count = util::extract_be<std::uint16_t>(buffer, pos);

    for (std::size_t i = 0; i < header.msg_count; ++i)
    {
        if (pos + 2 > buffer.size())
        {
            std::println(std::cerr, "Malformed packet (missing len but should have more messages)");
            break;
        }

        const auto msg_len{util::extract_be<std::uint16_t>(buffer, pos)};
        const auto msg_type{static_cast<itch::MessageType>(buffer[pos++])};

        const auto msg_bytes{buffer.subspan(pos, msg_len - 1)};
        switch (msg_type)
        {
        case itch::MessageType::SystemEvent:
            itch::parse_system_event_message(msg_bytes);
            break;
        case itch::MessageType::StockDirectory:
            itch::parse_stock_directory_message(msg_bytes);
            break;
        case itch::MessageType::StockTradingAction:
            itch::parse_stock_trading_action_message(msg_bytes);
            break;
        case itch::MessageType::RegSHORestriction:
            itch::parse_reg_sho_restriction_message(msg_bytes);
            break;
        case itch::MessageType::MarketParticipantPosition:
            itch::parse_market_participant_position_message(msg_bytes);
            break;
        case itch::MessageType::MWCBDeclineLevel:
            itch::parse_mwcb_decline_level_message(msg_bytes);
            break;
        case itch::MessageType::MWCBStatus:
            itch::parse_mwcb_status_message(msg_bytes);
            break;
        case itch::MessageType::IPOQuotingPeriodUpdate:
            itch::parse_ipo_quoting_period_update_message(msg_bytes);
            break;
        case itch::MessageType::LULDAuctionCollar:
            itch::parse_luld_auction_collar_message(msg_bytes);
            break;
        case itch::MessageType::OperationalHalt:
            itch::parse_operational_halt_message(msg_bytes);
            break;
        case itch::MessageType::AddOrder: {
            const auto msg{itch::parse_add_order_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .add(msg.order_reference_number, msg.shares, msg.price, msg.side);
            break;
        }
        case itch::MessageType::AddOrderMPID: {
            const auto msg{itch::parse_add_order_mpid_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .add(msg.order_reference_number, msg.shares, msg.price, msg.side);
            break;
        }
        case itch::MessageType::OrderExecuted: {
            const auto msg{itch::parse_order_executed_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .reduce(msg.order_reference_number, msg.executed_shares);
            break;
        }
        case itch::MessageType::OrderExecutedWithPrice: {
            const auto msg{itch::parse_order_executed_with_price_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .reduce(msg.order_reference_number, msg.executed_shares);
            break;
        }
        case itch::MessageType::OrderCancel: {
            const auto msg{itch::parse_order_cancel_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .reduce(msg.order_reference_number, msg.canceled_shares);
            break;
        }
        case itch::MessageType::OrderDelete: {
            const auto msg{itch::parse_order_delete_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .remove(msg.order_reference_number);
            break;
        }
        case itch::MessageType::OrderReplace: {
            const auto msg{itch::parse_order_replace_message(msg_bytes)};
            market.get_book(msg.header.stock_locate)
                .replace(msg);
            break;
        }
        case itch::MessageType::Trade:
            itch::parse_trade_message(msg_bytes);
            break;
        case itch::MessageType::CrossTrade:
            itch::parse_cross_trade_message(msg_bytes);
            break;
        case itch::MessageType::BrokenTrade:
            itch::parse_broken_trade_message(msg_bytes);
            break;
        case itch::MessageType::NOII:
            itch::parse_noii_message(msg_bytes);
            break;
        case itch::MessageType::RPII:
            itch::parse_rpii_message(msg_bytes);
            break;
        case itch::MessageType::DirectListingPriceDiscovery:
            itch::parse_direct_listing_price_discovery_message(msg_bytes);
            break;
        default:
            std::println(std::cerr, "Unknown message type: {}", static_cast<char>(msg_type));
            break;
        }
        pos += msg_len - 1;
    }
}
