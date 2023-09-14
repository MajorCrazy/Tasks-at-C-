// Practice_1_Second_Year.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <list>
#include <complex>
#include <functional>

using namespace std;

// Псевдоним для комплексных чисел
using Complex = complex<double>;

// Функция для сравнения двух комплексных чисел по модулю
bool compareComplexByMagnitude(const Complex& a, const Complex& b) {
    return abs(a) < abs(b);
}

// Функция для добавления элемента в отсортированный список
template<class T>
void push(list<T>& lst, const T& element) {
    typename list<T>::iterator it = lst.begin();
    while (it != lst.end() && compareComplexByMagnitude(*it, element)) {
        ++it;
    }
    lst.insert(it, element);
}

// Функция для удаления элемента из списка
template<class T>
bool pop(list<T>& lst, const T& element) {
    typename list<T>::iterator it = lst.begin();
    while (it != lst.end()) {
        if (*it == element) {
            lst.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

// Функция для фильтрации списка с использованием предиката P
template<class T, class Predicate>
list<T> filter(const list<T>& lst, Predicate P) {
    list<T> filteredList;
    for (const T& element : lst) {
        if (P(element)) {
            filteredList.push_back(element);
        }
    }
    return filteredList;
}

// Функция для вывода содержимого списка с помощью итераторов
template<class T>
void printList(const list<T>& lst) {
    for (const T& element : lst) {
        cout << element << " ";
    }
    cout << endl;
}

int main() {
    list<Complex> complexList;

    // Добавляем комплексные числа в список
    push(complexList, Complex(2.0, 3.0));
    push(complexList, Complex(1.0, 4.0));
    push(complexList, Complex(3.0, 2.0));
    push(complexList, Complex(5.0, 6.0));

    cout << "Initial list: ";
    printList(complexList);

    // Удаляем элемент из списка
    Complex toRemove(3.0, 2.0);
    bool removed = pop(complexList, toRemove);
    if (removed) {
        cout << "Removed " << toRemove << endl;
    }
    else {
        cout << "Element " << toRemove << " not found in the list" << endl;
    }

    cout << "List after removal: ";
    printList(complexList);

    // Фильтрация списка
    auto filterPredicate = [](const Complex& c) {
        return static_cast<int>(real(c)) % 2 == 0; // Проверяем, четная ли действительная часть
    };

    list<Complex> filtered = filter(complexList, filterPredicate);

    cout << "Filtered list: ";
    printList(filtered);

    return 0;
}
