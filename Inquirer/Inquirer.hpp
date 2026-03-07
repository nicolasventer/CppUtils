#pragma once

#include <string>
#include <vector>

class Inquirer
{
public:
	static std::string input(const std::string& question, const std::string& default_value = "");

	static bool confirm(const std::string& question, bool def = true);

	static std::string select(const std::string& question, const std::vector<std::string>& options, size_t default_index = 0);

	static std::vector<std::string> multiselect(
		const std::string& question, const std::vector<std::string>& options, const std::vector<size_t>& defaults = {});
};
