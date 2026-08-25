#include "Assert.h"
#include <cstring>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 2 || std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0)
	{
		std::cerr << "usage: " << argv[0] << " pass | fail\n";
		return 0;
	}

	if (std::strcmp(argv[1], "pass") == 0)
	{
		ASSERT(argc > 1, "expected subcommand");
		return 0;
	}

	if (std::strcmp(argv[1], "fail") == 0)
	{
		ASSERT(false, "deliberate failure (fail)");
		return 0;
	}

	std::cerr << "unknown: " << argv[1] << " (try pass or fail)\n";
	return 2;
}
