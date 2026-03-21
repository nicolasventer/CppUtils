#include "Random.hpp"
#include <iostream>

int main()
{
	static const int RANDOM_MAX_INT = 10;
	static const int RANDOM_COUNT = 5;

	// Integer random generator
	auto intGen = RANDOM_INSTANCE.range<int>(1, RANDOM_MAX_INT);

	std::cout << "Random integers:\n";
	for (int i = 0; i < RANDOM_COUNT; ++i)
	{
		std::cout << intGen.next() << '\n';
	}

	// Floating point random generator
	auto floatGen = RANDOM_INSTANCE.range<float>(0.0F, 1.0F);

	std::cout << "\nRandom floats:\n";
	for (int i = 0; i < RANDOM_COUNT; ++i)
	{
		std::cout << floatGen.next() << '\n';
	}

	return 0;
}
