#include "inquirer.hpp"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace ansi
{
	const std::string reset = "\033[0m";
	const std::string green = "\033[32m";
	const std::string cyan = "\033[36m";
	const std::string gray = "\033[90m";
} // namespace ansi

#ifdef _WIN32
const std::string ARROW = "> ";
#else
const std::string ARROW = "❯ ";
#endif

const std::string RADIO_ON = "◉";
const std::string RADIO_OFF = "◯";
const std::string CHECK_ON = "◼";
const std::string CHECK_OFF = "◻";

enum Key
{
	KEY_NONE,
	KEY_UP,
	KEY_DOWN,
	KEY_ENTER,
	KEY_SPACE,
	KEY_CHAR
};

struct KeyEvent
{
	Key type;
	char ch;
};

#ifdef _WIN32
class RawMode
{
public:
	RawMode() {}
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

	~RawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig); }
};
#endif

void ctrl_c_exit()
{
	std::cout << ansi::reset << "\nCancelled.\n";
	std::exit(130);
}

KeyEvent read_key()
{

#ifdef _WIN32

	int c = _getch();

	if (c == 3) ctrl_c_exit();

	if (c == 13) return {KEY_ENTER, 0};
	if (c == ' ') return {KEY_SPACE, 0};

	if (c == 224 || c == 0)
	{
		int k = _getch();
		if (k == 72) return {KEY_UP, 0};
		if (k == 80) return {KEY_DOWN, 0};
	}

	return {KEY_CHAR, (char)c};

#else

	char c;
	read(STDIN_FILENO, &c, 1);

	if (c == 3) ctrl_c_exit();

	if (c == '\n') return {KEY_ENTER, 0};
	if (c == ' ') return {KEY_SPACE, 0};

	if (c == '\033')
	{
		char seq[2];
		read(STDIN_FILENO, &seq[0], 1);
		read(STDIN_FILENO, &seq[1], 1);

		if (seq[0] == '[')
		{
			if (seq[1] == 'A') return {KEY_UP, 0};
			if (seq[1] == 'B') return {KEY_DOWN, 0};
		}
	}

	return {KEY_CHAR, c};

#endif
}

void clear_lines(size_t n)
{
	for (size_t i = 0; i < n; i++) std::cout << "\033[1A\033[2K";
}

std::string Inquirer::input(const std::string& question, const std::string& default_value)
{
	RawMode rm;
	std::string buffer;

	while (true)
	{
		std::cout << "\r\033[2K";
		std::cout << ansi::cyan << "? " << question << ": " << ansi::reset;

		if (buffer.empty() && !default_value.empty()) std::cout << ansi::gray << "(" << default_value << ")" << ansi::reset;
		else
			std::cout << buffer;

		std::cout.flush();

		auto key = read_key();

		if (key.type == KEY_ENTER) break;

		if (key.type == KEY_CHAR && (key.ch == 127 || key.ch == 8))
		{
			if (!buffer.empty()) buffer.pop_back();
		}
		else if (key.type == KEY_CHAR)
			buffer += key.ch;
		else if (key.type == KEY_SPACE)
			buffer += ' ';
	}

	if (buffer.empty() && !default_value.empty()) buffer = default_value;

	std::cout << "\r\033[2K";
	std::cout << ansi::green << "✔ " << question << ": " << buffer << ansi::reset << "\n";

	return buffer;
}

bool Inquirer::confirm(const std::string& question, bool def)
{
	std::cout << ansi::cyan << "? " << question << (def ? " (Y/n): " : " (y/N): ") << ansi::reset;

	std::string input;
	std::getline(std::cin, input);

	bool result = def;

	if (!input.empty())
	{
		char c = input[0];

		if (c == 'y' || c == 'Y') result = true;
		if (c == 'n' || c == 'N') result = false;
	}

	std::cout << ansi::green << "✔ " << question << ": " << (result ? "Yes" : "No") << ansi::reset << "\n";

	return result;
}

std::string Inquirer::select(const std::string& question, const std::vector<std::string>& options, size_t default_index)
{
	RawMode rm;
	size_t selected = default_index;

	std::cout << ansi::cyan << "? " << question << "\n";
	std::cout << ansi::gray << "  ↑↓ navigate • enter submit" << ansi::reset << "\n";

	while (true)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			if (i == selected) std::cout << ansi::green << ARROW << RADIO_ON << " " << options[i] << ansi::reset << "\n";
			else
				std::cout << "  " << RADIO_OFF << " " << options[i] << "\n";
		}

		auto key = read_key();

		if (key.type == KEY_UP && selected > 0) selected--;

		if (key.type == KEY_DOWN && selected + 1 < options.size()) selected++;

		if (key.type == KEY_ENTER) break;

		clear_lines(options.size());
	}

	clear_lines(options.size());

	std::cout << ansi::green << "✔ " << question << ": " << options[selected] << ansi::reset << "\n";

	return options[selected];
}

std::vector<std::string> Inquirer::multiselect(
	const std::string& question, const std::vector<std::string>& options, const std::vector<size_t>& defaults)
{
	RawMode rm;

	size_t cursor = 0;
	std::vector<char> selected(options.size(), 0);

	for (auto i : defaults)
		if (i < options.size()) selected[i] = 1;

	std::cout << ansi::cyan << "? " << question << "\n";
	std::cout << ansi::gray << "  ↑↓ navigate • space select • a all • i invert • enter submit" << ansi::reset << "\n";

	while (true)
	{
		for (size_t i = 0; i < options.size(); i++)
		{
			std::string mark = selected[i] ? CHECK_ON : CHECK_OFF;

			if (i == cursor) std::cout << ansi::green << ARROW << mark << " " << options[i] << ansi::reset << "\n";
			else
				std::cout << "  " << mark << " " << options[i] << "\n";
		}

		auto key = read_key();

		if (key.type == KEY_UP && cursor > 0) cursor--;

		if (key.type == KEY_DOWN && cursor < options.size() - 1) cursor++;

		if (key.type == KEY_SPACE) selected[cursor] = !selected[cursor];

		if (key.type == KEY_CHAR)
		{
			if (key.ch == 'a')
				for (auto& s : selected) s = 1;

			if (key.ch == 'i')
				for (auto& s : selected) s = !s;
		}

		if (key.type == KEY_ENTER) break;

		clear_lines(options.size());
	}

	clear_lines(options.size());

	std::vector<std::string> result;

	for (size_t i = 0; i < options.size(); i++)
		if (selected[i]) result.push_back(options[i]);

	std::cout << ansi::green << "✔ " << question << ansi::reset << "\n";

	return result;
}
