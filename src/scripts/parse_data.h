#pragma once


#include <string>
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cassert>
#include <iomanip>

// Функция удаляет все точки из входной строки и возвращает обновленную строку
std::string remove_dots(const std::string& input) {
    std::string result;
    for (char c : input)
        if (c != '.')
            result += c;
    return result;
}

// Объявление функции для проверки, является ли строка бинарной
bool is_binary_string(const std::string &bin_string);

// Функция конвертирует бинарную строку в десятичное число
int binary_to_decimal(const std::string& binary) {
    int result = 0;
    int intPower = 1;
    for (int i = binary.size() - 1; i >= 0; i--) {
        // Умножение текущего разряда на соответствующую степень двойки и добавление к результату
        result += ((i == 0) ? -1 : 1) * (binary[i] == '1') * intPower;
        intPower *= 2;
    }
    return result; // Возврат десятичного числа
}

// Функция конвертирует бинарную строку в шестнадцатеричную строку
std::string binary_to_hex(const std::string& binary) {
    unsigned long decimal = std::bitset<64>(binary).to_ulong();
    std::stringstream ss;
    ss << std::hex << std::uppercase << decimal;

    auto s = ss.str();
    // Добавление нулей в начало строки, чтобы количество бит соответствовало размеру бинарной строки
    while (s.size() * 4 != binary.size())
        s = "0" + s;
    return s;
}

// Функция конвертирует шестнадцатеричную строку в бинарную строку
std::string hex_to_binary(const std::string &hex_string) {
    std::string hex = hex_string;
    // Проверка и удаление префикса "0x" из шестнадцатеричной строки
    if (hex.rfind("0x", 0) == 0) {
        hex = hex.substr(2);
    }

    unsigned long decimal_number = std::stoul(hex, nullptr, 16);
    std::string binary_string = std::bitset<64>(decimal_number).to_string();

    binary_string.erase(0, binary_string.find_first_not_of('0'));

    return binary_string;
}

// Функция проверяет, является ли данная строка бинарной (состоит только из символов '0' и '1')
bool is_binary_string(const std::string &bin_string) {
    for (char x : bin_string)
        if (x != '0' && x != '1')
            return false;
    return true;
}

// Функция преобразует бинарную строку в n-битное представление
std::string convert_to_n_bits(const std::string &bin_string, size_t n) {
    if (bin_string.length() == n)
        return bin_string;
    else if (bin_string.length() > n)
        return bin_string.substr(bin_string.length() - n);
    return std::string(n - bin_string.length(), '0') + bin_string;
}

// Функции для преобразования бинарной строки в фиксированное количество бит
std::string convert_to_64_bits(const std::string &bin_string) {
    return convert_to_n_bits(bin_string, 64);
}

std::string convert_to_32_bits(const std::string &bin_string) {
    return convert_to_n_bits(bin_string, 32);
}

std::string convert_to_16_bits(const std::string &bin_string) {
    return convert_to_n_bits(bin_string, 16);
}

// Функция преобразует число в бинарную строку
std::string number_to_binary(unsigned long number) {
    if (number == 0) return "0";
    std::string binaryStr = "";

    while (number > 0) {
        binaryStr += (number % 2 == 0) ? '0' : '1';
        number /= 2;
    }
    std::reverse(binaryStr.begin(), binaryStr.end());
    return binaryStr;
}

std::string get_res(long long value) {
    auto a = std::to_string(value % 10);
    auto b = std::to_string((value / 10) % 10);
    auto c = std::to_string((value / 100) % 10);
    return std::to_string(value / 1000) + "." + c + b + a;
}

// Функция преобразует фиксированную точку бинарного числа в строку с десятичной записью и округлением
std::string convert_float_fixed_point_to_answer(std::string binary, int rounding) {
    
    // Находим позицию десятичной точки в строке
    int it = binary.find(".");

    // Определяем количество бит в целой и дробной части
    int int_size_part = it;
    int frac_size_part = binary.size() - it - 1;
    
    // Преобразуем бинарную строку в целое значение, убрав точку
    long long value_ = binary_to_decimal(remove_dots(binary));

    // Если нет дробной части, возвращаем значение с нулевыми дробными цифрами
    if (int_size_part == -1) {
        return std::to_string(value_) + ".000";
    }

    // Выделяем целочисленную и дробную часть значения
    long long value_int = value_ >> frac_size_part;
    long long value_frac = value_ & ((1 << frac_size_part) - 1);

    // Увеличиваем дробную часть для удобства обработки
    for (int i = 0; i < 3; ++i)
        value_frac *= 10;

    // Boolean флаги для определения того, насколько остаточная часть превышает половину
    bool residue_is_more_null = false;
    bool residue_is_more_half = false;
    bool residue_is_half = false;

    // Устанавливаем флаг, если остающаяся часть больше нуля
    if (value_frac % (1 << frac_size_part) > 0)
        residue_is_more_null = true;
    
    // Устанавливаем флаг, если остающаяся часть больше половины
    if (2 * (value_frac % (1ll << frac_size_part)) > (1ll << frac_size_part))
        residue_is_more_half = true;

    // Устанавливаем флаг, если остающаяся часть равна половине
    if (2 * (value_frac % (1ll << frac_size_part)) == (1ll << frac_size_part))
        residue_is_half = true;

    // Определяем окончательную дробную часть
    value_frac >>= frac_size_part;

    // Объединяем целую и дробную части для итогового значения
    long long value = value_int * 1000 + value_frac;

    std::string result;

    // Случай округления в сторону нуля
    if (rounding == 0) {
        if (value >= 0)
            result = get_res(value);
        else {
            value = std::abs(value);
            if (residue_is_more_null) value -= 1; // Вычитаем 1, если нужно устранить остаток более нуля
            result = "-" + get_res(value);
        }
    }
    // Случай округления до ближайшего четного
    else if (rounding == 1) {
        if (residue_is_more_half)
            value += 1; // Добавляем 1, если остаток больше половины
        else {
            if (residue_is_half) {
                if (value % 2) {
                    value += 1; // Добавляем 1, если текущее значение последней цифры нечётное
                }
            }
        }
        if (value >= 0) {
            result = get_res(value);
        } else {
            value = std::abs(value);
            result = "-" + get_res(value);
        }
    }
    // Случай округления в сторону бесконечности
    else if (rounding == 2) {
        if (residue_is_more_null)
            value += 1; // Добавляем 1 к последней цифре, если остаток больше нуля
        if (value >= 0) {
            result = get_res(value);
        } else {
            value = std::abs(value);
            result = "-" + get_res(value);
        }
    }
    // Случай округления к нулю
    else if (rounding == 3) {
        if (value < 0) {
            value = std::abs(value);
            result = "-" + get_res(value);
        } else result = get_res(value);
    
    } else assert(false); // Непредвиденные значения округления

    if (result == "-0.000") result = "0.000"; // Исправление отрицательного нуля
    return result; // Возвращаем итоговый результат
}