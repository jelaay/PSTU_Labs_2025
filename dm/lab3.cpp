#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>

using namespace std;

// Глобальные переменные (как делают начинающие)
vector<string> table;
vector<string> minterms;
vector<string> prime_implicants;

// Функция для перевода двоичного в десятичное
int bin_to_dec(string bin) {
    int result = 0;
    int power = 1;
    for (int i = bin.length() - 1; i >= 0; i--) {
        if (bin[i] == '1') {
            result += power;
        }
        power *= 2;
    }
    return result;
}

// Функция для подсчета единиц в строке
int count_ones(string s) {
    int count = 0;
    for (char c : s) {
        if (c == '1') count++;
    }
    return count;
}

// Проверка можно ли склеить две строки
bool can_combine(string a, string b) {
    int diff_count = 0;
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) diff_count++;
    }
    return diff_count == 1;
}

// Склеивание двух строк
string combine(string a, string b) {
    string result = "";
    for (int i = 0; i < a.length(); i++) {
        if (a[i] == b[i]) {
            result += a[i];
        }
        else {
            result += "-";
        }
    }
    return result;
}

// Вывод таблицы истинности
void print_truth_table(vector<int> func) {
    cout << "\nТаблица истинности:" << endl;
    cout << "x3 x2 x1 x0 | f" << endl;
    cout << "------------|---" << endl;

    for (int i = 0; i < 16; i++) {
        // Переводим i в двоичное
        int temp = i;
        string bin = "";
        for (int j = 0; j < 4; j++) {
            bin = to_string(temp % 2) + bin;
            temp /= 2;
        }

        cout << bin[0] << "  " << bin[1] << "  " << bin[2] << "  " << bin[3] << "  | ";
        cout << func[i] << endl;
    }
}

// Нахождение СДНФ
string find_sdnf(vector<int> func) {
    string sdnf = "СДНФ: F = ";
    bool first = true;

    for (int i = 0; i < 16; i++) {
        if (func[i] == 1) {
            if (!first) sdnf += " + ";
            first = false;

            // Переводим i в двоичное
            int temp = i;
            string bin = "";
            for (int j = 0; j < 4; j++) {
                bin = to_string(temp % 2) + bin;
                temp /= 2;
            }

            // Добавляем конъюнкцию
            sdnf += "(";
            if (bin[0] == '0') sdnf += "!x3";
            else sdnf += "x3";

            sdnf += " & ";
            if (bin[1] == '0') sdnf += "!x2";
            else sdnf += "x2";

            sdnf += " & ";
            if (bin[2] == '0') sdnf += "!x1";
            else sdnf += "x1";

            sdnf += " & ";
            if (bin[3] == '0') sdnf += "!x0";
            else sdnf += "x0";

            sdnf += ")";
        }
    }

    if (first) sdnf += "0";
    return sdnf;
}

// Алгоритм Квайна-МакКласки
void quine_mccluskey(vector<int> func) {
    // Шаг 1: Собираем минтермы
    vector<vector<string>> groups(5); // Максимум 4 единицы может быть

    for (int i = 0; i < 16; i++) {
        if (func[i] == 1) {
            // Переводим в двоичное
            int temp = i;
            string bin = "";
            for (int j = 0; j < 4; j++) {
                bin = to_string(temp % 2) + bin;
                temp /= 2;
            }

            minterms.push_back(bin);
            int ones = count_ones(bin);
            groups[ones].push_back(bin);
        }
    }

    cout << "\nЭтап 1: Группировка по количеству единиц" << endl;
    for (int i = 0; i < 5; i++) {
        if (!groups[i].empty()) {
            cout << "Группа " << i << " единиц: ";
            for (string s : groups[i]) {
                cout << s << " ";
            }
            cout << endl;
        }
    }

    // Склеивание
    cout << "\nЭтапы склеивания:" << endl;
    vector<string> current = minterms;
    vector<string> next;
    vector<pair<string, string>> combinations;
    int stage = 1;

    while (true) {
        cout << "\nСклеивание " << stage++ << ":" << endl;
        next.clear();
        combinations.clear();

        vector<bool> used(current.size(), false);

        // Пробуем склеить все комбинации
        for (int i = 0; i < current.size(); i++) {
            for (int j = i + 1; j < current.size(); j++) {
                if (can_combine(current[i], current[j])) {
                    string combined = combine(current[i], current[j]);
                    next.push_back(combined);
                    combinations.push_back({ current[i], current[j] });
                    used[i] = true;
                    used[j] = true;
                    cout << current[i] << " + " << current[j] << " = " << combined << endl;
                }
            }
        }

        // Добавляем непокрытые
        for (int i = 0; i < current.size(); i++) {
            if (!used[i]) {
                prime_implicants.push_back(current[i]);
            }
        }

        // Удаляем дубликаты
        sort(next.begin(), next.end());
        next.erase(unique(next.begin(), next.end()), next.end());

        if (next.empty()) break;

        current = next;
    }

    // Добавляем оставшиеся
    for (string s : current) {
        prime_implicants.push_back(s);
    }

    // Удаляем дубликаты
    sort(prime_implicants.begin(), prime_implicants.end());
    prime_implicants.erase(unique(prime_implicants.begin(), prime_implicants.end()), prime_implicants.end());

    cout << "\nПростые импликанты: ";
    for (string s : prime_implicants) {
        cout << s << " ";
    }
    cout << endl;
}

// Импликантная матрица
void print_implicant_matrix(vector<int> func) {
    cout << "\nИмпликантная матрица:" << endl;
    cout << "       ";

    // Заголовки столбцов
    vector<int> ones_positions;
    for (int i = 0; i < 16; i++) {
        if (func[i] == 1) {
            cout << i << " ";
            ones_positions.push_back(i);
        }
    }
    cout << endl;

    // Проверяем покрытие
    for (string implicant : prime_implicants) {
        cout << implicant << " | ";

        for (int pos : ones_positions) {
            // Переводим pos в двоичное
            int temp = pos;
            string bin = "";
            for (int j = 0; j < 4; j++) {
                bin = to_string(temp % 2) + bin;
                temp /= 2;
            }

            // Проверяем покрывает ли импликант эту позицию
            bool covers = true;
            for (int k = 0; k < 4; k++) {
                if (implicant[k] != '-' && implicant[k] != bin[k]) {
                    covers = false;
                    break;
                }
            }

            if (covers) cout << "X  ";
            else cout << "   ";
        }
        cout << endl;
    }
}

// Нахождение МДНФ (простой алгоритм)
string find_mdnf(vector<int> func) {
    // Простой подход: берем все простые импликанты
    string mdnf = "МДНФ: F = ";
    bool first_term = true;

    for (string imp : prime_implicants) {
        if (!first_term) mdnf += " + ";
        first_term = false;

        mdnf += "(";
        if (imp[0] == '0') mdnf += "!x3";
        else if (imp[0] == '1') mdnf += "x3";

        if (imp[1] != '-') {
            mdnf += " & ";
            if (imp[1] == '0') mdnf += "!x2";
            else mdnf += "x2";
        }

        if (imp[2] != '-') {
            mdnf += " & ";
            if (imp[2] == '0') mdnf += "!x1";
            else mdnf += "x1";
        }

        if (imp[3] != '-') {
            mdnf += " & ";
            if (imp[3] == '0') mdnf += "!x0";
            else mdnf += "x0";
        }

        mdnf += ")";
    }

    if (first_term) mdnf += "0";

    return mdnf;
}

int main() {
    setlocale(LC_CTYPE, "rus");
    vector<int> func(16);

    cout << "Введите вектор функции из 16 символов (0 или 1):" << endl;
    string input;
    cin >> input;

    // Проверка ввода
    if (input.length() != 16) {
        cout << "Ошибка! Нужно ровно 16 символов!" << endl;
        return 1;
    }

    for (int i = 0; i < 16; i++) {
        if (input[i] == '0') func[i] = 0;
        else if (input[i] == '1') func[i] = 1;
        else {
            cout << "Ошибка! Можно вводить только 0 или 1!" << endl;
            return 1;
        }
    }

    // Выполняем все пункты
    print_truth_table(func);

    string sdnf = find_sdnf(func);
    cout << "\n" << sdnf << endl;

    quine_mccluskey(func);
    print_implicant_matrix(func);

    string mdnf = find_mdnf(func);
    cout << "\n" << mdnf << endl;

    return 0;
}