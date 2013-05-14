#include "TUI.h"
#include "Brian.h"
#include "Dharmesh.h"
#include "Sung.h"
#include "Ryan.h"
#include "Sunny.h"

#include <string>
#include <sstream>

TUI::TUI(Crypto *crypto, HANDLE hOutput, HANDLE hInput) {
	this->crypto = crypto;
	this->hOutput = hOutput;
	this->hInput = hInput;
	dataIndex = 0;
	algorithmIndex = 0;
	curLine = std::string();
	commandLines = std::vector<std::string>();
	encrypting = false;
	decrypting = false;
	loading = false;
	error = false;
	altPressed = false;

	if (this->crypto->getCipher() == NULL)
		this->crypto->setCipher(new Brian());

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

	try {
		crypto->loadInputFile(inFilePath);
	} catch (int e) {
		// Durp
	}

	printPage();
}

TUI::~TUI() {
	delete crypto;
}

int TUI::processNext() {
	const static std::string prompt = "> ";

	if (encrypting || decrypting || loading) {
		if (encrypting || decrypting) {
			if (!crypto->getInputData()->empty()) {
				if (!key.empty()) {
					try {
						if (encrypting) {
							crypto->encrypt();
							encrypting = false;
						} else if (decrypting) {
							crypto->decrypt();
							decrypting = false;
						}
						if (!outFilePath.empty())
							crypto->saveOutputFile(outFilePath);
					} catch (int e) {
						switch (e) {
							case -1:
								setError("Invalid file size");
								break;
							case 0:
								setError("Unable to save to output file");
								break;
						}
					}
				} else {
					setError("No key set");
					encrypting = false;
					decrypting = false;
				}
			} else {
				setError("No input data loaded");
				encrypting = false;
				decrypting = false;
			}
		} else {
			try {
				crypto->loadInputFile(inFilePath);
			} catch (int e) {
				setError("Unable to load input file");
			}

			loading = false;
		}

		return CONTINUE_REFRESH;
	}

	int keyPressed = getLine();

	if (error) {
		error = false;
		return CONTINUE_REFRESH;
	}

	switch (keyPressed) {
		case VK_MENU:
			if (altPressed)
				return CONTINUE_NOREFRESH;
			else
				altPressed = true;
			break;
		case ALT_RELEASED:
			altPressed = false;
			return CONTINUE_REFRESH;
		case VK_UP:
			if (dataIndex > 0)
				dataIndex--;
			break;
		case VK_DOWN:
			dataIndex++;
			break;
		case VK_PRIOR:
			if (dataIndex > NUM_DATA_LINES)
				dataIndex -= NUM_DATA_LINES;
			else
				dataIndex = 0;
			break;
		case VK_NEXT:
			dataIndex += NUM_DATA_LINES;
			break;
		case VK_LEFT:
			if (algorithmIndex > 0)
				algorithmIndex--;
			setCipher();
			break;
		case VK_RIGHT:
			if (algorithmIndex < 4)
				algorithmIndex++;
			setCipher();
			break;
		case VK_RETURN:
			int returnValue = processLine(curLine);
			commandLines.push_back(prompt + curLine);
			curLine = std::string();
			return returnValue;
	}

	return CONTINUE_REFRESH;
}

void TUI::setCipher() {
	switch (algorithmIndex) {
		case 0:
			crypto->setCipher(new Brian());
			break;
		case 1:
			crypto->setCipher(new Dharmesh());
			break;
		case 2:
			crypto->setCipher(new Sung());
			break;
		case 3:
			crypto->setCipher(new Ryan());
			break;
		case 4:
			crypto->setCipher(new Sunny());
			break;
	}

	char buff[Cipher::BLOCK_SIZE];
	for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
		buff[i] = key.at(i % key.length());
	crypto->getCipher()->setKey(buff);
}

int TUI::processLine(std::string line) {
	line = lowerString(line);

	int spaceIndex = line.find(' ');

	if (spaceIndex == std::string::npos) {
		if (line.compare("exit") == 0)
			return EXIT;
		else if (line.compare("encrypt") == 0)
			encrypting = true;
		else if (line.compare("decrypt") == 0)
			decrypting = true;
		else
			setError("Invalid command");
	} else {
		std::string substr1 = line.substr(0, spaceIndex);

		if (substr1.compare("in") == 0 || substr1.compare("out") == 0 || substr1.compare("key") == 0) {
			std::string substr2 = line.substr(spaceIndex + 1, line.length() - spaceIndex);

			if (substr1.compare("key") == 0) {
				if (line.length() > spaceIndex) {
					this->key = substr2;
					char buff[Cipher::BLOCK_SIZE];
					for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
						buff[i] = substr2.at(i % substr2.length());
					crypto->getCipher()->setKey(buff);
				} else {
					setError("Invalid key length");
				}
			} else {
				if (substr2.compare("clear") == 0) {
					if (substr1.compare("in") == 0) {
						crypto->clearInput();
						inFilePath = std::string();
					} else {
						crypto->clearOutput();
						outFilePath = std::string();
					}
				} else if (substr1.compare("out") == 0 && substr2.compare("text") == 0) {
					outFilePath = std::string();
					crypto->clearOutput();
				} else {
					int spaceIndex2 = line.find(' ', spaceIndex + 1);

					if (spaceIndex2 != std::string::npos) {
						substr2 = line.substr(spaceIndex + 1, spaceIndex2 - (spaceIndex + 1));

						if (substr2.compare("text") == 0 || substr2.compare("file") == 0) {
							if (substr2.compare("text") == 0) {
								if (substr1.compare("in") == 0) {
									if (line.length() > spaceIndex2) {
										std::string substr3 = line.substr(spaceIndex2 + 1, line.length() - (spaceIndex2 + 1));
										std::vector<char> inData = std::vector<char>();
										for (int i = 0; i < substr3.length(); i++)
											inData.push_back(substr3.at(i));
										inFilePath = std::string();
										crypto->setInputData(inData);
									} else {
										setError("Invalid text length");
									}
								} 
							} else {
								if (line.length() > spaceIndex2) {
									std::string substr3 = line.substr(spaceIndex2 + 1, line.length() - (spaceIndex2 + 1));
								
									if (substr1.compare("in") == 0) {
										inFilePath = substr3;
										crypto->clearInput();
										loading = true;
									} else {
										outFilePath = substr3;
										crypto->clearOutput();
									}
								} else {
									setError("Invalid command");
								}
							}
						} else {
							setError("Invalid command");
						}
					} else {
						setError("Invalid command");
					}
				}
			}
		} else {
			setError("Invalid command");
		}
	}
	
	return CONTINUE_REFRESH;
}

std::string TUI::lowerString(std::string str) {
	char *buff = new char[str.length() + 1];
	for (int i = 0; i < str.length(); i++)
		buff[i] = tolower(str.at(i));
	buff[str.length()] = '\0';

	str = std::string(buff);
	delete [] buff;

	return str;
}

void TUI::setError(std::string errorMessage) {
	error = true;
	this->errorMessage = errorMessage;
	showCursor(false);
}

void TUI::showCursor(bool hide) {
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hOutput, &cci);
		cci.bVisible = hide;
		SetConsoleCursorInfo(hOutput, &cci);
}

int TUI::getLine() {
	INPUT_RECORD prInput;
	DWORD lpNumRead, lpNumWritten;

	const static int length = 256;
	char chBuffer[length];
	chBuffer[0] = '\0';
	int index = curLine.copy(chBuffer, curLine.length(), 0);

	while (true) {
		ReadConsoleInput(hInput, &prInput, 1, &lpNumRead);
		if (prInput.EventType == KEY_EVENT) {
			if (prInput.Event.KeyEvent.bKeyDown == true) {
				if (!error) {
					if (isprint(prInput.Event.KeyEvent.uChar.AsciiChar)) {
						chBuffer[index++ % length] = prInput.Event.KeyEvent.uChar.AsciiChar;
						WriteConsole(hOutput, &prInput.Event.KeyEvent.uChar.AsciiChar, 1, &lpNumWritten, NULL);
					} else if (iscntrl(prInput.Event.KeyEvent.uChar.AsciiChar)) {
						switch (prInput.Event.KeyEvent.wVirtualKeyCode) {
							case VK_MENU:
							case VK_UP:
							case VK_DOWN:
							case VK_PRIOR:
							case VK_NEXT:
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
									return VK_RETURN;
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
					return 0;
				}
			} else {
				if (prInput.Event.KeyEvent.wVirtualKeyCode == VK_MENU)
					return ALT_RELEASED;
			}
		}
	}
}

void TUI::printPage() {
	const static char prompt[] = "> ";
	const static char blank = ' ';
	DWORD cWritten;

	HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTextAttribute(hScreenBuffer, FG_BLACK | BG_WHITE);

	PagePrinter pagePrinter = PagePrinter();

	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);

	StringPrinter spHeader1 = StringPrinter("Alpha Team", hScreenBuffer);
	LinePrinter lpHeader1 = LinePrinter(SCREEN_WIDTH, spHeader1, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader1);

	StringPrinter spHeader2 = StringPrinter("Final Project", hScreenBuffer);
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
	pagePrinter.addLinePrinter(lpBlank);
	printCommands(pagePrinter, hScreenBuffer);

	pagePrinter.printPage();

	if (!error) {
		for (int i = 0; i < LEFT_MARGIN; i++)
				WriteConsole(hScreenBuffer, &blank, 1, &cWritten, NULL);

		WriteConsole(hScreenBuffer, prompt, 2, &cWritten, NULL);
		WriteConsole(hScreenBuffer, curLine.c_str(), curLine.length(), &cWritten, NULL);
	}

	SetConsoleActiveScreenBuffer(hScreenBuffer);
	CloseHandle(hOutput);
	hOutput = hScreenBuffer;

	if (error)
		showCursor(false);
}

void TUI::printData(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
	const static int HEADER_LINES = 4;
	const static int NUM_COMMANDS = 5;
	const static int PATH_MARGIN = 7;

	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpHalfBlank = LinePrinter(SCREEN_WIDTH - (SCREEN_WIDTH / 2), NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter spDataInHeader = StringPrinter("Data In:", FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpDataInHeader = LinePrinter(SCREEN_WIDTH / 2, spDataInHeader, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter sp;
	LinePrinter lp;

	StringPrinter spPathIn;
	StringPrinter spModeIn;
	if (inFilePath.empty())
		spModeIn = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
	else
		spModeIn = StringPrinter("Mode: File", FG_BLACK | BG_WHITE, hScreenBuffer);
	spModeIn.addAttribute(5, FG_BROWN | BG_WHITE);
	if (!inFilePath.empty()) {
		std::string pathStr;
		if (inFilePath.length() + 6 > (SCREEN_WIDTH / 2) - (PATH_MARGIN * 2))
			pathStr = "Path: " + inFilePath.substr(0, ((SCREEN_WIDTH / 2) - (PATH_MARGIN * 2)) - 9) + "...";
		else
			pathStr = "Path: " + inFilePath;
		spPathIn = StringPrinter(pathStr, FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathIn.addAttribute(5, FG_BROWN | BG_WHITE);
	}
	LinePrinter lpModeIn(SCREEN_WIDTH / 2, spModeIn, FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpPathIn(SCREEN_WIDTH / 2, LinePrinter::ALIGN_CENTER, PATH_MARGIN, PATH_MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
	lpPathIn.addStringPrinter(spPathIn);

	if (loading) {
		// Print loading...
		pagePrinter.addLinePrinter(lpDataInHeader);
		pagePrinter.addLinePrinter(lpHalfBlank);
		pagePrinter.addLinePrinter(lpBlank);
		pagePrinter.addLinePrinter(lpModeIn);
		pagePrinter.addLinePrinter(lpHalfBlank);
		if (!inFilePath.empty())
			pagePrinter.addLinePrinter(lpPathIn);
		else
			pagePrinter.addLinePrinter(lpHalfBlank);
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

		sp = StringPrinter("Commands:", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		pagePrinter.addLinePrinter(lpBlank);

		sp = StringPrinter("key \"text string for key\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		sp.addAttribute(5, FG_GREY | BG_WHITE);
		sp.addAttribute(24, FG_BLACK | BG_WHITE);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("in [file/text/clear] \"location of file or text string for input\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		sp.addAttribute(22, FG_GREY | BG_WHITE);
		sp.addAttribute(63, FG_BLACK | BG_WHITE);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("out [file/text/clear] \"location of file for output\"", FG_BLACK | BG_WHITE, hScreenBuffer);
		sp.addAttribute(23, FG_GREY | BG_WHITE);
		sp.addAttribute(50, FG_BLACK | BG_WHITE);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("encrypt", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("decrypt", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);

		for (int i = 0; i < NUM_DATA_LINES - ((NUM_DATA_LINES - (NUM_COMMANDS + 2)) / 2) - (NUM_COMMANDS + 2); i++)
			pagePrinter.addLinePrinter(lpBlank);

		pagePrinter.addLinePrinter(lpBlank);
	} else {
		StringPrinter spPathOut;
		StringPrinter spModeOut;

		if (outFilePath.empty())
			spModeOut = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
		else
			spModeOut = StringPrinter("Mode: File", FG_BLACK | BG_WHITE, hScreenBuffer);
		spModeOut.addAttribute(5, FG_BROWN | BG_WHITE);
		if (!outFilePath.empty()) {
			std::string pathStr;
			if (outFilePath.length() + 6 > (SCREEN_WIDTH / 2) - (PATH_MARGIN * 2))
				pathStr = "Path: " + outFilePath.substr(0, ((SCREEN_WIDTH / 2) - (PATH_MARGIN * 2)) - 9) + "...";
			else
				pathStr = "Path: " + outFilePath;
			spPathOut = StringPrinter(pathStr, FG_BLACK | BG_WHITE, hScreenBuffer);
			spPathOut.addAttribute(5, FG_BROWN | BG_WHITE);
		}
		LinePrinter lpModeOut(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spModeOut, FG_BLACK | BG_WHITE, hScreenBuffer);
		LinePrinter lpPathOut(SCREEN_WIDTH - (SCREEN_WIDTH / 2), LinePrinter::ALIGN_CENTER, PATH_MARGIN, PATH_MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
		lpPathOut.addStringPrinter(spPathOut);

		StringPrinter spDataOutHeader = StringPrinter("Data Out:", FG_BLACK | BG_WHITE, hScreenBuffer);
		LinePrinter lpDataOutHeader = LinePrinter(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spDataOutHeader, FG_BLACK | BG_WHITE, hScreenBuffer);

		if (crypto->getOutputData()->empty() || encrypting || decrypting) {
			// Print input normally, print blanks for data out
			pagePrinter.addLinePrinter(lpDataInHeader);
			if (encrypting || decrypting)
				pagePrinter.addLinePrinter(lpDataOutHeader);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);
			pagePrinter.addLinePrinter(lpBlank);

			pagePrinter.addLinePrinter(lpModeIn);
			if (encrypting || decrypting) 
				pagePrinter.addLinePrinter(lpModeOut);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);

			if (!inFilePath.empty())
				pagePrinter.addLinePrinter(lpPathIn);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);

			if (encrypting || decrypting) {
				if (!outFilePath.empty())
					pagePrinter.addLinePrinter(lpPathOut);
				else
					pagePrinter.addLinePrinter(lpHalfBlank);
			} else
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
			pagePrinter.addLinePrinter(lpModeIn);
			pagePrinter.addLinePrinter(lpModeOut);
			if (!inFilePath.empty())
				pagePrinter.addLinePrinter(lpPathIn);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);
			if (!outFilePath.empty())
				pagePrinter.addLinePrinter(lpPathOut);
			else
				pagePrinter.addLinePrinter(lpHalfBlank);

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
		const static int WIDTH = (SCREEN_WIDTH - (MARGIN * 2)) / 5;

		LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

		StringPrinter sp;
		LinePrinter lp;

		sp = StringPrinter("Algorithm:", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		pagePrinter.addLinePrinter(lpBlank);

		lp = LinePrinter(WIDTH + MARGIN, LinePrinter::ALIGN_CENTER, MARGIN, 0, FG_BLACK | BG_WHITE, hScreenBuffer);
		if (algorithmIndex == 0) {
				sp = StringPrinter("[Brian]", FG_BLACK | BG_WHITE, hScreenBuffer);
				sp.addAttribute(1, FG_BROWN | BG_WHITE);
				sp.addAttribute(6, FG_BLACK | BG_WHITE);
		} else 
				sp = StringPrinter("Brian", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp.addStringPrinter(sp);
		pagePrinter.addLinePrinter(lp);

		if (algorithmIndex == 1) {
			sp = StringPrinter("[Dharmesh]", FG_BLACK | BG_WHITE, hScreenBuffer);
			sp.addAttribute(1, FG_BROWN | BG_WHITE);
			sp.addAttribute(9, FG_BLACK | BG_WHITE);
		} else
			sp = StringPrinter("Dharmesh", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);

		if (algorithmIndex == 2) {
			sp = StringPrinter("[Sung]", FG_BLACK | BG_WHITE, hScreenBuffer);
			sp.addAttribute(1, FG_BROWN | BG_WHITE);
			sp.addAttribute(5, FG_BLACK | BG_WHITE);
		} else 
			sp = StringPrinter("Sung", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
				
		if (algorithmIndex == 3) {
			sp = StringPrinter("[Ryan]", FG_BLACK | BG_WHITE, hScreenBuffer);
			sp.addAttribute(1, FG_BROWN | BG_WHITE);
			sp.addAttribute(5, FG_BLACK | BG_WHITE);
		} else
			sp = StringPrinter("Ryan", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);

		lp = LinePrinter(SCREEN_WIDTH - (WIDTH * 4 + MARGIN), LinePrinter::ALIGN_CENTER, 0, MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
		if (algorithmIndex == 4) {
			sp = StringPrinter("[Sunny]", FG_BLACK | BG_WHITE, hScreenBuffer);
			sp.addAttribute(1, FG_BROWN | BG_WHITE);
			sp.addAttribute(6, FG_BLACK | BG_WHITE);
		} else
			sp = StringPrinter("Sunny", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp.addStringPrinter(sp);
		pagePrinter.addLinePrinter(lp);
}

void TUI::printCommands(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter sp;
	LinePrinter lp;

	if (error) {
		pagePrinter.addLinePrinter(lpBlank);
		pagePrinter.addLinePrinter(lpBlank);

		sp = StringPrinter("[Error] " + errorMessage, FG_RED | BG_WHITE, hScreenBuffer);
		sp.addAttribute(7, FG_BLACK | BG_WHITE);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
		sp = StringPrinter("Press any key to continue...", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);
	} else {
		sp = StringPrinter("Terminal:", FG_BLACK | BG_WHITE, hScreenBuffer);
		lp = LinePrinter(SCREEN_WIDTH, sp, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lp);

		pagePrinter.addLinePrinter(lpBlank);

		int num = commandLines.size() < NUM_COMMAND_HISTORY_LINES ? commandLines.size() : NUM_COMMAND_HISTORY_LINES;

		for (int i = 0; i < num; i++) {
			lp = LinePrinter(SCREEN_WIDTH, LinePrinter::ALIGN_LEFT, LEFT_MARGIN, RIGHT_MARGIN, FG_BLACK | BG_WHITE, hScreenBuffer);
			sp = StringPrinter(commandLines[(commandLines.size() - num) + i], FG_BLACK | BG_WHITE, hScreenBuffer);
			sp.addAttribute(1, FG_GREY | BG_WHITE);
			lp.addStringPrinter(sp);
			pagePrinter.addLinePrinter(lp);
		}
	}
}