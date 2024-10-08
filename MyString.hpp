#ifndef _MYSTRING_
#define _MYSTRING_
short const INITSIZE = 64;
short const STRCOEF = 15;
#include <iostream>
#include <fstream>
class MyString
{
public:
	MyString(size_t initSize = INITSIZE);
	MyString(MyString&& other) noexcept;
	MyString(const MyString& other);
	MyString(char symbol);
	MyString(const char* str);

	MyString& operator=(MyString&& other) noexcept;
	MyString& operator+=(const MyString& other);
	MyString& operator=(const MyString& other);

	~MyString();

	/// returns the content of the (data + index) memory address
	/// no bound checking is performed
	const char& operator[](size_t index) const;
	char& operator[](size_t index);

	/// returns the content of the (data + index) memory address
	/// bound checking is performed
	/// throws std::out_of_range exeption when index >= size
	char at(size_t index) const;
	char& at(size_t index);

	void push_back(char symbol);
	void clear();

	bool operator==(const MyString& other) const;
	bool operator!=(const MyString& other) const;
	bool operator<=(const MyString& other) const;
	bool operator>=(const MyString& other) const;
	bool operator>(const MyString& other) const;
	bool operator<(const MyString& other) const;

	const char* getData() const;
	size_t getSize() const;
	size_t getCapacity() const;

private:
	void allocateMemory(size_t sizeToAllocate);
	void copyFromOther(const MyString& other);
	void moveToMine(MyString&& other);
	void resize(size_t sizeToResize);
	void free();

private:
	char* data = nullptr;
	size_t capacity = 0;
	size_t size = 0;
};

std::ostream& operator<<(std::ostream& os, const MyString& string);
std::istream& operator>>(std::istream& is, MyString& string);

const MyString& getIthArgument(const MyString& sentence, MyString& word, size_t index, bool whole = false);

MyString operator+(const MyString& mine, const MyString& other);
MyString cutString(const MyString& str, size_t beg, int end);


#endif