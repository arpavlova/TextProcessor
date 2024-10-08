#ifndef _FILEMANAGER_
#define _FILEMANAGER_
#include "File.hpp"

class FileManager
{
public:
	File& operator[](int index);
	const File& operator[](int index) const;
	File& getCurrentFile();

	void addFile(const MyString& fileName);
	void removeFile(const MyString& fileName);
	void clearFiles();
	void changeCurrentFile(const MyString& fileName);
	void getInfo() const;
	int contains(const MyString& fileName) const;
	size_t getFilesCount() const;

	static FileManager& getInstance()
	{
		static FileManager instance;
		return instance;
	}

private:
	FileManager() = default;

	FileManager(const FileManager& other) = delete;
	FileManager& operator=(const FileManager& other) = delete;

	~FileManager();

	int currentFile = 0;
	MyVector<File*> files;
	Display* sink = nullptr;
};
typedef FileManager TheFileManager;
#endif