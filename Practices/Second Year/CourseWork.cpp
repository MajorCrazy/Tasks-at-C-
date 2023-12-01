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
    struct Destination {
        int duration;
        double cost;

        Destination(int dur, double co) {
            duration = dur;
            cost = co;
        }
    };

    struct InefficientPath {
        string from;
        string to;
    };

    vector<InefficientPath> inefficientPaths;
    vector<string> destinations;
    vector<vector<Destination>> adjacencyMatrix;
    vector<string> currentPath;
    vector<vector<string>> allPaths;

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

    // Метод для поиска машрута между двумя пунктами по наилучшей длительности(алгоритм Дейкстры)
    void findFastestRouteDijkstra(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<int> distance(destinations.size(), numeric_limits<int>::max());
        vector<int> parent(destinations.size(), -1);
        vector<bool> visited(destinations.size(), false);

        distance[from] = 0;

        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            int u = -1;
            for (size_t i = 0; i < destinations.size(); ++i) {
                if (!visited[i] && (u == -1 || distance[i] < distance[u]))
                    u = i;
            }

            visited[u] = true;

            for (size_t v = 0; v < destinations.size(); ++v) {
                if (!visited[v] && adjacencyMatrix[u][v].duration > 0 && distance[u] != numeric_limits<int>::max() &&
                    distance[u] + adjacencyMatrix[u][v].duration < distance[v]) {
                    distance[v] = distance[u] + adjacencyMatrix[u][v].duration;
                    parent[v] = u;
                }
                else {
                    //// Путь считается неэффективным, выведите сообщение или сохраните информацию
                    //cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";  //Смотреть пояснение о комментариях в строке 270
                    //// Или сохраните информацию в структуре данных для последующего анализа
                    //inefficientPaths.push_back({ destinations[u], destinations[v] });
                }
            }
        }

        vector<string> FastestRoute;
        for (int v = to; v != -1; v = parent[v]) {
            FastestRoute.push_back(destinations[v]);
        }

        reverse(FastestRoute.begin(), FastestRoute.end());

        cout << "\nFastest Route using Dijkstra algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : FastestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal duration: " << distance[to] << endl;
    }
    // Метод для поиска машрута между двумя пунктами по наилучшей цене(алгоритм Дейкстры)
    void findCheapestRouteDijkstra(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<int> distance(destinations.size(), numeric_limits<int>::max());
        vector<int> parent(destinations.size(), -1);
        vector<bool> visited(destinations.size(), false);

        distance[from] = 0;

        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            int u = -1;
            for (size_t i = 0; i < destinations.size(); ++i) {
                if (!visited[i] && (u == -1 || distance[i] < distance[u]))
                    u = i;
            }

            visited[u] = true;

            for (size_t v = 0; v < destinations.size(); ++v) {
                if (!visited[v] && adjacencyMatrix[u][v].cost > 0 && distance[u] != numeric_limits<int>::max() &&
                    distance[u] + adjacencyMatrix[u][v].cost < distance[v]) {
                    distance[v] = distance[u] + adjacencyMatrix[u][v].cost;
                    parent[v] = u;
                }
                //else {
                //    // Путь считается неэффективным, выведите сообщение или сохраните информацию
                //    cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";  //Так как выводов в консоли слишком много, лучше
                //    // Или сохраните информацию в структуре данных для последующего анализа                                  //просто при необходимости в остальных алгоритмах
                //    inefficientPaths.push_back({ destinations[u], destinations[v] });                                        //комменить/убрать комменты
                //}
            }
        }

        vector<string> cheapestRoute;
        for (int v = to; v != -1; v = parent[v]) {
            cheapestRoute.push_back(destinations[v]);
        }

        reverse(cheapestRoute.begin(), cheapestRoute.end());

        cout << "\nCheapest Route using Dijkstra algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : cheapestRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal cost: " << distance[to] << endl;
    }
    
    void findFastestRouteBellmanFord(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<double> distance(destinations.size(), numeric_limits<double>::max());
        vector<int> parent(destinations.size(), -1);

        distance[from] = 0;

        // Применяем алгоритм Беллмана-Форда
        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            for (size_t u = 0; u < destinations.size(); ++u) {
                for (size_t v = 0; v < destinations.size(); ++v) {
                    if (adjacencyMatrix[u][v].duration > 0 && distance[u] != numeric_limits<double>::max() &&
                        distance[u] + adjacencyMatrix[u][v].duration < distance[v]) {
                        distance[v] = distance[u] + adjacencyMatrix[u][v].duration;
                        parent[v] = u;
                    }
                    //else {
                    //    // Путь считается неэффективным, выведите сообщение или сохраните информацию
                    //    cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";  //см. строку 270
                    //    // Или сохраните информацию в структуре данных для последующего анализа
                    //    inefficientPaths.push_back({ destinations[u], destinations[v] });
                    //}
                }
            }
        }

        // Проверяем наличие отрицательных циклов
        for (size_t u = 0; u < destinations.size(); ++u) {
            for (size_t v = 0; v < destinations.size(); ++v) {
                if (adjacencyMatrix[u][v].duration > 0 && distance[u] != numeric_limits<double>::max() &&
                    distance[u] + adjacencyMatrix[u][v].duration < distance[v]) {
                    cerr << "Negative cycle detected. Unable to find the route." << endl;
                    return;
                }
            }
        }

        // Строим маршрут
        vector<string> bellmanFordRoute;
        for (int v = to; v != -1; v = parent[v]) {
            bellmanFordRoute.push_back(destinations[v]);
        }

        reverse(bellmanFordRoute.begin(), bellmanFordRoute.end());

        // Выводим результат
        cout << "\nRoute using Bellman-Ford algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : bellmanFordRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal duration: " << distance[to] << endl;
    }

    void findCheapestRouteBellmanFord(const string& fromCity, const string& toCity) {
        int from = distance(destinations.begin(), find(destinations.begin(), destinations.end(), fromCity));
        int to = distance(destinations.begin(), find(destinations.begin(), destinations.end(), toCity));

        vector<double> distance(destinations.size(), numeric_limits<double>::max());
        vector<int> parent(destinations.size(), -1);

        distance[from] = 0;

        // Применяем алгоритм Беллмана-Форда
        for (size_t count = 0; count < destinations.size() - 1; ++count) {
            for (size_t u = 0; u < destinations.size(); ++u) {
                for (size_t v = 0; v < destinations.size(); ++v) {
                    if (adjacencyMatrix[u][v].cost > 0 && distance[u] != numeric_limits<double>::max() &&
                        distance[u] + adjacencyMatrix[u][v].cost < distance[v]) {
                        distance[v] = distance[u] + adjacencyMatrix[u][v].cost;
                        parent[v] = u;
                    }
                    //else {
                    //    // Путь считается неэффективным, выведите сообщение или сохраните информацию
                    //    cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";   //см. строку 270
                    //    // Или сохраните информацию в структуре данных для последующего анализа
                    //    inefficientPaths.push_back({ destinations[u], destinations[v] });
                    //}
                }
            }
        }

        // Проверяем наличие отрицательных циклов
        for (size_t u = 0; u < destinations.size(); ++u) {
            for (size_t v = 0; v < destinations.size(); ++v) {
                if (adjacencyMatrix[u][v].cost > 0 && distance[u] != numeric_limits<double>::max() &&
                    distance[u] + adjacencyMatrix[u][v].cost < distance[v]) {
                    cerr << "Negative cycle detected. Unable to find the route." << endl;
                    return;
                }
            }
        }

        // Строим маршрут
        vector<string> bellmanFordRoute;
        for (int v = to; v != -1; v = parent[v]) {
            bellmanFordRoute.push_back(destinations[v]);
        }

        reverse(bellmanFordRoute.begin(), bellmanFordRoute.end());

        // Выводим результат
        cout << "\nRoute using Bellman-Ford algorithm from " << fromCity << " to " << toCity << ": ";
        for (const auto& city : bellmanFordRoute) {
            cout << city << " -> ";
        }
        cout << "\nTotal cost: " << distance[to] << endl;
    }

    void runFloydWarshallDuration() {
        size_t n = destinations.size();

        // Создаем матрицу для хранения кратчайших путей между всеми парами вершин
        vector<vector<int>> FastestDuration(n, vector<int>(n, numeric_limits<int>::max()));

        // Инициализируем матрицу кратчайших путей текущими значениями длительности
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == j) {
                    FastestDuration[i][j] = 0;
                }
                else if (adjacencyMatrix[i][j].duration > 0) {
                    FastestDuration[i][j] = adjacencyMatrix[i][j].duration;
                }
            }
        }

        // Алгоритм Флойда
        for (size_t k = 0; k < n; ++k) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (FastestDuration[i][k] != numeric_limits<int>::max() &&
                        FastestDuration[k][j] != numeric_limits<int>::max() &&
                        FastestDuration[i][k] + FastestDuration[k][j] < FastestDuration[i][j]) {
                        FastestDuration[i][j] = FastestDuration[i][k] + FastestDuration[k][j];
                    }
                }
            }
        }

        // Выводим результат
        cout << "\nFastest durations between all pairs of destinations with Floyd algorithm:" << endl;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (FastestDuration[i][j] != numeric_limits<int>::max()) {
                    cout << "From " << destinations[i] << " to " << destinations[j] << ": " << FastestDuration[i][j] << " hours" << endl;
                }
            }
        }
    }

    void runFloydWarshallCost() {
        size_t n = destinations.size();

        // Создаем матрицу для хранения кратчайших путей между всеми парами вершин
        vector<vector<int>> CheapestDuration(n, vector<int>(n, numeric_limits<int>::max()));

        // Инициализируем матрицу кратчайших путей текущими значениями стоимости
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == j) {
                    CheapestDuration[i][j] = 0;
                }
                else if (adjacencyMatrix[i][j].cost > 0) {
                    CheapestDuration[i][j] = adjacencyMatrix[i][j].cost;
                }
            }
        }

        // Алгоритм Флойда
        for (size_t k = 0; k < n; ++k) {
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    if (CheapestDuration[i][k] != numeric_limits<int>::max() &&
                        CheapestDuration[k][j] != numeric_limits<int>::max() &&
                        CheapestDuration[i][k] + CheapestDuration[k][j] < CheapestDuration[i][j]) {
                        CheapestDuration[i][j] = CheapestDuration[i][k] + CheapestDuration[k][j];
                    }
                }
            }
        }

        // Выводим результат
        cout << "\nCheapest durations between all pairs of destinations with Floyd algorithm:" << endl;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (CheapestDuration[i][j] != numeric_limits<int>::max()) {
                    cout << "From " << destinations[i] << " to " << destinations[j] << ": " << CheapestDuration[i][j] << " units" << endl;
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

                    // Используйте здесь ваш баланс между длительностью и ценой
                    double combinedCost = costWeight * costToV + durationWeight * durationToV + heuristicCost;

                    if (combinedCost < totalCost[v]) {
                        totalCost[v] = combinedCost;
                        parent[v] = u;
                        pq.push({ combinedCost, v });
                    } else {
                        // Путь считается неэффективным, выведите сообщение или сохраните информацию
                        cout << "Inefficient path from " << destinations[u] << " to " << destinations[v] << " is discarded.\n";
                        // Или сохраните информацию в структуре данных для последующего анализа
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

    void processingThreadFindRoute(const string& fromCity) {
        for (size_t i = 0; i < destinations.size(); ++i) {
            // Ваш код обработки вершины

            // Пример: вызов метода поиска маршрута для каждой вершины
            findRoute(fromCity, destinations[i]);

            // Периодически выводим текущий прогресс
           
            cout << "\nProgress: " << i + 1 << " out of " << destinations.size() << " vertices processed.\n";
            
        }
    }

    void proccesingThreadFindCheapestRouteDijkstra(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findCheapestRouteDijkstra(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the cheapest route is completed.\n";
    }

    void proccesingThreadFindFastestRouteDijkstra(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findFastestRouteDijkstra(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the fastest route is completed.\n";
    }

    void proccesingThreadFindCheapestRouteBellmanFord(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findCheapestRouteBellmanFord(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the cheapest route is completed.\n";
    }

    void proccesingThreadFindFastestRouteBellmanFord(const string& fromCity) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findCheapestRouteBellmanFord(fromCity, destinations[i]);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the fastest route is completed.\n";
    }

    void proccesingThreadFindBalancedRouteAStar(const string& fromCity, double durationWeight, double costWeight) {

        for (size_t i = 0; i < destinations.size(); ++i) {
            findBalancedRouteAStar(fromCity, destinations[i], durationWeight, costWeight);

            // Выводим текущий прогресс
            cout << "Progress: " << i + 1 << " from " << destinations.size() << " destinations processed.\n";
        }

        cout << "Calculation of the fastest route is completed.\n";
    }

    void MeasuringTimeOfAStar(const string& fromCity, const string& toCity, double durationWeight, double costWeight) {
        auto start = chrono::high_resolution_clock::now();
        findBalancedRouteAStar(fromCity, toCity, durationWeight, costWeight);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "A* Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeDijkstraDuration(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findFastestRouteDijkstra(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Dijkstra Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeDijkstraCost(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findCheapestRouteDijkstra(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Dijkstra Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeBellmanFordCost(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findCheapestRouteBellmanFord(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Bellman-Ford Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeBellmanFordDuration(const string& fromCity, const string& toCity) {
        auto start = chrono::high_resolution_clock::now();
        findFastestRouteBellmanFord(fromCity, toCity);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Bellman-Ford Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeFloydWarshallDuration() {
        auto start = chrono::high_resolution_clock::now();
        runFloydWarshallDuration();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Floyd-Warshall Execution time: " << duration.count() << " microseconds\n";
    }

    void MeasuringTimeFloydWarshallCost() {
        auto start = chrono::high_resolution_clock::now();
        runFloydWarshallCost();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Floyd-Warshall Execution time: " << duration.count() << " microseconds\n";
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

    travelSystem.findBalancedRouteAStar("Moscow", "Lisbon", 2.0, 1.0);

    travelSystem.processingThreadFindRoute("Moscow");
    
    cout << "\n-------------Example of Display Dijkstra proccesing---------------------\n";

    travelSystem.proccesingThreadFindCheapestRouteDijkstra("Moscow");
    travelSystem.proccesingThreadFindFastestRouteDijkstra("Moscow");

    cout << "\n-----------------------------End of example-----------------------------\n";

    travelSystem.findCheapestRouteDijkstra("Moscow", "Lisbon");
    travelSystem.findCheapestRouteDijkstra("Warsaw", "London");
    travelSystem.findCheapestRouteDijkstra("Warsaw", "New-York");


    travelSystem.findFastestRouteDijkstra("Moscow", "Lisbon");
    travelSystem.findFastestRouteDijkstra("Warsaw", "London");;
    travelSystem.findFastestRouteDijkstra("Warsaw", "New-York");

    cout << "\n-------------Example of Display Bellman-Ford proccesing---------------------\n";

    travelSystem.proccesingThreadFindCheapestRouteBellmanFord("Moscow");
    travelSystem.proccesingThreadFindFastestRouteBellmanFord("Moscow");

    cout << "\n-----------------------------End of example-----------------------------\n";

    travelSystem.findCheapestRouteBellmanFord("Moscow", "Lisbon");
    travelSystem.findCheapestRouteBellmanFord("Warsaw", "London");
    travelSystem.findCheapestRouteBellmanFord("Warsaw", "New-York");
    
    travelSystem.findFastestRouteBellmanFord("Moscow", "Lisbon");
    travelSystem.findFastestRouteBellmanFord("Warsaw", "London");
    travelSystem.findFastestRouteBellmanFord("Warsaw", "New-York");

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
    travelSystemWithNegativeWeight.findRoute("Moscow", "Lisbon");

    travelSystemWithNegativeWeight.findCheapestRouteBellmanFord("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.findCheapestRouteBellmanFord("Warsaw", "London");
    travelSystemWithNegativeWeight.findCheapestRouteBellmanFord("Warsaw", "New-York");

    travelSystemWithNegativeWeight.findFastestRouteBellmanFord("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.findFastestRouteBellmanFord("Warsaw", "London");
    travelSystemWithNegativeWeight.findFastestRouteBellmanFord("Warsaw", "New-York");

    travelSystem.runFloydWarshallDuration();
    travelSystem.runFloydWarshallCost();

    travelSystem.MeasuringTimeOfAStar("Moscow", "Lisbon", 2.0, 1.0);

    travelSystem.MeasuringTimeDijkstraDuration("Moscow", "Lisbon");
    travelSystem.MeasuringTimeDijkstraDuration("Warsaw", "London");
    travelSystem.MeasuringTimeDijkstraDuration("Warsaw", "New-York");

    travelSystem.MeasuringTimeDijkstraCost("Moscow", "Lisbon");
    travelSystem.MeasuringTimeDijkstraCost("Warsaw", "London");
    travelSystem.MeasuringTimeDijkstraCost("Warsaw", "New-York");

    travelSystem.MeasuringTimeBellmanFordCost("Moscow", "Lisbon");
    travelSystem.MeasuringTimeBellmanFordCost("Warsaw", "London");
    travelSystem.MeasuringTimeBellmanFordCost("Warsaw", "New-York");

    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordCost("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordCost("Warsaw", "London");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordCost("Warsaw", "New-York");

    travelSystem.MeasuringTimeBellmanFordDuration("Moscow", "Lisbon");
    travelSystem.MeasuringTimeBellmanFordDuration("Warsaw", "London");
    travelSystem.MeasuringTimeBellmanFordDuration("Warsaw", "New-York");

    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordDuration("Moscow", "Lisbon");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordDuration("Warsaw", "London");
    travelSystemWithNegativeWeight.MeasuringTimeBellmanFordDuration("Warsaw", "New-York");

    travelSystem.MeasuringTimeFloydWarshallCost();
    travelSystem.MeasuringTimeFloydWarshallDuration();

    return 0;
}