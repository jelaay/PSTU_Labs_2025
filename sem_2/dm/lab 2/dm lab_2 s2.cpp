#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const int V = 10;    // Размерность матрицы 10х10
const int INF = 1e9; // Условная бесконечность для отсутствующих путей

void solvePrim(const vector<vector<int>>& graph) {
    // Массив для отслеживания посещенных вершин
    vector<bool> visited(V, false);

    // Начинаем с первой вершины (индекс 0)
    visited[0] = true;

    int totalWeight = 0;
    int edgesCount = 0;

    cout << "\n2) Ребра остовного дерева:" << endl;
    cout << "---------------------------" << endl;
    cout << "  Ребро    |  Вес" << endl;
    cout << "---------------------------" << endl;

    // Остовное дерево в графе из V вершин всегда имеет ровно V-1 ребро
    while (edgesCount < V - 1) {
        int minWeight = INF;
        int startV = -1; // Откуда идем (из дерева)
        int endV = -1;   // Куда идем (во вне)

        // Проходим по всем вершинам, которые уже в дереве
        for (int i = 0; i < V; i++) {
            if (visited[i]) {
                // Ищем самое легкое ребро, ведущее к непосещенной вершине
                for (int j = 0; j < V; j++) {
                    // Условия: вершина j не в дереве, связь существует (не 0)
                    if (!visited[j] && graph[i][j] > 0) {
                        if (graph[i][j] < minWeight) {
                            minWeight = graph[i][j];
                            startV = i;
                            endV = j;
                        }
                    }
                }
            }
        }

        // Если нашли подходящее ребро
        if (endV != -1) {
            visited[endV] = true; // Добавляем новую вершину в "освоенные"
            totalWeight += minWeight;
            edgesCount++;

            // Вывод ребра (прибавляем 1 к индексам для красоты 1-10)
            cout << setw(2) << (startV + 1) << " <-> " << setw(2) << (endV + 1)
                << " |  " << minWeight << endl;
        }
    }

    cout << "---------------------------" << endl;
    cout << "3) Суммарный вес дерева: " << totalWeight << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<vector<int>> graph(V, vector<int>(V));
    ifstream file("matrix.txt");

    if (!file.is_open()) {
        cout << "Файл matrix.txt не найден!" << endl;
        return 1;
    }

    // 1) Чтение и вывод матрицы
    cout << "1) Исходная матрица расстояний:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            file >> graph[i][j];
            cout << setw(3) << graph[i][j] << " ";
        }
        cout << endl;
    }
    file.close();

    // Запуск алгоритма
    solvePrim(graph);

    return 0;
}