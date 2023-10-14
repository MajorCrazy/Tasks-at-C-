// Понамарёв Никита, КМБО-04-22, Вариант 17
// Задание 1.3 - 1.5

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Customers
{
protected:
	string Last_Name;
	string First_Name;
	string City;
	string Street;
	int House_Number;
	int Flat_Number;
	int Account_Number;
	double Average_Check;
public:
	Customers() 
	{
		this->Last_Name = "";
		this->First_Name = "";
		this->City = "";
		this->Street = "";
		this->House_Number = 0;
		this->Flat_Number = 0;
		this->Account_Number = 0;
		this->Average_Check = 0.0;
	}

	Customers(string ls, string fn, string c, string st, int hn, int fnn, int an, double ac) : Last_Name(ls), First_Name(fn), City(c), Street(st), House_Number(hn), Flat_Number(fnn), Account_Number(an), Average_Check(ac) {}

	Customers(const Customers& cus)
	{
		this->Last_Name = cus.Last_Name;
		this->First_Name = cus.First_Name;
		this->City = cus.City;
		this->Street = cus.Street;
		this->House_Number = cus.House_Number;
		this->Flat_Number = cus.Flat_Number;
		this->Account_Number = cus.Account_Number;
		this->Average_Check = cus.Average_Check;
	}

	double GetNumberOfAverageCheck() const { return Average_Check; }

	friend ostream& operator<<(ostream& s, const Customers& value);
	friend istream& operator>>(istream& s, Customers& value);

	bool operator>(const Customers& f);
	bool operator<(const Customers& f);
};

ostream& operator<<(ostream& stream, const Customers& value)
{
	return stream << "Firstname: " << value.First_Name << ", Lastname: " << value.Last_Name << ", City: " << value.City << ", Street: " << value.Street << ", House Number: " << value.House_Number <<", Flat Number: " << value.Flat_Number << ", Account Number: " << value.Account_Number << ", Average Check: " << value.Average_Check << ";" << "\n\n";
}

istream& operator>>(istream& stream, Customers& value)
{
	return stream >> value.First_Name >> value.Last_Name >> value.City >> value.Street >> value.House_Number >> value.Flat_Number >> value.Account_Number >> value.Average_Check;
}

bool Customers::operator>(const Customers& f)
{
	if (Average_Check > f.Average_Check)
	{
		return true;
	}
	else if (Average_Check == f.Average_Check)
	{
		if (Account_Number < f.Account_Number)
		{
			return true;
		}
		else if (Account_Number == f.Account_Number)
		{
			if (Last_Name < f.Last_Name)
			{
				return true;
			}
			else if (Last_Name == f.Last_Name)
			{
				return First_Name < f.First_Name;
			}
		}
	}
	return false;
}

bool Customers::operator<(const Customers& f)
{
	if (Average_Check < f.Average_Check)
	{
		return true;
	}
	else if (Average_Check == f.Average_Check)
	{
		if (Account_Number > f.Account_Number)
		{
			return true;
		}
		else if (Account_Number == f.Account_Number)
		{
			if (Last_Name > f.Last_Name)
			{
				return true;
			}
			else if (Last_Name == f.Last_Name)
			{
				return First_Name > f.First_Name;
			}
		}
	}
	return false;
}

class Exception : public std::exception
{
protected:
	char* str;
public:
	Exception(const char* s)
	{
		if (s == NULL)
		{
			cout << "\n ExeptionError";
		}
		else
		{
			if (str = new char[strlen(s) + 1])
			{
				strcpy_s(str, strlen(s) + 1, s);
			}
		}
	}
	Exception(const Exception& e)
	{
		if (str = new char[strlen(e.str) + 1])
		{
			strcpy_s(str, strlen(e.str) + 1, e.str);
		}
	}
	~Exception()
	{
		if (str != nullptr)
		{
			delete[] str;
		}
	}
	virtual void print() const
	{
		cout << "Exception: " << str;
	}
};
class NotEnoughSpaceException : public Exception
{
public:
	NotEnoughSpaceException() : Exception("Not enought memory") {}
	void print() const
	{
		cout << "\n NotEnoughSpaceException: " << str;
	}
};
class WrongSizeException : public Exception
{
public:
	WrongSizeException() : Exception("Wrong Size") {}
	virtual ~WrongSizeException() {}
	void print() const
	{
		cout << "\n WrongSizeException: " << str;
	}
};
class NullPtrException : public Exception
{
public:
	NullPtrException() : Exception("Null pointer") {}
	virtual ~NullPtrException() {}
	void print() const
	{
		cout << "\n NullPtrException: " << str;
	}
};

template <class T>
class Element
{
private:
	Element* next;
	Element* prev;
	T field;
public:
	virtual Element* getNext() const { return next; }
	virtual void setNext(Element* value) { next = value; }

	virtual Element* getPrevious() const { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }

	virtual T getValue() const { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T> friend ostream& operator << (ostream& ustream, const Element<T>& obj);

	Element(T value = NULL, Element* Next = nullptr, Element* Prev = nullptr)
	{
		field = value;
		next = Next;
		prev = Prev;
	}
};

template<class T>
ostream& operator << (ostream& ustream, const Element<T>& obj)
{
	ustream << obj.field;
	return ustream;
}

template <class T>
class LinkedListParent
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	virtual int Count() const { return count; }
	virtual Element<T>* getBegin() const { return head; }
	virtual Element<T>* getEnd() const { return tail; }

	LinkedListParent()
	{
		head = tail = nullptr;
		count = 0;
	}

	virtual Element<T>* push(T value) = 0;

	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		if (this->head != nullptr)
		{
			Element<T>* current = this->head;
			while (current != nullptr)
			{
				Element<T>* to_delete = current;
				current = current->getNext();
				delete to_delete;
			}
			this->head = nullptr;
			this->count = 0;
			this->head = nullptr;
		}
	}

	virtual Element<T>* operator[](int index) const
	{
		if (index < 0 || index > count)
		{
			throw WrongSizeException();
		}

		Element<T>* current = head;
		for (int i = 0; current != nullptr && i < index; i++)
		{
			current = current->getNext();
		}
		return current;
	}

	template<class T> friend ostream& operator << (ostream& ustream, const LinkedListParent<T>& obj);
	template<class T> friend istream& operator >> (istream& ustream, LinkedListParent<T>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, const LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name()) {
		ustream << obj.count << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << current->getValue() << " ";
		return ustream;
	}
	ustream << "\nLength: " << obj.count << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != nullptr; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << current->getValue() << "\n";
	return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
	ustream >> obj.count;

	if (obj.head != nullptr)
	{
		Element<T>* current = obj.head;
		while (current != nullptr)
		{
			Element<T>* to_delete = current;
			current = current->getNext();
			delete to_delete;
		}
		obj.head = nullptr;
		obj.count = 0;
		obj.head = nullptr;
	}

	T value = NULL;
	for (int i = 0; i < obj.count; i++)
	{
		ustream >> value;
		obj.push(value);
	}
	return ustream;
}

template<typename ValueType>
class ListIterator : public iterator<input_iterator_tag, ValueType>
{
public:
	Element<ValueType>* ptr;
	ListIterator() { ptr = nullptr; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
	ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

	bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
	bool operator==(ListIterator const& other) const { return ptr == other.ptr; }


	Element<ValueType>& operator*() const
	{
		if (ptr == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			return *ptr;
		}
	}

	ListIterator& operator++() 
	{
		if (ptr == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			ptr = ptr->getNext();
			return *this;
		}
	}
	ListIterator& operator++(int v) 
	{
		if (ptr == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			ptr = ptr->getNext();
			return *this;
		}
	}
	ListIterator& operator--() 
	{
		if (ptr == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			ptr = ptr->getPrevious();
			return *this;
		}
	}
	ListIterator& operator--(int v) 
	{
		if (ptr == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			ptr = ptr->getPrevious();
			return *this;
		}
	}
};

template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { }
	virtual ~IteratedLinkedList() { }

	ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
	ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }
};

template <class T>
class Queue : public IteratedLinkedList<T>
{
public:
	Queue() : IteratedLinkedList<T>() {}

	virtual Element<T>* push(T Value)
	{
		Element<T>* elem;
		if (!(elem = new Element<T>(Value)))
		{
			throw NotEnoughSpaceException();
		}
		else
		{
			if (this->tail != nullptr)
			{
				this->tail->setNext(elem);
				elem->setPrevious(this->tail);
				this->tail = elem;
			}
			else
			{
				this->tail = this->head = elem;
			}
			this->count++;
			return elem;
		}
	}

	virtual T pop()
	{
		if (this->head == nullptr)
		{
			throw NullPtrException();
		}
		else
		{
			T Value = this->head->getValue();
			Element<T>* nextElem = this->head->getNext();
			delete this->head;
			this->head = nextElem;
			if (this->head == nullptr)
			{
				this->tail = nullptr;
			}
			else
			{
				this->head->setPrevious(nullptr);
			}
			this->count--;
			return Value;
		}
	}

	virtual ~Queue()
	{
		Element<T>* current = this->head;
		while (current != nullptr)
		{
			Element<T>* to_delete = current;
			current = current->getNext();
			delete to_delete;
		}
		this->head = nullptr;
		this->tail = nullptr;
		this->count = 0;
	}
};

template <class T>
class SortedQueue : public Queue<T>
{
public:
	SortedQueue() : Queue<T>() {}

	Element<T>* push(T Value)
	{
		Element<T>* elem;
		if (!(elem = new Element<T>(Value)))
		{
			throw NotEnoughSpaceException();
		}
		else
		{
			if (this->head == nullptr)
			{
				this->head = this->tail = elem;
			}
			else
			{
				Element<T>* current = this->head;
				Element<T>* prev = nullptr;

				while (current != nullptr && Value < current->getValue())
				{
					prev = current;
					current = current->getNext();
				}

				if (prev == nullptr)
				{
					elem->setNext(this->head);
					this->head->setPrevious(elem);
					this->head = elem;
				}
				else
				{
					prev->setNext(elem);
					elem->setPrevious(prev);
					elem->setNext(current);
					if (current == nullptr)
					{
						this->tail = elem;
					}
				}
			}

			this->count++;
			return elem;
		}
	}

	virtual ~SortedQueue() {}
};

template <class T>
Queue<T>* filter(IteratedLinkedList<T>* lst, bool (*function)(T))
{
	if (function == nullptr)
	{
		throw NullPtrException();
	}
	Queue<T>* st = new Queue<T>;
	ListIterator<T> it = lst->begin();
	while (it != nullptr)
	{
		if (function((*it).getValue()))
		{
			st->push((*it).getValue());
		}
		it++;
	}
	return st;
}

template <class T>
void filter(LinkedListParent<T>& lst, bool (*function)(T))
{
	if (function == nullptr)
	{
		throw NullPtrException();
	}
	Element<T>* elem = this->head;
	while (elem != nullptr)
	{
		if (function(elem->getValue()))
		{
			lst.push(elem->getValue());
		}
		elem = elem->getNext();
	}
}

template <class T, template <class> class list>
void PrintList(list<T>& lst)
{
	cout << lst << "\n";
}

bool Predicate(Customers f)
{
	return f.GetNumberOfAverageCheck() > 1000.0;
}

int main()
{
	setlocale(LC_ALL, "RU_ru");

	try {
		Queue<int> queue;
		queue.push(2);
		queue.push(4);
		queue.push(13);
		queue.push(-2);
		queue.push(4);
		queue.push(12);
		std::cout << queue.Count();
		std::cout << "\n";
		int n = queue.pop();
		std::cout << "\nDeleted Element = " << n;
		std::cout << queue << "\n";
		std::cout << "\nValue by index in the Queue class = " << queue[1]->getValue();

		std::cout << queue;
		std::cout << "\nIterators:\n";
		ListIterator<int> iterator = queue.begin();
		while (iterator != queue.end())
		{
			std::cout << *iterator << " ";
			iterator++;
		}
		std::cout << *iterator << " ";
		std::cout << endl;

		SortedQueue<int> Queue2;
		Queue2.push(2);
		Queue2.push(4);
		Queue2.push(13);
		Queue2.push(-2);
		Queue2.push(4);
		Queue2.push(12);
		std::cout << Queue2.Count();
		std::cout << "\n";
		int n1 = Queue2.pop();
		std::cout << "\nDeleted Element = " << n1;
		std::cout << Queue2 << endl;
		std::cout << "\nValue by index in the SortedQueue class = " << Queue2[1]->getValue();

		std::cout << Queue2;
		std::cout << "\nIterators:\n";
		ListIterator<int> iterator1 = Queue2.begin();
		while (iterator1 != Queue2.end())
		{
			std::cout << *iterator1 << " ";
			iterator1++;
		}
		std::cout << *iterator1 << " ";
		std::cout << endl;

		std::cout << "\nQueue of Customers:\n";
		Queue<Customers> CustomersQueue;
		CustomersQueue.push(Customers("Иванов", "Петр", "Москва", "Ленина", 10, 5, 123456, 1500));
		CustomersQueue.push(Customers("Петров", "Иван", "Санкт-Петербург", "Примерная", 15, 3, 789012, 1200));
		CustomersQueue.push(Customers("Сидоров", "Анна", "Москва", "Мира", 20, 7, 345678, 1800));
		CustomersQueue.push(Customers("Дягтерёв", "Евгений", "Севастополь", "Гоголя", 27, 6, 245698, 500));
		PrintList(CustomersQueue);
		std::cout << endl;

		std::cout << "\nSortedQueue of Customers:\n";
		SortedQueue<Customers> CustomersSortedQueue;
		CustomersSortedQueue.push(Customers("Иванов", "Петр", "Москва", "Ленина", 10, 5, 123456, 1500));
		CustomersSortedQueue.push(Customers("Петров", "Иван", "Санкт-Петербург", "Примерная", 15, 3, 789012, 1200));
		CustomersSortedQueue.push(Customers("Сидоров", "Анна", "Москва", "Мира", 20, 7, 345678, 1800));
		CustomersSortedQueue.push(Customers("Дягтерёв", "Евгений", "Севастополь", "Гоголя", 27, 6, 245698, 500));
		PrintList(CustomersSortedQueue);
		std::cout << endl;

		std::cout << "\nSorted Queue of Customers without last element:\n";
		CustomersSortedQueue.pop();
		PrintList(CustomersSortedQueue);
		std::cout << endl;

		std::cout << "\nFiltred SortedQueue of Customers:\n";
		Queue<Customers>* filtered = filter<Customers>(&CustomersSortedQueue, &Predicate);
		PrintList(*filtered);
		std::cout << endl;
	}
	catch (...) {}
}
