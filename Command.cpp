#include "Command.hpp"
void Command::setOperands(const MyString& userInput, bool whole)
{
	MyString operand;
	size_t i = 1;
	for (i; i <= operandsCount; ++i)
	{
		bool wholeSentence = (whole && i == operandsCount);
		getIthArgument(userInput, operand, i + 1, wholeSentence);
		if (operand.getSize() == 0)
		{
			if (i + 1 < operandsCount || i == operandsCount)
				throw std::invalid_argument("operands count is invalid\n");
			else
				return;
		}
		operands.pushBack(operand);
		operand.clear();
	}
	if(!whole)
	{
		getIthArgument(userInput, operand, i + 1);
		if (operand.getSize() != 0)
			throw std::invalid_argument("operands count is invalid\n");
	}
}

void Command::setOperands(const MyVector<MyString>& userInput)
{
	operands = userInput;
}

void Command::setOperation(const BaseFilter* op)
{
	delete operation;
	operation = op;
}
void Command::setSink(const Display* snk)
{
	delete sink;
	sink = snk;
}
const Display* Command::getSink() const
{
	if(!sink)
		throw std::runtime_error("Sink ptr is nullptr in const Display* Command::getSink() const\n");
	return sink;
}

const BaseFilter* Command::getFilter() const
{
	if (!operation)
		throw std::runtime_error("Filter ptr is nullptr in const BaseFilter* Command::getFilter() const\n");
	return operation;
}

const MyVector<MyString>& Command::getOperands() const
{
	return operands;
}

CommandMap Command::getCode() const
{
	return code;
}

//void Command::clear()
//{
//	free();
//}

void Command::free()
{
	operands.clear();
	delete operation;
	operation = nullptr;
	delete sink;
	sink = nullptr;
}

void Command::copyFromOther(const Command& other)
{
	const BaseFilter* opBuffer = nullptr;
	const Display* sinkBuffer = nullptr;
	if(other.operation)
	    opBuffer = other.operation->clone();
	try
	{
		if(other.sink)
		    sinkBuffer = other.sink->clone();
		operandsCount = other.operandsCount;
		code = other.code;
		operands = other.operands;
		free();
		operation = opBuffer;
		opBuffer = nullptr;
		sink = sinkBuffer;
		sinkBuffer = nullptr;
	}
	catch (std::bad_alloc& exc)
	{
		delete opBuffer;
		throw;
	}
}

Command::Command(const Command& other)
{
	copyFromOther(other);
}

Command& Command::operator=(const Command& other)
{
	if(this != & other)
	    copyFromOther(other);
	return *this;
}

Command::~Command()
{
	free();
}

void Command::setCode(CommandMap c)
{
	if ((int)c <= (int)CommandMap::Invalid || (int)c >= (int)CommandMap::Count)
		throw std::logic_error("Command code has invalid value in void Command::setCode(CommandMap c)\n");
	code = c;
}

void Command::setOperandsCount(int cnt)
{
	if (cnt < 0)
		throw std::logic_error("cnt < 0 in void Command::setOperandsCount(int cnt)\n");
	operandsCount = cnt;
}
