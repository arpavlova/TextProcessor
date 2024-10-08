#ifndef _MYVECTOR_
#define _MYVECTOR_

short const VECTORINITIALSIZE = 64;
short const COEF = 10;
template <typename T>
class MyVector
{

public:
	MyVector(size_t initSize = VECTORINITIALSIZE)
	{
		size_t newSize = VECTORINITIALSIZE > initSize ? VECTORINITIALSIZE : initSize;
		data = new T[newSize];
		capacity = newSize;
	}
	MyVector(const MyVector<T>& other)
	{
		copyFromOther(other);
	}
	MyVector(MyVector<T>&& other)
	{
		moveToMine(std::move(other));
	}
	MyVector<T>& operator=(const MyVector<T>& other)
	{
		if (this != &other)
			copyFromOther(other);
		return*this;
	}
	MyVector<T>& operator=(MyVector<T>&& other)
	{
		if (this != &other)
			moveToMinee(std::move(other));
		return *this;
	}
	bool operator==(const MyVector<T>& other) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (data[i] != other.data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const MyVector<T>& other) const
	{
		return !(*this == other);
	}
	~MyVector()
	{
		free();
	}
	void pushBack(const T& element)
	{
		if (size >= capacity)
			resize(capacity * 2);
		data[size++] = element;
	}
	void popBack()
	{
		if (size > 0)
			size--;
	}
	T& peekBack()
	{
		if (size == 0)
			throw std::out_of_range("Can't peek last element of an empty vector\n");
		if (size > 0)
			return data[size - 1];
	}
	void clear()
	{
		size = 0;
		if (capacity > COEF * VECTORINITIALSIZE)
			resize(VECTORINITIALSIZE);
	}
	T& operator[](size_t index)
	{
		return data[index];
	}
	const T& operator[](size_t index) const
	{
		return data[index];
	}
	size_t getSize() const
	{
		return size;
	}
	size_t getCapacity() const
	{
		return capacity;
	}
	bool isEmpty() const
	{
		return size == 0;
	}
	const T& at(size_t index) const
	{
		if (index >= size)
			throw std::out_of_range("The index is >= size of the MyVector object\n");
		return data[index];
	}
	T& at(size_t index)
	{
		if (index >= size)
			throw std::out_of_range("The index is >= size of the MyVector object\n");
		return data[index];
	}
	void remove(size_t index)
	{
		if (index >= size)
			throw std::out_of_range("index >= size in void MyVector::remove(size_t index)\n");
		for (int i = index; i < size - 1; ++i)
			data[i] = data[i + 1];
		size--;
	}
private:
	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 0;
	void free()
	{
		delete[] data;
		data = nullptr;
		size = capacity = 0;
	}
	void resize(size_t newCapacity)
	{
		if (newCapacity < VECTORINITIALSIZE)
			newCapacity = VECTORINITIALSIZE;
		T* newData = new T[newCapacity];
		for (size_t i = 0; i < size; ++i)
		{
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
		capacity = newCapacity;
	}
	void copyFromOther(const MyVector<T>& other)
	{
		T* newData = new T[other.capacity];
		for (size_t i = 0; i < other.size; ++i)
		{
			newData[i] = other[i];
		}
		free();
		data = newData;
		size = other.size;
		capacity = other.capacity;
	}
	void moveToMine(MyVector<T>&& other)
	{
		free();
		data = other.data;
		size = other.size;
		capacity = other.capacity;
		other.data = nullptr;
		other.size = other.capacity = 0;
	}
};

#endif

