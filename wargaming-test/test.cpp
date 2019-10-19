#include "pch.h"
#include "../wargaming/TimeWeightedAverage.h"

// OrderBook -- begin

TEST(OrderBook, Empty) {
	using namespace std;
	using namespace wargaming;

	OrderBook b;

	EXPECT_ANY_THROW(b.GetCurrentMaxPrice());
	EXPECT_TRUE(b.IsEmpty());
	EXPECT_ANY_THROW(b.GetOrder(1));
}

TEST(OrderBook, FirstInsert) {
	using namespace std;
	using namespace wargaming;

	OrderBook b;

	constexpr int id{ 9 };

	EXPECT_NO_THROW(b.Insert(utils::TimePointMs{ 2000ms }, id, 20.0));
	EXPECT_DOUBLE_EQ(b.GetCurrentMaxPrice(), 20.0);
	EXPECT_FALSE(b.IsEmpty());
	EXPECT_NO_THROW(b.GetOrder(id));
}

TEST(OrderBook, InsertAndErase) {
	using namespace std;
	using namespace wargaming;

	OrderBook b;

	constexpr int id{ 9 };

	b.Insert(utils::TimePointMs{ 2000ms }, 9, 20.0);

	b.Erase(b.GetOrder(id));

	EXPECT_ANY_THROW(b.GetCurrentMaxPrice());
	EXPECT_TRUE(b.IsEmpty());
	EXPECT_ANY_THROW(b.GetOrder(id));
}

TEST(OrderBook, InsertOrderWithSameIdTwoTimes) {
	using namespace std;
	using namespace wargaming;

	OrderBook b;

	constexpr utils::TimePointMs tp{ 2000ms };
	constexpr int id{ 9 };
	constexpr double price{ 20.0 };

	b.Insert(tp, id, price);
	EXPECT_ANY_THROW(b.Insert(utils::TimePointMs{ 1500ms }, id, 15.0));
	EXPECT_DOUBLE_EQ(b.GetCurrentMaxPrice(), 20.0);
	EXPECT_NO_THROW(b.GetOrder(id));

	auto it{ b.GetOrder(id) };

	Order o{ it->second };

	EXPECT_EQ(o.timepoint, tp);
	EXPECT_EQ(o.id, id);
	EXPECT_EQ(*o.priceIt, price);
}

TEST(OrderBook, HighestOrderPrice) {
	using namespace std;
	using namespace wargaming;

	OrderBook b;

	b.Insert(utils::TimePointMs{ 1500ms }, 9, 15.0);
	b.Insert(utils::TimePointMs{ 2000ms }, 10, 30.0);
	EXPECT_DOUBLE_EQ(b.GetCurrentMaxPrice(), 30.0);
}

// OrderBook -- end
// TimeWeightedAverage -- begin

TEST(TimeWeightedAverage, Empty) {
	using namespace std;
	using namespace wargaming;

	TimeWeightedAverage a;

	EXPECT_DOUBLE_EQ(a.GetAverage(), 0.0);
}

TEST(TimeWeightedAverage, FirstInsert) {
	using namespace std;
	using namespace wargaming;

	TimeWeightedAverage a;

	constexpr utils::TimePointMs tp{ 2000ms };
	constexpr int id{ 9 };
	constexpr double price{ 20.0 };

	EXPECT_NO_THROW(a.Insert(tp, id, price));
	EXPECT_NO_THROW(a.Erase(tp+200ms, id));
	EXPECT_DOUBLE_EQ(a.GetAverage(), price);
}

TEST(TimeWeightedAverage, InsertNegativePrice) {
	using namespace std;
	using namespace wargaming;

	TimeWeightedAverage a;

	constexpr utils::TimePointMs tp{ 2000ms };
	constexpr int id{ 9 };

	EXPECT_ANY_THROW(a.Insert(tp, id, -20.0));
	EXPECT_ANY_THROW(a.Erase(tp+200ms, id));
	EXPECT_DOUBLE_EQ(a.GetAverage(), 0.0);
}

TEST(TimeWeightedAverage, InsertAndErase) {
	using namespace std;
	using namespace wargaming;

	TimeWeightedAverage a;

	constexpr utils::TimePointMs tp{ 2000ms };
	constexpr int id{ 9 };
	constexpr double price{ 20.0 };

	a.Insert(tp, id, 20.0);
	a.Erase(tp+200ms, id);

	EXPECT_DOUBLE_EQ(a.GetAverage(), price);
}

TEST(TimeWeightedAverage, InsertOrderWithSameIdTwoTimes) {
	using namespace std;
	using namespace wargaming;

	TimeWeightedAverage a;

	constexpr utils::TimePointMs tp{ 2000ms };
	constexpr int id{ 9 };
	constexpr double price{ 20.0 };

	a.Insert(tp, id, price);
	EXPECT_ANY_THROW(a.Insert(utils::TimePointMs{ 1500ms }, id, 15.0));
	a.Erase(tp+200ms, id);
	EXPECT_DOUBLE_EQ(a.GetAverage(), price);
}

// TimeWeightedAverage -- end
// InputFiles -- begin
TEST(InputFiles, BasicTest) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/basic_test_input.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 10.5);
}

TEST(InputFiles, InsertEraseOne) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/one_record.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 13.0);
}

TEST(InputFiles, EmptyIntervalsBetweenOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_intervals_between_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 14.0);
}

TEST(InputFiles, MultipleSamePrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/multiple_same_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 13.0);
}

TEST(InputFiles, EmptyOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 0.0);
}

TEST(InputFiles, MonotonicMaxPrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/monotonic_max_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	constexpr double shouldBe{ (1000*10 + 200*15 + 1800*17) / (1000.0 + 200 + 1800) };

	EXPECT_DOUBLE_EQ(average.GetAverage(), shouldBe);
}

TEST(InputFiles, OneHighestPrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/one_highest.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 20.0);
}

TEST(InputFiles, MonotonicEraseRecentlyHighestPrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/monotonic_erase_recently_highest_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	constexpr double shouldBe{ (1000*10 + 200*15 + 200*17 + 100*15 + 1500*10) / (1000.0 + 200 + 200 + 100 + 1500) };

	EXPECT_DOUBLE_EQ(average.GetAverage(), shouldBe);
}

// InputFiles -- end



