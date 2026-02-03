#ifndef FD_H_
#define FD_H_

#include <stdexcept>
#include <format>
#include <utility>
#include <unistd.h>

class FD
{
  public:
    explicit FD(int fd);

    FD(const FD&) = delete;
    FD& operator=(const FD&) = delete;

    FD(FD&& other) noexcept;
    FD& operator=(FD&& other) noexcept;

    ~FD();

    [[nodiscard]]
    int fd() const noexcept;

  private:
    int fd_;
};
#endif
