#pragma once
#include <iostream>
#include <string>
#include "src/scripts/parse_data.h" // Подключение пользовательского модуля для парсинга данных
#include "src/float_/float_fixed_point.h" // Подключение модуля, связанного с фиксированной точкой вещественных чисел
#include "src/float_/float_exceptions.h" // Подключение модуля для работы с исключениями, связанными с числами с плавающей запятой

// Функция для подсчета вхождений символа 'ch' в строке 'str'
int count_char(const std::string& str, char ch) {
    int count = 0; // Переменная для хранения количества вхождений
    // Перебор каждого символа в строке
    for (char current_char : str)
        if (current_char == ch) // Если текущий символ равен искомому
            ++count; // Увеличиваем счетчик
    return count; // Возвращаем количество вхождений
}

// Функция для проверки, состоит ли строка только из цифр
bool is_digits_only(const std::string& str) {
    // Перебор каждого символа в строке
    for (char ch : str)
        if (!isdigit(ch)) // Если символ не является цифрой
            return false; // Возвращаем false
    return true; // Если все символы - цифры, возвращаем true
}

// Функция для разбора строки, представляющей вещественное число, и получения целой и дробной частей
std::pair<int, int> get_param_point_fixed_float(std::string s) {
    // Утверждение, что в строке ровно одна точка
    assert(count_char(s, '.') == 1);
    // Утверждение, что строка не начинается и не заканчивается точкой
    assert(s[0] != '.' && s.back() != '.');
    std::string s1; // Переменная для хранения целой части
    std::string s2; // Переменная для хранения дробной части
    bool flag = false; // Флаг для переключения с целой части на дробную

    // Перебор каждого символа в строке
    for (char x : s) {
        if (x == '.') { // При встрече точки
            flag = true; // Устанавливаем флаг, что нужно запоминать дробную часть
            continue; // Переходим к следующему символу
        }
        if (flag) s2.push_back(x); // Если флаг установлен, добавляем символ в дробную часть
        else s1.push_back(x); // Иначе добавляем символ в целую часть
    }
    // Утверждение, что целая и дробная части содержат только цифры
    assert(is_digits_only(s1));
    assert(is_digits_only(s2));

    // Возвращаем пары чисел, полученные из целой и дробной частей
    return {stoi(s1), stoi(s2)};
}

// Функция операции над двумя числами с фиксированной точкой 'fp1' и 'fp2', основанная на операторе строки 's'
FloatFixedPoint op(const FloatFixedPoint& fp1, const std::string& s, const FloatFixedPoint& fp2, int rounding) {
    if (s == "+") return fp1 + fp2; // Возврат результата сложения
    else if (s == "-") return fp1 - fp2; // Возврат результата вычитания
    else if (s == "*") return mul(fp1, fp2, rounding); // Возврат результата умножения с учетом округления
    else if (s == "/") return div(fp1, fp2, rounding); // Возврат результата деления с учетом округления
    else assert(false); // Генерация ошибки если оператор неизвестен
}


// Функция обработки командной строки для работы с числами с фиксированной точкой
std::string command_float_fixed_point(int argc, char* argv[]) {
    // Значение параметра о количестве бит для целой и дробной части
    std::string param_value = argv[1];
    
    // Находим количество бит для целой и дробной части с помощью функции get_param_point_fixed_float
    auto[cnt_bits_int_part, cnt_bits_frac_part] = get_param_point_fixed_float(param_value);
    int all_bits = cnt_bits_int_part + cnt_bits_frac_part; // Общее количество бит

    // Считываем значение округления из аргументов
    int rounding = atoi(argv[2]);
    
    // Конвертируем первые входные данные в формат фиксированной точки
    std::string binary_str_1 = convert_to_n_bits(hex_to_binary(argv[3]), all_bits);
    FloatFixedPoint fp1(binary_str_1, cnt_bits_int_part, cnt_bits_frac_part);
    
    if (argc == 4) {
        // Если передано только одно число, возвращаем его представление с фиксированной точкой
        return convert_float_fixed_point_to_answer(fp1.to_string(), rounding);
    } else {
        // Если передано два числа и операция между ними
        std::string operation = argv[4];
        
        // Конвертируем второе входное число
        std::string binary_str_2 = convert_to_n_bits(hex_to_binary(argv[5]), all_bits);
        FloatFixedPoint fp2(binary_str_2, cnt_bits_int_part, cnt_bits_frac_part);

        try {
            // Выполняем операцию над двумя числами
            FloatFixedPoint fp_res = op(fp1, operation, fp2, rounding);
    
            // Возвращаем результат выполнения операции
            return convert_float_fixed_point_to_answer(fp_res.to_string(), rounding);

        } catch (const ZeroDivisionError& err) {
            // Обработка ошибки деления на ноль
            return "div_by_0";
        }
    }
}