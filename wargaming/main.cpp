#include <map>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ios>
#include <limits>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace std::chrono {

	inline istream& operator>>(istream& is, milliseconds& m) {
		milliseconds::rep mrep;
		is >> mrep;
		m = milliseconds{ mrep };
		return is;
	}

	inline ostream& operator<<(ostream& is, milliseconds m) {
		return is << m.count();
	}

}

struct Order {
	milliseconds when{ 0 };
	bool  insert{ false };
	int id{ -1 };
	double price{ 0 };
};

inline ostream& operator<<(ostream& os, const Order& o) {
	return os << "{ " << o.when << ' ' << o.insert << ' ' << o.id << ' ' << o.price << " }";
}

class OrderBook {
public:
	double GetCurrentMaxPrice() const {
		if (orders.empty()) throw runtime_error{ "OrderBook::GetCurrentMaxPrice(): no inserted orders" };

		double d{ orders.begin()->second.price };
		for (const auto& [id, order] : orders) {
			if (order.price > d) d = order.price;
		}
		return d;
	}

	void Insert(const Order& o) {
		auto [_, inserted] {orders.emplace(o.id, o)};
		if (!inserted) throw runtime_error{ "OrderBook::Insert(): order already inserted" };
	}

	void Erase(int id) {
		if (orders.erase(id) != 1) {
			throw runtime_error{ "OrderBook::Erase(): cannot erase order" };
		}
	}

	Order GetOrder(int id) { 
		auto it{ orders.find(id) };
		if (it == orders.end()) throw runtime_error{ "OrderBook::GetOrder(): id not found" };
		return it->second;
	}

	bool IsEmpty() const { return orders.empty();  }

private:
	map<int, Order> orders;
};

class Stats {
public:
	void Update(const Order& o, double price) {
		auto dur{ (o.when - current).count() };
		sum += dur;
		result += dur * price;
		current = o.when;
	}

	double GetResult() const { return result / sum;  }

	long long sum{ 0 };
	double result{ 0 };
	milliseconds current{ 0 };
};

class TimeWeightedAverage {
public:
	void Insert(const Order & o) {
		if (orders.IsEmpty()) {
			orders.Insert(o);
			s.current = o.when;
			cout << "Order: " << o << '\n';
			cout << "I Current: " << s.current << '\n';
			cout << "I Sum: " << s.sum << '\n';
		} else {
			if (orders.GetCurrentMaxPrice() < o.price) {
				s.Update(o, orders.GetCurrentMaxPrice());
			}

			orders.Insert(o);

			cout << "Order: " << o << '\n';
			cout << "I Current: " << s.current << '\n';
			cout << "I Sum: " << s.sum << '\n';
		}
	}

	void Erase(const Order& o) {
		Order order{ orders.GetOrder(o.id) };
		orders.Erase(o.id);

		cout << "Erasing: " << o << '\n';

		if (orders.IsEmpty()) {
			s.Update(o, order.price);
			cout << "E Current: " << s.current << '\n';
			cout << "E Sum: " << s.sum << '\n';
		} else if (order.price > orders.GetCurrentMaxPrice()) {
			s.Update(o, order.price);
			cout << "E Current: " << s.current << '\n';
			cout << "E Sum: " << s.sum << '\n';
		}
	}

	Stats GetStats() const { return s; }
private:
	Stats s;
	OrderBook orders;
};

void ReadFromFile(const string& path, TimeWeightedAverage& avg) {
	ifstream file{ path };
	if (!file) throw runtime_error{ "Cannot open file: \"" + path + "\"" };

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

int main(int argc, char* argv[]) try {
	if (argc != 2) throw runtime_error{ "Usage: "s + argv[0] + " input_file_name" };

	const string filename{ argv[1] };

	TimeWeightedAverage average;

	ReadFromFile(filename, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';
	cout << average.GetStats().GetResult() << '\n';
}
catch (const exception & e) {
	cerr << "std::exception caught: " << e.what() << '\n';
}