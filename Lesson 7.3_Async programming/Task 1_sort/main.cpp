#include <iostream>
#include <thread>
#include <future>
#include <vector>

void AsyncSort(std::promise<std::vector<int>>&& prom_vec, std::vector<int>& arr)
{
	int min;
	int minIndex;
	for(int j = 0; j < arr.size(); j++)
	{
		min = arr[j];
		minIndex = j;
		for (int i = j; i < arr.size(); i++)
		{
			min = min < arr[i] ? min : arr[i];
			minIndex = min < arr[i] ? minIndex : i;
		}
		if (min < arr[j])
			std::swap(arr[j], arr[minIndex]);
		else continue;
	}
	
	prom_vec.set_value(arr);
}

int main()
{
	std::vector<int> arr{ 5, 3, 48, 10, 4, 50, 1, 5 };

	std::promise<std::vector<int>> prom;
	std::future<std::vector<int>> ft = prom.get_future();
		
	std::cout << "Before sort:" << std::endl;
	for (const auto& el : arr)
		std::cout << el << " ";
	std::cout << std::endl;
	
	std::thread th(AsyncSort, std::move(prom), std::ref(arr));	
	ft.get();
	
	std::cout << "After sort:" << std::endl;
	for (const auto& el : arr)
		std::cout << el << " ";

	th.join();

	return 0;
}