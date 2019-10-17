#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include "TimeWeightedAverage.h"

namespace wargaming {

	double OrderBook::GetCurrentMaxPrice() const {
		if (orders.empty()) throw runtime_error{ "OrderBook::GetCurrentMaxPrice(): no inserted orders" };

		double d{ orders.begin()->second.price };
		for (const auto& [id, order] : orders) {
			if (order.price > d) d = order.price;
		}
		return d;
	}

	void OrderBook::Insert(const Order& o) {
		auto [_, inserted] {orders.emplace(o.id, o)};
		if (!inserted) throw runtime_error{ "OrderBook::Insert(): order already inserted" };
	}

	void OrderBook::Erase(int id) {
		if (orders.erase(id) != 1) {
			throw runtime_error{ "OrderBook::Erase(): cannot erase order" };
		}
	}

	Order OrderBook::GetOrder(int id) {
		auto it{ orders.find(id) };
		if (it == orders.end()) throw runtime_error{ "OrderBook::GetOrder(): id not found" };
		return it->second;
	}

	void Stats::Update(utils::TimePointMs tp, double price) {
		auto dur{ tp - current };
		sum += dur;
		result += dur.count() * price;
		current = tp;
	}

	void TimeWeightedAverage::Insert(const Order& o) {
		if (o.price == 0) return;
		if (o.price < 0) throw runtime_error{ "TimeWeightedAverage::Insert(): price < 0" };

		if (orders.IsEmpty()) {
			s.current = o.when;
		}
		else if (double pMax{ orders.GetCurrentMaxPrice() };  pMax < o.price) {
			s.Update(o.when, pMax);
		}

		orders.Insert(o);

		cout << "Order: " << o << '\n';
		cout << "I Current: " << s.current << '\n';
		cout << "I Sum: " << s.sum << '\n';
	}

	void TimeWeightedAverage::Erase(const Order& o) {
		double price { orders.GetOrder(o.id).price };
		orders.Erase(o.id);

		cout << "Erasing: " << o << '\n';

		if (orders.IsEmpty() || price > orders.GetCurrentMaxPrice()) {
			s.Update(o.when, price);
			cout << "E Current: " << s.current << '\n';
			cout << "E Sum: " << s.sum << '\n';
		}
	}

	void ReadOrdersAndStore(ifstream& file, TimeWeightedAverage& avg) {
		for (string s; getline(file, s);) {
			Order o;
			istringstream ss{ s };
			string op;
			if (ss >> o.when >> op >> o.id) {
				if (ss >> o.price) {
					o.insert = (op == "I");
				}

				if (o.insert) {
					avg.Insert(o);
				}
				else {
					avg.Erase(o);
				}
			}
		}
	}

}
