#include <Windows.h>
#include <vector>

class StringPrinter {
	private:
		std::string str;
		std::vector<WORD> attributes;
		std::vector<int> attributeIndices;
		HANDLE hOutput;

	public:
		const static WORD DEFAULT_ATTRIBUTES = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

		StringPrinter();
		StringPrinter(StringPrinter &stringPrinter);
		StringPrinter(std::string str, std::vector<WORD> attributes, std::vector<int> attributeIndices, HANDLE hOutput);

		void printString();
		void setString(std::string str);
		std::string getString();
		void addAttribute(WORD attribute, int attributeIndex);
		void setAttributes(std::vector<WORD> attributes, std::vector<int> attributeIndices);
		const std::vector<WORD> * getAttributes();
		const std::vector<int> * getAttributeIndices();
		void setOutputHandle(HANDLE hOutput);
		HANDLE getOutputHandle();
};