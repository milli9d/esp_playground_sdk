/**
 *
 */
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cinttypes>

#include "smart_timer.hpp"

namespace espp {
/* constructor */
smart_timer::smart_timer(std::string name) : _name(name)
{
    _start = std::chrono::high_resolution_clock::now();
}

smart_timer::~smart_timer()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> _end = std::chrono::high_resolution_clock::now();

    auto time_elapsed = _end - _start;

    uint32_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count();

    printf("%s took%" PRId32 "ms\n", _name.c_str(), ms);
}

} // namespace espp