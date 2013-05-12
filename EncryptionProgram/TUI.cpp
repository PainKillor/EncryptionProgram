#include "TUI.h"

#include <string>
#include <sstream>

TUI::TUI(Crypto *crypto, HANDLE hOutput, HANDLE hInput) {
	this->crypto = crypto;
	this->hOutput = hOutput;
	this->hInput = hInput;
	dataIndex = 0;
	algorithmIndex = 0;
	commandLines = std::vector<std::string>();
	encrypting = false;
	decrypting = false;
	loading = false;
	error = false;
	altPressed = false;

	key = "SD324hAXk34hdfierh";
	commandLines.push_back("> EPIC COMMAND1");
	commandLines.push_back("> EPIC COMMAND2");
	commandLines.push_back("> EPIC COMMAND3");
	commandLines.push_back("> EPIC COMMAND4");

	_COORD coord;
    coord.X = SCREEN_WIDTH;
    coord.Y = SCREEN_HEIGHT;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = SCREEN_HEIGHT - 1;
    Rect.Right = SCREEN_WIDTH - 1;

    SetConsoleScreenBufferSize(hOutput, coord);            // Set Buffer Size
    SetConsoleWindowInfo(hOutput, TRUE, &Rect);            // Set Window Size
	SetConsoleTextAttribute(hOutput, FG_BLACK | BG_WHITE);

	printPage();
}

TUI::~TUI() {
	delete crypto;
}

int TUI::processNext() {
	curLine = std::string();
	int keyPressed = getLine();

	//if (keyPressed != 0) {
	//	if (keyPressed == VK_UP) {
	//		if (index > NUM_LINES)
	//			index--;
	//	} else if (keyPressed = VK_DOWN) {
	//		if (index < lines.size()) 
	//			index++;
	//	}
	//} else {
	//	if (strcmp(chBuffer, "exit") != 0) {
	//		lines.push_back(string(chBuffer));
	//		chBuffer[0] = '\0';
	//		index++;
	//	}
	//}

	if (keyPressed == VK_MENU) {
		if (altPressed) {
			return CONTINUE_NOREFRESH;
		} else {
			altPressed = true;
		}
	} else if (keyPressed == ALT_RELEASED) {
		altPressed = false;
		return CONTINUE_REFRESH;
	} else if (keyPressed == VK_UP) {
		if (dataIndex > 0) dataIndex --;
	} else if (keyPressed == VK_DOWN) {
		dataIndex++;
	} else if (keyPressed == VK_LEFT) {
		if (dataIndex > NUM_DATA_LINES)
			dataIndex -= NUM_DATA_LINES;
		else
			dataIndex = 0;
	} else if (keyPressed == VK_RIGHT) {
		dataIndex += NUM_DATA_LINES;
	}
	
	if (curLine.compare("exit") == 0)
		return EXIT;
	else
		return CONTINUE_REFRESH;
}

int TUI::getLine() {
	INPUT_RECORD prInput;
	DWORD lpNumRead, lpNumWritten;

	const static int length = 256;
	char chBuffer[length];
	int index = curLine.copy(chBuffer, curLine.length(), 0);

	while (true) {
		ReadConsoleInput(hInput, &prInput, 1, &lpNumRead);
		if (prInput.EventType == KEY_EVENT) {
			if (prInput.Event.KeyEvent.bKeyDown == true) {
				if (isprint(prInput.Event.KeyEvent.uChar.AsciiChar)) {
					chBuffer[index++ % length] = prInput.Event.KeyEvent.uChar.AsciiChar;
					WriteConsole(hOutput, &prInput.Event.KeyEvent.uChar.AsciiChar, 1, &lpNumWritten, NULL);
				} else if (iscntrl(prInput.Event.KeyEvent.uChar.AsciiChar)) {
					switch (prInput.Event.KeyEvent.wVirtualKeyCode) {
						case VK_MENU:
						case VK_UP:
						case VK_DOWN:
						case VK_LEFT:
						case VK_RIGHT:
							curLine = std::string(chBuffer);
							return prInput.Event.KeyEvent.wVirtualKeyCode;
					}

					if (index > 0) {
						const static char chBackup[] = "\b \b";
						const static char chNextLine[] = "\r\n";

						switch (prInput.Event.KeyEvent.uChar.AsciiChar) {
							case '\b':
								WriteConsole(hOutput, chBackup, 3, &lpNumWritten, NULL);
								index--;
								break;
							case '\r':
								WriteConsole(hOutput, chNextLine, 2, &lpNumWritten, NULL);
								curLine = std::string(chBuffer);
								return 0;
						}
					}
				}

				if (index % length == 0) {
					if (index == length)
						chBuffer[length - 1] = '\0';
					else
						chBuffer[0] = '\0';
				} else {
					chBuffer[index % length] = '\0';
				}
			} else {
				if (prInput.Event.KeyEvent.wVirtualKeyCode == VK_MENU)
					return ALT_RELEASED;
			}
		}
	}
}

void TUI::printPage() {
	HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTextAttribute(hScreenBuffer, FG_BLACK | BG_WHITE);

	PagePrinter pagePrinter = PagePrinter();

	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpBlank);

	StringPrinter spHeader1 = StringPrinter("Alpha Team", hScreenBuffer);
	LinePrinter lpHeader1 = LinePrinter(SCREEN_WIDTH, spHeader1, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader1);

	StringPrinter spHeader2 = StringPrinter("Encryption Program", hScreenBuffer);
	LinePrinter lpHeader2 = LinePrinter(SCREEN_WIDTH, spHeader2, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader2);

	StringPrinter spHeader3 = StringPrinter("CISP 400", hScreenBuffer);
	LinePrinter lpHeader3 = LinePrinter(SCREEN_WIDTH, spHeader3, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader3);

	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);

	if (!key.empty()) {
		StringPrinter spKey = StringPrinter("Key: " + key, FG_BLACK | BG_WHITE, hScreenBuffer);
		spKey.addAttribute(5, FG_BROWN | BG_WHITE);
		LinePrinter lpKey = LinePrinter(SCREEN_WIDTH, spKey, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lpKey);
	} else {
		pagePrinter.addLinePrinter(lpBlank);
	}

	pagePrinter.addLinePrinter(lpBlank);
	printData(pagePrinter, hScreenBuffer);
	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);
	printAlgorithm(pagePrinter, hScreenBuffer);
	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);
	printCommands(pagePrinter, hScreenBuffer);

	pagePrinter.printPage();

	SetConsoleActiveScreenBuffer(hScreenBuffer);
	CloseHandle(hOutput);
	hOutput = hScreenBuffer;
}

void TUI::printData(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
	const static int HEADER_LINES = 3;
	const static int NUM_COMMANDS = 5;

	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpHalfBlank = LinePrinter(SCREEN_WIDTH - (SCREEN_WIDTH / 2), NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter spDataInHeader = StringPrinter("Data In", FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpDataInHeader = LinePrinter(SCREEN_WIDTH / 2, spDataInHeader, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter sp;
	LinePrinter lp;

	StringPrinter spPathIn;
	if (!inFilePath.empty()) {
		spPathIn = StringPrinter("Mode: File   Path: " + inFilePath, FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathIn.addAttribute(5, FG_BROWN | BG_WHITE);
		spPathIn.addAttribute(10, FG_BLACK | BG_WHITE);
		spPathIn.addAttribute(18, FG_BROWN | BG_WHITE);
	} else {
		spPathIn = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathIn.addAttribute(5, FG_BROWN | BG_WHITE);
	}
	LinePrinter lpPathIn(SCREEN_WIDTH / 2, spPathIn, FG_BLACK | BG_WHITE, hScreenBuffer);

	if (loading) {
		// Print loading...
		pagePrinter.addLinePrinter(lpDataInHeader);
		pagePrinter.addLinePrinter(lpHalfBlank);
		pagePrinter.addLinePrinter(lpBlank);
		pagePrinter.addLinePrinter(lpPathIn);
		pagePrinter.addLinePrinter(lpHalfBlank);

		for (int i = 0; i < NUM_DATA_LINES / 2; i++)
			pagePrinter.addLinePrinter(lpBlank);

		sp = StringPrinter("Loading...", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		pagePrinter.addLinePrinter(lpHalfBlank);

		for (int i = 0; i < NUM_DATA_LINES - (NUM_DATA_LINES / 2) - 1; i++)
			pagePrinter.addLinePrinter(lpBlank);

		pagePrinter.addLinePrinter(lpBlank);

	} else if (altPressed || crypto->getInputData()->empty()) {
		// Print command list
		for (int i = 0; i < HEADER_LINES; i++) 
			pagePrinter.addLinePrinter(lpBlank);

		for (int i = 0; i < (NUM_DATA_LINES - (NUM_COMMANDS + 2)) / 2; i++)
			pagePrinter.addLinePrinter(lpBlank);

		sp = StringPrinter("Commands", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		pagePrinter.addLinePrinter(lpBlank);

		sp = StringPrinter("key \"text string for key\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("in (file/text/clear) \"location of file or text string for input\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("out (file/text/clear) \"location of file for output\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("encrypt", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("decrypt", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);

		for (int i = 0; i < NUM_DATA_LINES - ((NUM_DATA_LINES - (NUM_COMMANDS + 1)) / 2); i++)
			pagePrinter.addLinePrinter(lpBlank);

		pagePrinter.addLinePrinter(lpBlank);
	} else {
		StringPrinter spPathOut;
		if (!outFilePath.empty()) {
			spPathOut = StringPrinter("Mode: File   Path: " + outFilePath, FG_BLACK | BG_WHITE, hScreenBuffer);
			spPathOut.addAttribute(5, FG_GREY | BG_WHITE);
			spPathOut.addAttribute(10, FG_BLACK | BG_WHITE);
			spPathOut.addAttribute(18, FG_GREY | BG_WHITE);
		} else {
			spPathOut = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
			spPathOut.addAttribute(5, FG_GREY | BG_WHITE);
		}

		LinePrinter lpPathOut(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spPathOut, FG_BLACK | BG_WHITE, hScreenBuffer);

		StringPrinter spDataOutHeader = StringPrinter("Data Out", FG_BLACK | BG_WHITE, hScreenBuffer);
		LinePrinter lpDataOutHeader = LinePrinter(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spDataOutHeader, FG_BLACK | BG_WHITE, hScreenBuffer);

		if (crypto->getOutputData()->empty() || encrypting || decrypting) {
			// Print input normally, print blanks for data out
			pagePrinter.addLinePrinter(lpDataInHeader);
			if (encrypting || decrypting)
				pagePrinter.addLinePrinter(lpDataOutHeader);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);
			pagePrinter.addLinePrinter(lpBlank);
			pagePrinter.addLinePrinter(lpPathIn);
			if (encrypting || decrypting) 
				pagePrinter.addLinePrinter(lpPathOut);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);

			int numLines = ceil((double) crypto->getInputData()->size() / (double) DATA_WIDTH);

			for (int i = dataIndex; i < dataIndex + NUM_DATA_LINES; i++) {
				if (i < numLines) {
					int endIndex = (i + 1) * DATA_WIDTH < crypto->getInputData()->size() ? (i + 1) * DATA_WIDTH : crypto->getInputData()->size();
					endIndex -= i * DATA_WIDTH;

					char line[DATA_WIDTH + 1];
					for (int o = 0; o < endIndex; o++)
						line[o] = (*crypto->getInputData())[i * DATA_WIDTH + o];
					line[endIndex] = '\0';

					sp = StringPrinter(line, FG_GREY | BG_WHITE, hScreenBuffer);
					lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_GREY | BG_WHITE, hScreenBuffer);
					pagePrinter.addLinePrinter(lp);
					if (encrypting || decrypting) {
						if (i - dataIndex == NUM_DATA_LINES / 2) {
							if (encrypting)
								sp = StringPrinter("Encrypting...", FG_GREY | BG_WHITE, hScreenBuffer);
							else
								sp = StringPrinter("Decrypting...", FG_GREY | BG_WHITE, hScreenBuffer);

							lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_GREY | BG_WHITE, hScreenBuffer);
							pagePrinter.addLinePrinter(lp);
						} else {
							pagePrinter.addLinePrinter(lpHalfBlank);
						}
					} else {
						pagePrinter.addLinePrinter(lpHalfBlank);
					}
				} else {
					pagePrinter.addLinePrinter(lpHalfBlank);
					if (encrypting || decrypting) {
						if (i - dataIndex == NUM_DATA_LINES / 2) {
							if (encrypting)
								sp = StringPrinter("Encrypting...", FG_GREY | BG_WHITE, hScreenBuffer);
							else
								sp = StringPrinter("Decrypting...", FG_GREY | BG_WHITE, hScreenBuffer);

							lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_GREY | BG_WHITE, hScreenBuffer);
							pagePrinter.addLinePrinter(lp);
						} else {
							pagePrinter.addLinePrinter(lpHalfBlank);
						}
					} else {
						pagePrinter.addLinePrinter(lpHalfBlank);
					}
				}
			}

			std::stringstream index;
			index << dataIndex << " - " << (dataIndex + NUM_DATA_LINES) << " (" << numLines << ")";

			sp = StringPrinter(index.str(), FG_BLACK | BG_WHITE, hScreenBuffer);
			lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
			pagePrinter.addLinePrinter(lp);
			pagePrinter.addLinePrinter(lpHalfBlank);
		} else {
			// Print everything
			pagePrinter.addLinePrinter(lpDataInHeader);
			pagePrinter.addLinePrinter(lpDataOutHeader);
			pagePrinter.addLinePrinter(lpBlank);
			pagePrinter.addLinePrinter(lpPathIn);
			pagePrinter.addLinePrinter(lpPathOut);

			int numLinesIn = ceil((double) crypto->getInputData()->size() / (double) DATA_WIDTH);
			int numLinesOut = ceil((double) crypto->getOutputData()->size() / (double) DATA_WIDTH);

			for (int i = dataIndex; i < dataIndex + NUM_DATA_LINES; i++) {
				if (i < numLinesOut) {
					int inEndIndex = (i + 1) * DATA_WIDTH < crypto->getInputData()->size() ? (i + 1) * DATA_WIDTH : crypto->getInputData()->size();
					inEndIndex -= i * DATA_WIDTH;

					int outEndIndex = (i + 1) * DATA_WIDTH < crypto->getOutputData()->size() ? (i + 1) * DATA_WIDTH : crypto->getOutputData()->size();
					outEndIndex -= i * DATA_WIDTH;

					char line[DATA_WIDTH + 1];

					if (i < numLinesIn) {
						for (int o = 0; o < inEndIndex; o++)
							line[o] = (*crypto->getInputData())[i * DATA_WIDTH + o];
						line[inEndIndex] = '\0';

						sp = StringPrinter(line, FG_GREY | BG_WHITE, hScreenBuffer);
						lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_GREY | BG_WHITE, hScreenBuffer);
						pagePrinter.addLinePrinter(lp);
					} else {
						pagePrinter.addLinePrinter(lpHalfBlank);
					}

					for (int o = 0; o < outEndIndex; o++)
						line[o] = (*crypto->getOutputData())[i * DATA_WIDTH + o];
					line[outEndIndex] = '\0';

					sp = StringPrinter(line, FG_GREY | BG_WHITE, hScreenBuffer);
					lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_GREY | BG_WHITE, hScreenBuffer);
					pagePrinter.addLinePrinter(lp);
					
				} else {
					pagePrinter.addLinePrinter(lpBlank);
				}
			}

			std::stringstream index;
			index << dataIndex << " - " << (dataIndex + NUM_DATA_LINES) << " (" << numLinesIn << ")";

			sp = StringPrinter(index.str(), FG_BLACK | BG_WHITE, hScreenBuffer);
			lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
			pagePrinter.addLinePrinter(lp);
			
			index.str(std::string());
			index << dataIndex << " - " << (dataIndex + NUM_DATA_LINES) << " (" << numLinesOut << ")";

			sp = StringPrinter(index.str(), FG_BLACK | BG_WHITE, hScreenBuffer);
			lp = LinePrinter(SCREEN_WIDTH / 2, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
			pagePrinter.addLinePrinter(lp);
		}
	}
}

void TUI::printAlgorithm(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
		const static int MARGIN = 30;

		LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

		StringPrinter sp;
		LinePrinter lp;

		sp = StringPrinter("Algorithm", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		pagePrinter.addLinePrinter(lpBlank);

		lp = LinePrinter(SCREEN_WIDTH, LinePrinter::ALIGN_CENTER, MARGIN, MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);

		if (algorithmIndex == 0) {
				sp = StringPrinter("[Brian]", FG_BLACK | BG_WHITE, hScreenBuffer);
				lp.addStringPrinter(sp);
		} else {
				sp = StringPrinter("Brian", FG_BLACK | BG_WHITE, hScreenBuffer);
				lp.addStringPrinter(sp);
		}

		if (algorithmIndex == 1) {
			sp = StringPrinter("[Dharmesh]", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		} else {
			sp = StringPrinter("Dharmesh", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		}

		if (algorithmIndex == 2) {
			sp = StringPrinter("[Sung]", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		} else {
			sp = StringPrinter("Sung", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		}
				
		if (algorithmIndex == 3) {
			sp = StringPrinter("[Ryan]", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		} else {
			sp = StringPrinter("Ryan", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		}

		if (algorithmIndex == 4) {
			sp = StringPrinter("[Sunny]", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		} else {
			sp = StringPrinter("Sunny", FG_BLACK | BG_WHITE, hScreenBuffer);
			lp.addStringPrinter(sp);
		}

		pagePrinter.addLinePrinter(lp);
}

void TUI::printCommands(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
		const static std::string prompt = "> ";

		LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

		StringPrinter sp;
		LinePrinter lp;

		for (int i = 0; i < NUM_COMMAND_HISTORY_LINES; i++) {
			if (commandLines.size() >= NUM_COMMAND_HISTORY_LINES - i) {
				lp = LinePrinter(SCREEN_WIDTH, LinePrinter::ALIGN_LEFT, LEFT_MARGIN, RIGHT_MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
				sp = StringPrinter(commandLines[(commandLines.size() - NUM_COMMAND_HISTORY_LINES) + i], FG_BLACK | BG_WHITE, hScreenBuffer);
				lp.addStringPrinter(sp);
				pagePrinter.addLinePrinter(lp);
			} else {
				pagePrinter.addLinePrinter(lpBlank);
			}
		}

		lp = LinePrinter(SCREEN_WIDTH, LinePrinter::ALIGN_LEFT, LEFT_MARGIN, RIGHT_MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
		sp = StringPrinter(prompt, FG_BLACK | BG_WHITE, hScreenBuffer);
		lp.addStringPrinter(sp);
		pagePrinter.addLinePrinter(lp);
}