#ifndef _COMMANDMANAGER_
#define _COMMANDMANAGER_
#include "Filter.hpp"
#include "Command.hpp"
#include "FileManager.hpp"
#include "Alias.hpp"

class CommandManager
{
public:
	void introduction() const;
	void showCommandMap() const;
	void handleInput();
	void execute();

	bool isRunning() const;

	static CommandManager& getInstance()
	{
		static CommandManager instance("Commands.txt");
		return instance;
	}

	void addAlias(const MyVector<MyString>& params);
private:
	CommandManager(const char* name);
	CommandManager(const CommandManager& other) = delete;
	CommandManager& operator=(const CommandManager& other) = delete;

	~CommandManager() = default;
	bool running = true;
	Command operation;
	MyString commandsFileName;

	HashTableAlias aliases;

public:
	//this method is public only for test purposes
	void setCommand(const MyString& userInput);
};

typedef CommandManager TheCommandManager;

#endif