#include <iostream>
#include <Windows.h>

#include "LinePrinter.h"
#include "StringPrinter.h"
#include "Sung.h"

//void SetWindow(HANDLE hOutput, int Width, int Height) {
//    _COORD coord;
//    coord.X = Width;
//    coord.Y = Height;
//
//    _SMALL_RECT Rect;
//    Rect.Top = 0;
//    Rect.Left = 0;
//    Rect.Bottom = Height - 1;
//    Rect.Right = Width - 1;
//
//    SetConsoleScreenBufferSize(hOutput, coord);            // Set Buffer Size
//    SetConsoleWindowInfo(hOutput, TRUE, &Rect);            // Set Window Size
//} 

int main() {
	//const static int width = 120;
	//const static int height = 40;

	//const static WORD background = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;

	//HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	//HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleTextAttribute(hOutput, background);
	//SetWindow(hOutput, width, height);

	//HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//SetConsoleTextAttribute(hScreenBuffer, background);

	//LinePrinter lp = LinePrinter(width, NULL, NULL, NULL, background, hScreenBuffer);
	//StringPrinter spHeader1 = StringPrinter("Alpha Team", hScreenBuffer);
	//LinePrinter lpHeader1 = LinePrinter(width, spHeader1, background, hScreenBuffer);
	//StringPrinter spHeader2 = StringPrinter("Encryption Program", hScreenBuffer);
	//LinePrinter lpHeader2 = LinePrinter(width, spHeader2, background, hScreenBuffer);
	//StringPrinter spHeader3 = StringPrinter("CISP 400", hScreenBuffer);
	//LinePrinter lpHeader3 = LinePrinter(width, spHeader3, background, hScreenBuffer);

	//StringPrinter spKey = StringPrinter("Key: SD324hAXk34hdfierh", background, hScreenBuffer);
	//spKey.addAttribute(5, background | FOREGROUND_INTENSITY);
	//LinePrinter lpKey = LinePrinter(width, spKey, background, hScreenBuffer);

	//StringPrinter spDataInHeader = StringPrinter("Data In", background, hScreenBuffer);
	//StringPrinter spDataOutHeader = StringPrinter("Data Out", background, hScreenBuffer);
	//LinePrinter lpDataInHeader = LinePrinter(width / 2, spDataInHeader, background, hScreenBuffer);
	//LinePrinter lpDataOutHeader = LinePrinter(width / 2, spDataOutHeader, background, hScreenBuffer);

	//StringPrinter spPathIn = StringPrinter("Path: C:\\Path\\To\\Infile.txt", background, hScreenBuffer);
	//spPathIn.addAttribute(6, background | FOREGROUND_INTENSITY);
	//StringPrinter spPathOut = StringPrinter("Path: C:\\Path\\To\\Outfile.txt", background, hScreenBuffer);
	//spPathOut.addAttribute(6, background | FOREGROUND_INTENSITY);
	//LinePrinter lpPathIn(width / 2, spPathIn, background, hScreenBuffer);
	//LinePrinter lpPathOut(width / 2, spPathOut, background, hScreenBuffer);


	//lp.printLine();
	//lpHeader1.printLine();
	//lpHeader2.printLine();
	//lpHeader3.printLine();
	//lp.printLine();
	//lp.printLine();
	//lpKey.printLine();
	//lp.printLine();
	//lpDataInHeader.printLine();
	//lpDataOutHeader.printLine();
	//lpPathIn.printLine();
	//lpPathOut.printLine();

	//SetConsoleActiveScreenBuffer(hScreenBuffer);
	//CloseHandle(hOutput);

	Cipher *sung = new Sung();

	char key[Cipher::BLOCK_SIZE];
	for (int i = 0; i < Cipher::BLOCK_SIZE - 1; i++) {
		key[i] = 65 + i;
	}
	key[Cipher::BLOCK_SIZE - 1] = '\0';

	sung->setKey(key);

	char data[] = "thisismydatalolo";

	std::cout << data;
	std::cout << std::endl;

	sung->encrypt(data);

	std::cout << data;
	std::cout << std::endl;

	sung->decrypt(data);

	std::cout << data;

	std::cin.get();

	delete sung;

	return 0;
}