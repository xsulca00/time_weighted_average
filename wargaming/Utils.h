#pragma once

#include <chrono>
#include <iostream>

namespace wargaming::utils {
	using namespace std::chrono;

	// Steady clock is suitable for difference between timepoints
	using TimePointMs = time_point<steady_clock, milliseconds>;	
}

namespace std::chrono {

	inline istream& operator>>(istream& is, milliseconds& m) {
		milliseconds::rep mrep;
		is >> mrep;
		m = milliseconds{ mrep };
		return is;
	}

	inline ostream& operator<<(ostream& os, milliseconds m) {
		return os << m.count() << " ms";
	}

	// Input from stream (file, string...)
	inline istream& operator>>(istream& is, wargaming::utils::TimePointMs& tp) {
		milliseconds m{ 0 };
		is >> m;
		tp = wargaming::utils::TimePointMs{ m };
		return is;
	}

	// Output to stream (file, string...)
	inline ostream& operator<<(ostream& os, wargaming::utils::TimePointMs tp) {
		return os << tp.time_since_epoch();
	}

}
