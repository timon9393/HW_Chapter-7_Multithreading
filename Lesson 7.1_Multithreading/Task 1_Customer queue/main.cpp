#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>

void client(const int& max, int& count, bool& limit)
{
	while (count < max)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		++count;
		std::cout << "Клиентов: " << count << "\tДобавился новый клиент\n";
	}
	std::cout << "Достигнут лимит клиентов" << std::endl;
	limit = 1;
}

void oper (int& count, bool& limit)
{
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		--count;
		std::cout << "Клиентов: " << count << "\tОператор обслужил клиента\n";
	} while (count > 0 || !limit);
	std::cout << "Оператор закончил свою работу" << std::endl;
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int max = 10;
	int count = 0;
	bool limit = 0;

	std::thread thread1 (client, std::ref(max), std::ref(count), std::ref(limit));
	std::thread thread2 (oper, std::ref(count), std::ref(limit));
	thread1.join();
	thread2.join();

	return 0;
}