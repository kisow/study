#include <iostream>
#include <chrono>
#include <thread>
#include <future>

// @brief 임의의 시간을 기다린 뒤 문자를 찍는 함수. 100번 찍고나서 입력값 c를 반환.
int func(char c) {
	for(auto i=0;i<100;i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << c;
		std::cout.flush();
	}
	return c;
}

int main() {

	auto f1 = []() { return func('+'); };
	auto f2 = []() { return func('-'); };

	std::future<int> r1 = std::async(std::launch::async, f1);
	int r2 = f2();

	int r = r1.get() + r2;
	std::cout << "\n\nfunc1() + func2() = " << r << std::endl ;
	return 0;
}
