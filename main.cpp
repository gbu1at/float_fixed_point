#include <iostream>
#include <string>
#include "src/scripts/parse_data.h" // Подключение пользовательского модуля для парсинга данных
#include "main.h" // Подключение заголовочного файла (предположительно для объявления функций или классов)

// Объявление функции подсчета количества символов в строке
int count_char(const std::string& str, char ch);

// Объявление функции проверки, содержит ли строка только цифры
bool is_digits_only(const std::string& str);

// Объявление функции для получения фиксированной точки вещественного числа
std::pair<int, int> get_param_point_fixed_float(std::string s);

// Объявление функции обработки команды с фиксированной точкой вещественного числа
std::string command_float_fixed_point(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    // Переменная для хранения результата
    std::string res;

    try {
        // Вызов функции обработки команды и сохранение результата
        res = command_float_fixed_point(argc, argv);
        // Вывод результата на экран
        std::cout << res << "\n";
    }
    catch(const std::exception& e) {
        // Обработка исключений: вывод сообщения об ошибке
        std::cout << e.what() << "\n";
    }
}