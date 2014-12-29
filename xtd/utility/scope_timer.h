#pragma once
#include <chrono>
#include <string>

//------------------------------------------------------------------------------

class scope_timer final
{
public: // types

    typedef std::chrono::high_resolution_clock clock;

    typedef clock::time_point time;

    typedef clock::duration duration;

    static time now() { static clock _clock; return _clock.now(); }

private: // fields

    const std::string _name;

    time _start;

public: // structor

    scope_timer(const char* name)
    : _name(name)
    , _start(now())
    {}

    scope_timer(const std::string& name)
    : _name(name)
    , _start(now())
    {}

   ~scope_timer()
    {
        auto ms = double(elapsed_us()) / 1000.0 ;

        printf("| %12.3f ms - %s\n", ms, _name.c_str());
    }

public: // operators

    operator const bool() const { return true; }

public: // methods

    duration elapsed() const
    {
        return now() - _start;
    }

    uint64_t elapsed_us() const
    {
        using namespace std::chrono;

        return duration_cast<microseconds>(elapsed()).count();
    }

};

//------------------------------------------------------------------------------
// usage:
//
//     TIMED_SCOPE("my cool code") { ... }
//
//     TIMED_SCOPE(__func__) { ... }
//

#define TIMED_SCOPE(name) if (scope_timer _scope_timer = name)

//------------------------------------------------------------------------------
// usage:
//
//     TIMED_CALL(myFunction(args));
//

#define TIMED_CALL(call) if (scope_timer _scope_timer = #call) call

//------------------------------------------------------------------------------
