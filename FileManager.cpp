#include "FileManager.hpp"
#include "Sink.hpp"

File& FileManager::operator[](int index)
{
	if (index >= files.getSize())
		throw std::out_of_range("index >= files.getSize() in const File& FileManager::operator[](int index) const\n");
	return *files[index];
}

const File& FileManager::operator[](int index) const
{
	if (index >= files.getSize())
		throw std::out_of_range("index >= files.getSize() in const File& FileManager::operator[](int index) const\n");
	return *files[index];
}

File& FileManager::getCurrentFile() 
{
	if (files.isEmpty())
		throw std::logic_error("There are not any opened files\n");
	return *files[currentFile];
}

void FileManager::addFile(const MyString& fileName)
{
	if (contains(fileName) == -1)
	{
		std::ifstream ifile(fileName.getData());
		if (!ifile.is_open())
		{
			std::ofstream ofile(fileName.getData(), std::ios::app);
			if(!ofile.is_open())
			    throw std::runtime_error(("Can not open file " + fileName + "\n").getData());
			ofile.close();
			ifile.open(fileName.getData());
			if (!ifile.is_open())
				throw std::runtime_error(("Can not open file " + fileName + "\n").getData());
		}
		ifile.clear();
		File* file = new File(ifile, fileName.getData());
		files.pushBack(file);
		ifile.close();
		currentFile = files.getSize() - 1;
	}
}

void FileManager::removeFile(const MyString& fileName)
{
	int alreadyOpened = contains(fileName);
	if (alreadyOpened != -1)
	{
		if (files[alreadyOpened]->hasUnsavedChanges())
		{
			char buff;
			std::cout << "file " << files[alreadyOpened]->getName() << " has unsaved changes, do you want to save them? y/n\n";
			do
			{
				std::cin >> buff;
			}while (buff != 'y' && buff != 'n');
			std::cin.ignore();
			if (buff == 'y')
			{
				files[alreadyOpened]->saveChanges(files[alreadyOpened]->getName());
			}
		}
		delete files[alreadyOpened];
		files[alreadyOpened] = nullptr;
		files.remove(alreadyOpened);
	}
}

void FileManager::clearFiles()
{
	while(!files.isEmpty())
		removeFile(TheFileManager::getInstance()[getFilesCount() - 1].getName());
}

void FileManager::changeCurrentFile(const MyString& fileName)
{
	int index = contains(fileName);
	if (index < 0)
		throw std::runtime_error(("File " + fileName + " is not opened\n").getData());
	currentFile = index;
}

void FileManager::getInfo() const
{
	std::cout << "\n\n*********Files Info************\n\n";
	for (size_t i = 0; i < files.getSize(); ++i)
	{
		std::cout << i + 1 << ".File: " << files[i]->getName() << "\n"
			<< "Lines count: " << files[i]->getConten().getLinesCount()
			<< ", Symbols count: " << files[i]->getConten().getSymbolsCount()
			<< ", Unsaved changes: ";
		if (files[i]->hasUnsavedChanges())
			std::cout << "yes\n\n";
		else
			std::cout << "no\n\n";
	}
}

int FileManager::contains(const MyString& fileName) const
{
	for (size_t i = 0; i < files.getSize(); ++i)
	{
		if (strcmp(files[i]->getName().getData(), fileName.getData()) == 0)
		{
			return i;
		}
	}
	return -1;
}

size_t FileManager::getFilesCount() const
{
	return files.getSize();
}

FileManager::~FileManager()
{
	for (size_t i = 0; i < files.getSize(); ++i)
	{
		delete files[i];
		files[i] = nullptr;
	}
	delete sink;
	sink = nullptr;
}
