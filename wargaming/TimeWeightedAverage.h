#pragma once

#include <map>
#include <chrono>
#include "Utils.h"
#include "Order.h"

namespace wargaming {
	using namespace std;
	using namespace std::chrono;

	class OrderBook {
	public:
		double GetCurrentMaxPrice() const;
		void Insert(const Order& o);
		void Erase(int id);
		Order GetOrder(int id);
		bool IsEmpty() const { return orders.empty(); }
	private:
		map<int, Order> orders;
	};

	class Stats {
	public:
		void Update(utils::TimePointMs tp, double price);
		double GetResult() const { return (result == 0) ? 0 : result/sum.count(); }

		milliseconds sum{ 0 };
		double result{ 0 };
		utils::TimePointMs current;
	};

	class TimeWeightedAverage {
	public:
		void Insert(const Order& o);
		void Erase(const Order& o);
		Stats GetStats() const { return s; }
	private:
		Stats s;
		OrderBook orders;
	};

	void ReadOrdersAndStore(ifstream& file, TimeWeightedAverage& avg);

}
