#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <chrono>
#include <ctime>
using namespace std;



template <class Function>
double GetMinTime(Function&& f, int repeat = 10) {

	double min_val = 1.0e9;

	for (int i = 0; i < repeat; i++) {

		// https://en.cppreference.com/w/cpp/chrono/steady_clock
		auto start = chrono::steady_clock::now();
		f();
		auto finish = chrono::steady_clock::now();

		double t = chrono::duration<double>(finish - start).count();

		if (t < min_val) {
			min_val = t;
		}
	}
	return min_val;
}

void ArrayGenerator(int* arr, size_t size, int mode) {
	for (size_t i = 0; i < size; i++) {
		if (mode == 0) {
			arr[i] = rand() % 100 + 1;
		}
		else if (mode == 1) {
			arr[i] = -(rand() % 100) - 1;
		}
		else {
			arr[i] = rand() % 201 - 100;
		}
	}
}

double GetPercent(double before, double after) {
	return (before - after) / before * 100.0;
}

// Task 1
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
size_t CountPositive(int* x, size_t n) {
	size_t count = 0;

	for (size_t i = 0; i < n; i++) {
		if (x[i] > 0) {
			count++;
		}
	}
	return count;
}

size_t OptCountPositive(int* x, size_t n) {
	size_t count = n;
	size_t i = 0;

	for (; i + 4 <= n; i += 4) {
		count += ((x[i] - 1) >> 31) + ((x[i + 1] - 1) >> 31) + ((x[i + 2] - 1) >> 31) + ((x[i + 3] - 1) >> 31);
	}
	for (; i < n; i++) {
		count += (x[i] - 1) >> 31;
	}
	return count;
}

// Task 2
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void BubbleSort(int* x, int n) {
	for (int p = n - 1; p >= 1; p--) {
		for (int i = 0; i < p; i++) {
			if (x[i] > x[i + 1]) {
				int r = x[i]; x[i] = x[i + 1]; x[i + 1] = r;
			}
		}
	}
}

void OptBubbleSort(int* x, int n) {
	for (int p = 1; p < n; p++) {
		for (int i = p - 1; i >= 0; i--) {
			if (x[i] > x[i + 1]) {
				int r = x[i]; x[i] = x[i + 1]; x[i + 1] = r;
			}
		}
	}
}

// Task 3
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void MulPol(int* x, int* y, int* z, int n) {
	memset(z, 0, 2 * n * sizeof(int));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			z[i + j] += x[i] * y[j];
}

void MulPolOpt1(int* x, int* y, int* z, int n) {
	memset(z, 0, 2 * n * sizeof(int));

	for (int j = 0; j < n; j++) {
		if (y[j]) {
			for (int i = 0; i < n; i++)
				z[i + j] += x[i] * y[j];
		}
	}
}

void MulPolOpt2(int* x, int* y, int* z, int n) {
	memset(z, 0, 2 * n * sizeof(int));

	for (int j = 0; j < n; j++) {
		if (y[j] != 0) {
			if (y[j] == 1) {
				for (int i = 0; i < n; i++) z[i + j] += x[i];
			}
			else {
				for (int i = 0; i < n; i++) z[i + j] -= x[i];
			}
		}
	}
}

void MulPolOpt3(int* x, int* y, int* z, int n) {
	memset(z, 0, 2 * n * sizeof(int));
	for (int j = 0; j < n; j++) {
		if (y[j] != 0) {
			int* pz = z + j;
			if (y[j] == 1) {
				for (int i = 0; i < n; i++) pz[i] += x[i];
			}
			else {
				for (int i = 0; i < n; i++) pz[i] -= x[i];
			}
		}
	}
}

// Task 4
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template <class T>
void RoundArray(const T* x, long long* r, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (x[i] >= 0) {
			r[i] = (long long)(x[i] + 0.5);
		}
		else {
			r[i] = (long long)(x[i] - 0.5);
		}
	}
}

template <class T>
void OptRoundArray(const T* x, long long* r, size_t n) {
	for (size_t i = 0; i < n; i++) {
		r[i] = (long long)(x[i] + copysign(0.5, (double)x[i]));
	}
}

template <class T>
void FillRealArray(T* x, size_t n) {
	for (size_t i = 0; i < n; i++) {
		x[i] = (T)((rand() % 20001 - 10000) / 100.0);
	}
}

template <class T>
void TestRound(size_t n, const char* name) {
	T* x = new T[n];
	long long* r1 = new long long[n];
	long long* r2 = new long long[n];
	FillRealArray(x, n);

	double t1 = GetMinTime([&] { RoundArray(x, r1, n); });
	double t2 = GetMinTime([&] { OptRoundArray(x, r2, n); });

	bool equal = true;
	for (size_t i = 0; i < n; i++) {
		if (r1[i] != r2[i]) {
			equal = false;
			break;
		}
	}
	cout << name << ":   RoundArray = " << t1 * 1000 << " ms   OptRoundArray = "
		<< t2 * 1000 << " ms   faster by " << GetPercent(t1, t2) << " %"
		<< (equal ? "" : "   RESULTS DIFFER") << endl;

	delete[] x;
	delete[] r1;
	delete[] r2;
}

int main()
{
	srand((unsigned int)time(NULL));

	// https://en.cppreference.com/cpp/io/manip/setprecision
	cout << fixed << setprecision(4);



	const char* separator = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=";
	cout << "Task 1 " << separator << endl;

	const size_t arr_size = 5000000;
	int* arr = new int[arr_size];
	const char* mode_names[3] = { "all positive", "all negative", "random      " };

	for (int mode = 0; mode < 3; mode++) {
		ArrayGenerator(arr, arr_size, mode);
		volatile size_t res1 = 0, res2 = 0;

		double t1 = GetMinTime([&] { res1 = CountPositive(arr, arr_size); });
		double t2 = GetMinTime([&] { res2 = OptCountPositive(arr, arr_size); });

		cout << mode_names[mode] << ":  CountPositive = " << t1 * 1000 << " ms"
			<< "   OptCountPositive = " << t2 * 1000 << " ms"
			<< "   faster by " << GetPercent(t1, t2) << " %"
			<< "   (" << res1 << " / " << res2 << ")" << endl;
	}

	delete[] arr;
	cout << "\n\n\n" << endl;



	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	cout << "Task 2 " << separator << endl;
	const int sort_size = 10000;
	int* src = new int[sort_size];
	int* work1 = new int[sort_size];
	int* work2 = new int[sort_size];

	ArrayGenerator(src, sort_size, 2);

	double ts1 = GetMinTime([&] {
		memcpy(work1, src, sort_size * sizeof(int));
		BubbleSort(work1, sort_size);}, 5);
	double ts2 = GetMinTime([&] {
		memcpy(work2, src, sort_size * sizeof(int));
		OptBubbleSort(work2, sort_size);}, 5);


	bool sorted_equal = true;
	for (int i = 0; i < sort_size; i++) {
		if (work1[i] != work2[i]) {
			sorted_equal = false;
			break;
		}
	}
	cout << "Array size = " << sort_size << endl;
	cout << "BubbleSort    = " << ts1 << " s" << endl;
	cout << "OptBubbleSort = " << ts2 << " s" << endl;
	cout << "Faster by " << GetPercent(ts1, ts2) << " %   ("
		<< ts1 / ts2 << " times)" << endl;

	cout << "Results " << (sorted_equal ? "match" : "DIFFER!") << endl;

	delete[] src;
	delete[] work1;
	delete[] work2;
	cout << "\n\n\n" << endl;



	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	cout << "Task 3 " << separator << endl;
	const int pol_size = 10000;
	int* px = new int[pol_size];
	int* py = new int[pol_size];
	int* pz[4];

	for (int i = 0; i < 4; i++) {
		pz[i] = new int[2 * pol_size];
	}
	ArrayGenerator(px, pol_size, 2);

	for (int i = 0; i < pol_size; i++) {
		py[i] = rand() % 3 - 1;
	}

	double tp[4];
	tp[0] = GetMinTime([&] { MulPol(px, py, pz[0], pol_size); }, 5);
	tp[1] = GetMinTime([&] { MulPolOpt1(px, py, pz[1], pol_size); }, 5);
	tp[2] = GetMinTime([&] { MulPolOpt2(px, py, pz[2], pol_size); }, 5);
	tp[3] = GetMinTime([&] { MulPolOpt3(px, py, pz[3], pol_size); }, 5);

	const char* pol_names[4] = { "MulPol    ", "MulPolOpt1", "MulPolOpt2", "MulPolOpt3" };

	for (int k = 0; k < 4; k++) {
		bool same = (memcmp(pz[0], pz[k], 2 * pol_size * sizeof(int)) == 0);

		cout << pol_names[k] << " = " << tp[k] << " s   faster than MulPol by "
			<< GetPercent(tp[0], tp[k]) << " %"
			<< (same ? "" : "   RESULTS DIFFER!") << endl;
	}

	delete[] px;
	delete[] py;


	for (int i = 0; i < 4; i++) {
		delete[] pz[i];
	}
	cout << "\n\n\n" << endl;



	cout << "Task 4 " << separator << endl;
	const size_t round_size = 5000000;

	TestRound<float>(round_size, "float ");
	TestRound<double>(round_size, "double");

	cout << "\n\n\n" << endl;





	system("pause");
	return 0;
}