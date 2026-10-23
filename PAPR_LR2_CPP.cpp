#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

template <class Function>
double GetMinTime(Function&& f, int repeat = 20) {
	double best = 1.0e9;
	for (int i = 0; i < repeat; i++) {
		auto start = chrono::steady_clock::now();
		f();
		auto finish = chrono::steady_clock::now();
		double t = chrono::duration<double>(finish - start).count();
		if (t < best) best = t;
	}
	return best;
}

void FillArray(int* arr, int n, int mode) {
	for (int i = 0; i < n; i++) {
		if (mode == 0)      arr[i] = rand() % 100 + 1;
		else if (mode == 1) arr[i] = -(rand() % 100) - 1;
		else                arr[i] = rand() % 201 - 100;
	}
}

void PrintResult(const char* name, double t_if, double t_noif, bool same) {
	double percent = (t_if - t_noif) / t_if * 100.0;
	cout << name
		<< "with if = " << t_if * 1000 << " ms   "
		<< "without if = " << t_noif * 1000 << " ms   "
		<< "faster by " << percent << " %" << endl;
}

// Task 1
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int CountPositive(const int* x, int n) {
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (x[i] > 0) {
			count++;
		}
	}
	return count;
}

int CountPositiveNoIf(const int* x, int n) {
	int count = 0;
	for (int i = 0; i < n; i++) {
		count -= (-x[i]) >> 31;
	}
	return count;
}

void TestTask1() {
	const int n = 5000000;
	vector<int> arr(n);
	const char* names[3] = { "all positive: ", "all negative: ", "random:       " };

	for (int mode = 0; mode < 3; mode++) {
		FillArray(arr.data(), n, mode);
		volatile int r1 = 0, r2 = 0;

		double t1 = GetMinTime([&] { r1 = CountPositive(arr.data(), n); });
		double t2 = GetMinTime([&] { r2 = CountPositiveNoIf(arr.data(), n); });

		PrintResult(names[mode], t1, t2, r1 == r2);
	}
}

// Task 2
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void BubbleSort(int* x, int n) {
	for (int last = n - 1; last >= 1; last--) {
		for (int i = 0; i < last; i++) {
			if (x[i] > x[i + 1]) {
				int tmp = x[i];
				x[i] = x[i + 1];
				x[i + 1] = tmp;
			}
		}
	}
}

void BubbleSortNoIf(int* x, int n) {
	for (int last = n - 1; last >= 1; last--) {
		for (int i = 0; i < last; i++) {
			int a = x[i];
			int b = x[i + 1];

			int mask = (b - a) >> 31;

			x[i] = (a & ~mask) | (b & mask);
			x[i + 1] = (b & ~mask) | (a & mask);
		}
	}
}

void TestTask2() {
	const int n = 10000;
	vector<int> src(n), w1(n), w2(n);
	FillArray(src.data(), n, 2);

	double t1 = GetMinTime([&] {
		w1 = src;
		BubbleSort(w1.data(), n);
		}, 5);
	double t2 = GetMinTime([&] {
		w2 = src;
		BubbleSortNoIf(w2.data(), n);
		}, 5);

	PrintResult("random: ", t1, t2, w1 == w2);
}

// Task 3
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void MulPol(const int* x, const int* y, int* z, int n) {
	for (int k = 0; k < 2 * n; k++) {
		z[k] = 0;
	}

	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			if (y[j] == 1) {
				z[i + j] += x[i];
			}
			else if (y[j] == -1) {
				z[i + j] -= x[i];
			}
		}
	}
}

void MulPolNoIf(const int* x, const int* y, int* z, int n) {
	for (int k = 0; k < 2 * n; k++) {
		z[k] = 0;
	}

	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			z[i + j] += x[i] * y[j];
		}
	}
}

void TestTask3() {
	const int n = 10000;
	vector<int> x(n), y(n), z1(2 * n), z2(2 * n);

	FillArray(x.data(), n, 2);
	for (int i = 0; i < n; i++) {
		y[i] = rand() % 3 - 1;
	}

	double t1 = GetMinTime([&] { MulPol(x.data(), y.data(), z1.data(), n); }, 5);
	double t2 = GetMinTime([&] { MulPolNoIf(x.data(), y.data(), z2.data(), n); }, 5);

	PrintResult("random: ", t1, t2, z1 == z2);
}

// Task 4
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <class T>
void RoundArray(const T* x, int* r, int n) {
	const T half = (T)0.5;
	for (int i = 0; i < n; i++) {
		if (x[i] >= 0) {
			r[i] = (int)(x[i] + half);
		}
		else {
			r[i] = (int)(x[i] - half);
		}
	}
}

template <class T>
void RoundArrayNoIf(const T* x, int* r, int n) {
	for (int i = 0; i < n; i++) {
		r[i] = (int)std::round(x[i]);
	}
}

template <class T>
void TestRound(const char* name) {
	const int n = 5000000;
	vector<T> x(n);
	vector<int> r1(n), r2(n);

	for (int i = 0; i < n; i++) {
		x[i] = (T)((rand() % 20001 - 10000) / 100.0);
	}

	double t1 = GetMinTime([&] { RoundArray(x.data(), r1.data(), n); });
	double t2 = GetMinTime([&] { RoundArrayNoIf(x.data(), r2.data(), n); });

	PrintResult(name, t1, t2, r1 == r2);
}

void TestTask4() {
	TestRound<float>("float:  ");
	TestRound<double>("double: ");
}

int main() {
	srand((unsigned int)time(NULL));
	cout << fixed << setprecision(4);

	cout << "Task 1 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
	TestTask1();

	cout << "\nTask 2 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
	TestTask2();

	cout << "\nTask 3 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
	TestTask3();

	cout << "\nTask 4 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
	TestTask4();

	cout << endl;
	system("pause");
	return 0;
}