#include "File.hpp"

File::File(std::ifstream& istr, const char* setName)
{
	char buffer[1024];
	while (istr.getline(buffer, 1024))
		content.addLine(buffer);
	if (!istr.good() && !istr.eof())
		throw std::runtime_error("Can not load file content\n");
	name = setName;
}
void File::removeLastOperation()
{
	if (operations.getSize() == 0)
		throw std::runtime_error(("No operations for removing in file " + name + "\n").getData());
	operations.popBack();
	if(operations.getSize() == 0)
		unsavedChanges = false;
}

void File::addOperation(const Command& operation)
{
	operations.pushBack(operation);
	unsavedChanges = true;
	if (operations.getSize() == MAXUNDOSCOUNT)
	{
		saveChanges(name);
		removeOperations();
	}
}


void File::removeOperations()
{
	while(!operations.isEmpty())
		removeLastOperation();
}

bool File::hasUnsavedChanges() const
{
	return unsavedChanges;
}

void File::saveChanges(const MyString& path)
{
	std::ofstream ofile;
	if (name != path)
		ofile.open(path.getData());
	else
		ofile.open(name.getData());
	if (!ofile.is_open())
		throw std::runtime_error(("Can not open file " + path + "for saving changes\n").getData());
	for (size_t i = 0; i < operations.getSize(); ++i)
	{
		operations[i].getFilter()->filterText(content);
	}
	for (size_t i = 1; i + 1 <= content.getLinesCount(); ++i)
	{
		ofile << content.getLine(i) << "\n";
	}
	if (content.getLinesCount() > 0)
		ofile << content.getLine(content.getLinesCount());
	removeOperations();
	ofile.close();
}

void File::print(const Display* sink) const
{
	sink->displayFile(content);
}

const MyString& File::getName() const
{
	return name;
}

const Text& File::getConten() const
{
	return content;
}