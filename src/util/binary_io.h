#ifndef BINARY_IO_H_
#define BINARY_IO_H_

#include <span>
#include <cstring>

namespace util
{
template <typename T>
T extract(std::span<const std::byte> bytes, std::size_t& pos)
{
    T value{};
    std::memcpy(&value, &bytes[pos], sizeof(T));
    pos += sizeof(T);
    return value;
}

template <typename T>
T extract_be(std::span<const std::byte> bytes, std::size_t& pos)
{
    auto value{extract<T>(bytes, pos)};
    value = std::byteswap(value);
    return value;
}

}

#endif
