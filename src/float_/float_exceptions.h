#pragma once

#include <exception>

// **Класс исключения для ошибок деления на ноль**
class ZeroDivisionError : public std::exception {
public:

    ZeroDivisionError(const char* message) : msg_(message) {}
    

    const char* what() const noexcept override {
        return msg_;
    }
private:

    const char* msg_;
};

// **Класс исключения для ошибок, связанных с неправильным использованием аргументов типа float**
class FloatArgumentsError : public std::exception {
public:

    FloatArgumentsError(const char* message) : msg_(message) {}
    
    const char* what() const noexcept override {
        return msg_;
    }
private:
    const char* msg_;
};

// **Класс исключения для ошибок операций с числами с плавающей запятой**
class FloatOperationError : public std::exception {
public:
    FloatOperationError(const char* message) : msg_(message) {}
    
    const char* what() const noexcept override {
        return msg_;
    }
private:

    const char* msg_;
};

// **Класс исключения для ошибок неизвестной стратегии округления**
class UnknownRoundingError : public std::exception {
public:
    UnknownRoundingError(const char* message) : msg_(message) {}
    
    const char* what() const noexcept override {
        return msg_;
    }
private:
    const char* msg_;
};