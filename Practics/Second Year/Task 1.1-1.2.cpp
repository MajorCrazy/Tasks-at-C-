// Понамарёв Никита, КМБО-04-22, Вариант 17
// Задание 1.1-1.2

#include <iostream>
#include <list>

using namespace std;

class Customers
{
public:
	string Last_Name;
	string First_Name;
	string City;
	string Street;
	int House_Number;
	int Flat_Number;
	int Account_Number;
	double Average_Check;

	Customers(string ls, string fn, string c, string st, int hn, int fnn, int an, double ac) : Last_Name(ls), First_Name(fn), City(c), Street(st), House_Number(hn), Flat_Number(fnn), Account_Number(an), Average_Check(ac) {}

	bool operator<(const Customers& Other) const
	{
		if (Average_Check != Other.Average_Check)
		{
			return Average_Check > Other.Average_Check;
		}

		if (Account_Number != Other.Account_Number)
		{
			return Account_Number > Other.Account_Number;
		}

		return Last_Name < Other.Last_Name;
	}
};

template<class T>
void push(list<T>& lst, const T& Value)
{
	class list<T>::const_iterator it = lst.begin();
	while (it != lst.end() && Value < *it)
	{
		++it;
	}
	lst.insert(it, Value);
}

template<class T>
T pop(list<T>& lst)
{
	if (!lst.empty())
	{
		T First_Value = lst.front();
		lst.pop_front();
		return First_Value;
	}
}

bool Predicate(double Module, double Value)
{
	return abs(Module) < Value;
}

list <double> filter(list<double>& lst, double Value)
{
	list<double> Result;
	for (class list<double>::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		const double i = *it;
		if (Predicate(i, Value))
		{
			Result.push_back(i);
		}
	}
	return Result;
}

template<class T>
void PrintList(const list<T>& lst)
{
	for (class list<T>::const_iterator it = lst.begin(); it != lst.end(); ++it)
	{
		const T& ptr = *it;
		cout << "Имя: " << ptr.First_Name << "\n" << "Фамилия: " << ptr.Last_Name << "\n" << "Средняя сумма чека: " << ptr.Average_Check << "\n";
	}
	cout << "---------------" << "\n";
}



int main()
{
	setlocale(LC_ALL, "RU_ru");

	// Задание 1.1

	list<double> List1 = { 1, 2.5, -3.4, 12, 7.2 };		//Создание списка и его демонстрация
	cout << "Список чисел: " << "\n";
	for (double c : List1)
	{
		cout << "[" << c << "]" << "\n";
	}
	cout << "---------------" << "\n";

	push(List1, 2.1); //Демонстрация функции push() для списка
	push(List1, -4.2);
	cout << "Список чисел: " << "\n";
	for (double c : List1)
	{
		cout << "[" << c << "]" << "\n";
	}
	cout << "---------------" << "\n";

	list<double> FiltredList = filter(List1, 5); //Демонстрация функции filter() для списка
	cout << "Список чисел: " << "\n";
	for (double c : FiltredList)
	{
		cout << "[" << c << "]" << "\n";
	}
	cout << "---------------" << "\n";

	pop(List1);
	cout << "Список чисел: " << "\n";
	for (double c : List1)
	{
		cout << "[" << c << "]" << "\n";
	}
	cout << "---------------" << "\n";		//Демонстарция функции pop() для списка

	// Задание 1.2

	list<Customers> CustomersList;

	push(CustomersList, Customers("Иванов", "Петр", "Москва", "Ленина", 10, 5, 123456, 1500));
	push(CustomersList, Customers("Петров", "Иван", "Санкт-Петербург", "Примерная", 15, 3, 789012, 1200));
	push(CustomersList, Customers("Сидорова", "Анна", "Москва", "Мира", 20, 7, 345678, 1800));
	push(CustomersList, Customers("Дягтерёв", "Евгений", "Севастополь", "Гоголя", 27, 6, 245698, 500));

	cout << "Список покупателей" << "\n";
	PrintList(CustomersList);

	if (!CustomersList.empty())
	{
		Customers UpdatedCustomersList = pop(CustomersList);
		cout << "Удалённый покупатель с наивысшим приоритетом:" << "\n";
		cout << "Имя: " << UpdatedCustomersList.First_Name << "\n" << "Фамилия: " << UpdatedCustomersList.Last_Name << "\n" << "Средняя сумма чека: " << UpdatedCustomersList.Average_Check << "\n";
	}

	cout << "Список покупателей после удаления: " << "\n";
	PrintList(CustomersList);
};
