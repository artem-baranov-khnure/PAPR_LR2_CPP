#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <type_traits>
#include <windows.h>
#include <ctime>
using namespace std;



template <class Function>
double GetMinTime(Function&& f, int repeat = 10) {

	LARGE_INTEGER freq, start, finish;

	// https://learn.microsoft.com/ru-ru/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
	QueryPerformanceFrequency(&freq);

	double min_val = 1.0e9;

	for (int i = 0; i < repeat; i++) {

		// https://learn.microsoft.com/ru-ru/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
		QueryPerformanceCounter(&start);
		f();
		QueryPerformanceCounter(&finish);

		double t = (double)(finish.QuadPart - start.QuadPart) / freq.QuadPart;

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
// При виконанні використалися "Пз-2.pdf", приклад 2
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
	size_t count = 0;

	for (size_t i = 0; i < n; i++) {
		count += (unsigned long long)(-(long long)x[i]) >> 63;
	}
	return count;
}

// Task 2
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// При виконанні використалися "Пз-2.pdf", приклад 3
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
// При виконанні використалися "Пз-2.pdf", приклад 4
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
// При виконанні використалися "2025_Лекція_5.pdf", ст. 26 (1 спосіб округлення)
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
	typedef typename conditional<sizeof(T) == 8, unsigned long long, unsigned int>::type Bits;
	for (size_t i = 0; i < n; i++) {
		Bits sign = (*(const Bits*)&x[i]) >> (sizeof(T) * 8 - 1);
		r[i] = (long long)((double)x[i] + (0.5 - (double)sign));
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