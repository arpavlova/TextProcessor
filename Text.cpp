#include "Text.hpp"

Text::Text(const MyString& str)
{
}

Text::Text(std::istream& istr)
{
}

size_t Text::getLinesCount() const
{
	return text.getSize();
}

size_t Text::getSymbolsCount() const
{
	size_t count = 0;
	for (size_t i = 0; i < text.getSize(); ++i)
		count += text[i].getSize();
	return count;
}

const MyString& Text::getLine(size_t linePos) const
{
	if (linePos > text.getSize() || linePos == 0)
		throw std::out_of_range("The line position is outside the text\n");
	return text[linePos - 1];
}

MyString& Text::getLine(size_t linePos)
{
	if (linePos > text.getSize() || linePos == 0)
		throw std::out_of_range("The line position is outside the text\n");
	return text[linePos - 1];
}

void Text::addLine(const MyString& line)
{
	text.pushBack(line);
}

void Text::removeLine(size_t index)
{
	if(index == 0 || index > text.getSize())
		throw std::out_of_range("index >= text.getSize() || index == 0 in void Text::removeLine(size_t index)\n");
	text.remove(index - 1);
}

void Text::addLine(const MyString& line, size_t index)
{
	if (index == 1 && text.getSize() == 0)
	{
		addLine(line);
		return;
	}
	if (index > text.getSize() || index == 0)
		throw std::out_of_range("index >= text.getSize() || index == 0 in void Text::addLine(const MyString& line, size_t index)\n");
	if (index - 1 == text.getSize())
	{
		addLine(line);
		return;
	}
	text.pushBack(text[text.getSize() - 1]);
	for (int i = text.getSize() - 1; i > index - 1; --i)
		text[i] = text[i - 1];
	text[index - 1] = line;
}

void Text::replaceLine(size_t index,const MyString& line)
{
	if (index >= text.getSize())
	    throw std::out_of_range("index >= text.getSize() in Text::replaceLine(size_t index,const MyString& line)\n");
	text[index] = line;
}