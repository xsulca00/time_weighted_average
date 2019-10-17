#include "pch.h"
#include "../wargaming/TimeWeightedAverage.h"
TEST(BasicTest, BasicTest) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/basic_test_input.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';

	EXPECT_DOUBLE_EQ(average.GetStats().GetResult(), 10.5);
}

TEST(BasicTest, InsertEraseOne) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/one_record.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';

	EXPECT_DOUBLE_EQ(average.GetStats().GetResult(), 13.0);
}

TEST(BasicTest, EmptyIntervalsBetweenOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_intervals_between_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';

	EXPECT_DOUBLE_EQ(average.GetStats().GetResult(), 14.0);
}

TEST(BasicTest, MultipleSamePrice) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/multiple_same_price.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';

	EXPECT_DOUBLE_EQ(average.GetStats().GetResult(), 13.0);
}

TEST(BasicTest, EmptyOrders) {
	using namespace std;

	const string pathToFile{ "../wargaming/test_inputs/empty_orders.txt" };

	ifstream file{ pathToFile };
	if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

	wargaming::TimeWeightedAverage average;
	wargaming::ReadOrdersAndStore(file, average);

	cout << "Result: " << average.GetStats().result << '\n';
	cout << "Sum: " << average.GetStats().sum << '\n';

	EXPECT_DOUBLE_EQ(average.GetStats().GetResult(), 0.0);
}

