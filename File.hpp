#ifndef _FILE_
#define _FILE_
#include "Text.hpp"
#include "MyVector.hpp"
#include "MyString.hpp"
#include "Command.hpp"

const size_t MAXUNDOSCOUNT = 3;

class Display;
class File
{
public:
	File(std::ifstream& ostr, const char* name);
	void removeLastOperation();
	void addOperation(const Command& operation);
	void removeOperations();
	bool hasUnsavedChanges() const;
	void saveChanges(const MyString& path);
	void print(const Display* sink) const;
	const MyString& getName() const;
	const Text& getConten() const;
private:
	bool unsavedChanges = false;
	MyString name;
	Text content;
	MyVector<Command> operations;
};
#endif