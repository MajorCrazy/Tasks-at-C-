// Понамарёв Никита, КМБО-04-22
// Вариант 17

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <locale>
#include <vector>
#include <set>
using namespace std;

//string Capitalize(string str) {
//    if (!str.empty()) {
//        str[0] = toupper(str[0]);
//    }
//    return str;
//}

bool Predicate(double V, double Threshold)
{
    return V > Threshold;
}

map<string, double> filter(const map<string, double>& mp, double Threshold) {
    map<string, double> Result;
    for (const auto& it : mp) {
        if (Predicate(it.second, Threshold)) {
            Result.insert(it);
        }
    }
    return Result;
}

void FindElem(const map<string, double>& mp, string key)
{
    auto it = mp.find(key);
    if (it != mp.end())
    {
        cout << "\nFound Element:\n";
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
    }
    else
    {
        cout << "\nElement not found\n";
    }
}

void FindElem(const map<string, double>& mp, double v)
{
    for (const auto& it : mp) 
    {
        if (it.second == v)
        {
            cout << "\nFound Element:\n";
            cout << "Key: " << it.first << ", Value: " << it.second << "\n";
            break;
        }
    }
}

void PrintMap(const map<string, double>& mp)
{
    auto it = mp.begin();
    cout << "\nMap: \n";
    while (it != mp.end())
    {
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
        it++;
    }
}

vector<double> Values(const map<string, double>& mp)
{
    set<double> Val;
    for (const auto& it : mp)
    {
        Val.insert(it.second);
    }
    return vector<double>(Val.begin(), Val.end());
}

void PrintVector(const vector<double>& vec)
{
    cout << "\nЗначения вектора: ";
    for (double Value : vec)
    {
        cout << Value << " ";
    }
    cout << "\n";
}

//void CheckOut(map<string, double>& mp)
//{
//    setlocale(LC_ALL, "RU_ru");
//
//    string NewKey;
//    double NewValue;
//    cout << "\nВведите ключ:";
//    cin.ignore();
//    getline(cin, NewKey);
//
//    if (mp.find(Capitalize(NewKey)) != mp.end())
//    {
//        throw logic_error("Ошибка: Дублирующийся ключ!");
//    }
//
//    cout << "\nВведите значение: ";
//    cin >> NewValue;
//
//    mp[Capitalize(NewKey)] = NewValue;
//}

int main()
{
    setlocale(LC_ALL, "RU_ru");

    map<string, double> AvgCheck;

    AvgCheck["Ivanov Petr"] = 1500.0;
    AvgCheck["Petrov Ivan"] = 1200.0;
    AvgCheck["Sidorova Anna"] = 1800.0;
    AvgCheck["Dyagteryov Evgeniy"] = 500.0;

    PrintMap(AvgCheck);

    FindElem(AvgCheck, "Ivanov Petr");
    FindElem(AvgCheck, 1200.0);

    /*CheckOut(AvgCheck);*/
    PrintMap(AvgCheck);

    vector<double> Values_Of_AvgCheck = Values(AvgCheck);
    PrintVector(Values_Of_AvgCheck);

    map<string, double> filtred_AvgCheck = filter(AvgCheck, 1000.0);
    
    PrintMap(filtred_AvgCheck);

    FindElem(filtred_AvgCheck, "Sidorova Anna");
    FindElem(filtred_AvgCheck, 1500.0);

    vector<double> Values_Of_Filtred_AvgCheck = Values(filtred_AvgCheck);
    PrintVector(Values_Of_Filtred_AvgCheck);
