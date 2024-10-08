#ifndef _COMMAND_
#define _COMMAND_
#include "Filter.hpp"
#include "Sink.hpp"

enum class CommandMap
{
	Invalid = -1,
	Open, // one parameter - the name of the file
	Save, // zero parameters
	SaveAs, // one parameter - the name of the new path
	Replace, // two parameters - old word and new word
	RemoveWord,// one parameter
	ToUpper, // zero parameters
	ToLower, // zero parameters
	RemoveStartingEmptyPlaces, // zero parameters
	RemoveSEndingEmptyPlaces, // zero parameters
	Sort, // zero parameters
	Shuffle, // zero parameters
	RemoveDuplicates, // zero parameters
	Undo, // zero parameters - pops one command and its operands
	Print,// zero parameters - the text of the file should be shown on the screen
	Center, // zero parameters - the text of the file should be shown on the center of the screen
	Wrap, // one parameter - the wanted columns count
	SwitchFile, // one parameter - the name of the file for update
	RemoveLine,  //one parameter line index
	AddLine,  //two parameters line index and line
	ReplaceLine, //two parameters position and new line
	FilesInfo, //gets all files information
	Close, //removes an active file from the FileManager
	Exit, // zero parameters
	Alias, 
	UserDefined,
	Count
};


class Command
{
public:
	Command() = default;
	Command(const Command& other);
	Command& operator=(const Command& other);
	~Command();

	void setCode(CommandMap c);
	void setOperandsCount(int cnt);
	void setOperands(const MyString& userInput, bool whole = false);
	void setOperands(const MyVector<MyString>& userInput);
	void setOperation(const BaseFilter* op);
	void setSink(const Display* sink);

	const Display* getSink() const;
	const BaseFilter* getFilter() const;
	const MyVector<MyString>& getOperands() const;
	CommandMap getCode() const;
	void free();

private:
	void copyFromOther(const Command& other);

	short operandsCount = 0;
	CommandMap code = CommandMap::Invalid;
	const BaseFilter* operation = nullptr;
	const Display* sink = nullptr;
	MyVector<MyString> operands;

};

#endif