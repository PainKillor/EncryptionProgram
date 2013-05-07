#include "PagePrinter.h"

PagePrinter::PagePrinter(const PagePrinter &pagePrinter) {
	// Copy constructor
}

PagePrinter::PagePrinter(std::vector<LinePrinter> linePrinters) {
	// Default/Parametized constructor
}

void PagePrinter::printPage() {
	// Invoke print on all LinePrinters
}

void PagePrinter::addLinePrinter(LinePrinter linePrinter) {
	// Add linePrinter to linePrinters
}

void PagePrinter::setLinePrinters(std::vector<LinePrinter> linePrinters) {
	// Set this->linePrinters = linePrinters
}

const std::vector<LinePrinter> * getLinePrinters() {
	// Return reference to linePrinters
}