#include "Sink.hpp"
#include "helpers.hpp"
#include <cmath>

Display* Display::clone() const
{
	return new Display(*this);
}

void Display::displayFile(const Text& text) const
{
	for (size_t i = 0; i < text.getLinesCount(); ++i)
		std::cout << text.getLine(i + 1) << "\n";
}


WrapDisplay::WrapDisplay(const MyVector<MyString>& operands)
	:Display()
{
	if (operands.getSize() == 0)
	{
		columnsCount = INITCOLUMNSCOUNT;
		return;
	}
	if (!isNumber(operands[0].getData()))
		throw std::invalid_argument("wrap command expects number as parameter\n");
	int count = std::stoi(operands[0].getData());
	if(count <= 0)
		throw std::invalid_argument("wrap command expects number > 0\n");
	columnsCount = count;
}
WrapDisplay* WrapDisplay::clone() const
{
	return new WrapDisplay(*this);
}

void WrapDisplay::displayFile(const Text& text) const
{
	size_t symbolsCount = 0;
	for (size_t i = 0; i < text.getLinesCount(); ++i)
	{
		for (size_t j = 0; j < text.getLine(i + 1).getSize(); ++j)
		{
			if (text.getLine(i + 1)[j] == '\n')
			    symbolsCount = 0;
			if (symbolsCount != 0 || text.getLine(i + 1)[j] != ' ')
			{
				std::cout << text.getLine(i + 1)[j];
				++symbolsCount;
			}
			if (symbolsCount == columnsCount)
			{
				if (!wholeWord(text.getLine(i + 1) + j))
				{
					while (*(text.getLine(i + 1).getData() + j) && text.getLine(i + 1)[j] != ' ')
					{
						++j;
						std::cout << text.getLine(i + 1)[j];
					}
				}
				std::cout << '\n';
				symbolsCount = 0;
			}
		}
		std::cout << '\n';
		symbolsCount = 0;
	}
}

CenterDisplay::CenterDisplay(const MyVector<MyString>& operands)
	:Display()
{
	if (operands.getSize() == 0)
	{
		width = INITCOLWIDTH;
		return;
	}
	if (!isNumber(operands[0].getData()))
		throw std::invalid_argument("wrap command expects number as parameter\n");
	int whitespacesCount = std::stoi(operands[0].getData());
	if (whitespacesCount <= 0)
		throw std::invalid_argument("wrap command expects number > 0\n");
	width = whitespacesCount;
}

CenterDisplay* CenterDisplay::clone() const
{
	return new CenterDisplay(*this);
}

void CenterDisplay::displayFile(const Text& text) const
{
	size_t spacesCount = 0;
	for (size_t i = 0; i < text.getLinesCount(); ++i)
	{
		spacesCount = (width > text.getLine(i + 1).getSize() / 2) ? width - (text.getLine(i + 1).getSize() / 2) : 0;
		for (size_t j = 0; j < spacesCount; ++j)
			std::cout << " ";
		std::cout << text.getLine(i + 1) << "\n";
	}
}