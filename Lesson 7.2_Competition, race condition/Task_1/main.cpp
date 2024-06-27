#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <Windows.h>

void client(const int& max, std::atomic<int>& aCount, bool& limit)
{
	while (aCount.load(std::memory_order::memory_order_seq_cst) < max)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		aCount.store(++aCount, std::memory_order::memory_order_seq_cst);
		std::cout << "��������: " << aCount.load(std::memory_order::memory_order_seq_cst) << "\t��������� ����� ������\n";
	}
	std::cout << "��������� ����� ��������" << std::endl;
	limit = 1;
}

void oper(std::atomic<int>& aCount, bool& limit)
{
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		aCount.store(--aCount, std::memory_order::memory_order_seq_cst);
		std::cout << "��������: " << aCount.load(std::memory_order::memory_order_seq_cst) << "\t�������� �������� �������\n";
	} while (aCount.load(std::memory_order::memory_order_seq_cst) > 0 || !limit);
	std::cout << "�������� �������� ���� ������" << std::endl;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int max = 10;
	bool limit = 0;
	std::atomic<int> aCount;
	aCount.store(0);

	std::thread thread1(client, std::ref(max), std::ref(aCount), std::ref(limit));
	std::thread thread2(oper, std::ref(aCount), std::ref(limit));
	thread1.join();
	thread2.join();

	return 0;
}