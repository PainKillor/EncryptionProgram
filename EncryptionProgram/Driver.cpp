#include "TUI.h"

int main() {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	Crypto *crypto = new Crypto();
	TUI tui = TUI(crypto, hOutput, hInput);

	// Process and reprint textual user interface until user types "exit"
	while (tui.processNext() != TUI::EXIT)
		tui.printPage();

	return 0;
}