#include <iostream>
#include <Windows.h>

#include "LinePrinter.h"
#include "StringPrinter.h"

void SetWindow(HANDLE hOutput, int Width, int Height) {
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;

    SetConsoleScreenBufferSize(hOutput, coord);            // Set Buffer Size
    SetConsoleWindowInfo(hOutput, TRUE, &Rect);            // Set Window Size
} 

int main() {
	const static int width = 90;
	const static int height = 30;

	const static WORD background = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hOutput, background);
	SetWindow(hOutput, width, height);

	HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTextAttribute(hScreenBuffer, background);

	LinePrinter lp = LinePrinter(width, NULL, NULL, NULL, background, hScreenBuffer);
	StringPrinter spHeader1 = StringPrinter("Alpha Team", hScreenBuffer);
	LinePrinter lpHeader1 = LinePrinter(width, spHeader1, background, hScreenBuffer);
	StringPrinter spHeader2 = StringPrinter("Encryption Program", hScreenBuffer);
	LinePrinter lpHeader2 = LinePrinter(width, spHeader2, background, hScreenBuffer);
	StringPrinter spHeader3 = StringPrinter("CISP 400", hScreenBuffer);
	LinePrinter lpHeader3 = LinePrinter(width, spHeader3, background, hScreenBuffer);

	StringPrinter spKey = StringPrinter("Key: SD324hAXk34hdfierh", FOREGROUND_RED | FOREGROUND_GREEN | background, hScreenBuffer);
	spKey.addAttribute(5, background | FOREGROUND_INTENSITY);
	LinePrinter lpKey = LinePrinter(width, spKey, background, hScreenBuffer);

	StringPrinter spDataInHeader = StringPrinter("[Data In]", background, hScreenBuffer);
	StringPrinter spDataOutHeader = StringPrinter("[Data Out]", background, hScreenBuffer);
	LinePrinter lpDataHeaders = LinePrinter(width, LinePrinter::ALIGN_CENTER, 14, 14, background, hScreenBuffer);
	lpDataHeaders.addStringPrinter(spDataInHeader);
	lpDataHeaders.addStringPrinter(spDataOutHeader);

	lp.printLine();
	lpHeader1.printLine();
	lpHeader2.printLine();
	lpHeader3.printLine();
	lp.printLine();
	lp.printLine();
	lpKey.printLine();
	lp.printLine();
	lpDataHeaders.printLine();

	SetConsoleActiveScreenBuffer(hScreenBuffer);
	CloseHandle(hOutput);

	std::cin.get();

	return 0;
}