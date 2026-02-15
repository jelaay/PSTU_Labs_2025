#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>

using namespace std;

const int MIN_VAL = -30;
const int MAX_VAL = 30;

class NumberSet {
private:
    vector<int> elements;
    string name;

public:
    NumberSet(const string& setName = "") : name(setName) {}

    void addElement(int num) {
        if (isValidNumber(num) && !contains(num)) {
            elements.push_back(num);
        }
    }

    bool isValidNumber(int num) const {
        return num >= MIN_VAL && num <= MAX_VAL;
    }

    bool contains(int num) const {
        for (int elem : elements) {
            if (elem == num) {
                return true;
            }
        }
        return false;
    }

    void clear() {
        elements.clear();
    }

    int size() const {
        return elements.size();
    }

    bool empty() const {
        return elements.empty();
    }

    vector<int> getElements() const {
        return elements;
    }

    void setName(const string& setName) {
        name = setName;
    }

    string getName() const {
        return name;
    }

    void print() const {
        cout << name << " = {";
        if (elements.empty()) {
            cout << "}" << endl;
            return;
        }

        for (int i = 0; i < elements.size(); i++) {
            cout << elements[i];
            if (i < elements.size() - 1) {
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }

    void inputManual() {
        cout << "\n--- Ручной ввод множества " << name << " ---" << endl;
        cout << "Вводите целые числа от " << MIN_VAL << " до " << MAX_VAL << endl;
        cout << "Для завершения ввода введите число вне диапазона" << endl;

        clear();
        int count = 1;

        while (true) {
            cout << "Элемент " << count << ": ";
            int num;
            cin >> num;

            if (!isValidNumber(num)) {
                cout << "Завершение ввода." << endl;
                break;
            }

            if (contains(num)) {
                cout << "Это число уже есть в множестве!" << endl;
            }
            else {
                addElement(num);
                cout << "Добавлено: " << num << endl;
                count++;
            }
        }
    }

    void inputRandom() {
        cout << "\n--- Случайное заполнение множества " << name << " ---" << endl;

        int count;
        while (true) {
            cout << "Сколько элементов сгенерировать? (0-" << (MAX_VAL - MIN_VAL + 1) << "): ";
            cin >> count;

            if (count >= 0 && count <= (MAX_VAL - MIN_VAL + 1)) {
                break;
            }
            cout << "Неверное количество! Попробуйте снова." << endl;
        }

        clear();
        srand(time(0));

        int generated = 0;
        while (generated < count) {
            int num = MIN_VAL + rand() % (MAX_VAL - MIN_VAL + 1);

            if (!contains(num)) {
                addElement(num);
                generated++;
            }

            if (size() == (MAX_VAL - MIN_VAL + 1)) {
                break;
            }
        }

        cout << "Сгенерировано " << size() << " случайных элементов" << endl;
    }

    void inputByConditions() {
        cout << "\n--- Задание множества " << name << " по условиям ---" << endl;

        cout << "\nУсловие по знаку:" << endl;
        cout << "1 - положительные (>0)" << endl;
        cout << "2 - отрицательные (<0)" << endl;
        cout << "3 - неотрицательные (>=0)" << endl;
        cout << "4 - неположительные (<=0)" << endl;
        cout << "5 - любые" << endl;
        cout << "Выберите: ";

        int signChoice;
        cin >> signChoice;

        cout << "\nУсловие по четности:" << endl;
        cout << "1 - четные" << endl;
        cout << "2 - нечетные" << endl;
        cout << "3 - любые" << endl;
        cout << "Выберите: ";

        int parityChoice;
        cin >> parityChoice;

        cout << "\nКратность (введите число, 0 - без условия): ";
        int multiple;
        cin >> multiple;

        clear();

        for (int num = MIN_VAL; num <= MAX_VAL; num++) {
            bool satisfiesSign = false;
            bool satisfiesParity = false;
            bool satisfiesMultiple = false;

            switch (signChoice) {
            case 1: satisfiesSign = (num > 0); break;
            case 2: satisfiesSign = (num < 0); break;
            case 3: satisfiesSign = (num >= 0); break;
            case 4: satisfiesSign = (num <= 0); break;
            case 5: satisfiesSign = true; break;
            default: satisfiesSign = true;
            }

            switch (parityChoice) {
            case 1: satisfiesParity = (num % 2 == 0); break;
            case 2: satisfiesParity = (num % 2 != 0); break;
            case 3: satisfiesParity = true; break;
            default: satisfiesParity = true;
            }

            if (multiple == 0) {
                satisfiesMultiple = true;
            }
            else {
                satisfiesMultiple = (num % multiple == 0);
            }

            if (satisfiesSign && satisfiesParity && satisfiesMultiple) {
                addElement(num);
            }
        }

        cout << "По заданным условиям найдено " << size() << " элементов" << endl;
    }
};

class SetCalculator {
private:
    NumberSet sets[3];
    NumberSet universalSet;

    const char UNION_SYMBOL = 'u';
    const char INTERSECTION_SYMBOL = 'n';
    const char DIFFERENCE_SYMBOL = '-';
    const char SYM_DIFFERENCE_SYMBOL = 's';

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    NumberSet unionOperation(const NumberSet& set1, const NumberSet& set2) { //объединение
        NumberSet result;
        vector<int> elements1 = set1.getElements();
        vector<int> elements2 = set2.getElements();

        for (int num : elements1) {
            result.addElement(num);
        }

        for (int num : elements2) {
            if (!set1.contains(num)) {
                result.addElement(num);
            }
        }

        return result;
    }

    NumberSet intersectionOperation(const NumberSet& set1, const NumberSet& set2) {//пересечение
        NumberSet result;
        vector<int> elements1 = set1.getElements();

        for (int num : elements1) {
            if (set2.contains(num)) {
                result.addElement(num);
            }
        }

        return result;
    }

    NumberSet differenceOperation(const NumberSet& set1, const NumberSet& set2) {//пазность
        NumberSet result;
        vector<int> elements1 = set1.getElements();

        for (int num : elements1) {
            if (!set2.contains(num)) {
                result.addElement(num);
            }
        }

        return result;
    }

    NumberSet symmetricDifferenceOperation(const NumberSet& set1, const NumberSet& set2) {//симметрическая разность
        NumberSet diff1 = differenceOperation(set1, set2);
        NumberSet diff2 = differenceOperation(set2, set1);
        return unionOperation(diff1, diff2);
    }

    NumberSet complementOperation(const NumberSet& set) {//дополнение
        NumberSet result;
        for (int num = MIN_VAL; num <= MAX_VAL; num++) {
            if (!set.contains(num)) {
                result.addElement(num);
            }
        }
        return result;
    }

    // Методы для работы с формулами
    NumberSet evaluateFormula(const string& formula) {
        size_t pos = 0;
        string cleanedFormula;
        for (char c : formula) {
            if (c != ' ') cleanedFormula += c;
        }
        return parseExpression(cleanedFormula, pos);
    }

    NumberSet parseExpression(const string& expr, size_t& pos) {
        NumberSet result = parseTerm(expr, pos);

        while (pos < expr.size()) {
            char op = expr[pos];
            if (op == UNION_SYMBOL || op == SYM_DIFFERENCE_SYMBOL) {
                pos++;
                NumberSet right = parseTerm(expr, pos);

                if (op == UNION_SYMBOL) {
                    result = unionOperation(result, right);
                }
                else {
                    result = symmetricDifferenceOperation(result, right);
                }
            }
            else {
                break;
            }
        }
        return result;
    }

    NumberSet parseTerm(const string& expr, size_t& pos) {
        NumberSet result = parseFactor(expr, pos);

        while (pos < expr.size()) {
            char op = expr[pos];
            if (op == INTERSECTION_SYMBOL || op == DIFFERENCE_SYMBOL) {
                pos++;
                NumberSet right = parseFactor(expr, pos);

                if (op == INTERSECTION_SYMBOL) {
                    result = intersectionOperation(result, right);
                }
                else {
                    result = differenceOperation(result, right);
                }
            }
            else {
                break;
            }
        }
        return result;
    }

    NumberSet parseFactor(const string& expr, size_t& pos) {
        if (pos >= expr.size()) {
            return NumberSet();
        }

        if (expr[pos] == '(') {
            pos++;
            NumberSet result = parseExpression(expr, pos);
            if (pos < expr.size() && expr[pos] == ')') {
                pos++;
            }
            else {
                cout << "Ошибка: отсутствует закрывающая скобка" << endl;
            }
            return result;
        }
        else if (isdigit(expr[pos])) {
            int setIndex = expr[pos] - '1';
            pos++;

            if (setIndex >= 0 && setIndex < 3) {
                return sets[setIndex];
            }
            else {
                cout << "Ошибка: недопустимый номер множества" << endl;
                return NumberSet();
            }
        }
        else {
            cout << "Ошибка: недопустимый символ '" << expr[pos] << "'" << endl;
            pos++;
            return NumberSet();
        }
    }

    void inputSet(int index) {
        if (index < 0 || index > 2) return;

        cout << "\n--- Задание множества " << sets[index].getName() << " ---" << endl;
        cout << "1 - ручной ввод" << endl;
        cout << "2 - случайное заполнение" << endl;
        cout << "3 - по условиям" << endl;
        cout << "Выберите способ: ";

        int method;
        cin >> method;
        clearInputBuffer();

        switch (method) {
        case 1: sets[index].inputManual(); break;
        case 2: sets[index].inputRandom(); break;
        case 3: sets[index].inputByConditions(); break;
        default:
            cout << "Неверный выбор, используется ручной ввод" << endl;
            sets[index].inputManual();
        }
    }

    void handleSetInput() {
        cout << "\n--- ЗАДАНИЕ МНОЖЕСТВ ---" << endl;
        cout << "1 - задать множество A" << endl;
        cout << "2 - задать множество B" << endl;
        cout << "3 - задать множество C" << endl;
        cout << "4 - задать все множества" << endl;
        cout << "Выберите: ";

        int setChoice;
        cin >> setChoice;
        clearInputBuffer();

        if (setChoice >= 1 && setChoice <= 3) {
            inputSet(setChoice - 1);
        }
        else if (setChoice == 4) {
            for (int i = 0; i < 3; i++) {
                inputSet(i);
            }
        }
        else {
            cout << "Неверный выбор!" << endl;
        }
    }

    void handleUnion() {
        cout << "Выберите множества для объединения (например: 12 для A u B): ";
        string setsChoice;
        cin >> setsChoice;
        clearInputBuffer();

        if (setsChoice.empty()) return;

        NumberSet result;
        bool firstSet = true;

        for (char c : setsChoice) {
            int idx = c - '1';
            if (idx >= 0 && idx < 3) {
                if (firstSet) {
                    result = sets[idx];
                    firstSet = false;
                }
                else {
                    result = unionOperation(result, sets[idx]);
                }
            }
        }

        result.setName("Объединение");
        cout << "\nРезультат: ";
        result.print();
    }

    void handleIntersection() {
        cout << "Выберите множества для пересечения (например: 12 для A n B): ";
        string setsChoice;
        cin >> setsChoice;
        clearInputBuffer();

        if (setsChoice.empty()) return;

        NumberSet result = sets[setsChoice[0] - '1'];
        for (int i = 1; i < setsChoice.length(); i++) {
            int idx = setsChoice[i] - '1';
            if (idx >= 0 && idx < 3) {
                result = intersectionOperation(result, sets[idx]);
            }
        }

        result.setName("Пересечение");
        cout << "\nРезультат: ";
        result.print();
    }

    void handleDifference() {
        cout << "Введите разность в формате 12 для A - B: ";
        string diff;
        cin >> diff;
        clearInputBuffer();

        if (diff.length() == 2) {
            int set1 = diff[0] - '1';
            int set2 = diff[1] - '1';

            if (set1 >= 0 && set1 < 3 && set2 >= 0 && set2 < 3) {
                NumberSet result = differenceOperation(sets[set1], sets[set2]);
                result.setName("Разность " + sets[set1].getName() + " - " + sets[set2].getName());
                cout << "\nРезультат: ";
                result.print();
            }
        }
    }

    void handleSymmetricDifference() {
        cout << "Выберите два множества (например: 12 для A s B): ";
        string setsChoice;
        cin >> setsChoice;
        clearInputBuffer();

        if (setsChoice.length() == 2) {
            int set1 = setsChoice[0] - '1';
            int set2 = setsChoice[1] - '1';

            if (set1 >= 0 && set1 < 3 && set2 >= 0 && set2 < 3) {
                NumberSet result = symmetricDifferenceOperation(sets[set1], sets[set2]);
                result.setName("Симметрическая разность " + sets[set1].getName() + " s " + sets[set2].getName());
                cout << "\nРезультат: ";
                result.print();
            }
        }
    }

    void handleComplement() {
        cout << "Выберите множество для дополнения (1-A, 2-B, 3-C): ";
        int setChoice;
        cin >> setChoice;
        clearInputBuffer();

        if (setChoice >= 1 && setChoice <= 3) {
            NumberSet result = complementOperation(sets[setChoice - 1]);
            result.setName("Дополнение " + sets[setChoice - 1].getName());
            cout << "\nРезультат: ";
            result.print();
        }
    }

    void handleFormula() {
        cout << "\n--- ВЫЧИСЛЕНИЕ ФОРМУЛЫ ---" << endl;
        cout << "Доступные множества: 1-A, 2-B, 3-C" << endl;
        cout << "Операции: " << endl;
        cout << "  u (объединение) - A u B" << endl;
        cout << "  n (пересечение) - A n B" << endl;
        cout << "  - (разность)    - A - B" << endl;
        cout << "  s (сим. разность) - A s B" << endl;
        cout << "Можно использовать скобки ()" << endl;
        cout << endl;
        cout << "Примеры:" << endl;
        cout << "  1u2          = A u B" << endl;
        cout << "  (1u2)n3      = (A u B) n C" << endl;
        cout << "  (1-2)s3      = (A - B) s C" << endl;
        cout << "  1n(2u3)      = A n (B u C)" << endl;
        cout << "  1u2u3        = A u B u C" << endl;
        cout << endl;

        while (true) {
            cout << "Введите формулу (или 'back' для возврата): ";
            string formula;
            getline(cin, formula);

            if (formula == "back" || formula == "назад") {
                return;
            }

            if (formula.empty()) {
                cout << "Формула не может быть пустой!" << endl;
                continue;
            }

            bool valid = true;
            for (char c : formula) {
                if (c != ' ' && c != '1' && c != '2' && c != '3' &&
                    c != 'u' && c != 'n' && c != '-' && c != 's' &&
                    c != '(' && c != ')') {
                    cout << "Недопустимый символ: '" << c << "'" << endl;
                    valid = false;
                    break;
                }
            }

            if (!valid) continue;

            try {
                NumberSet result = evaluateFormula(formula);
                result.setName("Результат формулы: " + formula);
                cout << "\n Результат: ";
                result.print();
                cout << endl;
            }
            catch (const exception& e) {
                cout << "Ошибка при вычислении формулы: " << e.what() << endl;
            }
        }
    }

    void handleOperations() {
        cout << "\n--- ОПЕРАЦИИ НАД МНОЖЕСТВАМИ ---" << endl;
        cout << "1 - Объединение" << endl;
        cout << "2 - Пересечение" << endl;
        cout << "3 - Разность" << endl;
        cout << "4 - Симметрическая разность" << endl;
        cout << "5 - Дополнение" << endl;
        cout << "6 - Вычисление формулы" << endl;
        cout << "Выберите операцию: ";

        int operation;
        cin >> operation;
        clearInputBuffer();

        switch (operation) {
        case 1: handleUnion(); break;
        case 2: handleIntersection(); break;
        case 3: handleDifference(); break;
        case 4: handleSymmetricDifference(); break;
        case 5: handleComplement(); break;
        case 6: handleFormula(); break;
        default: cout << "Неверная операция!" << endl;
        }
    }

public:
    SetCalculator() {
        sets[0].setName("A");
        sets[1].setName("B");
        sets[2].setName("C");
        universalSet.setName("Универсум");

        for (int i = MIN_VAL; i <= MAX_VAL; i++) {
            universalSet.addElement(i);
        }
    }

    void showAllSets() {
        cout << "\n--- ТЕКУЩИЕ МНОЖЕСТВА ---" << endl;
        universalSet.print();
        for (int i = 0; i < 3; i++) {
            sets[i].print();
        }
    }

    void run() {
        cout << "=== КАЛЬКУЛЯТОР МНОЖЕСТВ ===" << endl;
        cout << "Универсум: целые числа от " << MIN_VAL << " до " << MAX_VAL << endl;

        while (true) {
            cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << endl;
            cout << "1 - Задать множества" << endl;
            cout << "2 - Показать множества" << endl;
            cout << "3 - Операции над множествами" << endl;
            cout << "4 - Выход" << endl;
            cout << "Выберите действие: ";

            int choice;
            cin >> choice;
            clearInputBuffer();

            switch (choice) {
            case 1: handleSetInput(); break;
            case 2: showAllSets(); break;
            case 3: handleOperations(); break;
            case 4:
                cout << "До свидания!" << endl;
                return;
            default:
                cout << "Неверный выбор! Попробуйте снова." << endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    SetCalculator calculator;
    calculator.run();
    return 0;
}