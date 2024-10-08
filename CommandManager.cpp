#include "CommandManager.hpp"
#include <iostream>

void CommandManager::introduction() const
{
	std::cout << "You can directly type commands or you can press 'm' for command map\n";
	//std::cout << "Enter command: ";
}

void CommandManager::showCommandMap() const
{
	std::ifstream intro(commandsFileName.getData());
	if (!intro.is_open())
		throw std::runtime_error("Can not open command map file\n");
	MyString currentLine;
	while (intro.good())
	{
		intro >> currentLine;
		std::cout << currentLine << std::endl;
	}
	intro.close();
}

void CommandManager::handleInput()
{
	MyString userInput;
	std::cout << "Enter command: ";
	std::cin >> userInput;
	/*std::cout << "Status: ";
	std::cout << "\033[1A\033[" << userInput.getSize() << "D\033[" << strlen("Enter command: ") << "C";
	for (size_t i = 0; i < userInput.getSize(); ++i)
		std::cout << " ";
	std::cout << "\033[" << userInput.getSize() << "D";*/
	setCommand(userInput);
}

void CommandManager::execute()
{
	try
	{
		if (operation.getCode() == CommandMap::Open)
		{
			TheFileManager::getInstance().addFile(operation.getOperands()[0]);
		}
		else if (operation.getCode() == CommandMap::Close)
		{
			TheFileManager::getInstance().removeFile(operation.getOperands()[0]);
		}
		else if (operation.getCode() == CommandMap::Save)
		{
			TheFileManager::getInstance().getCurrentFile().saveChanges(TheFileManager::getInstance().getCurrentFile().getName());
		}
		else if (operation.getCode() == CommandMap::SaveAs)
		{
			TheFileManager::getInstance().getCurrentFile().saveChanges(operation.getOperands()[0]);
		}
		else if (operation.getCode() == CommandMap::Print || operation.getCode() == CommandMap::Wrap || operation.getCode() == CommandMap::Center)
		{
			TheFileManager::getInstance().getCurrentFile().print(operation.getSink());
		}
		else if (operation.getCode() == CommandMap::SwitchFile)
		{
			TheFileManager::getInstance().changeCurrentFile(operation.getOperands()[0]);
		}
		else if (operation.getCode() == CommandMap::FilesInfo)
		{
			TheFileManager::getInstance().getInfo();
		}
		else if (operation.getCode() == CommandMap::Undo)
		{
			TheFileManager::getInstance().getCurrentFile().removeLastOperation();
		}
		else if (operation.getCode() == CommandMap::Exit)
		{
			TheFileManager::getInstance().clearFiles();
		}
		else if (operation.getCode() == CommandMap::Alias)
		{
			TheCommandManager::getInstance().addAlias(operation.getOperands());
		}
		else if (operation.getCode() == CommandMap::UserDefined)
		{
			MyVector<MyString> commandSequence = operation.getOperands();
			for (size_t i = 0; i < commandSequence.getSize(); ++i)
			{
				TheCommandManager::getInstance().setCommand(commandSequence[i]);
				TheCommandManager::getInstance().execute();
			}
		}
		else
			TheFileManager::getInstance().getCurrentFile().addOperation(operation);
	}
	catch (std::invalid_argument& exc)
	{
		std::cout << exc.what();
		handleInput();
	}
	catch (...)
	{
		std::cout << "\n\n*******Something went wrong******\nThe program is terminating...\n\n";
		TheFileManager::getInstance().clearFiles();
		running = false;
	}
}

bool CommandManager::isRunning() const
{
	return running;
}

void CommandManager::addAlias(const MyVector<MyString>& params)
{
	aliases.addElement(params);
}

CommandManager::CommandManager(const char* name)
{
	if (!name)
		throw std::logic_error("name in CommandManager(const char* name) is nullptr\n");
	commandsFileName = name;
}

void CommandManager::setCommand(const MyString& userInput)
{
	MyString commandInput;
	BaseFilter* filter = nullptr;
	Display* sink = nullptr;
	operation.free();
	getIthArgument(userInput, commandInput, 1);
	try
	{
		if (commandInput == "open")
		{
			operation.setCode(CommandMap::Open);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
		}
		else if (commandInput == "save")
		{
			operation.setCode(CommandMap::Save);
			operation.setOperandsCount(0);
			operation.setOperands(userInput);
		}
		else if (commandInput == "saveas")
		{
			operation.setCode(CommandMap::SaveAs);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
		}
		else if (commandInput == "replace")
		{
			operation.setCode(CommandMap::Replace);
			operation.setOperandsCount(4);
			operation.setOperands(userInput);
			filter = new ReplaceWordFilter(operation.getOperands());
		}
		else if (commandInput == "remove")
		{
			operation.setCode(CommandMap::RemoveWord);
			operation.setOperandsCount(3);
			operation.setOperands(userInput);
			filter = new RemovalWordFilter(operation.getOperands());
		}
		else if (commandInput == "toupper")
		{
			operation.setCode(CommandMap::ToUpper);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
		    filter = new EncodeFilter(operation.getOperands(), std::toupper);
		}
		else if (commandInput == "tolower")
		{
			operation.setCode(CommandMap::ToLower);
 			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new EncodeFilter(operation.getOperands(), std::tolower);
		}
		else if (commandInput == "removewhitespacesbeg")
		{
			operation.setCode(CommandMap::RemoveStartingEmptyPlaces);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new RemoveEmptyPlacesAtFront(operation.getOperands());
		}
		else if (commandInput == "removewhitespacesend")
		{
			operation.setCode(CommandMap::RemoveSEndingEmptyPlaces);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new RemoveEmptyPlacesAtEnd(operation.getOperands());
		}
		else if (commandInput == "sort")
		{
			operation.setCode(CommandMap::Sort);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new LexSortFilter(operation.getOperands(), compare);
		}
		else if (commandInput == "shuffle")
		{
			operation.setCode(CommandMap::Shuffle);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new ShuffleFilter(operation.getOperands());
		}
		else if (commandInput == "removeduplicates")
		{
			operation.setCode(CommandMap::RemoveDuplicates);
			operation.setOperandsCount(2);
			operation.setOperands(userInput);
			filter = new RemoveDublicatesFilter(operation.getOperands());
		}
		else if (commandInput == "undo")
		{
			operation.setCode(CommandMap::Undo);
			operation.setOperandsCount(0);
		}
		else if (commandInput == "switchfile")
		{
			operation.setCode(CommandMap::SwitchFile);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
		}
		else if (commandInput == "print")
		{
			operation.setCode(CommandMap::Center);
			operation.setOperandsCount(0);
			operation.setOperands(userInput);
			sink = new Display();
		}
		else if (commandInput == "center")
		{
			operation.setCode(CommandMap::Center);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
			sink = new CenterDisplay(operation.getOperands());
		}
		else if (commandInput == "wrap")
		{
			operation.setCode(CommandMap::Wrap);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
			sink = new WrapDisplay(operation.getOperands());
		}
		else if (commandInput == "removeline")
		{
			operation.setCode(CommandMap::RemoveLine);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
			filter = new RemoveLineFilter(operation.getOperands());
		}
		else if (commandInput == "addline")
		{
			operation.setCode(CommandMap::AddLine);
			operation.setOperandsCount(2);
			operation.setOperands(userInput, true);
			filter = new AddLineFilter(operation.getOperands());
		}
		else if (commandInput == "replaceline")
		{
			operation.setCode(CommandMap::ReplaceLine);
			operation.setOperandsCount(2);
			operation.setOperands(userInput, true);
			filter = new ReplaceLineFilter(operation.getOperands());
		}
		else if (commandInput == "filesinfo")
		{
			operation.setCode(CommandMap::FilesInfo);
			operation.setOperandsCount(0);
			operation.setOperands(userInput);
		}
		else if (commandInput == "close")
		{
			operation.setCode(CommandMap::Close);
			operation.setOperandsCount(1);
			operation.setOperands(userInput);
		}
		else if (commandInput == "exit")
		{
			operation.setCode(CommandMap::Exit);
			operation.setOperandsCount(0);
			operation.setOperands(userInput);
			running = false;
		}
		else if (commandInput == "alias")
		{
			operation.setCode(CommandMap::Alias);
			operation.setOperandsCount(2);
			operation.setOperands(userInput, true);
		}
		else if (aliases.contains(commandInput))
		{
			operation.setCode(CommandMap::UserDefined);
			MyVector<MyString> tokens;
			operation.setOperands(parseSequence(aliases.getElement(aliases.contains(commandInput)).getSequnece(), tokens));
		
		}
		else if (commandInput == "m")
		{
			showCommandMap();
			handleInput();
		}
		else
		{
			std::cout << "The command is not recognized\n(you can press 'm' for commands info)\n";
			handleInput();
		}
	}
	catch (std::invalid_argument& exc)
	{
		std::cout << exc.what();
		handleInput();
	}
	catch (...)
	{
		std::cout << "\n\n*******Something went wrong******\nThe program is terminating...\n\n";
		TheFileManager::getInstance().clearFiles();
		running = false;
	}
	if (sink || filter)
	{
		operation.setSink(sink);
		operation.setOperation(filter);
	}
}
