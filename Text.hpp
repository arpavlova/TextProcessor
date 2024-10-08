#ifndef _TEXT_
#define _TEXT_
#include "MyString.hpp"
#include "MyVector.hpp"

class Text
{
public:
	Text() = default;
	Text(const MyString& str);
	Text(std::istream& istr);

	size_t getLinesCount() const;
	size_t getSymbolsCount() const;
	const MyString& getLine(size_t linePos) const;
	MyString& getLine(size_t linePos);

	void addLine(const MyString& line);
	void removeLine(size_t index);
	void addLine(const MyString& line, size_t index);
	void replaceLine(size_t index,const MyString& line);

private:
	MyVector<MyString> text;
};


#endif

