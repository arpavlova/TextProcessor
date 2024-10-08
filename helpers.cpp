#include "helpers.hpp"
bool isPunctuation(char symbol)
{
	/*static */const char* punctuation = ".,!?:'-(){} /[];";
	while (*punctuation)
	{
		if (symbol == *punctuation)
			return true;
		++punctuation;
	}
	return false;
}

bool isWordDetected(size_t j, const MyString& line, const MyString& word)
{
	if (j + word.getSize() > line.getSize())
		return false;
	const char* buffer = std::strstr(line.getData() + j, word.getData());
	return  (buffer == (line.getData() + j) 
		    && (j == 0 || isPunctuation(line[j - 1])) 
		    && (j + word.getSize() == line.getSize() || isPunctuation(line[j + word.getSize()])));
}

size_t getNumberLength(int number)
{
	number = number < 0 ? -1 * number : number;
	size_t len = 1;
	while (number > 9)
	{
		++len;
		number /= 10;
	}
	return len;
}

bool isNumber(const char* str)
{
	if (!str || str[0] == '\0')
		return false;
	if (*str == '-' && *(str + 1))
		++str;
	size_t i = 0;
	while(*str) 
	{
		if ((*str == '0' && i == 0) || !isdigit(*str))
			return false;
		++str;
		++i;
	}
	return true;
}
bool wholeWord(const MyString& word)
{
	return(*(word.getData() + 1) && isPunctuation(*(word.getData() + 1)) || !*(word.getData() + 1));
}

MyVector<MyString> parseSequence(const MyString& sequence, MyVector<MyString>& tokens)
{
	MyString token;
	for (size_t i = 0; i < sequence.getSize(); ++i)
	{
		if (sequence[i] == ';')
		{
			tokens.pushBack(token);
			token.clear();
		}
		else
			token.push_back(sequence[i]);
	}
	tokens.pushBack(token);
	return tokens;
}


bool hasDotAtPos(const MyString& str, size_t pos)
{
	if (str.getSize() > pos && str[pos] == '.')
		return true;
	return false;
}

bool compare(const MyString& line1, const MyString& line2)
{
	if (isNumber(line1.getData()) && isNumber(line2.getData()))
	{
		int number1 = std::stoi(line1.getData());
		int number2 = std::stoi(line2.getData());
		return number1 <= number2;
	}
	return line1 <= line2;
}
/*
1.cat
2. cat
3.cat?
cat
Mycat
*/