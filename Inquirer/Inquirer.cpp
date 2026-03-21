#include "inquirer.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace ansi
{
	static const auto RESET = "\033[0m";
	static const auto GREEN = "\033[32m";
	static const auto CYAN = "\033[36m";
	static const auto GRAY = "\033[90m";
} // namespace ansi

#ifdef _WIN32
static const auto ARROW = "> ";
#else
static const auto ARROW = "❯ ";
#endif

static const auto RADIO_ON = "◉";
static const auto RADIO_OFF = "◯";
static const auto CHECK_ON = "◼";
static const auto CHECK_OFF = "◻";

enum class EKey : uint8_t
{
	None,
	Up,
	Down,
	Enter,
	Space,
	Char
};

struct KeyEvent
{
	EKey type;
	char ch;
};

#ifdef _WIN32
class RawMode
{
};
#else
class RawMode
{
	termios orig;

public:
	RawMode()
	{
		tcgetattr(STDIN_FILENO, &orig);
		termios raw = orig;
		raw.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	}
	RawMode(const RawMode&) = delete;
	RawMode& operator=(const RawMode&) = delete;
	RawMode(RawMode&&) noexcept = default;
	RawMode& operator=(RawMode&&) noexcept = default;

	~RawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig); }
};
#endif

static void ctrlCExit()
{
	std::cout << ansi::RESET << "\nCancelled.\n";
	std::exit(130);
}

static KeyEvent readKey()
{

#ifdef _WIN32

	int c = _getch();

	if (c == 3) ctrlCExit();

	if (c == 13) return {EKey::Enter, 0};
	if (c == ' ') return {EKey::Space, 0};

	if (c == 224 || c == 0)
	{
		int k = _getch();
		if (k == 72) return {EKey::Up, 0};
		if (k == 80) return {EKey::Down, 0};
	}

	return {EKey::Char, (char)c};

#else

	char c;
	read(STDIN_FILENO, &c, 1);

	if (c == 3) ctrl_c_exit();

	if (c == '\n') return {EKey::Enter, 0};
	if (c == ' ') return {EKey::Space, 0};

	if (c == '\033')
	{
		char seq[2];
		read(STDIN_FILENO, &seq[0], 1);
		read(STDIN_FILENO, &seq[1], 1);

		if (seq[0] == '[')
		{
			if (seq[1] == 'A') return {EKey::Up, 0};
			if (seq[1] == 'B') return {EKey::Down, 0};
		}
	}

	return {EKey::Char, c};

#endif
}

static void clearLines(size_t n)
{
	for (size_t i = 0; i < n; i++) std::cout << "\033[1A\033[2K";
}

std::string Inquirer::input(const std::string& question, const std::string& default_value)
{
	[[maybe_unused]] RawMode rm;
	std::string buffer;

	while (true)
	{
		std::cout << "\r\033[2K";
		std::cout << ansi::CYAN << "? " << question << ": " << ansi::RESET;

		if (buffer.empty() && !default_value.empty()) std::cout << ansi::GRAY << "(" << default_value << ")" << ansi::RESET;
		else std::cout << buffer;

		std::cout.flush();

		auto key = readKey();

		if (key.type == EKey::Enter) break;

		if (key.type == EKey::Char && (key.ch == 127 || key.ch == 8))
		{
			if (!buffer.empty()) buffer.pop_back();
		}
		else if (key.type == EKey::Char) buffer += key.ch;
		else if (key.type == EKey::Space) buffer += ' ';
	}

	if (buffer.empty() && !default_value.empty()) buffer = default_value;

	std::cout << "\r\033[2K";
	std::cout << ansi::GREEN << "✔ " << question << ": " << buffer << ansi::RESET << "\n";

	return buffer;
}

bool Inquirer::confirm(const std::string& question, bool def)
{
	std::cout << ansi::CYAN << "? " << question << (def ? " (Y/n): " : " (y/N): ") << ansi::RESET;

	std::string input;
	std::getline(std::cin, input);

	bool result = def;

	if (!input.empty())
	{
		char c = input[0];

		if (c == 'y' || c == 'Y') result = true;
		if (c == 'n' || c == 'N') result = false;
	}

	std::cout << ansi::GREEN << "✔ " << question << ": " << (result ? "Yes" : "No") << ansi::RESET << "\n";

	return result;
}

std::string Inquirer::select(const std::string& question, const std::vector<std::string>& options, size_t default_index)
{
	[[maybe_unused]] RawMode rm;
	size_t selected = default_index;

	std::cout << ansi::CYAN << "? " << question << "\n";
	std::cout << ansi::GRAY << "  ↑↓ navigate • enter submit" << ansi::RESET << "\n";

	while (true)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			if (i == selected) std::cout << ansi::GREEN << ARROW << RADIO_ON << " " << options[i] << ansi::RESET << "\n";
			else std::cout << "  " << RADIO_OFF << " " << options[i] << "\n";
		}

		auto key = readKey();

		if (key.type == EKey::Up && selected > 0) selected--;

		if (key.type == EKey::Down && selected + 1 < options.size()) selected++;

		if (key.type == EKey::Enter) break;

		clearLines(options.size());
	}

	clearLines(options.size());

	std::cout << ansi::GREEN << "✔ " << question << ": " << options[selected] << ansi::RESET << "\n";

	return options[selected];
}

std::vector<std::string> Inquirer::multiselect(
	const std::string& question, const std::vector<std::string>& options, const std::vector<size_t>& defaults)
{
	[[maybe_unused]] RawMode rm;

	size_t cursor = 0;
	std::vector<char> selected(options.size(), 0);

	for (auto i : defaults)
		if (i < options.size()) selected[i] = 1;

	std::cout << ansi::CYAN << "? " << question << "\n";
	std::cout << ansi::GRAY << "  ↑↓ navigate • space select • a all • i invert • enter submit" << ansi::RESET << "\n";

	while (true)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			std::string mark = selected[i] ? CHECK_ON : CHECK_OFF;

			if (i == cursor) std::cout << ansi::GREEN << ARROW << mark << " " << options[i] << ansi::RESET << "\n";
			else std::cout << "  " << mark << " " << options[i] << "\n";
		}

		auto key = readKey();

		if (key.type == EKey::Up && cursor > 0) cursor--;

		if (key.type == EKey::Down && cursor < options.size() - 1) cursor++;

		if (key.type == EKey::Space) selected[cursor] = !selected[cursor];

		if (key.type == EKey::Char)
		{
			if (key.ch == 'a')
				for (auto& s : selected) s = 1;

			if (key.ch == 'i')
				for (auto& s : selected) s = !s;
		}

		if (key.type == EKey::Enter) break;

		clearLines(options.size());
	}

	clearLines(options.size());

	std::vector<std::string> result;

	for (size_t i = 0; i < options.size(); i++)
		if (selected[i]) result.push_back(options[i]);

	std::cout << ansi::GREEN << "✔ " << question << ansi::RESET << "\n";

	return result;
}
