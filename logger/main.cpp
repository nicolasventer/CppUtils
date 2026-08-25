#include "logger.h"
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[])
{
	if (argc < 2 || std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0)
	{
		(void)std::fprintf(stderr, "usage: %s demo | levels\n", argv[0]);
		return 0;
	}

	if (std::strcmp(argv[1], "demo") == 0)
	{
		LOG(INFO, "info line", "10:", 10);
		LOG(DEBUG, "debug line");
		LOG(WARN, "warning line");
		LOG(ERROR, "error line");
		LOG(FATAL, "fatal line");
		return 0;
	}

	if (std::strcmp(argv[1], "levels") == 0)
	{
		LOG(INFO, "each level uses a distinct color on stderr");
		return 0;
	}

	(void)std::fprintf(stderr, "unknown: %s (try demo or levels)\n", argv[1]);
	return 2;
}
