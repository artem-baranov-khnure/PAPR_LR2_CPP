#include "Task5.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {
	class TwoLevelPredictor {
		size_t bhr;
		size_t mask;
		std::vector<int> pht;

	public:
		TwoLevelPredictor(int historyBits) {
			const size_t phtSize = static_cast<size_t>(1) << historyBits;

			bhr = 0;
			mask = phtSize - 1;
			pht.assign(phtSize, 2);
		}

		bool predict() const {
			return pht[bhr] >= 2;
		}

		void update(bool taken) {
			if (taken && pht[bhr] < 3)
			{
				pht[bhr]++;
			}

			if (!taken && pht[bhr] > 0) {
				pht[bhr]--;
			}

			bhr = ((bhr << 1) | (taken ? 1 : 0)) & mask;
		}
	};

	std::vector<bool> patternTrace(int n, const std::string& pattern) {
		std::vector<bool> trace;
		for (int i = 0; i < n; i++)
			trace.push_back(pattern[i % pattern.size()] == '1');
		return trace;
	}

	std::vector<bool> randomTrace(int n, int percentTaken) {
		std::vector<bool> trace;
		for (int i = 0; i < n; i++)
			trace.push_back(rand() % 100 < percentTaken);
		return trace;
	}

	std::vector<bool> noisyPatternTrace(int n, const std::string& pattern, int noisePercent) {
		std::vector<bool> trace = patternTrace(n, pattern);
		for (int i = 0; i < n; i++)
			if (rand() % 100 < noisePercent)
				trace[i] = !trace[i];
		return trace;
	}

	double calculateAccuracy(int historyBits, const std::vector<bool>& trace) {
		TwoLevelPredictor predictor(historyBits);
		int correct = 0;

		for (bool actual : trace) {
			if (predictor.predict() == actual)
			{
				correct++;
			}

			predictor.update(actual);
		}
		return 100.0 * correct / trace.size();
	}

	std::vector<std::pair<bool, bool>> collectPredictions(int historyBits, const std::vector<bool>& trace, size_t steps) {
		TwoLevelPredictor predictor(historyBits);
		const size_t count = std::min(steps, trace.size());

		std::vector<std::pair<bool, bool>> results;
		results.reserve(count);

		for (size_t i = 0; i < count; i++) {
			const bool actual = trace[i];
			const bool predicted = predictor.predict();
			results.emplace_back(actual, predicted);
			predictor.update(actual);
		}
		return results;
	}

	void printPredictions(const std::vector<std::pair<bool, bool>>& predictions) {
		const int columnWidth = 11;

		std::cout << std::left
			<< std::setw(columnWidth) << "Step"
			<< std::setw(columnWidth) << "Actual"
			<< std::setw(columnWidth) << "Predicted"
			<< "Hit\n"
			<< std::string(columnWidth * 3 + 3, '-') << "\n";

		for (size_t i = 0; i < predictions.size(); i++) {
			const bool actual = predictions[i].first;
			const bool predicted = predictions[i].second;
			std::cout << std::setw(columnWidth) << i + 1
				<< std::setw(columnWidth) << (actual ? 1 : 0)
				<< std::setw(columnWidth) << (predicted ? 1 : 0)
				<< (actual == predicted ? '+' : '-') << "\n";
		}
		std::cout << std::right;
	}

	void printResults(int branchCount,
		const std::vector<std::string>& names,
		const std::vector<std::vector<bool>>& traces,
		const std::vector<int>& historySizes) {
		const int nameWidth = 24;
		const int columnWidth = 10;

		std::cout << branchCount << " branches per test\n\n";

		std::cout << std::left << std::setw(nameWidth) << "Test" << std::right;
		for (int bits : historySizes)
		{
			std::cout << std::setw(columnWidth) << ("N=" + std::to_string(bits));
		}
		std::cout << "\n" << std::string(nameWidth + columnWidth * historySizes.size(), '-') << "\n";

		std::cout << std::fixed << std::setprecision(2);
		for (size_t i = 0; i < names.size(); i++) {
			std::cout << std::left << std::setw(nameWidth) << names[i] << std::right;
			for (int bits : historySizes)
			{
				std::cout << std::setw(columnWidth) << calculateAccuracy(bits, traces[i]);
			}
			std::cout << "\n";
		}
	}

}

void TestTask5() {
	srand(1);
	const int N = 100000;
	const std::vector<int> historySizes = { 1, 2, 4, 8 };

	std::vector<std::string> names;
	std::vector<std::vector<bool>> traces;

	names.push_back("Random 50% taken");
	traces.push_back(randomTrace(N, 50));

	names.push_back("Random 90% taken");
	traces.push_back(randomTrace(N, 90));

	names.push_back("Always taken (1)");
	traces.push_back(patternTrace(N, "1"));

	names.push_back("Alternating (10)");
	traces.push_back(patternTrace(N, "10"));

	names.push_back("Pattern (11010010)");
	traces.push_back(patternTrace(N, "11010010"));

	names.push_back("Loop (1110)");
	traces.push_back(patternTrace(N, "1110"));

	names.push_back("1110 + 5% noise");
	traces.push_back(noisyPatternTrace(N, "1110", 5));

	names.push_back("1110 + 20% noise");
	traces.push_back(noisyPatternTrace(N, "1110", 20));

	printResults(N, names, traces, historySizes);

	const std::vector<bool> demoTrace = patternTrace(12, "110");

	std::cout << "\nPattern (110), N=1:\n";
	printPredictions(collectPredictions(1, demoTrace, demoTrace.size()));

	std::cout << "\nPattern (110), N=2:\n";
	printPredictions(collectPredictions(2, demoTrace, demoTrace.size()));
}