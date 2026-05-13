#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

const int SIZE = 6;
const long long INF = 1e15; // Достаточно большое число для "бесконечности"

typedef vector<vector<long long>> Matrix;

// Функция для вывода матрицы
void printMatrix(const Matrix& mat) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (mat[i][j] == INF || mat[i][j] == -INF)
                cout << setw(5) << "0"; // Выводим 0, если пути нет
            else
                cout << setw(5) << mat[i][j];
        }
        cout << endl;
    }
}

// "Умножение" матриц по Шимбеллу
Matrix shimbelMultiply(const Matrix& A, const Matrix& B, bool findMax) {
    Matrix C(SIZE, vector<long long>(SIZE, findMax ? -INF : INF));

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            for (int k = 0; k < SIZE; ++k) {
                // Проверяем, существуют ли оба участка пути
                if (A[i][k] != (findMax ? -INF : INF) && B[k][j] != (findMax ? -INF : INF)) {
                    long long currentPath = A[i][k] + B[k][j];
                    if (findMax) {
                        C[i][j] = max(C[i][j], currentPath);
                    }
                    else {
                        C[i][j] = min(C[i][j], currentPath);
                    }
                }
            }
        }
    }
    return C;
}

int main() {
    setlocale(LC_ALL, "Russian");

    string filename = "matrix.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return 1;
    }

    Matrix initialMatrix(SIZE, vector<long long>(SIZE));
    int mode;
    int steps;

    cout << "Выберите направление поиска (1 - Минимум, 2 - Максимум): ";
    cin >> mode;
    bool findMax = (mode == 2);

    cout << "Введите количество переходов (k): ";
    cin >> steps;

    // Считываем матрицу и заменяем 0 на INF/-INF
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            file >> initialMatrix[i][j];
            if (initialMatrix[i][j] == 0) {
                initialMatrix[i][j] = (findMax ? -INF : INF);
            }
        }
    }
    file.close();

    // Возведение в степень k
    Matrix result = initialMatrix;
    // Если k=1, оставляем как есть. Если больше — перемножаем.
    for (int s = 1; s < steps; ++s) {
        result = shimbelMultiply(result, initialMatrix, findMax);
    }

    cout << "\nРезультат (пути длиной " << steps << " через операции Шимбелла):\n";
    printMatrix(result);

    return 0;
}