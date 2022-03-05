//
// Created by saso on 3/5/22.
//

#ifndef WATERPIPES_EXECUTIONTIMER_H
#define WATERPIPES_EXECUTIONTIMER_H

#include <type_traits>
#include <chrono>
#include <iostream>
#include <sstream>

template<class Resolution = std::chrono::milliseconds>
class ExecutionTimer {
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
            std::chrono::high_resolution_clock,
            std::chrono::steady_clock>;
private:
    const Clock::time_point mStart = Clock::now();

public:
    ExecutionTimer() = default;

    ~ExecutionTimer() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << std::chrono::duration_cast<Resolution>(end - mStart).count()
                  << std::endl;
        std::cout << strStream.str() << std::endl;
    }

    inline void stop() {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "\n Stop Elapsed: "
                  << std::chrono::duration_cast<Resolution>(end - mStart).count()
                  << std::endl;
        std::cout << strStream.str() << std::endl;
    }

}; // ExecutionTimer

#endif //WATERPIPES_EXECUTIONTIMER_H
