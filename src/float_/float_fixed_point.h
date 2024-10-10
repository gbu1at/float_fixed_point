#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include "float_exceptions.h"
#include "../includes/bit_mask_fixed.h"

// Класс для представления чисел с фиксированной точкой
class FloatFixedPoint {
public:
    BitMaskFixed bits; // Битовая маска для хранения значения
    int cnt_bits_int_part; // Количество бит для целой части
    int cnt_bits_frac_part; // Количество бит для дробной части

    // Конструктор по умолчанию
    FloatFixedPoint() {}

    // Конструктор с указанием количества бит для целой и дробной части
    FloatFixedPoint(int cnt_bits_int_part, int cnt_bits_frac_part) 
        : cnt_bits_int_part(cnt_bits_int_part), cnt_bits_frac_part(cnt_bits_frac_part) {
        init(); // Инициализация битовой маски
    }

    // Конструктор из строки с указанием количества бит
    FloatFixedPoint(const std::string& s, int cnt_bits_int_part, int cnt_bits_frac_part) 
        : cnt_bits_int_part(cnt_bits_int_part), cnt_bits_frac_part(cnt_bits_frac_part) {
        if (!(s.size() == cnt_bits_int_part + cnt_bits_frac_part))
            throw FloatArgumentsError(""); // Проверка на корректность размера строки
        bits = BitMaskFixed(s); // Инициализация битовой маски из строки
    }

    // Конструктор из строки без указания количества бит
    FloatFixedPoint(const std::string& s) {
        auto it = find(s.begin(), s.end(), '.'); // Поиск точки в строке
        if (it == s.end())
            throw FloatArgumentsError(""); // Ошибка, если точка не найдена

        cnt_bits_int_part = it - s.begin(); // Количество бит целой части
        cnt_bits_frac_part = s.size() - cnt_bits_int_part - 1; // Количество бит дробной части

        bits = BitMaskFixed(s.size() - 1); // Инициализация битовой маски
        bool flag = false; // Флаг для отслеживания точки
        for (int i = 0; i < s.size(); ++i) {
            if (s[s.size() - i - 1] == '.') {
                if (!flag)
                    throw FloatArgumentsError(""); // Ошибка, если точка уже найдена
                flag = true;
                continue;
            }
            bits[i - flag] = ((s[s.size() - i - 1] == '0') ? 0 : 1); // Заполнение битовой маски
        }
    }

    // Инициализация битовой маски
    void init() {
        bits = BitMaskFixed(cnt_bits_int_part + cnt_bits_frac_part);
    }

    // Проверка корректности операции с другим числом с фиксированной точкой
    bool is_correct_op(const FloatFixedPoint& other) const {
        return (this->cnt_bits_int_part == other.cnt_bits_int_part && 
                this->cnt_bits_frac_part == other.cnt_bits_frac_part);
    }

    // Оператор сложения
    FloatFixedPoint operator+(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError(""); // Проверка на корректность операции
        FloatFixedPoint result(cnt_bits_int_part, cnt_bits_frac_part);
        result.bits = bits + other.bits; // Сложение битов
        return result;
    }

    // Оператор += для сложения с присваиванием
    FloatFixedPoint& operator+=(const FloatFixedPoint& other) {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        bits += other.bits; // Сложение битов с присваиванием
        return *this;
    }

    // Оператор -= для вычитания с присваиванием
    FloatFixedPoint& operator-=(const FloatFixedPoint& other) {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        bits -= other.bits; // Вычитание битов с присваиванием
        return *this;
    }

    // Унарный оператор отрицания
    FloatFixedPoint operator-() const {
        FloatFixedPoint result = *this;
        result.bits = -this->bits;        
        return result;
    }

    // Оператор вычитания
    FloatFixedPoint operator-(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        auto minus_other = -other;
        return *this + minus_other; // Использование оператора сложения для вычитания
    }

    // Оператор сравнения на равенство
    bool operator==(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        return bits == other.bits; // Сравнение битов
    }

    // Оператор сравнения на больше или равно
    bool operator>=(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        return bits >= other.bits; 
    }

    // Оператор сравнения на больше
    bool operator>(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        return bits > other.bits;
    }

    // Оператор сравнения на меньше или равно
    bool operator<=(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        return bits <= other.bits;
    }

    // Оператор сравнения на меньше
    bool operator<(const FloatFixedPoint& other) const {
        if (!is_correct_op(other))
            throw FloatOperationError("");
        return bits < other.bits;
    }

    // Оператор неравенства
    bool operator!=(const FloatFixedPoint& other) const {
        return !(*this == other);
    }

    // Дружественная функция для вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const FloatFixedPoint& fp) {
        os << fp.to_string();
        return os;
    }

    // Возвращает размер числа в битах
    int size() const {
        return bits.size();
    }

   /** Преобразует число в строку */
   std::string to_string(bool without_point = false) const {
       std::string s;
       for (int i = size() - 1; i >= 0; i--) {
           if (!without_point && i == size() - cnt_bits_int_part - 1) 
               s.push_back('.'); 
           s.push_back((bits[i] == 1) ? '1' : '0'); 
       }
       return s;
   }

   /** Проверяет, является ли число нулем */
   bool is_null() const {
       return bits.is_null();
   }

   /** Инвертирует биты числа */
   void inv() {
       bits.inv();
   }

   /** Возвращает индекс первого установленного бита */
   int first_one_bit() const {
       return bits.first_one_bit();
   }

   /** Оператор сдвига вправо с присваиванием */
   FloatFixedPoint& operator>>=(int x) {
       if (x < 0)
           throw FloatOperationError(""); 
       bits_step(-x); 
       return *this;
   }

   /** Оператор сдвига влево с присваиванием */
   FloatFixedPoint& operator<<=(int x) {
       if (x < 0)
           throw FloatOperationError(""); 
       bits_step(x); 
       return *this;
   }

   /** Оператор сдвига вправо */
   FloatFixedPoint operator>>(int x) const {
       if (x < 0)
           throw FloatOperationError(""); 
       auto copy_this = *this;
       copy_this.bits_step(-x); 
       return copy_this;
   }

   /** Оператор сдвига влево */
   FloatFixedPoint operator<<(int x) const {
       if (x < 0)
           throw FloatOperationError(""); 
       auto copy_this = *this;
       copy_this.bits_step(x); 
       return copy_this;
   }

private:
   /** Шаг изменения битов */
   void bits_step(int k) {
       if (k >= 0) 
           bits >>= k; 
       else 
           bits <<= k; 
   }
};

// Функция умножения двух чисел с фиксированной точкой с учетом округления
FloatFixedPoint mul(const FloatFixedPoint& self, const FloatFixedPoint& other, int rounding) {
   if (!self.is_correct_op(other))
       throw FloatOperationError(""); 

   BitMaskFixed result_all_bits(2 * self.size()); 

   int new_sign = 0;

   auto x = self.bits; 
   auto y = other.bits;

   if (x.back() == 1) { 
       x = -x; 
       new_sign = (new_sign + 1) & 1; 
   }
   
   if (y.back() == 1) { 
       y = -y; 
       new_sign = (new_sign + 1) & 1; 
   }

   x.resize(2 * self.size()); y.resize(2 * self.size()); 

   result_all_bits = x * y;

   if (new_sign == 1)
       result_all_bits = -result_all_bits;

   FloatFixedPoint result(self.cnt_bits_int_part, self.cnt_bits_frac_part);
   result.bits = result_all_bits.split(self.cnt_bits_frac_part, self.size() + self.cnt_bits_frac_part);
   
   auto trash = result_all_bits.split(0, self.cnt_bits_frac_part);

   /** Обработка округления */
   if (rounding == 0) { 
       if (new_sign == 1) { 
           result.bits += BitMaskFixed(!trash.is_null(), self.size()); 
       }
       return result; 
   } else if (rounding == 1) { 
       if (result.bits.first() == 0) { 
           if (!trash.is_null() && trash.back() == 1)
               if (!trash.split(0, self.cnt_bits_frac_part - 1).is_null())
                   result.bits += BitMaskFixed(1, self.size());
       } else { 
           if (!trash.is_null() && trash.back() == 1)
               result.bits += BitMaskFixed(1, self.size());
       }
       return result; 
   } else if (rounding == 2) { 
       result.bits += BitMaskFixed(!trash.is_null(), self.size()); 
       return result; 
   } else if (rounding == 3) { 
       return result; 
   } else throw UnknownRoundingError("");  
}

// Функция деления двух чисел с фиксированной точкой с учетом округления
FloatFixedPoint div(const FloatFixedPoint& self, const FloatFixedPoint& other, int rounding) {
   if (!self.is_correct_op(other))  
       throw FloatOperationError("");  
   
   if (other.is_null())  
      throw ZeroDivisionError("division by zero float with fixed point");  

   auto divisor = other.bits;  
   auto copy_self = self.bits;

   int new_sign = 0;

   if (divisor.back() == 1) {  
      divisor = -divisor;  
      new_sign = (new_sign + 1) & 1;  
   }

   if (copy_self.back() == 1) {  
      copy_self = -copy_self;  
      new_sign = (new_sign + 1) & 1;  
   }

   divisor.resize(divisor.size() + 1);  
   copy_self.resize(copy_self.size() + 1);  

   int first_one_bit_in_this = copy_self.first_one_bit();  
   
   int first_one_bit_in_divisor = divisor.first_one_bit();  

   BitMaskFixed bits_result(3 * self.size());  

   int degree_2 = first_one_bit_in_this - first_one_bit_in_divisor;

   
    if (degree_2 > 0)
        divisor <<= degree_2;  
    else  
        copy_self <<= -degree_2;

    
    int SZ = 2 * self.size();

    for (int i = degree_2 + SZ; i >= 0; --i) {  
        if (divisor <= copy_self) {  
            bits_result[i] = 1;  
            copy_self -= divisor;  
        } else bits_result[i] = 0;  
        copy_self <<= 1;  
    }

    if (new_sign == 1)
        bits_result = -bits_result;

    FloatFixedPoint result(self.cnt_bits_int_part, self.cnt_bits_frac_part);
    result.bits = bits_result.split(SZ - self.cnt_bits_frac_part, SZ + self.cnt_bits_int_part);
    auto trash = bits_result.split(0, SZ - self.cnt_bits_frac_part);

    /** Обработка округления */
    if (rounding == 0) {  
        if (new_sign == 1) {  
            result.bits += BitMaskFixed(!trash.is_null(), self.size());  
        }
        return result;  
    } else if (rounding == 1) {  
        if (result.bits.first() == 0) {  
            if (trash.back() == 1)
                if (!trash.split(0, trash.size() - 1).is_null())
                    result.bits += BitMaskFixed(1, self.size());  
        } else {  
            if (trash.back() == 1)
                result.bits += BitMaskFixed(1, self.size());  
        }
        return result;  
    } else if (rounding == 2) {  
        result.bits += BitMaskFixed(!trash.is_null(), self.size());  
        return result;  
    } else if (rounding == 3) {  
        return result;  
    } else throw UnknownRoundingError("");  
}