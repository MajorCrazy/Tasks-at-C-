#include <iostream>
#include <cmath>

using namespace std;
class MyArrayParent

{
protected:
	int capacity; //сколько памяти выделено
	int count; //количество элементов 
	double* ptr; //массив
public:
	MyArrayParent(int Dimension = 100)
	{
		cout << "\nMyArray constructor\n";
		ptr = new double[Dimension];
		capacity = Dimension;
		count = 0;
	}

	//конструктор копий
	MyArrayParent(const MyArrayParent& V)
	{
		cout << "\nCopy constructor\n";
		count = V.count;
		capacity = V.capacity;
		ptr = new double[capacity];
		for (int i = 0; i < count; i++)
		{
			ptr[i] = (V.ptr)[i];
		}
	}

	//конструктор принимает существующий массив
	MyArrayParent(double* V, int len)
	{
		cout << "\nMyArray constructor\n";
		count = len;
		capacity = len;
		ptr = new double[len];
		for (int i = 0; i < count; i++)
		{
			ptr[i] = V[i];
		}
	}

	~MyArrayParent()
	{
		cout << "\nMyArray destructor\n";
		if (ptr != NULL)
		{
			delete[] ptr;
			ptr = NULL;
		}
	}

	int Capacity() { return capacity; }
	int Size() { return count; }

	double GetComponent(int index)
	{
		if (index >= 0 && index < count)
			return ptr[index];
		return -1;
	}

	void SetComponent(int index, double value)
	{
		if (index >= 0 && index < count)
			ptr[index] = value;
	}

	virtual void push(double value)
	{
		if (count < capacity)
		{
			ptr[count] = value;
			count++;
		}
		else
			cout << "В массиве нет места!\n";
	}

	//удаление элемента с конца
	void RemoveLastValue()
	{
		if (count >= 0)
			count--;
	}

	double& operator[](int index)
	{
		return ptr[index];
	}

	MyArrayParent& operator=(const MyArrayParent& V)
	{
		delete[] ptr;
		capacity = V.capacity;
		count = V.count;
		ptr = new double[capacity];
		for (int i = 0; i < count; i++)
		{
			ptr[i] = V.ptr[i];
		}
		return *this;
	}

	void print()
	{
		cout << "\nMyArr, size: " << count << ", values: {";
		int i = 0;
		for (i = 0; i < count; i++)
		{
			cout << ptr[i];
			if (i != count - 1)
				cout << ", ";
		}
		cout << "}\n";
	}

	int IndexOf(double value, bool bFindFromStart) // доделал поиск с конца 
	{
		int i = 0;
		if (bFindFromStart)
		{
			for (i = 0; i < count; i++)
			{
				if (ptr[i] == value)
					return i;
			}

		}
		else
		{
			for (i = count - 1; i >= 0; i--)
			{
				if (ptr[i] == value)
					return i;
			}
		}
		return -1;
	}
};

class MyArrayChild : public MyArrayParent
{
public:
	MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension)
	{
		cout << "\nMyArrayChild constructor\n";
	}

	MyArrayChild(double* V, int len) : MyArrayParent(V, len)
	{
		cout << "\nMyArrayChild constructor\n";
	}

	~MyArrayChild() { cout << "\nMyArrayChild destructor\n"; }

	//удаление элемента
	void RemoveAt(int index)
	{
		if (index < count && index >= 0)
		{
			for (int i = index; i < count - 1; i++)
				ptr[i] = ptr[i + 1];
			count--;
		}
		else
			cout << "Ошибка индекса при удалении элемента!\n";
	}

	//вставка элемента
	void InsertAt(double value, int index)
	{
		if (index == 0 && count == 0)
		{
			push(value);
			return;
		}
		if (index < count && index >= 0)
		{
			if (count != capacity)
				count++;
			for (int i = count - 2; i >= index; i--)
				ptr[i + 1] = ptr[i];
			ptr[index] = value;
		}
		else
			cout << "Ошибка индекса при вставке!\n";
	}

	MyArrayChild SpecFunc()
	{
		MyArrayChild res(*this);

		for (int i = 0; i < res.count; i++)
		{
			double sum = 0;
			int count = 0;
			for (int j = 0; j < res.count; j++)
			{
				if (i != j && res.ptr[i] == res.ptr[j])
				{
					sum += res.ptr[j];
					count++;
				}
			}
			if (count > 0)
			{
				double average = sum / count;
				for (int j = 0; j < res.count; j++)
				{
					if (i != j && res.ptr[i] == res.ptr[j])
					{
						res.ptr[j] = average;
					}
				}
			}
		}
		return res;
	}

	// Перегрузка оператора + 
	MyArrayChild operator+(double value) {
		MyArrayChild res = *this;
		res.push(value);
		return res;
	}

	// Функция выделения подпоследователньости
	MyArrayChild SubSequence(int StartIndex = 0, int Length = -1)
	{
		MyArrayChild res(abs(Length) * 2);
		res.count = Length;
		if (Length < 0) {
			res.count = 0;
			return res;
		}
		for (int i = 0; i < Length; i++)
			res.ptr[i] = ptr[i + StartIndex];
		return res;
	}

};

class MySortedArray : public MyArrayChild
{
protected:
	int more_bin(double x)
	{
		int left = 0, right = count - 1, mean = (right + left) / 2;

		while (right > left + 1)
		{
			if (ptr[mean] == x)
				return mean;
			if (ptr[mean] > x)
				right = mean;
			else
				left = mean;
			mean = (right + left) / 2;
		}
		if (ptr[left] >= x)
			return left;
		else
			return right;
	}

public:
	MySortedArray(int s = 100) : MyArrayChild(s)
	{
		cout << "\nSorted constructor\n";
	}
	~MySortedArray() { cout << "\nSorted destructor\n"; }

	virtual void push(double value)
	{
		if (count < capacity)
		{
			if (count == 0)
			{
				MyArrayParent::push(value);
				return;
			}
			int index = more_bin(value);
			if (index == count - 1)
				if (ptr[index] <= value)
				{
					MyArrayChild::push(value);
					return;
				}
			InsertAt(value, index);
		}
		else
			cout << "В массиве нет места!\n";
	}

	virtual int IndexOf(double x)
	{
		int a = more_bin(x);
		if (ptr[a] == x)
			return a;
		else
			return -1;
	}

	MySortedArray SpecFunc()
	{
		MySortedArray res(*this);

		for (int i = 0; i < res.count; i++)
		{
			double sum = 0;
			int count = 0;
			for (int j = 0; j < res.count; j++)
			{
				if (i != j && res.ptr[i] == res.ptr[j])
				{
					sum += res.ptr[j];
					count++;
				}
			}
			if (count > 0)
			{
				double average = sum / count;
				for (int j = 0; j < res.count; j++)
				{
					if (i != j && res.ptr[i] == res.ptr[j])
					{
						res.ptr[j] = average;
					}
				}
			}
		}
		return res;
	}

};

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Проверка #2.1: " << endl;

	MyArrayParent arr(15), arr2(10);

	for (int i = 0; i < 10; i++)
	{
		arr2.push(20 - i);
	}

	arr2.print();
	cout << arr2.IndexOf(17, true); // поиск сначала
	cout << "\n" << arr2.IndexOf(14, false); // поиск с конца

	MyArrayParent arr3(arr2);
	arr3.print();

	arr = arr2;
	arr.print();

	cout << "Элемент с индексом 3: " << arr[3] << endl;

	double* temp = new double[20];

	for (int i = 0; i < 20; i++)
	{
		temp[i] = i + 5;
	}

	MyArrayParent arr4(temp, 20);
	arr4.print();

	cout << "Индекс элемента 94: " << arr4.IndexOf(94, true) << endl;

	cout << "\nПроверка #2.2: " << endl;
	MyArrayChild arr5(10);
	arr5.push(5);
	arr5.push(6);
	arr5.push(3);
	arr5.push(3);
	arr5.push(6);
	arr5.push(6);
	arr5.push(5);
	arr5.InsertAt(3, 6);
	arr5.SpecFunc();
	arr5.print();



	cout << "\nПроверка #2.3: " << endl;
	MySortedArray arr7(10);
	for (int i = 0; i < 10; i++)
	{
		arr7.push(4 + i);
	}
	cout << "Отсортированный массив: ";
	arr7.print();
	double arr1[10] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

	MyArrayChild arr9;
	int i = 0;
	for (i = 10; i >= 0; i--)
	{
		arr9.push(i + 1);
	}
	arr9.print();
	MyArrayChild arr10 = arr9.SubSequence(6, 3); // Проверка функции веделения подпоследовательности
	arr10.print();
	arr10 = arr10 + 5; // Проверка перегрузки оператора +
	arr10.print();

	cout << "Индекс элемента 4: " << arr7.IndexOf(4) << endl;



	return 0;
}
