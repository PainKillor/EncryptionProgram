#include "StringPrinter.h"
#include <iostream>

int main() {
	StringPrinter sp = StringPrinter();

	sp.setString("lolololol");
	sp.addAttribute(FOREGROUND_GREEN | FOREGROUND_INTENSITY, 0);
	sp.addAttribute(FOREGROUND_RED | FOREGROUND_INTENSITY, 3);
	sp.addAttribute(FOREGROUND_BLUE | FOREGROUND_INTENSITY, 5);
	sp.addAttribute(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, 7);

	sp.printString();

	std::cin.get();

	return 0;
}