#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>

void Sum(std::vector<int>& sum, std::vector<int>& vec1, std::vector<int>& vec2, int start, int end)
{
	for (int i = start; i <= end; i++)
		sum[i] = vec1[i] + vec2[i];
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::vector<int>sizes = { 1000, 10000, 100000, 1000000 };
	std::vector<int>threads = { 1, 2, 4, 8, 16 };
	std::vector<int> vec1, vec2, sum;
	std::vector<std::vector<double>> timeValues(5, std::vector<double>(4));
	std::vector<std::thread> thread_arr;

	std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl << std::endl;
	std::cout << "\t\t  1000\t\t  10000\t\t  100000\t  1000000" << std::endl;

	for (const auto& size : sizes)
	{
		vec1.resize(size, 4);
		vec2.resize(size, 3);
		sum.resize(size);

		for (const auto& th : threads)
		{
			int chunk = size / th;

			auto timeStart = std::chrono::steady_clock::now();
			for (int i = 0; i < th; i++)
			{
				int start = (i == 0) ? i * chunk : i * chunk + 1;
				int end = (i == th - 1) ? vec1.size() - 1 : start + chunk;

				thread_arr.emplace_back(std::thread([&sum, &vec1, &vec2, start, end] {
					Sum(sum, vec1, vec2, start, end);
					}));
			}

			for (int i = 0; i < th; i++)
			{
				thread_arr[i].join();
			}

			auto timeEnd = std::chrono::steady_clock::now();
			std::chrono::duration<double> calcTime = timeEnd - timeStart;

			// вычисление индексов элементов из range based for и заполнение соотв. элементов вектора
			timeValues[&th - &*std::begin(threads)][&size - &*std::begin(sizes)] = calcTime.count();

			thread_arr.clear();
		}
	}

	// Вывод на экран таблицы со значениями времени
	for (const auto& vec : timeValues)
	{
		std::cout << threads[&vec - &*std::begin(timeValues)] << " потоков\t";
		for (const auto& el : vec)
		{
			std::cout << el << "s\t";
		}
		std::cout << std::endl;
	}

	return 0;
}