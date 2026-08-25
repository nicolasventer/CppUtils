#include "assert.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		(void)fprintf(stderr, "usage: %s pass | fail\n", argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "pass") == 0)
	{
		ASSERT(argc > 1, "expected subcommand");
		return 0;
	}

	if (strcmp(argv[1], "fail") == 0)
	{
		ASSERT(0, "deliberate failure (fail)");
		return 0;
	}

	(void)fprintf(stderr, "unknown: %s (try pass or fail)\n", argv[1]);
	return 2;
}
