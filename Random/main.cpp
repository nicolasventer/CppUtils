#include "Random.hpp"
#include <iostream>

int main()
{
	// Integer random generator
	auto intGen = RANDOM_INSTANCE.Range<int>(1, 10);

	std::cout << "Random integers:\n";
	for (int i = 0; i < 5; ++i)
	{
		std::cout << intGen.Next() << std::endl;
	}

	// Floating point random generator
	auto floatGen = RANDOM_INSTANCE.Range<float>(0.0f, 1.0f);

	std::cout << "\nRandom floats:\n";
	for (int i = 0; i < 5; ++i)
	{
		std::cout << floatGen.Next() << std::endl;
	}

	return 0;
}
