#include "MyString.hpp"

MyString::MyString(size_t initSize)
{
     allocateMemory(initSize);
}

MyString::MyString(const MyString& other)
{
    copyFromOther(other);
}

MyString::MyString(MyString&& other) noexcept
{
    moveToMine(std::move(other));
}

MyString::MyString(char symbol) 
    : MyString()
{
    data[size++] = symbol;
    data[size] = '\0';
}
MyString::MyString(const char* str)
{
    if (!str)
        throw std::logic_error("Parameter in MyString(char* str) is nullptr\n");
    size_t strSize = strlen(str) + 1;
    allocateMemory(strSize);
    strcpy(data, str);
    size = strSize - 1;
}

MyString& MyString::operator=(const MyString& other)
{
    if (this != &other)
        copyFromOther(other);
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept
{
    if (this != &other)
        moveToMine(std::move(other));
    return *this;
}

bool MyString::operator!=(const MyString& other) const
{
    return !(*this == other);
}

MyString& MyString::operator+=(const MyString& other)
{
    size_t newSize = size + other.size + 1;
    if (newSize > capacity)
        resize(newSize);
    if(this != &other)
        strcat(data, other.data);
    else
    {
        MyString buffer(*this);
        strcat(data, buffer.getData());
    }
    size = newSize - 1;
    return *this;
}


MyString::~MyString()
{
    free();
}

char& MyString::operator[](size_t index)
{
    return data[index];
}

const char& MyString::operator[](size_t index) const
{
    return data[index];
}

char& MyString::at(size_t index)
{
    if (index >= size)
        throw std::out_of_range("Index for access to MyString is invalid\n");
    return data[index];
}

char MyString::at(size_t index) const
{
    if (index >= size)
        throw std::out_of_range("Index for access to MyString is invalid\n");
    return data[index];
}

bool MyString::operator==(const MyString& other) const
{
    return strcmp(data, other.data) == 0;
}

bool MyString::operator<(const MyString& other) const
{
    return strcmp(data, other.data) < 0;
}

bool MyString::operator<=(const MyString& other) const
{
    return *this < other || *this == other;
}

bool MyString::operator>(const MyString& other) const
{
    return !(*this <= other);
}

bool MyString::operator>=(const MyString& other) const
{
    return  *this > other || *this == other;
}

const char* MyString::getData() const
{
    return data;
}

size_t MyString::getSize() const
{
    return size;
}

size_t MyString::getCapacity() const
{
    return capacity;
}

void MyString::push_back(char symbol)
{
    if (size + 2 >= capacity)
        resize(capacity * 2);
    data[size++] = symbol;
    data[size] = '\0';
}

void MyString::clear()
{
    data[size] = '\0';
    size = 0;
    if (capacity > STRCOEF * INITSIZE)
        resize(INITSIZE);
}

void MyString::copyFromOther(const MyString& other)
{
    char* DataBuff = new char[other.capacity];
    strcpy(DataBuff, other.data);
    free();
    data = DataBuff;
    size = other.size;
    capacity = other.capacity;
}

void MyString::moveToMine(MyString&& other)
{
    free();
    data = other.data;
    size = other.size;
    capacity = other.capacity;
    other.data = nullptr;
    other.size = other.capacity = 0;
}

void MyString::resize(size_t sizeToResize)
{
    char* buffData = new char[sizeToResize > INITSIZE ? sizeToResize : INITSIZE];
    strcpy(buffData, data);
    free();
    data = buffData;
    capacity = sizeToResize > INITSIZE ? sizeToResize : INITSIZE;
}

void MyString::allocateMemory(size_t sizeToAllocate)
{
    free();
    capacity = sizeToAllocate > INITSIZE ? sizeToAllocate : INITSIZE;
    data = new char[capacity];
    data[0] = '\0';
}

void MyString::free()
{
    delete[] data;
    data = nullptr;
    capacity = 0;
}

std::istream& operator>>(std::istream& is, MyString& string)
{
    char buff[1024];
    is.getline(buff, 1024);
    string = buff;
    return is;
}

std::ostream& operator<<(std::ostream& os, const MyString& string)
{
    return os << string.getData();
}

const MyString& getIthArgument(const MyString& sentence, MyString& word, size_t index, bool whole)
{
    size_t i = 0;
    while (sentence[i] && index != 1)
    {
        if (sentence[i] == ' ')
            index--;
        i++;
    }
    for (i; i < sentence.getSize(); ++i)
    {
        if (sentence[i] == ' ' && !whole)
            break;
        word.push_back(sentence[i]);
    }
    return word;
}

MyString operator+(const MyString& mine, const MyString& other)
{
    MyString buffer(mine);
    return buffer += other;
}

MyString operator+(const char* str, const MyString& other)
{
    return other + MyString(str);
}

MyString cutString(const MyString& str, size_t beg, int end)
{
    if (beg > end || beg >= str.getSize() || end > str.getSize())
        throw std::out_of_range("beg > end || beg >= str.getSize() || end > str.getSize() in MyString cutString(const MyString& str, size_t beg, size_t end)\n");
    MyString buffer(end - beg + 1);
    for (size_t i = beg; i < end + 1; ++i)
        buffer.push_back(str[i]);
    return buffer;
}