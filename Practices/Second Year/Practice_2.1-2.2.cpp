// Понамарёв Никита, КМБО-04-22
// Вариант 17

#include <iostream>
#include <map>
#include <vector>
#include <set>
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

    bool operator==(const Customers& Other) const {
        return Last_Name == Other.Last_Name && First_Name == Other.First_Name && City == Other.City && Street == Other.Street && House_Number == Other.House_Number &&
            Flat_Number == Other.Flat_Number && Account_Number == Other.Account_Number && Average_Check == Other.Average_Check;
    }
};

ostream& operator<<(ostream& os, const Customers& customer) {
     os << customer.Last_Name << " " << customer.First_Name << " " << customer.City
        << " " << customer.Street << " " << customer.House_Number << " " << customer.Flat_Number
        << " " << customer.Account_Number << " " << customer.Average_Check;
    return os;
}

template<class Value>
bool Predicate(Value V, Value Threshold)
{
    return V > Threshold;
}

template<class Key, class Value>
map<Key, Value> Map_filter(const map<Key, Value>& mp, Value Threshold) {
    map<Key, Value> Result;
    for (const auto& it : mp) {
        if (Predicate(it.second, Threshold)) {
            Result.insert(it);
        }
    }
    return Result;
}

template<class Key, class Value>
void FindValueMap(const map<Key, Value>& mp, Key key)
{
    bool found = false;
    auto it = mp.find(key);
    if (it != mp.end())
    {
        cout << "\nFound Element:\n";
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
        found = true;
    }
}

template<class Key, class Value>
void FindKeyMap(const map<Key, Value>& mp, Value v)
{
    bool found = false;
    for (const auto& it : mp)
    {
        if (it.second == v)
        {
            cout << "\nFound Element:\n";
            cout << "Key: " << it.first << ", Value: " << it.second << "\n";
            found = true;
        }

    }
    if (!found)
    {
        cout << "\nElement " << v << " not found\n";
    }
}

template<class Key, class Value>
void PrintMap(const map<Key, Value>& mp)
{
    cout << "\nMap: \n";
    for (auto it = mp.begin(); it != mp.end(); it++)
    {
        cout << "Customer: " << it->first << ", Average Check: " << it->second << "\n";
    }
}

template<class Key, class Value>
vector<Value> Values_Map(const map<Key, Value>& mp)
{
    set<Value> Val;
    for (const auto& it : mp)
    {
        Val.insert(it.second);
    }
    return vector<Value>(Val.begin(), Val.end());
}

template<class Value>
void PrintVector(const vector<Value>& vec)
{
    cout << "\nVector values: ";
    for (Value Value : vec)
    {
        cout << Value << " ";
    }
    cout << "\n";
}

template<class Key, class Value>
void PrintMultiMap(const multimap<Key, Value>& mmap)
{
    auto it = mmap.begin();
    cout << "\nMultimap: \n";
    while (it != mmap.end())
    {
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
        it++;
    }
}

template<class Key, class Value>
void FindValueMultiMap(const multimap<Key, Value>& mmapp, Key key)
{
    auto it = mmapp.find(key);
    if (it != mmapp.end())
    {
        cout << "\nFound Element:\n";
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
    }
    else
    {
        cout << "\nElement " << key << " not found\n";
    }
}

template<class Key, class Value>
void FindKeyMultiMap(const multimap<Key, Value>& mmap, Value v)
{
    bool found = false;
    for (const auto& it : mmap)
    {
        if (it.second == v)
        {
            cout << "\nFound Element:\n";
            cout << "Key: " << it.first << ", Value: " << it.second << "\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "\nElement " << v << " not found\n";
    }
}

template<class Key, class Value>
multimap<Key, Value> MultiMap_filter(const multimap<Key, Value>& mmap, Value Threshold)
{
    multimap<Key, Value> Result;
    for (const auto& it : mmap)
    {
        if (Predicate(it.second, Threshold))
        {
            Result.insert(it);
        }
    }
    return Result;
}

template<class Key, class Value>
vector<Value> Values_MultiMap(const multimap<Key, Value>& mp)
{
    set<Value> Val;
    for (const auto& it : mp)
    {
        Val.insert(it.second);
    }
    return vector<Value>(Val.begin(), Val.end());
}

template<class Key, class Value>
multimap<Key, Value> SameElem(const multimap<Key, Value>& mmap, const Key& key)
{
    multimap<Key, Value> Result;

    auto range = mmap.equal_range(key);
    for (auto it = range.first; it != range.second; ++it)
    {
        Result.emplace(it->first, it->second);
    }

    return Result;
}

int main()
{
    map<Customers, double> MyMap;

    Customers customer1("Ivanov", "Petr", "Moscow", "Lenin", 10, 5, 123456, 1500.0);
    Customers customer2("Petrov", "Ivan", "Saint-Petersburg", "Primernaya", 15, 3, 789012, 1200.0);
    Customers customer3("Sidorova", "Anna", "Moscow", "Mira", 20, 7, 345678, 1800.0);
    Customers customer4("Dyagterev", "Evgeniy", "Sevastopol", "Gogolya", 27, 6, 245698, 500.0);

    cout << "\n-----Test try-catch map 1-----" << endl;

    try
    {
        MyMap[customer1] = 1500.0;
        MyMap[customer2] = 1200.0;
        MyMap[customer3] = 1800.0;
        MyMap[customer4] = 500.0;

        Customers Key = customer1;
        double Value = 1750.0;

        if (MyMap.find(Key) != MyMap.end())
        {
            throw invalid_argument("Element with that key already exists.");
        }

        MyMap[Key] = Value;
    }
    catch (const invalid_argument& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    PrintMap(MyMap);

    FindValueMap(MyMap, customer1);

    FindKeyMap(MyMap, 1200.0);
    FindKeyMap(MyMap, 1750.0);

    map<Customers, double> MyFilteredMap = Map_filter(MyMap, 1000.0);
    PrintMap(MyFilteredMap);

    FindValueMap(MyFilteredMap, customer3);

    FindKeyMap(MyFilteredMap, 1200.0);
    FindKeyMap(MyFilteredMap, 1000.0);

    vector<double> Result = Values_Map(MyMap);
    PrintVector(Result);

    vector<double> Result1 = Values_Map(MyFilteredMap);
    PrintVector(Result1);

    cout << "\n-----Practice 2.2-----" << endl;

    multimap<Customers, double> MyMultiMap;

    MyMultiMap.emplace(customer1, 1500.0);
    MyMultiMap.emplace(customer2, 1200.0);
    MyMultiMap.emplace(customer3, 1800.0);
    MyMultiMap.emplace(customer4, 500.0);
    MyMultiMap.emplace(customer1, 1950.0);
    MyMultiMap.emplace(customer1, 1000.0);

    PrintMultiMap(MyMultiMap);

    FindValueMultiMap(MyMultiMap, customer2);

    FindKeyMultiMap(MyMultiMap, 1800.0);
    FindKeyMultiMap(MyMultiMap, 1900.0);

    multimap<Customers, double> MyFilteredMultiMap = MultiMap_filter(MyMultiMap, 1200.0);

    PrintMultiMap(MyFilteredMultiMap);

    FindValueMultiMap(MyFilteredMultiMap, customer2);

    FindKeyMultiMap(MyFilteredMultiMap, 1800.0);
    FindKeyMultiMap(MyFilteredMultiMap, 500.0);

    vector<double> Result3 = Values_MultiMap(MyMultiMap);
    PrintVector(Result3);

    vector<double> Result4 = Values_MultiMap(MyFilteredMultiMap);
    PrintVector(Result4);

    multimap<Customers, double> SameElement_of_MyMultimap = SameElem(MyMultiMap, customer1);
    PrintMultiMap(SameElement_of_MyMultimap);

    multimap<Customers, double> SameElement_of_MyFilteredMultiMap = SameElem(MyFilteredMultiMap, customer1);
    PrintMultiMap(SameElement_of_MyFilteredMultiMap);
}
