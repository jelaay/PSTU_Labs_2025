#include <iostream>
#include <string>


using namespace std;

bool isConsonant(char ch) {
    return (ch == 'b' || ch == 'c' || ch == 'd');
}

bool isVowel(char ch) {
    return (ch == 'a' || ch == 'e');
}

bool validateWord(const std::string& word) {
    if (word.empty()) return false; 

    int state = 0; 

    for (char ch : word) {

        if (!isConsonant(ch) && !isVowel(ch)) {
            return false;
        }

        switch (state) {
        case 0: 
            if (isConsonant(ch)) state = 1; else state = 2;
            break;
        case 1: 
        case 2:
            if (isConsonant(ch)) state = 3; else state = 4;
            break;
        case 3: 
        case 4: 
            if (isConsonant(ch)) state = 5;
            else state = 6; 
            break;
        case 5: 
            if (isConsonant(ch)) state = 1; else state = 2;
            break;
        case 6: 
            state = 6;
            break;
        }
    }


    if (state == 1 || state == 3 || state == 5) {
        return true; 
    }
    else {
        return false;
    }
}

int main() {
    setlocale(LC_CTYPE, "rus");
    string word;
    cout << "Введите слово (алфавит a, b, c, d, e): ";
    cin >> word;

    if (validateWord(word)) {
        cout << "УСПЕХ: Слово принадлежит языку!" << endl;
    }
    else {
        cout << "ОШИБКА: Слово НЕ принадлежит языку!" << endl;
    }

    return 0;
}