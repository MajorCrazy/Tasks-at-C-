#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>
#include <cmath>
#include <thread>
#include <chrono>
#include <utility>

using namespace std;

class TravelSystem {
private:
    // Хранит информацию о рейсах
    struct Destination {
        int duration;
        double cost;

        Destination(int dur, double co) {
            duration = dur;
            cost = co;
        }
    };
    // Хранит отсечённые пути
    struct InefficientPath {
        string from;
        string to;
    };

    vector<InefficientPath> inefficientPaths;
    vector<string> destinations;
    vector<vector<Destination>> adjacencyMatrix;
    vector<string> currentPath;
    vector<vector<string>> allPaths;
    // Алгоритм поиска в глубину, используется в методе findRoute()
    void dfs(int current, int destination) {
        if (current == destination) {
            allPaths.push_back(currentPath);
            return;
        }

        for (size_t i = 0; i < destinations.size(); ++i) {
            if (adjacencyMatrix[current][i].cost > 0) {
                // Проверка на посещенность вершины
                if (find(currentPath.begin(), currentPath.end(), destinations[i]) == currentPath.end()) {
                    // Помечаем вершину как посещенную
                    currentPath.push_back(destinations[i]);
                    dfs(i, destination);
                    // Отмечаем вершину как непосещенную после завершения DFS
                    currentPath.pop_back();
                }
            }
        }
    }
public:
    // Метод для добавления пункта назначения
    void addDestination(const string& dest) {
        destinations.push_back(dest);

        // Обновляем матрицу смежности
        for (auto& row : adjacencyMatrix) {
            row.emplace_back(0, 0); // Добавляем новый элемент с нулевыми значениями
        }

        // Добавляем новую строку с нулевыми значениями
        vector<Destination> newRow(destinations.size(), Destination(0, 0));
        adjacencyMatrix.push_back(newRow);
    }

    // Метод для создания рейса между двумя пунктами
    void createFlight(const string& fromCity, const string& toCity, double cost, int duration) {
        auto fromIndex = find(destinations.begin(), destinations.end(), fromCity);
        auto toIndex = find(destinations.begin(), destinations.end(), toCity);

        if (fromIndex != destinations.end() && toIndex != destinations.end()) {
            int from = distance(destinations.begin(), fromIndex);
            int to = distance(destinations.begin(), toIndex);

            // Обновляем матрицу смежности с информацией о рейсе
            adjacencyMatrix[from][to].cost = cost;
            adjacencyMatrix[to][from].cost = cost; // Предполагаем, что рейс двусторонний

            adjacencyMatrix[from][to].duration = duration;
            adjacencyMatrix[to][from].duration = duration;
        }
        else {
            cerr << "One or both cities not found." << endl;
        }
    }

    // Метод для вывода информации о рейсах
    void displayFlights() const {
        for (size_t i = 0; i < destinations.size(); ++i) {
            for (size_t j = 0; j < destinations.size(); ++j) {
                if (adjacencyMatrix[i][j].cost > 0) {
                    cout << "Flight from " << destinations[i] << " to " << destinations[j]
                        << " Cost: " << adjacencyMatrix[i][j].cost << " Duration: " << adjacencyMatrix[i][j].duration << endl;
                }
            }
        }
    }

    // Метод для загрузки информации о пунктах назначения из файла
    void loadDestinationsFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string name;

            while (file >> name) {
                addDestination(name);
            }

            file.close();
        }
        else {
            cerr << "Unable to open the file: " << filename << endl;
        }
    }

    // Метод для загрузки информации о рейсах из файла
    void loadFlightsFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string from, to;
            double cost;
            int duration;

            while (file >> from >> to >> cost >> duration) {
                createFlight(from, to, cost, duration);
            }

            file.close();
        }
        else {
            cerr << "Unable to open the file: " << filename << endl;
        }
    }

    // Допустим, метод для вывода матрицы смежности для отладки
    void displayAdjacencyMatrix() {
        for (size_t i = 0; i < destinations.size(); ++i) {
            for (size_t j = 0; j < destinations.size(); ++j) {
                cout << adjacencyMatrix[i][j].cost << " ";
            }
            cout << endl;
        }

        cout << endl;

        for (size_t i = 0; i < destinations.size(); ++i) {
            for (size_t j = 0; j < destinations.size(); ++j) {
                cout << adjacencyMatrix[i][j].duration << " ";
            }
            cout << endl;
        }
    }

    // Метод для поиска маршрута между двумя пунктами
    void findRoute(const string& fromCity, const string& toCity) {
        currentPath.clear();
        allPaths.clear();

        auto fromIndex = find(destinations.begin(), destinations.end(), fromCity);
        auto toIndex = find(destinations.begin(), destinations.end(), toCity);

        if (fromIndex != destinations.end() && toIndex != destinations.end()) {
            int from = distance(destinations.begin(), fromIndex);
            int to = distance(destinations.begin(), toIndex);

            // Инициализируем текущий путь
            currentPath.push_back(destinations[from]);
            dfs(from, to);

            // Выводим найденные маршруты
            cout << "\nRoutes from " << fromCity << " to " << toCity << ":" << endl;
            for (const auto& path : allPaths) {
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i];
                    if (i < path.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;
            }
        }
        else {
            cerr << "\nOne or both cities not found." << endl;
        }
    }
    
    // Метод поиска пути с помощью алгоритма Дейкстры
    void findBestRouteDijkstra(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<vector<int>> distances(destinations.size(), vector<int>(2, numeric_limits<int>::max()));
        vector<vector<int>> parents(destinations.size(), vector<int>(2, -1));
        vector<vector<bool>> visited(destinations.size(), vector<bool>(2, false));

        distances[from][0] = 0; // 0 для длительности
        distances[from][1] = 0; // 1 для цены

        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            for (int weightType = 0; weightType < 2; ++weightType) {
                int u = -1;
                for (size_t i = 0; i < destinations.size(); ++i) {
                    if (!visited[i][weightType] && (u == -1 || distances[i][weightType] < distances[u][weightType]))
                        u = i;
                }

                visited[u][weightType] = true;

                for (size_t v = 0; v < destinations.size(); ++v) {
                    double edgeWeight = (weightType == 0) ? adjacencyMatrix[u][v].duration : adjacencyMatrix[u][v].cost;

                    if (!visited[v][weightType] && edgeWeight > 0 &&
                        distances[u][weightType] != numeric_limits<int>::max() &&
                        distances[u][weightType] + edgeWeight < distances[v][weightType]) {
                        distances[v][weightType] = distances[u][weightType] + edgeWeight;
                        parents[v][weightType] = u;
                    }
                    //else {
                    //    // Путь считается неэффективным, сохраняем информацию
                    //    cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";
                    //    inefficientPaths.push_back({ destinations[u], destinations[v] });
                    //}
                    // Если применить в коде, то будет и без того большой объём информации в консоли
                    // Поэтому, если интересно, то просто необходимо убрать комментарии
                }
            }
        }

        // Вывод результатов для длительности
        vector<string> fastestRoute;
        for (int v = to; v != -1; v = parents[v][0]) {
            fastestRoute.push_back(destinations[v]);
        }
        reverse(fastestRoute.begin(), fastestRoute.end());

        cout << "\nFastest Route using Dijkstra algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : fastestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal duration: " << distances[to][0] << endl;

        // Вывод результатов для цены
        vector<string> cheapestRoute;
        for (int v = to; v != -1; v = parents[v][1]) {
            cheapestRoute.push_back(destinations[v]);
        }
        reverse(cheapestRoute.begin(), cheapestRoute.end());

        cout << "\nCheapest Route using Dijkstra algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : cheapestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal cost: " << distances[to][1] << endl;
    }
    // Метод поиска пути с помощью алгоритма Беллмана-Форда
    bool findBestRouteBellmanFord(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<vector<int>> distances(destinations.size(), vector<int>(2, numeric_limits<int>::max()));
        vector<vector<int>> parents(destinations.size(), vector<int>(2, -1));

        distances[from][0] = 0; // 0 для длительности
        distances[from][1] = 0; // 1 для цены

        const double Max_Negative_Cost = -5000;
        const int Max_Negative_Duration = -50;

        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            for (size_t i = 0; i < destinations.size(); ++i) {
                for (size_t j = 0; j < destinations.size(); ++j) {
                    if (adjacencyMatrix[i][j].cost > 0) {
                        // Релаксация для продолжительности
                        if (distances[i][0] != numeric_limits<int>::max() &&
                            distances[i][0] + adjacencyMatrix[i][j].duration < distances[j][0]) {
                            distances[j][0] = distances[i][0] + adjacencyMatrix[i][j].duration;
                            if (distances[j][0] < Max_Negative_Duration) {
                                cerr << "\nNegative cycle has been detected\n";
                                return false;
                            }
                            parents[j][0] = i;
                        }
                        
                        // Релаксация для цены
                        if (distances[i][1] != numeric_limits<int>::max() &&
                            distances[i][1] + adjacencyMatrix[i][j].cost < distances[j][1]) {
                            distances[j][1] = distances[i][1] + adjacencyMatrix[i][j].cost;
                            if (distances[j][0] < Max_Negative_Cost) {
                                cerr << "\nNegative cycle has been detected\n";
                                return false;
                            }
                            parents[j][1] = i;
                        }
                    }
                }
            }
        }

        // Вывод результатов для длительности
        vector<string> fastestRoute;
        for (int v = to; v != -1; v = parents[v][0]) {
            fastestRoute.push_back(destinations[v]);
        }
        reverse(fastestRoute.begin(), fastestRoute.end());

        cout << "\nFastest Route using Bellman-Ford algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : fastestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal duration: " << distances[to][0] << endl;

        // Вывод результатов для цены
        vector<string> cheapestRoute;
        for (int v = to; v != -1; v = parents[v][1]) {
            cheapestRoute.push_back(destinations[v]);
        }
        reverse(cheapestRoute.begin(), cheapestRoute.end());

        cout << "\nCheapest Route using Bellman-Ford algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : cheapestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal cost: " << distances[to][1] << endl;

        // Отрицательных циклов не обнаружено
        return true;
    }
    // Метод поиска пути с помощью алгоритма Флойда
    void FloydWarshall() {
        size_t n = destinations.size();

        // Создаем матрицы для хранения кратчайших путей между всеми парами вершин
        vector<vector<int>> durations(n, vector<int>(n, numeric_limits<int>::max()));
        vector<vector<int>> costs(n, vector<int>(n, numeric_limits<int>::max()));

        // Инициализируем матрицы кратчайших путей текущими значениями
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == j) {
                    durations[i][j] = 0;
                }
                else if (adjacencyMatrix[i][j].duration > 0) {
                    durations[i][j] = adjacencyMatrix[i][j].duration;
                }
            }
        }

        // Алгоритм Флойда для длительности
        for (size_t k = 0; k < n; ++k) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (durations[i][k] != numeric_limits<int>::max() &&
                        durations[k][j] != numeric_limits<int>::max() &&
                        durations[i][k] + durations[k][j] < durations[i][j]) {
                        durations[i][j] = durations[i][k] + durations[k][j];
                    }
                }
            }
        }

        // Выводим результат для длительности
        cout << "\nFastest durations between all pairs of destinations with Floyd algorithm:" << endl;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (durations[i][j] != numeric_limits<int>::max()) {
                    cout << "From " << destinations[i] << " to " << destinations[j] << ": " << durations[i][j] << " hours" << endl;
                }
            }
        }

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == j) {
                    costs[i][j] = 0;
                }
                else if (adjacencyMatrix[i][j].cost > 0) {
                    costs[i][j] = adjacencyMatrix[i][j].cost;
                }
            }
        }

        // Алгоритм Флойда для стоимости
        for (size_t k = 0; k < n; ++k) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (costs[i][k] != numeric_limits<int>::max() &&
                        costs[k][j] != numeric_limits<int>::max() &&
                        costs[i][k] + costs[k][j] < costs[i][j]) {
                        costs[i][j] = costs[i][k] + costs[k][j];
                    }
                }
            }
        }

        // Выводим результат для стоимости
        cout << "\nCheapest costs between all pairs of destinations with Floyd algorithm:" << endl;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (costs[i][j] != numeric_limits<int>::max()) {
                    cout << "From " << destinations[i] << " to " << destinations[j] << ": " << costs[i][j] << " units" << endl;
                }
            }
        }
    }

    // Метод для поиска машрута между двумя пунктами по сбалансированным параметрам(алгоритм A*, модификация алгоритма Дейкстры),
    // а также, реализация процесса «жадного» поиска маршрута или отсечения перебираемых путей, которые оцениваются как неэффективные, для ускорения поиска.
    void findBalancedRouteAStar(const string& fromCity, const string& toCity, double durationWeight, double costWeight) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        // Определение эвристической функции для расстояния между двумя вершинами
        auto heuristic = [this](int city1, int city2) {
            return abs(city1 - city2);
            };

        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        vector<double> totalCost(destinations.size(), numeric_limits<double>::max());
        vector<int> parent(destinations.size(), -1);
        vector<bool> visited(destinations.size(), false);

        totalCost[from] = 0;
        pq.push({ 0, from });

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == to) {
                break; // Достигнута конечная вершина
            }

            visited[u] = true;

            for (size_t v = 0; v < destinations.size(); ++v) {
                if (!visited[v] && adjacencyMatrix[u][v].cost > 0 && adjacencyMatrix[u][v].duration > 0) {
                    double costToV = totalCost[u] + costWeight * adjacencyMatrix[u][v].cost;
                    double durationToV = totalCost[u] + durationWeight * adjacencyMatrix[u][v].duration;

                    double heuristicCost = heuristic(v, to);

                    // здесь баланс между длительностью и ценой
                    double combinedCost = costWeight * costToV + durationWeight * durationToV + heuristicCost;

                    if (combinedCost < totalCost[v]) {
                        totalCost[v] = combinedCost;
                        parent[v] = u;
                        pq.push({ combinedCost, v });
                    } else {
                        // Путь считается неэффективным
                        cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";
                        // сохраняет информацию в структуре данных для последующего анализа
                        inefficientPaths.push_back({ destinations[u], destinations[v] });
                    }
                }
            }
        }

        // Строим маршрут
        vector<string> balancedRoute;
        for (int v = to; v != -1; v = parent[v]) {
            balancedRoute.push_back(destinations[v]);
        }

        reverse(balancedRoute.begin(), balancedRoute.end());

        // Выводим результат
        cout << "\nBalanced route from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : balancedRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal cost: " << totalCost[to] << " Total duration: " << totalCost[to] / durationWeight << endl;
    }
    // Отображает прогресс выполнения алгоритма поиска в глубину
    void processingThreadFindRoute(const string& fromCity) {
        for (size_t i = 0; i < destinations.size(); ++i) {

            // вызов метода поиска маршрута для каждой вершины
            findRoute(fromCity, destinations[i]);
            // Периодически выводим текущий прогресс
            cout << "\nProgress: " << i + 1 << " out of " << destinations.size() << " vertices processed.\n";
            
        }
    }
    // Отображает прогресс выполнения алгоритма Дейкстры
    void proccesingThreadFindBestRouteDijkstra(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findBestRouteDijkstra(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the cheapest route is completed.\n";
    }
    // Отображает прогресс выполнения алгоритма Беллмана-Форда
    void proccesingThreadFindBestRouteBellmanFord(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findBestRouteBellmanFord(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the cheapest route is completed.\n";
    }
    // Отображает прогресс выполнения алгоритма А*
    void proccesingThreadFindBalancedRouteAStar(const string& fromCity, double durationWeight, double costWeight) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findBalancedRouteAStar(fromCity, destinations[i], durationWeight, costWeight);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the fastest route is completed.\n";
    }
    // Отображает производительность по времени алгоритма А*(в микросекундах)
    void MeasuringTimeOfAStar(const string& fromCity, const string& toCity, double durationWeight, double costWeight) {
        auto start = chrono::high_resolution_clock::now();
        findBalancedRouteAStar(fromCity, toCity, durationWeight, costWeight);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "A* Execution time: " << duration.count() << " microseconds\n";
    }
    // Отображает производительность по времени алгоритма Дейкстры(в микросекундах)
    void MeasuringTimeDijkstra(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findBestRouteDijkstra(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Dijkstra Execution time: " << duration.count() << " microseconds\n";
    }
    // Отображает производительность по времени алгоритма Беллмана-Форда(В микросекундах)
    void MeasuringTimeBellmanFord(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findBestRouteBellmanFord(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Bellman-Ford Execution time: " << duration.count() << " microseconds\n";
    }
    // Отображает производительность по времени алгоритма Флойда(в микросекундах)
    void MeasuringTimeFloydWarshall() {
        auto start = chrono::high_resolution_clock::now();
        FloydWarshall();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Floyd-Warshall Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeFindRoute(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findRoute(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "DFS time: " << duration.count() << " microseconds\n";
    }

};

int main() {
    TravelSystem travelSystem;

    // Загружаем информацию о пунктах назначения из файла
    travelSystem.loadDestinationsFromFile("destinations.txt");
    // Загружаем информацию о рейсах из файла
    travelSystem.loadFlightsFromFile("flights.txt");

    // Выводим информацию о рейсах
    travelSystem.displayFlights();

    // Выводим матрицу смежности для отладки
    travelSystem.displayAdjacencyMatrix();

    travelSystem.MeasuringTimeOfAStar("Moscow", "Lisbon", 2.0, 1.0);

    travelSystem.processingThreadFindRoute("Moscow");
    
    cout << "\n-------------Example of Display Dijkstra proccesing---------------------\n";

    travelSystem.proccesingThreadFindBestRouteDijkstra("Moscow");

    cout << "\n-----------------------------End of example-----------------------------\n";

    travelSystem.findBestRouteDijkstra("Moscow", "Lisbon");
    travelSystem.findBestRouteDijkstra("Warsaw", "London");
    travelSystem.findBestRouteDijkstra("Warsaw", "New-York");

    cout << "\n-------------Example of Display Bellman-Ford proccesing---------------------\n";

    travelSystem.proccesingThreadFindBestRouteBellmanFord("Moscow");

    cout << "\n-----------------------------End of example-----------------------------\n";

    travelSystem.findBestRouteBellmanFord("Moscow", "Lisbon");
    travelSystem.findBestRouteBellmanFord("Warsaw", "London");
    travelSystem.findBestRouteBellmanFord("Warsaw", "New-York");

    travelSystem.findBalancedRouteAStar("Moscow", "Lisbon", 2.0, 1.0);

    cout << "\n-------------Example of Display A* proccesing---------------------\n";

    travelSystem.proccesingThreadFindBalancedRouteAStar("Moscow", 2.0, 1.0);

    cout << "\n-----------------------------End of example-----------------------------\n";

    TravelSystem travelSystemWithNegativeWeight;
    // Загружаем информацию о пунктах назначения из файла
    travelSystemWithNegativeWeight.loadDestinationsFromFile("destinations.txt");
    // Загружаем информацию о рейсах из файла
    travelSystemWithNegativeWeight.loadFlightsFromFile("flights1.txt");

    travelSystemWithNegativeWeight.displayFlights();
    travelSystemWithNegativeWeight.displayAdjacencyMatrix();
    travelSystemWithNegativeWeight.MeasuringTimeFindRoute("Moscow", "Lisbon");

    travelSystemWithNegativeWeight.findBestRouteBellmanFord("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.findBestRouteBellmanFord("Warsaw", "London");
    travelSystemWithNegativeWeight.findBestRouteBellmanFord("Warsaw", "New-York");

    travelSystem.FloydWarshall();

    travelSystem.MeasuringTimeOfAStar("Moscow", "Lisbon", 2.0, 1.0);

    travelSystem.MeasuringTimeDijkstra("Moscow", "Lisbon");
    travelSystem.MeasuringTimeDijkstra("Warsaw", "London");
    travelSystem.MeasuringTimeDijkstra("Warsaw", "New-York");

    travelSystem.MeasuringTimeBellmanFord("Moscow", "Lisbon");
    travelSystem.MeasuringTimeBellmanFord("Warsaw", "London");
    travelSystem.MeasuringTimeBellmanFord("Warsaw", "New-York");

    travelSystemWithNegativeWeight.MeasuringTimeBellmanFord("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFord("Warsaw", "London");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFord("Warsaw", "New-York");

    travelSystem.MeasuringTimeFloydWarshall();
    travelSystemWithNegativeWeight.MeasuringTimeFloydWarshall();

    return 0;
}
