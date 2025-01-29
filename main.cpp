#include <iostream>
#include <string>

using namespace std;

// Функция для получения длины строки в символах (UTF-8)
size_t utf8_length(const string& str) {
    size_t length = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        if ((str[i] & 0xC0) != 0x80) { // Проверяем, является ли байт началом символа
            length++;
        }
    }
    return length;
}

// Функция для копирования символов в матрицу
void copy_to_matrix(const string& message, size_t rows, size_t cols, char matrix[][401]) {
    size_t index = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (index < message.length()) {
                // Найдем следующий символ и добавим его в матрицу
                size_t char_length = 1;
                if ((message[index] & 0x80) == 0) {
                    char_length = 1; // ASCII
                } else if ((message[index] & 0xE0) == 0xC0) {
                    char_length = 2; // 2 байта
                } else if ((message[index] & 0xF0) == 0xE0) {
                    char_length = 3; // 3 байта
                } else if ((message[index] & 0xF8) == 0xF0) {
                    char_length = 4; // 4 байта
                }

                // Копируем символ в матрицу
                for (size_t k = 0; k < char_length; ++k) {
                    matrix[i][j * 4 + k] = message[index + k]; // Умножаем на 4 для учета многобайтовых символов
                }
                index += char_length;
            } else {
                matrix[i][j * 4] = ' '; // Заполняем оставшиеся ячейки нулевыми символами
            }
        }
    }
}

int main() {
    // Ввод строки
    string kode;
    cout << "Введите код шифрования (русские буквы): ";
    getline(cin, kode);

    string message;
    cout << "Введите сообщение: ";
    getline(cin, message);

    size_t cols = utf8_length(kode); // Получаем количество столбцов
    size_t message_length = utf8_length(message);
    size_t rows = (message_length / cols) + (message_length % cols == 0 ? 0 : 1);

    // Создаем матрицу в виде двумерного массива
    const size_t MAX_ROWS = 100; // Максимальное количество строк
    char matrix[MAX_ROWS][401] = {}; // Инициализируем нулями

    // Копируем строку в матрицу
    copy_to_matrix(message, rows, cols, matrix);

    // Выводим матрицу по столбцам в одну строчку
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            if (matrix[i][j * 4] != '0') { // Проверяем на нулевой символ
                cout << string(&matrix[i][j * 4]); // Выводим весь символ
            } else {
                cout << " "; // Если ячейка пустая, выводим пробел
            }
        }
    }

    cout << endl;

    return 0;
}