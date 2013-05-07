#include "TUI.h"

TUI::TUI() {
	crypto = Crypto();
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	hInput = GetStdHandle(STD_INPUT_HANDLE);
}

TUI::TUI(Crypto crypto, HANDLE hOutput, HANDLE hInput) {
	this->crypto = crypto;
	this->hOutput = hOutput;
	this->hInput = hInput;
}

int TUI::processNext() {

	return 0;
}

int TUI::getLine() {

	return 0;
}

void TUI::printPage() {

}

void TUI::printHeader(PagePrinter &pagePrinter) {

}

void TUI::printKey(PagePrinter &pagePrinter) {

}

void TUI::printDataHeaders(PagePrinter &pagePrinter) {

}

void TUI::printData(PagePrinter &pagePrinter) {

}

void TUI::printAlgorithm(PagePrinter &pagePrinter) {

}

void printCommands(PagePrinter &pagePrinter) {

}