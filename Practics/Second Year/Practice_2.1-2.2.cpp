// Понамарёв Никита, КМБО-04-22
// Вариант 17

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <set>
using namespace std;

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
        else
        {
            cout << "\nElement " << v << " not found\n";
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
    cout << "\nVector values: ";
    for (double Value : vec)
    {
        cout << Value << " ";
    }
    cout << "\n";
}

void PrintMultimap(const multimap<string, double>& mmap)
{
    auto it = mmap.begin();
    cout << "\nMultimap: \n";
    while (it != mmap.end())
    {
        cout << "Key: " << it->first << ", Value: " << it->second << "\n";
        it++;
    }
}

void FindElem(const multimap<string, double>& mmapp, string key)
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

void FindElem(const multimap<string, double>& mmap, double v)
{
    for (const auto& it : mmap)
    {
        if (it.second == v)
        {
            cout << "\nFound Element:\n";
            cout << "Key: " << it.first << ", Value: " << it.second << "\n";
        }
    }
}

multimap<string, double> filter(const multimap<string, double>& mmap, double Threshold) 
{
    multimap<string, double> Result;
    for (const auto& it : mmap) 
    {
        if (Predicate(it.second, Threshold)) 
        {
            Result.insert(it);
        }
    }
    return Result;
}

vector<double> Values(const multimap<string, double>& mp)
{
    set<double> Val;
    for (const auto& it : mp)
    {
        Val.insert(it.second);
    }
    return vector<double>(Val.begin(), Val.end());
}

multimap<string, double> SameElem(const multimap<string, double>& mmap, string Key)
{
    multimap<string, double> Result;
    for (const auto& it : mmap)
    {
        if (it.first == Key)
        {
            Result.insert(it);
        }
    }

    return Result;
}

int main()
{
    setlocale(LC_ALL, "RU_ru");

    map<string, double> AvgCheck;

    AvgCheck["Ivanov Petr"] = 1500.0;
    AvgCheck["Petrov Ivan"] = 1200.0;
    AvgCheck["Sidorova Anna"] = 1800.0;
    AvgCheck["Dyagteryov Evgeniy"] = 500.0;

    PrintMap(AvgCheck);

    cout << "\n-----Test try-catch-----\n";
    try
    {
        string Key = "Ponamarev Nikita";
        double Value = 1900.0;

        if (AvgCheck.find(Key) != AvgCheck.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        AvgCheck[Key] = Value;
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMap(AvgCheck);

    cout << "\n-----Test try-catch 2-----\n";
    try
    {
        string Key = "Petrov Ivan";
        double Value = 1900.0;

        if (AvgCheck.find(Key) != AvgCheck.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        AvgCheck[Key] = Value;
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMap(AvgCheck);

    FindElem(AvgCheck, "Ivanov Petr");
    FindElem(AvgCheck, "Ivanov Ivan");
    FindElem(AvgCheck, 1200.0);
    FindElem(AvgCheck, 3100.0);

    PrintMap(AvgCheck);

    vector<double> Values_Of_AvgCheck = Values(AvgCheck);
    PrintVector(Values_Of_AvgCheck);

    map<string, double> filtered_AvgCheck = filter(AvgCheck, 1000.0);
    
    PrintMap(filtered_AvgCheck);

    cout << "\n-----Test try-catch 3-----\n";
    try
    {
        string Key = "Ponamarev Nikita";
        double Value = 1900.0;

        if (filtered_AvgCheck.find(Key) != filtered_AvgCheck.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        filtered_AvgCheck[Key] = Value;
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMap(filtered_AvgCheck);

    cout << "\n-----Test try-catch 4-----\n";
    try
    {
        string Key = "Kondrashov Alexander";
        double Value = 2000.0;

        if (filtered_AvgCheck.find(Key) != filtered_AvgCheck.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        filtered_AvgCheck[Key] = Value;
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMap(filtered_AvgCheck);

    FindElem(filtered_AvgCheck, "Sidorova Anna");
    FindElem(filtered_AvgCheck, "Petrov Petr");
    FindElem(filtered_AvgCheck, 1500.0);
    FindElem(filtered_AvgCheck, 3000.0);

    vector<double> Values_Of_Filtred_AvgCheck = Values(filtered_AvgCheck);
    PrintVector(Values_Of_Filtred_AvgCheck);

    cout << "\n----------Task 2.2----------\n";

    multimap<string, double> AvgCheck2;
    AvgCheck2.insert(AvgCheck2.begin(), { "Ivanov Petr", 1500.0 });
    AvgCheck2.insert(AvgCheck2.begin(), { "Petrov Ivan", 1200.0 });
    AvgCheck2.insert(AvgCheck2.begin(), { "Sidorova Anna", 1800.0 });
    AvgCheck2.insert(AvgCheck2.begin(), { "Dyagteryov Evgeniy", 500.0 });
    AvgCheck2.insert(AvgCheck2.begin(), { "Petrov Ivan", 2200.0 });
    AvgCheck2.insert(AvgCheck2.begin(), { "Sidorova Anna", 2400.0 });

    FindElem(AvgCheck2, "Petrov Ivan");
    FindElem(AvgCheck2, "Petrov Petr");
    FindElem(AvgCheck2, 500.0);
    FindElem(AvgCheck2, 400.0);

    PrintMultimap(AvgCheck2);

    cout << "\n-----Test try-catch 1-----\n";
    try
    {
        string Key = "Ivanov Petr";
        double Value = 1450.0;
        if (AvgCheck2.find(Key) != AvgCheck2.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        AvgCheck2.insert(AvgCheck2.begin(), { Key, Value });
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMultimap(AvgCheck2);

    cout << "\n-----Test try-catch 2-----\n";
    try
    {
        string Key = "Ponamarev Nikita";
        double Value = 1450.0;
        if (AvgCheck2.find(Key) != AvgCheck2.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        AvgCheck2.insert(AvgCheck2.begin(), { Key, Value });
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMultimap(AvgCheck2);

    vector<double> Values_Of_AvgCheck2 = Values(AvgCheck2);
    PrintVector(Values_Of_AvgCheck2);

    multimap<string, double> SameElem_of_AvgCheck2 = SameElem(AvgCheck2, "Sidorova Anna");
    PrintMultimap(SameElem_of_AvgCheck2);

    multimap<string, double> filtered_AvgCheck2 = filter(AvgCheck2, 1400.0);
    PrintMultimap(filtered_AvgCheck2);

    filtered_AvgCheck2.insert(filtered_AvgCheck2.begin(), { "Ivanov Petr", 2600.0 });

    FindElem(filtered_AvgCheck2, "Sidorova Anna");
    FindElem(filtered_AvgCheck2, "Ivanov Nikolay");
    FindElem(filtered_AvgCheck2, 1500.0);
    FindElem(filtered_AvgCheck2, 1250.0);

    PrintMultimap(filtered_AvgCheck2);

    cout << "\n-----Test try-catch 3-----\n";
    try
    {
        string Key = "Ponamarev Nikita";
        double Value = 1950.0;
        if (filtered_AvgCheck2.find(Key) != filtered_AvgCheck2.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        filtered_AvgCheck2.insert(filtered_AvgCheck2.begin(), { Key, Value });
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMultimap(filtered_AvgCheck2);

    cout << "\n-----Test try-catch 4-----\n";
    try
    {
        string Key = "Kondrashov Alexander";
        double Value = 2050.0;
        if (filtered_AvgCheck2.find(Key) != filtered_AvgCheck2.end())
        {
            throw invalid_argument("Element with key " + Key + " already exists.");
        }

        filtered_AvgCheck2.insert(filtered_AvgCheck2.begin(), { Key, Value });
    }
    catch (const invalid_argument& e)
    {
        cerr << "\nException: " << e.what() << "\n";
    }

    PrintMultimap(filtered_AvgCheck2);

    vector<double> Values_Of_filtered_AvgCheck2 = Values(filtered_AvgCheck2);
    PrintVector(Values_Of_filtered_AvgCheck2);

    multimap<string, double> SameElem_of_filtered_AvgCheck2 = SameElem(filtered_AvgCheck2, "Ivanov Petr");
    PrintMultimap(SameElem_of_filtered_AvgCheck2);
}
