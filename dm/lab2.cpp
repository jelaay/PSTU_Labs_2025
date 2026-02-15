#include <iostream>
#include <vector>

using namespace std;

//рефлексивностm
bool isReflexive(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        if (matrix[i][i] != 1) {
            return false;
        }
    }
    return true;
}

//антирефлексивность
bool isAntiReflexive(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        if (matrix[i][i] != 0) {
            return false;
        }
    }
    return true;
}

//симметричность
bool isSymmetric(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

//асимметричность
bool isAsymmetric(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (matrix[i][j] == 1 && matrix[j][i] == 1 && i != j) {
                return false;
            }
            if (matrix[i][j] == 1 && matrix[j][i] == 1) {
                return false;
            }
        }
    }
    return true;
}

//антисимметричность
bool isAntiSymmetric(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (i != j && matrix[i][j] == 1 && matrix[j][i] == 1) {
                return false;
            }
        }
    }
    return true;
}

//транзитивность
bool isTransitive(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (matrix[i][j] == 1 && matrix[j][k] == 1 && matrix[i][k] != 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

//связность
bool isConnected(const vector<vector<int>>& matrix) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (i != j && matrix[i][j] == 0 && matrix[j][i] == 0) {
                return false;
            }
        }
    }
    return true;
}

//ввод матрицы
void inputMatrix(vector<vector<int>>& matrix) {
    cout << "Введите матрицу 6x6 (0 или 1):" << endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> matrix[i][j];
        }
    }
}

//вывод матрицы
void printMatrix(const vector<vector<int>>& matrix) {
    cout << "Введенная матрица:" << endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    vector<vector<int>> matrix(6, vector<int>(6));

    inputMatrix(matrix);
    printMatrix(matrix);

    cout << "\nСвойства отношения:" << endl;

    bool reflexive = isReflexive(matrix);
    bool antiReflexive = isAntiReflexive(matrix);
    bool symmetric = isSymmetric(matrix);
    bool asymmetric = isAsymmetric(matrix);
    bool antiSymmetric = isAntiSymmetric(matrix);
    bool transitive = isTransitive(matrix);
    bool connected = isConnected(matrix);

    // Выводим все свойства с указанием, обладает ли отношение ими
    cout << "Рефлексивность: " << (reflexive ? "Да" : "Нет") << endl;
    cout << "Антирефлексивность: " << (antiReflexive ? "Да" : "Нет") << endl;
    cout << "Симметричность: " << (symmetric ? "Да" : "Нет") << endl;
    cout << "Асимметричность: " << (asymmetric ? "Да" : "Нет") << endl;
    cout << "Антисимметричность: " << (antiSymmetric ? "Да" : "Нет") << endl;
    cout << "Транзитивность: " << (transitive ? "Да" : "Нет") << endl;
    cout << "Связность: " << (connected ? "Да" : "Нет") << endl;

    /*
    cout << "\nОтношение обладает свойствами: ";
    bool hasProperties = false;
    if (reflexive) { cout << "рефлексивность "; hasProperties = true; }
    if (antiReflexive) { cout << "антирефлексивность "; hasProperties = true; }
    if (symmetric) { cout << "симметричность "; hasProperties = true; }
    if (asymmetric) { cout << "асимметричность "; hasProperties = true; }
    if (antiSymmetric) { cout << "антисимметричность "; hasProperties = true; }
    if (transitive) { cout << "транзитивность "; hasProperties = true; }
    if (connected) { cout << "связность "; hasProperties = true; }
    if (!hasProperties) cout << "нет особых свойств";
    cout << endl;
    */

    // Проверка на отношение эквивалентности
    if (reflexive && symmetric && transitive) {
        cout << "\nОтношение эквивалентности" << endl;
    }

    // Проверка на отношение порядка
    if (reflexive && antiSymmetric && transitive) {
        cout << "Отношение порядка" << endl;
        if (connected) {
            cout << "Отношение линейного порядка" << endl;
        }
        else {
            cout << "Отношение частичного порядка" << endl;
        }
    }

    // Проверка на отношение строгого порядка
    if (antiReflexive && antiSymmetric && transitive) {
        cout << "Отношение строгого порядка" << endl;
        if (connected) {
            cout << "Отношение строгого линейного порядка" << endl;
        }
    }

    return 0;
}