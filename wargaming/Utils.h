#pragma once

#include <chrono>
#include <iostream>

namespace wargaming::utils {
	using namespace std::chrono;

	using TimePointMs = time_point<system_clock, milliseconds>;	
}

namespace std::chrono {
	using wargaming::utils::TimePointMs;

	inline istream& operator>>(istream& is, TimePointMs& tp) {
		TimePointMs::duration::rep mrep;
		is >> mrep;
		tp = TimePointMs{ milliseconds{ mrep } };
		return is;
	}

	inline ostream& operator<<(ostream& os, TimePointMs tp) {
		return os << tp.time_since_epoch().count() << " ms";
	}

}
