#ifndef PAGEPRINTER_H
#define PAGEPRINTER_H

#include <vector>

#include "LinePrinter.h"

class PagePrinter {
	private:
		std::vector<LinePrinter> linePrinters;

	public:
		PagePrinter(const PagePrinter &pagePrinter);
		PagePrinter(std::vector<LinePrinter> linePrinters = std::vector<LinePrinter>());

		void printPage();
		void addLinePrinter(LinePrinter linePrinter);
		void setLinePrinters(std::vector<LinePrinter> linePrinters);
		const std::vector<LinePrinter> * getLinePrinters();
};

#endif