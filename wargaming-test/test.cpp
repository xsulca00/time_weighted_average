#include "pch.h"
#include "../wargaming/TimeWeightedAverage.h"
TEST(BasicTest, BasicTest) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/basic_test_input.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 10.5);
}

TEST(BasicTest, InsertEraseOne) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/one_record.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 13.0);
}

TEST(BasicTest, EmptyIntervalsBetweenOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_intervals_between_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 14.0);
}

TEST(BasicTest, MultipleSamePrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/multiple_same_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 13.0);
}

TEST(BasicTest, EmptyOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 0.0);
}

TEST(BasicTest, MonotonicMaxPrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/monotonic_max_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	constexpr double shouldBe{ (1000*10 + 200*15 + 1800*17) / (1000.0 + 200 + 1800) };

	EXPECT_DOUBLE_EQ(average.GetAverage(), shouldBe);
}

TEST(BasicTest, OneHighestPrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/one_highest.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	EXPECT_DOUBLE_EQ(average.GetAverage(), 20.0);
}


