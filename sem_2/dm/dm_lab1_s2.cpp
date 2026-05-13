#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int N = 10; // Размер матрицы

// Алгоритм обхода графа в глубину (DFS)
void dfs(int v, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v); // Добавляем текущую вершину в текущую компоненту связности
    for (int i = 0; i < N; ++i) {
        // Если есть ребро и вершина еще не посещена
        if (adj[v][i] == 1 && !visited[i]) {
            dfs(i, adj, visited, component);
        }
    }
}

int main() {
    // Устанавливаем русский язык в консоли (для Windows)
    setlocale(LC_ALL, "Russian");

    vector<vector<int>> adj(N, vector<int>(N));
    ifstream in("matrix.txt");

    if (!in.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл matrix.txt" << endl;
        cerr << "Убедитесь, что файл существует в той же папке, что и программа." << endl;
        return 1;
    }

    // 1) Считывание матрицы 10х10
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            in >> adj[i][j];
        }
    }
    in.close();

    // 2) Вывод считанной матрицы
    cout << "--- Исходная матрица смежности ---" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // *Проверка на ориентированность и преобразование в неориентированный граф
    bool isDirected = false;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (adj[i][j] != adj[j][i]) {
                isDirected = true;
                // Доопределяем ребра, чтобы граф стал симметричным (неориентированным)
                adj[i][j] = 1;
                adj[j][i] = 1;
            }
        }
    }

    if (isDirected) {
        cout << "[!] Граф был ориентированным. Преобразован в неориентированный (добавлены обратные ребра)." << endl;
    }
    else {
        cout << "[i] Граф изначально является неориентированным." << endl;
    }
    cout << endl;

    // 3) Поиск компонент связности
    vector<bool> visited(N, false);
    vector<vector<int>> components;

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            vector<int> component;
            dfs(i, adj, visited, component);
            components.push_back(component);
        }
    }

    cout << "--- Компоненты связности ---" << endl;
    cout << "Количество компонент связности: " << components.size() << endl;
    for (size_t k = 0; k < components.size(); ++k) {
        cout << "Компонента " << k + 1 << " включает вершины (нумерация с 0): ";
        for (int v : components[k]) {
            cout << v << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 4) Построение матрицы достижимости
    // В неориентированном графе вершина j достижима из i, 
    // если они принадлежат одной компоненте связности.
    vector<vector<int>> reach(N, vector<int>(N, 0));
    for (const auto& comp : components) {
        for (int u : comp) {
            for (int v : comp) {
                reach[u][v] = 1;
            }
        }
    }

    // Вывод матрицы достижимости
    cout << "--- Матрица достижимости ---" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << reach[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}