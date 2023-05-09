#include <iostream>
#include <algorithm>

using namespace std;

class MyArrayParent
{
protected:
	int capacity, count;
	double* ptr;
public:

	MyArrayParent(int dimension = 100)
	{
		ptr = new double[dimension];
		capacity = dimension;
		count = 0;
	}

	//Конструктором, принимающим и копирующим информацию из существующего массива.
	MyArrayParent(double* arr, int len)
	{
		count = len;
		capacity = len;
		ptr = new double[len];

		for (int i = 0; i < count; i++)
			ptr[i] = arr[i];
	}

	//Конструктором копий MyArrayParent (const MyArrayParent& v).	
	MyArrayParent(const MyArrayParent& array)
	{
		ptr = new double[array.capacity];
		count = array.count;
		capacity = array.capacity;

		for (int i = 0; i < array.capacity; i++)
			ptr[i] = array.ptr[i];
	}

	~MyArrayParent()
	{
		delete[] ptr;
	}

	int GetCapacity()
	{
		return capacity;
	}

	int GetSize()
	{
		return count;
	}

	double GetComponent(int n)
	{
		if (n < 0 || n >= capacity)
			return 0.0;

		return ptr[n];
	}

	void SetComponent(int n, double value)
	{
		if (n >= 0 && n < count)
			ptr[n] = value;
	}

	virtual void RemoveLastValue()
	{
		if (count > 0)
			count--;
	}

	//Оператором [ ] для обращения к элементу по индексу.
	double& operator[](int n)
	{
		return ptr[n];
	};

	virtual void Push(double value)
	{
		if (count < capacity)
		{
			ptr[count] = value;
			count++;
		}
	}

	//поиск элемента
	virtual int IndexOf(double value, bool bFindFromStart = true)
	{
		if (bFindFromStart)
		{
			for (int i = 0; i < count; i++)
				if (ptr[i] == value)
					return i;
		}
		else
		{
			for (int i = count - 1; i >= 0; i--)
				if (ptr[i] == value)
					return i;
		}

		return -1;
	};

	//Оператором =.
	MyArrayParent operator =(MyArrayParent array)
	{
		if (capacity != array.capacity)
		{
			capacity = array.capacity;
			delete[] ptr;
			ptr = new double[capacity];
		}

		count = array.count;

		for (int i = 0; i < array.capacity; i++)
			ptr[i] = array.ptr[i];

		return *this;
	};

	double GetAverage()
	{
		// Нахожу среднее арифметическое
		double average = 0.0;

		for (int i = 0; i < count; i++)
			average += ptr[i];

		average /= count;

		return average;
	};
};

class MyArrayChild : public MyArrayParent
{
public:
	MyArrayChild(int dimension = 100) : MyArrayParent(dimension) { };

	MyArrayChild(double* arr, int len) : MyArrayParent(arr, len) { };

	MyArrayChild(const MyArrayChild& arr) : MyArrayParent(arr) { };

	~MyArrayChild() { };

	//удаление элемента
	virtual void RemoveAt(int index = -1)
	{
		if (index < 0)
			return;

		for (int i = index; i < count; i++)
			ptr[i] = ptr[i + 1];

		count--;
	};

	//вставка элемента
	virtual void InsertAt(double value, int index = -1)
	{
		if (count >= capacity)
			return;

		if (index == -1)
			Push(value);

		if (index < 0 || index > count)
			return;

		for (int i = count; i > index; i--)
			ptr[i] = ptr[i - 1];

		ptr[index] = value;

		count++;
	};

	//Все повторяющиеся элементы (дубли) заменить средним арифме-тическим исходного массива
	MyArrayChild SpecFunc()
	{
		double sum = ptr[0];
		int num_uniques = 1;
		// подсчет суммы уникальных элементов
		for (int i = 1; i < count; i++) {
			if (ptr[i] != ptr[i - 1]) { // ptr[i] не является дублем
				sum += ptr[i];
				num_uniques++;
			}
		}
		if (num_uniques == 0) return; // нет уникальных элементов
		double mean = sum / num_uniques; // среднее арифметическое уникальных элементов
		// замена дублей на среднее арифметическое
		for (int i = 1; i < count; i++) {
			if (ptr[i] == ptr[i - 1]) { // ptr[i] является дублем
				ptr[i] = mean;
			}
		}
	}

	MyArrayChild SubSequence(int start, int length)
	{
		MyArrayChild sub(length);

		int end = start + length > count ? count : start + length;

		for (int i = start; i < end; i++)
			sub.Push(ptr[i]);

		return sub;
	}

	MyArrayChild operator+(double value)
	{
		MyArrayChild arr(*this);
		arr.Push(value);
		return arr;
	}
};

class MySortedArray : public MyArrayChild
{
protected:
	virtual int BinarySearchInsert(double value)
	{
		// Бинарный поиск

		int left = -1, right = count;

		while (left + 1 < right)
		{
			int midd = (left + right) / 2;

			if (ptr[midd] == value)
				return midd;
			else if (ptr[midd] > value)
				right = midd;
			else
				left = midd;
		}

		return right;
	}

	virtual int BinarySearchRecursion(double value, int left, int right)
	{
		int middle = (left + right) / 2;

		if (ptr[middle] == value)
			return middle;

		if (middle == right || middle == left)
			return -1;

		if (ptr[middle] < value)
			return BinarySearchRecursion(value, middle, right);
		else if (ptr[middle] > value)
			return BinarySearchRecursion(value, left, middle);

		return -1;
	}

public:
	MySortedArray(int dimension = 100) : MyArrayChild(dimension) { }

	MySortedArray(double* arr, int len) : MyArrayChild(arr, len) { };

	MySortedArray(const MySortedArray& arr) : MyArrayChild(arr) { };

	~MySortedArray() { }

	virtual void Push(double value) override
	{
		if (count >= capacity)
			return;

		InsertAt(value, BinarySearchInsert(value));
	}

	MySortedArray SpecFunc()
	{
		double sum = ptr[0];
		int num_uniques = 1;
		// подсчет суммы уникальных элементов
		for (int i = 1; i < count; i++) {
			if (ptr[i] != ptr[i - 1]) { // ptr[i] не является дублем
				sum += ptr[i];
				num_uniques++;
			}
		}
		if (num_uniques == 0) return; // нет уникальных элементов
		double mean = sum / num_uniques; // среднее арифметическое уникальных элементов
		// замена дублей на среднее арифметическое
		for (int i = 1; i < count; i++) {
			if (ptr[i] == ptr[i - 1]) { // ptr[i] является дублем
				ptr[i] = mean;
			}
		}
	}

	virtual int IndexOf(double value, bool bFindFromStart = true) override
	{
		int i = BinarySearchInsert(value);

		return ptr[i] == value ? i : -1;
	}
};

void PrintArray(MyArrayParent array)
{
	int capacity = array.GetCapacity();
	int size = array.GetSize();

	for (int i = 0; i < capacity; i++)
		if (i < size)
			cout << '[' << i << "]\t" << array.GetComponent(i) << endl;
		else
			cout << '[' << i << "]\t[_]" << endl;
}

int main()
{

	MyArrayChild arr(8);
	arr.Push(3);

	cout << "Array:" << endl; PrintArray(arr);

	MyArrayChild arr2 = arr + 2;
	arr2.Push(4);
	arr2.Push(6);
	arr2.Push(4);
	arr2.Push(4);
	arr2.Push(9);
	arr2.Push(8);

	cout << "Array2:" << endl; PrintArray(arr2);

	arr2.RemoveAt(7);
	arr2.InsertAt(1, 7);
	arr2.SpecFunc();
	
	cout << "Array3:" << endl; PrintArray(arr2);

	MyArrayParent arr3(12);
	arr3.Push(1);
	arr3.Push(4);
	arr3.Push(5);
	arr3.Push(8);
	arr3.Push(1);
	arr3.Push(9);
	cout << "До:" << endl; PrintArray(arr3);
	arr3.Push(6);
	cout << "После:" << endl; PrintArray(arr3);

	return 0;
}
