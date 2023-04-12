#include <iostream>

using namespace std;
template<class T>
class Element
{
	//protected:
public:
	//переместить в protected
	Element* next;
	Element* prev;
	T info;

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}

	/*Element(const Element& el)
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}*/

	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}

template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:

	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}

	//удалить первый/последний элемент и вернуть удаленное значение
	virtual Element<T>* pop() = 0;
	//добавить элемент в список
	virtual Element<T>* push(T value) = 0;

	//доступ по индексу
	virtual Element<T>& operator[](int index)
	{
		//if(index < 0 || index >= count) throw ...;
		Element<T>* current = head;
		for (int i = 0; current != NULL && i < index; current = current->next, i++);
		return *current;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);

	//доработать деструктор
	virtual ~LinkedList()
	{
		cout << "\nLinkedList Destructor";
		
		Element<T>* previous = NULL;
		for (Element<T>* current = head; current != NULL; current = current->next)
		{
			previous = current;
			current = current->next;
			delete previous;
		}
	}
};

template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current = el.head;
	for (; current != NULL; current = current->next)
		s << *current << " ";
	return s;
}

template<class T>
class Stack : public LinkedList<T>
{
public:
	Stack<T>() : LinkedList<T>() {}
	virtual ~Stack() { cout << "\nStack Destructor"; }

	virtual Element<T>* pop()
	{
		if (LinkedList<T>::tail == NULL) return NULL;

		Element<T>* res = LinkedList<T>::tail;

		if (LinkedList<T>::head == LinkedList<T>::tail)
		{
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
			LinkedList<T>::count = 0;
			return res;
		}

		Element<T>* current = LinkedList<T>::head;
		for (; current->next != LinkedList<T>::tail; current = current->next);
		current->next = NULL;
		LinkedList<T>::tail = current;
		LinkedList<T>::count--;
		return res;   
	}

	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		if (LinkedList<T>::tail != NULL)
		{
			LinkedList<T>::tail->next = newElem;
			LinkedList<T>::tail = LinkedList<T>::tail->next;
		}
		else
			LinkedList<T>::head = LinkedList<T>::tail = newElem;
		LinkedList<T>::count++;
		return LinkedList<T>::tail;
		//Можно ещё заместо LinkedList<T>:: написать this->
	}

	virtual Element<T>* insert(Element<T>* previous = NULL)
	{
		if (previous == NULL)
		{
			push(value);
			return;
		}
		Element<T>* next = previous->next;
		previous->next = inserted;
		inserted->next = next;
		LinkedList<T>::count++;
		return inserted;
	}
};

template<class T>
class DoubleSidedStack : public Stack<T>
{
public:
	DoubleSidedStack<T>() : Stack<T> {}
	virtual ~DoubleSidedStack<T>() { cout << "\nDoubleSidedStack Destructor"; }

	using LinkedList<T>::head;
	using LinkedList<T>::tail;
	using LinkedList<T>::count;
	
	virtual Element<T>* push(T value)
	{
		Element<T>* tail_before = tail;
		Stack<T>::push(value);
		tail->prev = tail_before;
		return tail;
	}

	virtual Element<T>* pop()
	{
		if (LinkedList<T>::head == LinkedList<T>::tail)
			return Stack<T>::pop();
		Element<T>* res = tail;
		Element<T>* newTail = tail->prev;
		newTail->next = NULL;
		tail->prev = NULL;
		tail = newTail;
		count--;
		return res;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		if (previous == NULL)
			return push(value);
		Element<T>* inserted = Stack<T>::insert(value, previous);
		inserted->next->prev = inserted;
		inserted->prev = previous;
	}

	template<class T>
	friend ostream& operator<<(ostream& s, DoubleSidedStack<T1>& e1);
};

template<class T>
ostream& operator<<(ostream& s, DoubleSidedStack<T>& e1)
{
	Element<T>* current = el.tail;
	for (; current != NULL; current = current->prev)
		s << *current << " ";
	return s;
}

int main()
{
	/*Stack<int> S;
	for (int i = 0; i < 100; i += 10)
		S.push(i);
	cout << S << "\n";*/
	/*Element<int>* current = S.pop();
	while (current != NULL)
	{
		cout << *current << " ";
		delete current;
		current = S.pop();
	}*/

	//LinkedList<int>* ptr = new DoubleSidedStack<int>;
	DoubleSidedStack<int> ds;
	for (int i = 0; i < 100; i += 10)
		ds.push(i);
	Element<int>* ptr = &ds[3];
	ds.insert(-100, ptr);
	//cout << "\nPop(): " << *(ds.pop()) << "\n";
	cout << ds << "\n";
	//delete ptr;
}

//На дом функция remove()
