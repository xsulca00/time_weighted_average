#pragma once

#include <chrono>
#include <iostream>

namespace wargaming::utils {
	using namespace std::chrono;

	using TimePointMs = time_point<system_clock, milliseconds>;	
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

	using wargaming::utils::TimePointMs;

	inline istream& operator>>(istream& is, TimePointMs& tp) {
		milliseconds m{ 0 };
		is >> m;
		tp = TimePointMs{ m };
		return is;
	}

	inline ostream& operator<<(ostream& os, TimePointMs tp) {
		return os << tp.time_since_epoch();
	}

}
