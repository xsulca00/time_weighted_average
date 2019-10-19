#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include "TimeWeightedAverage.h"

namespace wargaming {

	double OrderBook::GetCurrentMaxPrice() const {
		if (maxPrices.empty()) throw runtime_error{ "OrderBook::GetCurrentMaxPrice(): no inserted orders" };
		// maxPrices.rbegin() always contains the highest price, if not empty
		return *maxPrices.rbegin();
	}

	void OrderBook::Insert(utils::TimePointMs when, int id, double price) {
		// Stores order price in cache for faster lookup of the current max price
		auto [it, inserted] {orders.try_emplace(id, when, id, maxPrices.emplace(price))};
		if (!inserted) throw runtime_error{ "OrderBook::Insert(): order already inserted" };
	}

	void OrderBook::Erase(OrderMap::iterator it) {
		if (it != orders.end()) {
			maxPrices.erase(it->second.priceIt);
			orders.erase(it);
		}
	}

	OrderMap::iterator OrderBook::GetOrder(int id) {
		auto it{ orders.find(id) };
		if (it == orders.end()) throw runtime_error{ "OrderBook::GetOrder(): id not found" };
		return it;
	}

	void TimeWeightedAverage::Update(utils::TimePointMs tp, double price) {
		auto dur{ tp - current };
		sum += dur;
		result += dur.count() * price;
		current = tp;
	}

	void TimeWeightedAverage::Insert(utils::TimePointMs when, int id, double price) {
		// Allow zero cost orders - for example some product for free as bonus
		// if (o.price == 0) return;

		if (price < 0) throw runtime_error{ "TimeWeightedAverage::Insert(): price < 0" };

		// Set current highest price interval for first order
		if (orders.IsEmpty()) {
			current = when;
		}
		else if (double pMax{ orders.GetCurrentMaxPrice() };  pMax < price) {
			// Highest price changed, update time-weighted average and change
			// current begin of the highest price interval
			Update(when, pMax);
		}

		orders.Insert(when, id, price);

		cout << "I Current: " << current << '\n';
		cout << "I Result: " << result << '\n';
		cout << "I Sum: " << sum << '\n';
	}

	double TimeWeightedAverage::EraseOrder(int id) {
		auto it{ orders.GetOrder(id) };
		double price{ *it->second.priceIt };
		orders.Erase(it);
		return price;
	}

	void TimeWeightedAverage::Erase(utils::TimePointMs when, int id) {
		double price{ EraseOrder(id) };

		// Update time-weighted average if current erased order
		// contained highest price
		if (orders.IsEmpty() || price > orders.GetCurrentMaxPrice()) {
			Update(when, price);
			cout << "E Current: " << current << '\n';
			cout << "E Result: " << result << '\n';
			cout << "E Sum: " << sum << '\n';
		}
	}

	void ReadOrdersAndStore(ifstream& file, TimeWeightedAverage& avg) {
		for (string s; getline(file, s);) try {
			utils::TimePointMs when;
			int id{ -1 };
			double price{ -1 };
			string operation;
			// Parse [timepoint, operation, id, price] from the line
			// (price is optional - only for  orders to insert)
			if (istringstream ss{ s }; ss >> when >> operation >> id) {
				if (operation == "I" && ss >> price) {
					avg.Insert(when, id, price);
				}
				else if (operation == "E") {
					avg.Erase(when, id);
				}
				else {
					cerr << "ReadOrdersAndStore(): invalid order operation: \"" 
						 << operation << "\", skipping\n";
				}
			}
		}
		catch (const exception & e) {
			cerr << "Cannot proccess an order: " << e.what() << '\n';
		}
	}

}
