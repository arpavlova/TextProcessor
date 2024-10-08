#include "Filter.hpp"

//INRANGEFILTER
//=========================================================================
InRangeFilter::InRangeFilter(const MyVector<MyString>& operands, size_t pos)
{
	if (operands.getSize() < 2 + pos)
	{
		index1 = index2 = -1;
		return;
	}
	if (!isNumber(operands[pos].getData()) || !isNumber(operands[pos + 1].getData()))
		throw std::invalid_argument("Arguments are not numbers\n");
	int number1 = std::stoi(operands[pos].getData());
	int number2 = std::stoi(operands[pos + 1].getData());
	if (number1 <= 0 || number2 <= 0 || number1 > number2)
		throw std::invalid_argument("Range for processing the text is invalid\n");
	index1 = number1 - 1;
	index2 = number2 - 1;
}
void InRangeFilter::filterText(Text& text) const
{
	size_t beg = index1 < 0 ? 0 : index1;
	size_t end = index2 < 0 ? text.getLinesCount() : index2 + 1;
	if (end > text.getLinesCount())
		throw std::out_of_range("index2 >= text.getLinesCount() in void BaseFilter::filterText(Text& text) const\n");
	for (size_t i = beg; i < end; ++i)
	{
		specificFilterLogic(text.getLine(i + 1));
	}
}
//=========================================================================



//ENCODEFILTER
//=========================================================================
EncodeFilter::EncodeFilter(const MyVector<MyString>& operands, int (*setFunEncode)(int))
	:InRangeFilter(operands)
{
	funEncode = setFunEncode;
}
void EncodeFilter::specificFilterLogic(MyString& line) const
{
	if (!isQuote(line))
	{
		for (size_t j = 0; j < line.getSize(); ++j)
			line[j] = funEncode(line[j]);
	}
}
EncodeFilter* EncodeFilter::clone() const
{
	return new EncodeFilter(*this);
}
bool EncodeFilter::isQuote(const MyString& line) const
{
	size_t i = 0;
	int j = line.getSize() - 1;
	while (i < j)
	{
		if (line[i] == '"' && line[i] == line[j])
			return true;
		if (line[i] != line[j] && line[i] != ' ' && line[j] != ' ')
			return false;
		if (line[i] == ' ')
			i++;
		if (line[j] == ' ')
			--j;
	}
	return false;
}
//=========================================================================



//REMOVEEMPTYPLACES
//=========================================================================
RemoveEmptyPlacesFilter::RemoveEmptyPlacesFilter(const MyVector<MyString>& operands)
	:InRangeFilter(operands)
{
}

RemoveEmptyPlacesAtFront::RemoveEmptyPlacesAtFront(const MyVector<MyString>& operands)
	:RemoveEmptyPlacesFilter(operands)
{
}

void RemoveEmptyPlacesAtFront::specificFilterLogic(MyString& line) const
{
	size_t i = 0;
	while (i < line.getSize() && line[i] == ' ')
		++i;
	if (i == line.getSize())
		line = "";
	else if (i != 0)
		line = cutString(line, i, line.getSize() - 1);
}

RemoveEmptyPlacesAtFront* RemoveEmptyPlacesAtFront::clone() const
{
	return new RemoveEmptyPlacesAtFront(*this);
}

RemoveEmptyPlacesAtEnd::RemoveEmptyPlacesAtEnd(const MyVector<MyString>& operands)
	:RemoveEmptyPlacesFilter(operands)
{
}

void RemoveEmptyPlacesAtEnd::specificFilterLogic(MyString& line) const
{
	int i = line.getSize() - 1;
	while (i >= 0 && line[i] == ' ')
		--i;
	if (i < 0)
		line = "";
	else if (i != line.getSize() - 1)
		line = cutString(line, 0, i);
}
RemoveEmptyPlacesAtEnd* RemoveEmptyPlacesAtEnd::clone() const
{
	return new RemoveEmptyPlacesAtEnd(*this);
}
//=========================================================================


//REPLACEWORDFILTER
//=========================================================================
ReplaceWordFilter::ReplaceWordFilter(const MyVector<MyString>& operands, size_t pos)
	:InRangeFilter(operands, pos), wordToBeReplaced(operands[0])
{
	if(operands.getSize() > 1 && operands.getSize() != 3)
		wordToReplaceWith = operands[1];
}
BaseFilter* ReplaceWordFilter::clone() const
{
	return new ReplaceWordFilter(*this);
}
void ReplaceWordFilter::specificFilterLogic(MyString& line) const
{
	MyString buffer;
	for (size_t j = 0; j < line.getSize(); ++j)
	{
		if (isWordDetected(j, line, wordToBeReplaced))
		{
			buffer += wordToReplaceWith;
			if (replaceWithEmptyString
				&& j > 0
				&& j + wordToBeReplaced.getSize() < line.getSize()
				&& line[j - 1] == line[j + wordToBeReplaced.getSize()]
				&& line[j - 1] == ' ')
				++j;
			j += wordToBeReplaced.getSize();
		}
		buffer.push_back(line[j]);
	}
	line = buffer;
	buffer.clear();
}
//=========================================================================


//REMOVALWORDFILTER
//=========================================================================
RemovalWordFilter::RemovalWordFilter(const MyVector<MyString>& operands)
	:ReplaceWordFilter(operands, 1)
{
	replaceWithEmptyString = true;
}

RemovalWordFilter* RemovalWordFilter::clone() const
{
	return new RemovalWordFilter(*this);
}
//=========================================================================



//LEXSORTFILTER
//=========================================================================
LexSortFilter::LexSortFilter(const MyVector<MyString>& operands, bool(*comparator)(const MyString&, const MyString&))
	:InRangeFilter(operands), comparator(comparator)
{
}
void LexSortFilter::filterText(Text& text) const
{
	MyVector<MyString> sortableLines;
	MyVector<size_t> fixedLinesIndexes;
	size_t beg = index1 < 0 ? 0 : index1;
	size_t end = index2 < 0 ? text.getLinesCount() : index2 + 1;
	if (end > text.getLinesCount())
		throw std::out_of_range("index2 >= text.getLinesCount() in void LexSortFilter::filterText(Text& text) const\n");
	for (size_t i = beg; i < end; ++i)
	{
		if (sortableLine(text.getLine(i + 1)))
			sortableLines.pushBack(text.getLine(i + 1));
		else
			fixedLinesIndexes.pushBack(i);
	}
	quickSortRecursive(sortableLines, compare, 0, sortableLines.getSize() - 1);
	size_t fixedIndex = 0;
	size_t sortableIndex = 0;
	size_t i = beg;
	for (i; fixedIndex < fixedLinesIndexes.getSize() && sortableIndex < sortableLines.getSize(); ++i)
	{
		if (i < fixedLinesIndexes[fixedIndex])
			text.replaceLine(i, sortableLines[sortableIndex++]);
		else
			++fixedIndex;
	}
	while (sortableIndex < sortableLines.getSize())
	{
		text.replaceLine(i, sortableLines[sortableIndex++]);
		++i;
	}
}
LexSortFilter* LexSortFilter::clone() const
{
	return new LexSortFilter(*this);
}
bool LexSortFilter::sortableLine(const MyString& line) const
{
	if (isNumber(line.getData()) || isdigit(*line.getData()))
	{
		int number = std::stoi(line.getData());
		if (number != 0)
		{
			size_t numLen = getNumberLength(number);
			if (hasDotAtPos(line, numLen))
				return false;
		}
	}
	return true;
}
//=========================================================================



//SHUFFLEFILTER
//=========================================================================
ShuffleFilter::ShuffleFilter(const MyVector<MyString>& operands)
	:InRangeFilter(operands)
{
}
void ShuffleFilter::filterText(Text& text) const
{
	size_t beg = index1 < 0 ? 0 : index1;
	size_t end = index2 < 0 ? text.getLinesCount() : index2 + 1;
	if (end > text.getLinesCount())
		throw std::out_of_range("index2 >= text.getLinesCount() in void ShuffleFilter::filterText(Text& text) const\n");
	MyString buffer;
	int rand;
	for (size_t i = beg; i < end; ++i)
	{
		rand = beg + (std::rand() % (end - beg));
		buffer = text.getLine(rand + 1);
		text.replaceLine(rand, text.getLine(i + 1));
		text.replaceLine(i, buffer);
	}
}
ShuffleFilter* ShuffleFilter::clone() const
{
	return new ShuffleFilter(*this);
}
//=========================================================================


//REMOVEDUPLICATESFILTER
//=========================================================================
RemoveDublicatesFilter::RemoveDublicatesFilter(const MyVector<MyString>& operands)
	:InRangeFilter(operands)
{
}
/*open TextFiles\DuplicateTest\duplicate.txt*/

void RemoveDublicatesFilter::filterText(Text& text) const
{
	size_t beg = index1 < 0 ? 0 : index1;
	int end = index2 < 0 ? text.getLinesCount() : index2 + 1;
	if (end > text.getLinesCount())
		throw std::out_of_range("index2 >= text.getLinesCount() in void RemodeDublicatesFilter::filterText(Text& text) const\n");
	for (int i = beg + 1; i <= end; ++i)
	{
		for (int j = i + 1; j <= end; ++j)
		{
			if (text.getLine(i) == text.getLine(j))
			{
				text.removeLine(j);
				--i;
				--end;
				break;
			}
		}
	}
}
RemoveDublicatesFilter* RemoveDublicatesFilter::clone() const
{
	return new RemoveDublicatesFilter(*this);
}
//=========================================================================




//LINEFILTER
//=========================================================================
LineFilter::LineFilter(const MyVector<MyString>& operands)
	:BaseFilter()
{
	if (!isNumber(operands[0].getData()))
		throw std::invalid_argument("Argument is not a number\n");
	int number = std::stoi(operands[0].getData());
	if (number <= 0)
		throw std::invalid_argument("Index for processing is invalid\n");
	lineIndex = number;
}
//=========================================================================




//REMOVELINEFILTER
RemoveLineFilter::RemoveLineFilter(const MyVector<MyString>& operands)
	:LineFilter(operands)
{
}
void RemoveLineFilter::filterText(Text& text) const
{
	text.removeLine(lineIndex);
}
RemoveLineFilter* RemoveLineFilter::clone() const
{
	return new RemoveLineFilter(*this);
}
//=========================================================================



//ADDLINEFILTER
//=========================================================================
AddLineFilter::AddLineFilter(const MyVector<MyString>& operands)
	:LineFilter(operands), line(operands[1])
{
}
void AddLineFilter::filterText(Text& text) const
{
	text.addLine(line, lineIndex);
}
AddLineFilter* AddLineFilter::clone() const
{
	return new AddLineFilter(*this);
}
//=========================================================================


//REPLACELINEFILTER
//=========================================================================
ReplaceLineFilter::ReplaceLineFilter(const MyVector<MyString>& operands)
	:LineFilter(operands), line(operands[1])
{
}
void ReplaceLineFilter::filterText(Text& text) const
{
	text.replaceLine(lineIndex - 1, line);
}
ReplaceLineFilter* ReplaceLineFilter::clone() const
{
	return new ReplaceLineFilter(*this);
}
//=========================================================================