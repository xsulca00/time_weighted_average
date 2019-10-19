#pragma once

#include <ostream>
#include <set>
#include "Utils.h"

namespace wargaming {
	using namespace std;

	// Stores important information about order
	struct Order {
		Order(utils::TimePointMs tp, int iid, multiset<double>::iterator it)
			: timepoint{ tp }, id{ iid }, priceIt{ it } {}

		utils::TimePointMs timepoint;
		int id{ -1 };
		// iterator into max prices cache for faster erase
		multiset<double>::iterator priceIt;
	};

}