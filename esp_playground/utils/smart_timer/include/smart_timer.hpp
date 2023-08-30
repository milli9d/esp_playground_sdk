/**
 *
 */
#pragma once

#include <chrono>
#include <cstdio>
#include <string>

namespace CTCI {
class smart_timer
{
  public:
    /* constructors */
    smart_timer(std::string name);
    ~smart_timer();
    smart_timer() = delete;
    smart_timer(const smart_timer&) = delete;
    smart_timer(const smart_timer&&) = delete;
    smart_timer& operator=(const smart_timer&) = delete;
    smart_timer& operator=(const smart_timer&&) = delete;

  private:
    const std::string _name{};
    std::chrono::time_point<std::chrono::steady_clock> _start{};
};

} // namespace CTCI