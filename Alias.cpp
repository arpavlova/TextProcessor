#include "Alias.hpp"

Alias::Alias(const MyVector<MyString>& params)
{
    name = params[0];
    sequence = params[1];
}

const MyString& Alias::getName() const
{
    return name;
}

const MyString& Alias::getSequnece() const
{
    return sequence;
}

//this function is taken from the net
size_t HashTableAlias::hashName(const MyString& name) const
{
   size_t hash = 5381;
   int c;
   const char* str = name.getData();
   while ((c = *str++)) {
       hash = ((hash << 5) + hash) + c;
   }
   return hash;
}

int HashTableAlias::contains(const MyString& name) const
{

    size_t index = hashName(name) % MAXALIASCOUNT;
    if (elements[index].getName() == name)
        return index;
    return -1;
}

void HashTableAlias::addElement(const MyVector<MyString>& params)
{
    size_t index = hashName(params[0]) % MAXALIASCOUNT;
    elements[index] = Alias(params);
}

void HashTableAlias::removeElement(const MyString& name)
{
    size_t index = hashName(name);
}

const Alias& HashTableAlias::getElement(size_t index) const
{
    if (index < MAXALIASCOUNT)
        return elements[index];
}
