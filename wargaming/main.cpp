#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "TimeWeightedAverage.h"
#include "Order.h"

int main(int argc, char* argv[]) {
	using namespace std;
	using namespace std::chrono;

	try {
		if (argc != 2) throw runtime_error{ "Usage: "s + argv[0] + " input_file_name" };

		const string pathToFile{ argv[1] };

		ifstream file{ pathToFile };
		if (!file) throw runtime_error{ "Cannot open file: \"" + pathToFile + "\"" };

		wargaming::TimeWeightedAverage average;
		wargaming::ReadOrdersAndStore(file, average);

		cout << "Result: " << average.GetStats().result << '\n';
		cout << "Sum: " << average.GetStats().sum << '\n';
		cout << average.GetStats().GetResult() << '\n';
	}
	catch (const exception & e) {
		cerr << "std::exception caught: " << e.what() << '\n';
		// failure
		return EXIT_FAILURE;
	}
}