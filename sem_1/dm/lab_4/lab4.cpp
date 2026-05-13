#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cmath>

using namespace std;

// Структура для хранения функции
struct Function {
    string truthTable; // таблица истинности как строка
    int varsCount;     // количество переменных
};

// Классы Поста
class PostClasses {
public:
    bool T0; // сохраняет 0
    bool T1; // сохраняет 1
    bool S;  // самодвойственная
    bool M;  // монотонная
    bool L;  // линейная

    PostClasses() : T0(true), T1(true), S(true), M(true), L(true) {}
};

// Проверка на сохранение 0
bool checkT0(const Function& f) {
    return f.truthTable[0] == '0';
}

// Проверка на сохранение 1
bool checkT1(const Function& f) {
    return f.truthTable[f.truthTable.length() - 1] == '1';
}

// Проверка на самодвойственность
bool checkSelfDual(const Function& f) {
    int n = f.truthTable.length();
    for (int i = 0; i < n; i++) {
        if (f.truthTable[i] == f.truthTable[n - 1 - i]) {
            return false;
        }
    }
    return true;
}

// Проверка на монотонность
bool checkMonotone(const Function& f) {
    int n = f.varsCount;
    int size = 1 << n;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Если i <= j в смысле покрытия
            if ((i & j) == i) { // i покрывается j
                if (f.truthTable[i] == '1' && f.truthTable[j] == '0') {
                    return false;
                }
            }
        }
    }
    return true;
}

// Проверка на линейность (через полином Жегалкина)
bool checkLinear(const Function& f) {
    int n = f.varsCount;
    int size = 1 << n;

    // Преобразуем в вектор int
    vector<int> coeff(size, 0);
    for (int i = 0; i < size; i++) {
        coeff[i] = f.truthTable[i] - '0';
    }

    // Треугольник Паскаля (метод прямоугольника)
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > i; j--) {
            coeff[j] = (coeff[j] + coeff[j - (1 << i)]) % 2;
        }
    }

    // Проверяем, что только коэффициенты с весом <=1 ненулевые
    for (int i = 0; i < size; i++) {
        // Подсчет веса (количество единиц в бинарном представлении)
        int weight = 0;
        int temp = i;
        while (temp) {
            weight += temp & 1;
            temp >>= 1;
        }

        if (coeff[i] == 1 && weight > 1) {
            return false;
        }
    }

    return true;
}

// Ввод функции
Function inputFunction() {
    Function f;
    string table;

    cout << "Введите таблицу истинности функции (только 0 и 1): ";
    cin >> table;

    // Проверка на корректность длины
    int len = table.length();
    bool validLength = false;

    for (int i = 1; i <= 8; i++) {
        if (len == (1 << i)) {
            f.varsCount = i;
            validLength = true;
            break;
        }
    }

    if (!validLength) {
        cout << "Ошибка! Длина должна быть 2, 4, 8, 16, 32, 64, 128 или 256." << endl;
        // Возвращаем функцию по умолчанию
        f.varsCount = 1;
        f.truthTable = "00";
        return f;
    }

    // Проверка на корректность символов
    for (char c : table) {
        if (c != '0' && c != '1') {
            cout << "Ошибка! Можно вводить только 0 и 1." << endl;
            f.varsCount = 1;
            f.truthTable = "00";
            return f;
        }
    }

    f.truthTable = table;
    return f;
}

// Основная функция
int main() {
    setlocale(LC_ALL, "rus");

    vector<Function> functions;
    vector<PostClasses> classes;

    cout << "=== АНАЛИЗ СИСТЕМЫ БУЛЕВЫХ ФУНКЦИЙ ===\n" << endl;
    cout << "Введите количество функций в системе: ";
    int n;
    cin >> n;

    if (n <= 0) {
        cout << "Ошибка! Количество функций должно быть положительным!" << endl;
        return 1;
    }

    // Ввод функций
    for (int i = 0; i < n; i++) {
        cout << "\n--- Функция " << i + 1 << " ---" << endl;
        functions.push_back(inputFunction());
    }

    // Анализ каждой функции
    cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА ===\n" << endl;
    cout << "№ | T0 | T1 |  S |  M |  L " << endl;
    cout << "---------------------------" << endl;

    for (int i = 0; i < n; i++) {
        PostClasses pc;

        pc.T0 = checkT0(functions[i]);
        pc.T1 = checkT1(functions[i]);
        pc.S = checkSelfDual(functions[i]);
        pc.M = checkMonotone(functions[i]);
        pc.L = checkLinear(functions[i]);

        classes.push_back(pc);

        // Вывод таблицы
        cout << i + 1 << " |  "
            << (pc.T0 ? "+" : "-") << " |  "
            << (pc.T1 ? "+" : "-") << " |  "
            << (pc.S ? "+" : "-") << " |  "
            << (pc.M ? "+" : "-") << " |  "
            << (pc.L ? "+" : "-") << endl;
    }

    // Анализ системы
    bool hasT0 = false, hasT1 = false, hasS = false, hasM = false, hasL = false;

    for (int i = 0; i < n; i++) {
        if (!classes[i].T0) hasT0 = true;
        if (!classes[i].T1) hasT1 = true;
        if (!classes[i].S) hasS = true;
        if (!classes[i].M) hasM = true;
        if (!classes[i].L) hasL = true;
    }

    // Проверка на полноту (теорема Поста)
    bool isComplete = (hasT0 && hasT1 && hasS && hasM && hasL);

    // Проверка на базис
    bool isBasis = isComplete;

    // Для базиса нужно, чтобы система была полной, но при удалении любой функции переставала быть полной
    if (isComplete && n > 1) {
        // Проверяем, можно ли удалить какую-то функцию
        for (int removeIndex = 0; removeIndex < n; removeIndex++) {
            bool tempT0 = false, tempT1 = false, tempS = false, tempM = false, tempL = false;

            for (int i = 0; i < n; i++) {
                if (i == removeIndex) continue;

                if (!classes[i].T0) tempT0 = true;
                if (!classes[i].T1) tempT1 = true;
                if (!classes[i].S) tempS = true;
                if (!classes[i].M) tempM = true;
                if (!classes[i].L) tempL = true;
            }

            // Если после удаления одной функции система все еще полная
            if (tempT0 && tempT1 && tempS && tempM && tempL) {
                isBasis = false;
                break;
            }
        }
    }
    else if (!isComplete) {
        isBasis = false;
    }

    // Вывод ответов
    cout << "\n=== ОТВЕТЫ ===" << endl;
    cout << "1. Система " << (isComplete ? "ЯВЛЯЕТСЯ" : "НЕ ЯВЛЯЕТСЯ") << " полной" << endl;
    cout << "2. Система " << (isBasis ? "ЯВЛЯЕТСЯ" : "НЕ ЯВЛЯЕТСЯ") << " базисом" << endl;

    // Дополнительная информация
    if (!isComplete) {
        cout << "\nПричина: система не содержит функции, не принадлежащей следующим классам:" << endl;
        if (!hasT0) cout << "- Не сохраняющих константу 0" << endl;
        if (!hasT1) cout << "- Не сохраняющих константу 1" << endl;
        if (!hasS) cout << "- Несамодвойственных" << endl;
        if (!hasM) cout << "- Немонотонных" << endl;
        if (!hasL) cout << "- Нелинейных" << endl;
    }

    return 0;
}