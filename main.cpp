#include <iostream>
const size_t MAX_ROWS = 100;
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
    char matrix[MAX_ROWS][401] = {}; // Инициализируем нулями

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

    // создаем матрицу для кодировки
    char matrix2[MAX_ROWS * 4] = {}; // Увеличиваем размер для хранения всех символов

    int in = 0;
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            if (in < message.length()+1) {
                unsigned char first_byte = static_cast<unsigned char>(matrix[i][j * 4]);
                int char_length = 0;

                if ((first_byte & 0x80) == 0) {
                    char_length = 1; // ASCII
                } else if ((first_byte & 0xE0) == 0xC0) {
                    char_length = 2; // 2 байта
                } else if ((first_byte & 0xF0) == 0xE0) {
                    char_length = 3; // 3 байта
                } else if ((first_byte & 0xF8) == 0xF0) {
                    char_length = 4; // 4 байта
                }

                // Копируем символ в матрицу
                for (size_t k = 0; k < char_length; ++k) {
                    matrix2[in + k] = matrix[i][j * 4 + k]; // Копируем корректно
                }
                in += char_length;
            }
        }
    }

    // Выводим результат
    for (size_t i = 0; i < in; ++i) {
        cout << matrix2[i];
    }

    cout << endl;
    ///делаем расшифровку

    char matrix3[MAX_ROWS][401] = {}; // Инициализируем нулями

    size_t index2 = 0;
    for (size_t j = 0; j < cols; ++j) {
        for (size_t i = 0; i < rows; ++i) {
            if (index2 < message.length()+1) {
                // Найдем следующий символ и добавим его в матрицу
                size_t char_length = 1;
                if ((matrix2[index2] & 0x80) == 0) {
                    char_length = 1; // ASCII
                } else if ((matrix2[index2] & 0xE0) == 0xC0) {
                    char_length = 2; // 2 байта
                } else if ((matrix2[index2] & 0xF0) == 0xE0) {
                    char_length = 3; // 3 байта
                } else if ((matrix2[index2] & 0xF8) == 0xF0) {
                    char_length = 4; // 4 байта
                }

                // Копируем символ в матрицу
                for (size_t k = 0; k < char_length; ++k) {
                    matrix3[i][j * 4 + k] = matrix2[index2 + k]; // Умножаем на 4 для учета многобайтовых символов
                }
                index2 += char_length;
            }

        }
    }

    // создаем матрицу для кодировки
    char matrix4[MAX_ROWS * 4] = {}; // Увеличиваем размер для хранения всех символов

    int in2 = 0;
    for (size_t i = 0; i < rows ; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (in2 < message.length()+1) {
                unsigned char first_byte2 = static_cast<unsigned char>(matrix3[i][j * 4]);
                int char_length = 0;

                if ((first_byte2 & 0x80) == 0) {
                    char_length = 1; // ASCII
                } else if ((first_byte2 & 0xE0) == 0xC0) {
                    char_length = 2; // 2 байта
                } else if ((first_byte2 & 0xF0) == 0xE0) {
                    char_length = 3; // 3 байта
                } else if ((first_byte2 & 0xF8) == 0xF0) {
                    char_length = 4; // 4 байта
                }

                // Копируем символ в матрицу
                for (size_t k = 0; k < char_length; ++k) {
                    matrix4[in2 + k] = matrix3[i][j * 4 + k]; // Копируем корректно
                }
                in2 += char_length;
            }
        }
    }

    // Выводим результат
    for (size_t i = 0; i < in2; ++i) {
        cout << matrix4[i];
    }

    cout << endl;

    return 0;
}
