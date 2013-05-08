#include "PagePrinter.h"

PagePrinter::PagePrinter(const PagePrinter &pagePrinter)
{
    // Copy constructor
    std::vector<LinePrinter> linePrinters = pagePrinter.linePrinters;
}

PagePrinter::PagePrinter(std::vector<LinePrinter> linePrinter)
{
	// Default/Parametized constructor
	this->linePrinters = linePrinter;
}

void PagePrinter::printPage()
{
	// Invoke print on all LinePrinters
	for (std::vector<LinePrinter>::iterator it = linePrinters.begin(); it != linePrinters.end(); ++it)
    {
        it->printLine();
    }
}

void PagePrinter::addLinePrinter(LinePrinter linePrinter)
{
	// Add linePrinter to linePrinters
	linePrinters.push_back(linePrinter);
}

void PagePrinter::setLinePrinters(std::vector<LinePrinter> linePrinters)
{
	// Set this->linePrinters = linePrinters
	this->linePrinters = linePrinters;
}

const std::vector<LinePrinter> * PagePrinter::getLinePrinters()
{
	// Return reference to linePrinters
	return &linePrinters;
}
