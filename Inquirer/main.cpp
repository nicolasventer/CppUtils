#include "Inquirer.hpp"
#include "terminal.hpp"
#include <iostream>

int main()
{
	enable_ansi_terminal();

	std::string name = Inquirer::input("Project name", "my-app");

	std::string framework = Inquirer::select("Choose framework", {"React", "Vue", "Svelte", "Solid"}, 0);

	auto features = Inquirer::multiselect("Select features", {"TypeScript", "Router", "ESLint", "Tests"}, {0, 2});

	bool deploy = Inquirer::confirm("Deploy now", true);

	std::cout << "\nSummary\n";
	std::cout << "Name: " << name << "\n";
	std::cout << "Framework: " << framework << "\n";

	std::cout << "Features:\n";
	for (auto& f : features) std::cout << " - " << f << "\n";

	std::cout << "Deploy: " << (deploy ? "Yes" : "No") << "\n";
}
