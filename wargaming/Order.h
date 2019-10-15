#pragma once

#include <ostream>
#include "Utils.h"

namespace wargaming {
	using std::ostream;

	struct Order {
		utils::TimePointMs when;
		bool  insert{ false };
		int id{ -1 };
		double price{ 0 };
	};

	inline ostream& operator<<(ostream& os, const Order& o) {
		return os << "{ " << o.when << ' ' << o.insert << ' ' << o.id << ' ' << o.price << " }";
	}

}