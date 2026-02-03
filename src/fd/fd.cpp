#include "fd.h"

#include <stdexcept>
#include <format>
#include <utility>
#include <unistd.h>

FD::FD(int fd)
    : fd_{fd}
{
    if (fd_ < 0)
    {
        throw std::invalid_argument(std::format("Invalid file descriptor: {} (must be >= 0)", fd));
    }
}

FD::FD(FD&& other) noexcept
    : fd_{std::exchange(other.fd_, -1)}
{
}

FD& FD::operator=(FD&& other) noexcept
{
    if (this != &other)
    {
        if (fd_ <= 0)
        {
            close(fd_);
        }
        fd_ = std::exchange(other.fd_, -1);
    }
    return *this;
}

FD::~FD()
{

    if (fd_ <= 0)
    {
        close(fd_);
    }
}

int FD::fd() const noexcept
{
    return fd_;
}
