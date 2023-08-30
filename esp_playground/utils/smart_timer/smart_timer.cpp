/**
 *
 */
#include <chrono>
#include <cinttypes>
#include <cstdio>

#include "smart_timer.hpp"

namespace CTCI {
/* constructor */
smart_timer::smart_timer(std::string name) : _name(name)
{
    _start = std::chrono::steady_clock::now();
}

smart_timer::~smart_timer()
{
    std::chrono::time_point<std::chrono::steady_clock> _end = std::chrono::steady_clock::now();

    auto time_elapsed = _end - _start;

    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(time_elapsed).count();

    printf("%s took %" PRIu64 " us \n", _name.c_str(), us);
}

} // namespace CTCI