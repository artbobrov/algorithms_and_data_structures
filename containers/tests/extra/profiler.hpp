//
// Created by Artem Bobrov on 05.01.2018.
//

#ifndef COLLECTIONS_PROFILER_HPP
#define COLLECTIONS_PROFILER_HPP

#include <chrono>
#include <thread>

namespace debug {
	class profiler {
	public:
		typedef std::chrono::steady_clock clock;
		typedef clock::time_point time_point;
		typedef std::chrono::duration<double, std::ratio<1, 1>> duration_time;
	private:
		time_point _start;
		time_point _finish;

	public:
		inline void start() { _start = clock::now(); }

		inline void finish() { _finish = clock::now(); }

		inline void clear() { _start = _finish = time_point(); }

		inline void sleep_for(unsigned long long seconds = 0) {
			std::this_thread::sleep_for(std::chrono::seconds(static_cast<std::chrono::seconds::rep>(seconds)));
		}

		inline const double time() const noexcept { // in seconds
			return std::chrono::duration_cast<duration_time>(_finish - _start).count();
		}
	};
}

#endif //COLLECTIONS_PROFILER_HPP
