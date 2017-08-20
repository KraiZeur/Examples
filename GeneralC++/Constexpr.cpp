#include <iostream>
#include "time_sub.h"

int factorial(int n) {
    return n > 0 ? n * factorial( n - 1 ) : 1;
}

constexpr int factorial_constexpr(int n) {
    return n > 0 ? n * factorial_constexpr( n - 1 ) : 1;
}

int main(int argc, char const *argv[]) {
	
 	struct timespec start, end, result;
 	clock_gettime(CLOCK_REALTIME, &start);
	int val = factorial_constexpr(50);
	clock_gettime(CLOCK_REALTIME, &end);
  tsub(end,start,result);

	std::cout << "Value after factorial_constexpr = " << val << " time : " << result.tv_sec << "." << result.tv_nsec << std::endl;

	clock_gettime(CLOCK_REALTIME, &start);
	val = factorial(50);
	clock_gettime(CLOCK_REALTIME, &end);
	tsub(end,start,result);

	std::cout << "Value after factorial = " << val << " time : " << result.tv_sec << "." << result.tv_nsec << std::endl;

	return 0;
}