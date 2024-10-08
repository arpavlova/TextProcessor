#ifndef _HELPERS_
#define _HELPERS_
#include "Text.hpp"
#include "cstring"
#include <string>

bool isPunctuation(char symbol);
bool isWordDetected(size_t j, const MyString& line, const MyString& word);
size_t getNumberLength(int number);
bool isNumber(const char* str);
bool hasDotAtPos(const MyString& str, size_t pos);
bool compare(const MyString& line1, const MyString& line2);
bool wholeWord(const MyString& word);
MyVector<MyString> parseSequence(const MyString& sequnce, MyVector<MyString>& tokens);

template <class T>
void quickSortRecursive(MyVector<T>& sequence, bool (*comparator)(const T&, const T&), int left, int right)
{
	if (left < right) {
		int pivotIndex = partition(sequence, comparator, left, right);
		quickSortRecursive(sequence, comparator, left, pivotIndex - 1);
		quickSortRecursive(sequence, comparator, pivotIndex + 1, right);
	}
}

template <class T>
int partition(MyVector<T>& sequence, bool (*comparator)(const T&, const T&), int left, int right)
{
	T pivot = sequence[right];
	int i = left - 1;

	for (int j = left; j < right; j++) {
		if (comparator(sequence[j], pivot)) {
			i++;
			if(i != j)
			    std::swap(sequence[i], sequence[j]);
		}
	}
	std::swap(sequence[i + 1], sequence[right]);
	return i + 1;
}
#endif