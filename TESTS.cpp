#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MyString.hpp"
#include "MyVector.hpp"
#include <stdexcept>
#include "helpers.hpp"
#include "Command.hpp"
#include "CommandManager.hpp"
#include "FileManager.hpp"

TEST_CASE("Testing Sort on lines with fixed positions")
{
	Text source;
	MyString openSourceFile = "open TextFiles\\SortTests\\staticLines.txt";
	TheCommandManager::getInstance().setCommand(openSourceFile);
	TheCommandManager::getInstance().execute();
	source = FileManager::getInstance().getCurrentFile().getConten();
	MyString sortSourceFile = "sort";
	TheCommandManager::getInstance().setCommand(sortSourceFile);
	TheCommandManager::getInstance().execute();
	MyString saveByOtherName = "saveas TextFiles\\SortTests\\staticLinesProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(saveByOtherName);
	TheCommandManager::getInstance().execute();
	MyString openOutputFile = "open TextFiles\\SortTests\\staticLinesProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(openOutputFile);
	TheCommandManager::getInstance().execute();

	SECTION("Comparing content")
	{
		CHECK(FileManager::getInstance().getCurrentFile().getConten().getLinesCount() == source.getLinesCount());
		for(size_t i = 0; i < source.getLinesCount(); ++i)
			CHECK(FileManager::getInstance().getCurrentFile().getConten().getLine(i + 1) == source.getLine(i + 1));
	}

	MyString exit = "exit";
	TheCommandManager::getInstance().setCommand(exit);
	TheCommandManager::getInstance().execute();
}

TEST_CASE("Testing Sort on lines with no fixed positions")
{
	Text output;
	Text source;
	MyString openSourceFile = "open TextFiles\\SortTests\\nonstaticlines.txt";
	TheCommandManager::getInstance().setCommand(openSourceFile);
	TheCommandManager::getInstance().execute();
	source = FileManager::getInstance().getCurrentFile().getConten();
	MyString sortSourceFile = "sort";
	TheCommandManager::getInstance().setCommand(sortSourceFile);
	TheCommandManager::getInstance().execute();
	MyString saveByOtherName = "saveas TextFiles\\SortTests\\nonstaticlinesProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(saveByOtherName);
	TheCommandManager::getInstance().execute();
	MyString openOutputFile = "open TextFiles\\SortTests\\nonstaticlinesProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(openOutputFile);
	TheCommandManager::getInstance().execute();
	output = FileManager::getInstance().getCurrentFile().getConten();
	SECTION("Comparing content")
	{
		for (size_t i = 1; i <= 20; ++i)
		{
			CHECK(source.getLine(i) != output.getLine(i));
			CHECK(isNumber(output.getLine(i).getData()));
			CHECK(std::stoi(output.getLine(i).getData()) == i);
		}
	}

	MyString exit = "exit";
	TheCommandManager::getInstance().setCommand(exit);
	TheCommandManager::getInstance().execute();
}

TEST_CASE("Testing Sort with specified bounds")
{
	Text output;
	MyString openSourceFile = "open TextFiles\\SortTests\\specifiedbounds.txt";
	TheCommandManager::getInstance().setCommand(openSourceFile);
	TheCommandManager::getInstance().execute();
	MyString sortSourceFile = "sort 1 9";
	TheCommandManager::getInstance().setCommand(sortSourceFile);
	TheCommandManager::getInstance().execute();
	MyString saveByOtherName = "saveas TextFiles\\SortTests\\specifiedboundsProgramoutput.txt";
	TheCommandManager::getInstance().setCommand(saveByOtherName);
	TheCommandManager::getInstance().execute();
	MyString openOutputFile = "open TextFiles\\SortTests\\specifiedboundsProgramoutput.txt";
	TheCommandManager::getInstance().setCommand(openOutputFile);
	TheCommandManager::getInstance().execute();
	output = FileManager::getInstance().getCurrentFile().getConten();
	SECTION("Comparing content")
	{
		for (size_t i = 1; i <= 9; ++i)
		{
			CHECK(isNumber(output.getLine(i).getData()));
			CHECK(std::stoi(output.getLine(i).getData()) == i);
		}
		for (size_t i = 10; i < output.getLinesCount(); ++i)
		{
			if (output.getLine(i) != "")
			    CHECK(!(output.getLine(i) <= output.getLine(i + 1)));
		}
	}

	MyString exit = "exit";
	TheCommandManager::getInstance().setCommand(exit);
	TheCommandManager::getInstance().execute();
}


TEST_CASE("Testing Replace and Remove word")
{
	Text contentAfterReplacing;
	Text contentAfterRemoving;
	MyString command = "open TextFiles\\ReplaceWordTest\\cat.txt";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	command = "replace cat DOG";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	command = "saveas TextFiles\\ReplaceWordTest\\catProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	command = "open TextFiles\\ReplaceWordTest\\catProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	contentAfterReplacing = FileManager::getInstance().getCurrentFile().getConten();
	command = "remove cat";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	command = "saveas TextFiles\\ReplaceWordTest\\catProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	command = "open TextFiles\\ReplaceWordTest\\catProgramOutput.txt";
	TheCommandManager::getInstance().setCommand(command);
	TheCommandManager::getInstance().execute();
	contentAfterRemoving = FileManager::getInstance().getCurrentFile().getConten();

	SECTION("Comparing content")
	{
		CHECK(contentAfterRemoving.getLinesCount() == contentAfterReplacing.getLinesCount());
		for (size_t i = 0; i < contentAfterRemoving.getLinesCount(); ++i)
			CHECK(contentAfterRemoving.getLine(i + 1) == contentAfterReplacing.getLine(i + 1));
	}

	MyString exit = "exit";
	TheCommandManager::getInstance().setCommand(exit);
	TheCommandManager::getInstance().execute();
}


TEST_CASE("Testing void Text::addLine(const MyString& line)")
{
	Text text;
	text.addLine("First line");
	text.addLine("Second line");
	text.addLine("Third line");
	SECTION("Comparing content")
	{
		CHECK(text.getLine(1) == "First line");
		CHECK(text.getLine(2) == "Second line");
		CHECK(text.getLine(3) == "Third line");
	}
	SECTION("Proper size management")
	{
		CHECK(text.getLinesCount() == 3);
	}
}
TEST_CASE("Testing void Text::addLine(const MyString& line, size_t index)")
{
	Text text;
	text.addLine("First line");
	text.addLine("Third line");
	text.addLine("Fifth line");
	text.addLine("Second line", 2);
	text.addLine("Fourth line", 4);
	SECTION("Comparing content")
	{
		CHECK(text.getLine(1) == "First line");
		CHECK(text.getLine(2) == "Second line");
		CHECK(text.getLine(3) == "Third line");
		CHECK(text.getLine(4) == "Fourth line");
		CHECK(text.getLine(5) == "Fifth line");
	}
	SECTION("Proper size management")
	{
		CHECK(text.getLinesCount() == 5);
	}
}
TEST_CASE("Testing void Text::removeLine(size_t index)")
{
	Text text;
	text.addLine("First line");
	text.addLine("Second line");
	text.addLine("Useless");
	text.addLine("Third line");
	text.removeLine(3);
	SECTION("Comparing content")
	{
		CHECK(text.getLine(1) == "First line");
		CHECK(text.getLine(2) == "Second line");
		CHECK(text.getLine(3) == "Third line");
	}
	SECTION("Proper size management")
	{
		CHECK(text.getLinesCount() == 3);
	}
}
TEST_CASE("Testing void Text::replaceLine(size_t index,const MyString& line)")
{
	Text text;
	text.addLine("First line");
	text.addLine("Second line");
	text.addLine("Useless");
	text.addLine("Fourth line");
	text.replaceLine(2, "Third line");
	SECTION("Comparing content")
	{
		CHECK(text.getLine(1) == "First line");
		CHECK(text.getLine(2) == "Second line");
		CHECK(text.getLine(3) == "Third line");
		CHECK(text.getLine(4) == "Fourth line");
	}
	SECTION("Proper size management")
	{
		CHECK(text.getLinesCount() == 4);
	}
}
TEST_CASE("Testing Command setters with correct parameters") 
{
	Command command;
	SECTION("Testing void setOperands with two operands")
	{
		command.setOperandsCount(2);
		MyString userInput = "addline 89 this sentence should go in 89th line position in text. ";
		command.setOperands(userInput, true);
		CHECK(command.getOperands().getSize() == 2);
		CHECK(command.getOperands()[0] == "89");
		CHECK(command.getOperands()[1] == "this sentence should go in 89th line position in text. ");
	}
	SECTION("Testing void setOperands with three operands")
	{
		command.setOperandsCount(3);
		MyString userInput = "remove cat 1 5";
		command.setOperands(userInput);
		CHECK(command.getOperands().getSize() == 3);
		CHECK(command.getOperands()[0] == "cat");
		CHECK(command.getOperands()[1] == "1");
		CHECK(command.getOperands()[2] == "5");
	}
}
TEST_CASE("Testing Command setters with invalid parameters") {
	Command command;
	SECTION("Expecting exception throw when passing invalid values")
	{
		REQUIRE_THROWS_AS(command.setCode(CommandMap::Invalid), std::logic_error);
		REQUIRE_THROWS_AS(command.setCode(CommandMap::Count), std::logic_error);
		REQUIRE_THROWS_AS(command.setOperandsCount(-1), std::logic_error);
	}
	SECTION("Expecting exception throw when passing lesser operands")
	{
		command.setOperandsCount(3);
		MyString userInput = "remove cat 1";
		REQUIRE_THROWS_AS(command.setOperands(userInput), std::invalid_argument);
		command.setOperandsCount(3);
		userInput = "remove";
		REQUIRE_THROWS_AS(command.setOperands(userInput), std::invalid_argument);
	}
	SECTION("Expecting exception throw when passing more operands")
	{
		command.setOperandsCount(0);
		MyString userInput = "exit somethingUseless";
		REQUIRE_THROWS_AS(command.setOperands(userInput), std::invalid_argument);
		command.setOperandsCount(0);
		userInput = "open somethingUseless";
		REQUIRE_THROWS_AS(command.setOperands(userInput), std::invalid_argument);
	}
}
TEST_CASE("Testing Command getters") 
{
	Command command;
	SECTION("Expecting correct behaviour when getting nullptr data")
	{
		command.setSink(nullptr);
		command.setOperation(nullptr);
		REQUIRE_THROWS_AS(command.getSink(), std::runtime_error);
		REQUIRE_THROWS_AS(command.getFilter(), std::runtime_error);
	}
}

TEST_CASE("Testing Command proper memory management") {
	Command command1;
	MyString str1 = "dog";
	MyString str2 = "cat";
	MyVector<MyString> op;
	op.pushBack(str1);
	op.pushBack(str2);
	Display* sink = new Display();
	BaseFilter* filter = new ReplaceWordFilter(op);
	command1.setOperation(filter);
	command1.setSink(sink);
	SECTION("Testing Command& operator=(const Command& other)")
	{
		Command command2;
		command2 = command1;
		CHECK(command2.getSink() != sink);
		CHECK(command1.getSink() == sink);
		CHECK(command2.getFilter() != filter);
		CHECK(command1.getFilter() == filter);
		CHECK(command1.getOperands() == command2.getOperands());
		CHECK(command1.getCode() == command2.getCode());
	}
	SECTION("Testing Command(const Command& other)")
	{
		Command command2 = command1;
		CHECK(command2.getSink() != sink);
		CHECK(command1.getSink() == sink);
		CHECK(command2.getFilter() != filter);
		CHECK(command1.getFilter() == filter);
		CHECK(command1.getOperands() == command2.getOperands());
		CHECK(command1.getCode() == command2.getCode());
	}
	SECTION("Testing Command::free() function")
	{
		command1.free();
		REQUIRE_THROWS_AS(command1.getSink(), std::runtime_error);
		REQUIRE_THROWS_AS(command1.getFilter(), std::runtime_error);
	}
}
void checkEmpty(const MyString& str)
{
	CHECK(str.getSize() == 0);
	CHECK(str == "");
}

TEST_CASE("MyString::MyString(const char* str) constructs an empty string when passed the empty string")
{
	checkEmpty(MyString(""));
}

TEST_CASE("Testing MyString push_back()") {
	MyString str;

	SECTION("Initial size should be zero") {
		REQUIRE(str.getSize() == 0);
	}
	SECTION("Pushing back elements should increase the size") {
		str.push_back('a');
		CHECK(str.getSize() == 1);
		str.push_back('b');
		CHECK(str.getSize() == 2);
		str.push_back('c');
		CHECK(str[1] == 'b');
	}
}
TEST_CASE("Testing MyString at()") {
	MyString str("aaaaaaaaaa");

	SECTION("At() with index< size") 
	{
		CHECK(str.at(1) == 'a');
		REQUIRE_NOTHROW(str.at(str.getSize() - 1));
	}
	SECTION("At() with index >= size should throw out of range exception") {
		REQUIRE_THROWS_AS(str.at(str.getSize()), std::out_of_range);
	}
}
TEST_CASE("Testing MyString clear()") {
	size_t initCapacity = 20 * 64;
	MyString str(initCapacity);
	str.clear();
	CHECK(str.getCapacity() == INITSIZE);
	CHECK(str.getSize() == 0);
}
TEST_CASE("Testing MyString operator=") {
	MyString str1("DEMO STRING");
	MyString str2;
	str2 = str1;
	SECTION("Expecting equal sizes")
	{
		CHECK(str1.getSize() == str2.getSize());
	}
	SECTION("Expecting equal data")
	{
		CHECK(strcmp(str1.getData(), str2.getData()) == 0);
	}
}

TEST_CASE("Testing MyString operator+=") {
	MyString str1("DEMO STRING");
	MyString str2("FOR OPERATOR+=(CONST MYSTRING& OTHER)");
	SECTION("Expecting SUMMED sizes")
	{
		size_t str1Size = str1.getSize();
		str1 += str2;
		CHECK(str1.getSize() == str2.getSize() + str1Size);
	}
	SECTION("Expecting correct data when += on the same object")
	{
		str1 += str1;
		CHECK(strcmp(str1.getData(), "DEMO STRINGDEMO STRING") == 0);
	}
}

TEST_CASE("Testing isPunctuation(char symbol)")
{
	CHECK(isPunctuation('.'));
	CHECK(isPunctuation('?'));
	CHECK(isPunctuation(':'));
	CHECK(isPunctuation('!'));
	CHECK(isPunctuation(','));
}

TEST_CASE("Testing MyVector clear()") {
	size_t initCapacity = 16 * 64;
	MyVector<int> vector(initCapacity);
	vector.clear();
	CHECK(vector.getCapacity() == VECTORINITIALSIZE);
	CHECK(vector.getSize() == 0);
}

TEST_CASE("Testing MyVector pushBack(const T& element)") {
	MyVector<int> vector;
	for (size_t i = 1; i < 81; ++i)
		vector.pushBack(i);
	for (size_t i = 0; i < 80; ++i)
	{
		CHECK(vector[i] == i + 1);
	}
	CHECK(vector.getCapacity() > VECTORINITIALSIZE);
	CHECK(vector.getSize() == 80);
}

TEST_CASE("Testing MyVector at(size_t index)") {
	MyVector<int> vector;
	for (size_t i = 0; i < 3; ++i)
		vector.pushBack(i);
	REQUIRE_THROWS_AS(vector.at(vector.getSize()), std::out_of_range);
}

TEST_CASE("Testing MyVector& operator=(const MyVector& other)") {
	MyVector<int> vector1;
	MyVector<int> vector2(70);
	for (short i = 0; i < 3; ++i)
		vector2.pushBack(i);
	vector1 = vector2;
	CHECK(vector1.getCapacity() > VECTORINITIALSIZE);
	CHECK(vector1.getCapacity() == vector2.getCapacity());
	CHECK(vector1.getSize() == vector2.getSize());
	for (short i = 0; i < vector1.getSize(); ++i)
		CHECK(vector1[i] == vector2[i]);
}

TEST_CASE("Testing bool isPunctuation(char symbol)") 
{
	CHECK(isPunctuation('.'));
	CHECK(isPunctuation('-'));
	CHECK(isPunctuation('?'));
	CHECK(!isPunctuation('+'));
}

TEST_CASE("Testing bool isWordDetected(size_t j, const MyString& line, const MyString& word);")
{
	SECTION("Word at the beginnig")
	{
		MyString word = "simpleWord";
		MyString sentence = "simpleWord at begginig!";
		CHECK(isWordDetected(0, sentence, word));
		CHECK(!isWordDetected(1, sentence, word));
	}
	SECTION("Word in the middle")
	{
		MyString word = "simpleWord";
		MyString sentence = "Ckech for:simpleWord in the middle!";
		CHECK(isWordDetected(10, sentence, word));
		CHECK(!isWordDetected(0, sentence, word));
	}
	SECTION("Word in the end")
	{
		MyString word = "simpleWord";
		MyString sentence = "The word is simpleWord!";
		CHECK(isWordDetected(12, sentence, word));
		CHECK(!isWordDetected(0, sentence, word));
	}
}
TEST_CASE("Testing size_t getNumberLength(int number);")
{
	SECTION("1 digit number")
	{
		CHECK(getNumberLength(0) == 1);
		CHECK(getNumberLength(9) == 1);
	}
	SECTION("> 1 digit number")
	{
		CHECK(getNumberLength(1234) == 4);
		CHECK(getNumberLength(10) == 2);
	}
	SECTION("Negative number")
	{
		CHECK(getNumberLength(-1234) == 4);
		CHECK(getNumberLength(-10) == 2);
		CHECK(getNumberLength(-1) == 1);
	}
}
TEST_CASE("Testing bool isNumber(const char* str);")
{
	SECTION("Nullptr")
	{
		CHECK(!isNumber(nullptr));
	}
	SECTION("Empty")
	{
		CHECK(!isNumber(""));
	}
	SECTION("Negative number")
	{
		CHECK(isNumber("-12"));
		CHECK(!isNumber("-02"));
		CHECK(!isNumber("-"));
	}
	SECTION("Positive number")
	{
		CHECK(isNumber("1"));
		CHECK(isNumber("90"));
	}
	SECTION("Invalid number str")
	{
		CHECK(!isNumber("10-"));
		CHECK(!isNumber("90d"));
	}
}