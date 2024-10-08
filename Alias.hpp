#ifndef _ALIAS_
#define _ALIAS_
#include "MyString.hpp"
#include "MyVector.hpp"
const short MAXALIASCOUNT = 64;

class Alias
{
public:
	Alias() = default;
	Alias(const MyVector<MyString>& params);

	const MyString& getName() const;
	const MyString& getSequnece() const;

private:
	MyString name;
	MyString sequence;
};

class HashTableAlias
{
public:
	size_t hashName(const MyString& name) const;
	int contains(const MyString& name) const;
	void addElement(const MyVector<MyString>& params);
	void removeElement(const MyString& name);
	const Alias& getElement(size_t index) const;
private:
	size_t count = 0;
	Alias elements[MAXALIASCOUNT];
};

#endif