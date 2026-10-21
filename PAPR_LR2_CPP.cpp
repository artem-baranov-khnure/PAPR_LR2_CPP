#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>

class TwoLevelPredictor {
	static const int MAX_HISTORY_BITS = 20;

	unsigned bhr;
	unsigned mask;
	std::vector<int> pht;

public:
	TwoLevelPredictor(int historyBits) {
		if (historyBits < 1 || historyBits > MAX_HISTORY_BITS)
			throw std::invalid_argument(
				"historyBits must be in range 1.." + std::to_string(MAX_HISTORY_BITS) +
				", got " + std::to_string(historyBits));

		bhr = 0;
		mask = (1u << historyBits) - 1;
		pht.assign(1u << historyBits, 2);
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

double accuracy(int historyBits, const std::vector<bool>& trace) {
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

int main() {
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

	names.push_back("Loop (1110)");
	traces.push_back(patternTrace(N, "1110"));

	names.push_back("Pattern (11010010)");
	traces.push_back(patternTrace(N, "11010010"));

	names.push_back("1110 + 5% noise");
	traces.push_back(noisyPatternTrace(N, "1110", 5));

	names.push_back("1110 + 20% noise");
	traces.push_back(noisyPatternTrace(N, "1110", 20));

	std::cout << "Accuracy (%), " << N << " branches per test\n\n";

	std::cout << std::left << std::setw(24) << "Test" << std::right;
	for (int bits : historySizes)
	{
		std::cout << std::setw(10) << ("N=" + std::to_string(bits));
	}
	std::cout << "\n" << std::string(24 + 10 * historySizes.size(), '-') << "\n";

	std::cout << std::fixed << std::setprecision(2);
	for (size_t i = 0; i < names.size(); i++) {
		std::cout << std::left << std::setw(24) << names[i] << std::right;
		for (int bits : historySizes)
		{
			std::cout << std::setw(10) << accuracy(bits, traces[i]);
		}
		std::cout << "\n";
	}
	return 0;
}