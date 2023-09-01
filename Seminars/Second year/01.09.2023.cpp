#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

//bool compare_chars(char x, char y)
//{
//    x >= 'A' && x <= 'Z' ? x - 'A' : x - 'a';
//    y >= 'A' && y <= 'Z' ? y - 'A' : y - 'a';
//    return x < y;
//   
//    или
//    int num1, num2;
//    if (x >= 'A' && x <= 'Z')
//        num1 = x - 'A';
//    if (x >= 'a' && x <= 'z')
//        num1 = x - 'a';
//
//    if (y >= 'A' && y <= 'Z')
//        num2 = y - 'A';
//    if (y >= 'a' && y <= 'z')
//        num2 = y - 'a';
//
//    return (num1 < num2);
//}



int main()
{
    /*vector<int> v;
    for (int i = 0; i < 20; i++)
        v.push_back(i);
    for (int i = 0 ; i < 20; i++)
        cout << v[i] << " ";

    list <int> l;
    for (int i = 0; i < 20; i++)
        l.push_back(i);*/
    /*list<int>::iterator it = l.begin();
    for (list<int>::iterator it = l.begin(); it != l.end(); cout << *it++ << " ");*/
    
    /*cout << "\n";
    list<int>::iterator it = l.end(); (конфликтует со сточками 21-22)
    while (it != l.begin())
        cout << *--it << " ";*/

    /*list<char> l;
    for (int i = 0; i < 5; i++)
        l.push_back('A' + i);
    for (int i = 0; i < 5; i++)
        l.push_back('t' + i);
    for (int i = 0; i < 5; i++)
        l.push_back('a' + i);
    for (int i = 0; i < 5; i++)
        l.push_back('T' + i);
    for (list<char>::iterator it = l.begin(); it != l.end(); cout << *it++, cout << " ");
    l.sort(compare_chars); cout << "\n\n\n------\n\n\n";
    for (list<char>::iterator it = l.begin(); it != l.end(); cout << *it++, cout << " ");*/

    map<string, int> marks;
    marks["ivanov"] = 3;
    marks["petrov"] = 4;
    marks["sidorov"] = 3;
    marks["pavlov"] = 4;
    marks["kuznetsov"] = 5;
    cout << marks["test"] << "\n---\n";;

    //for (map < string, int>::iterator it = marks.begin(); it != marks.end(); cout << it->first<<": "<< it->second<< "\n", it++)
   // map < string, int>::iterator it = marks.begin();
    map < string, int>::iterator it = marks.find("petrov");
    while (it != marks.end())
    {
        cout << it->first << ": " << it->second << "\n";
        it++;
    }

    it = marks.find("popov");
    if (it != marks.end())
        cout << it->first << ": " << it->second << "\n";

    set<int> s;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            s.insert(j);

    set<int>::iterator it_set = s.begin();
    while (it_set != s.end())
    {
        cout << *it_set++ << " ";
    }

    return 0;
}

//На дом нужно at(), upper_bount, equal_range(?)

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
