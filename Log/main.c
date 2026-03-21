#include "Log.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		(void)fprintf(stderr, "usage: %s demo | levels\n", argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "demo") == 0)
	{
		LOG_INFO("info line");
		LOG(INFO, "info line");
		LOG(DEBUG, "debug line");
		LOG(WARN, "warning line");
		LOG(ERROR, "error line");
		LOG(FATAL, "fatal line");
		return 0;
	}

	if (strcmp(argv[1], "levels") == 0)
	{
		LOG(INFO, "each level uses a distinct color on stderr");
		return 0;
	}

	(void)fprintf(stderr, "unknown: %s (try demo or levels)\n", argv[1]);
	return 2;
}
