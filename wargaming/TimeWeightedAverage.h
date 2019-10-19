#pragma once

#include <map>
#include <set>
#include <chrono>
#include "Utils.h"
#include "Order.h"

namespace wargaming {
	using namespace std;
	using namespace std::chrono;

	using OrderMap = map<int, Order>;

	// Stores information about orders, supports optimized operations
	class OrderBook {
	public:
		double GetCurrentMaxPrice() const;
		// Order to insert contains timestamp, id and price
		void Insert(utils::TimePointMs when, int id, double price);
		// Order to erase contains timestamp and id
		void Erase(OrderMap::iterator it);
		// Returns iterator to stored order
		OrderMap::iterator GetOrder(int id);
		bool IsEmpty() const { return orders.empty(); }
	private:
		// Stores order prices
		multiset<double> maxPrices;
		// Stores important data for subsequent calculations
		OrderMap orders;
	};

	// Calculates time-weighted average highest price of orders.
	// Average is calculated incrementally and depends on order insert and its erase.
	// First an order must be inserted and then must be erased with dependence:
	// order insertion timepoint < order erasure timepoint
	class TimeWeightedAverage {
	public:
		// Order to insert contains timestamp, id and price
		void Insert(utils::TimePointMs when, int id, double price);
		// Order to erase contains timestamp and id
		void Erase(utils::TimePointMs when, int id);
		double GetAverage() const { return (result == 0) ? 0 : result / sum.count(); }
	private:
		double EraseOrder(int id);
		// Calculates time-weighted average increment
		void Update(utils::TimePointMs tp, double price);

		// Sum of all durations
		milliseconds sum{ 0 };
		// Addition of all interval durations multiplied by its highest price
		double result{ 0 };
		// Begin of interval with highest price
		utils::TimePointMs current;
		OrderBook orders;
	};

	// Parses orders from file and inserts them into time-weighted average
	void ReadOrdersAndStore(ifstream& file, TimeWeightedAverage& avg);

}
