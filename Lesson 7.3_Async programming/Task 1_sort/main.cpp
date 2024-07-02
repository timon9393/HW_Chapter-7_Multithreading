#include <iostream>
#include <thread>
#include <future>
#include <vector>

void FindMin(std::vector<int>& arr, int i, std::promise<int>&& prom)
{
	int min = arr[i];
	int minIndex = i;

	for (; i < arr.size(); i++)
	{
		min = min < arr[i] ? min : arr[i];
		minIndex = min < arr[i] ? minIndex : i;
	}		
	prom.set_value(minIndex);
}

void AsyncSort(std::vector<int>& arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		std::promise<int> prom;
		std::future<int> ft = prom.get_future();
		std::async(std::launch::async, FindMin, std::ref(arr), i, std::move(prom));
		auto minInfo = ft.get();
		if (arr[minInfo] < arr[i])
			std::swap(arr[i], arr[minInfo]);
	}
}


int main()
{
	std::vector<int> arr{ 5, 3, 48, 10, 4, 50, 1, 5 };
	std::cout << "Before sort:" << std::endl;
	for (const auto& el : arr)
		std::cout << el << " ";
	std::cout << std::endl;

	AsyncSort(arr);

	std::cout << "After sort:" << std::endl;
	for (const auto& el : arr)
		std::cout << el << " ";

	return 0;
}