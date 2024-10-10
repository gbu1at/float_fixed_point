#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include "../scripts/parse_data.h"


class BitMaskFixed {
public:

    std::vector<int> bits;

    BitMaskFixed() {}

    BitMaskFixed(const std::vector<int>& bits) : bits(bits) {
        for (int x : bits) assert(x == 0 || x == 1);
    }

    BitMaskFixed(int x, int sz) :
    BitMaskFixed(convert_to_n_bits(number_to_binary(x), sz))    
    {
    }

    BitMaskFixed(const std::string& s) {
        for (char x : s) {
            if (x == '1') bits.push_back(1);
            else if (x == '0') bits.push_back(0);
            else assert(false);
        }
        std::reverse(bits.begin(), bits.end());
    }

    BitMaskFixed(int sz) {
        bits.assign(sz, 0);
    }

    void inv() {
        for (size_t i = 0; i < bits.size(); ++i)
            bits[i] = bits[i] ^ 1;
    }

    bool operator==(const BitMaskFixed& other) const {
        assert(size() == other.size());
        for (int i = 0; i < size(); ++i) if (bits[i] != other.bits[i]) return false;
        return true;
    }

    bool operator!=(const BitMaskFixed& other) const {
        return !(*this == other);
    }


    bool operator<(const BitMaskFixed& other) const {
        assert(size() == other.size());
        for (int i = size() - 1; i >= 0; --i) {
            if (bits[i] < other.bits[i]) return true;
            else if (bits[i] > other.bits[i]) return false;
        }
        return false;
    }

    bool operator<=(const BitMaskFixed& other) const {
        return (*this < other || *this == other);
    }

    bool operator>(const BitMaskFixed& other) const {
        return !(*this <= other);
    }

    bool operator>=(const BitMaskFixed& other) const {
        return !(*this < other);
    }

    BitMaskFixed operator+(const BitMaskFixed& other) const {
        assert(size() == other.size());
        BitMaskFixed result(size());
        
        int carry = 0;
        
        for (size_t i = 0; i < size(); ++i) {
            int sum = bits[i] + other.bits[i] + carry;
            result.bits[i] = sum & 1;
            carry = sum >> 1;
        }

        return result;
    }

    BitMaskFixed operator+(int x) const {
        auto result = *this + BitMaskFixed(x, size());
        return result;
    }

    BitMaskFixed operator-(int x) const {
        auto result = *this - BitMaskFixed(x, size());
        return result;
    }

    BitMaskFixed& operator-=(int x) {
        *this -= BitMaskFixed(x, size());
        return *this;
    }

    BitMaskFixed& operator+=(int x) {
        *this += BitMaskFixed(x, size());
        return *this;
    }

    BitMaskFixed operator^(const BitMaskFixed& other) const {
        assert(size() == other.size());
        BitMaskFixed result(size());
        for (size_t i = 0; i < size(); ++i)
            result.bits[i] = bits[i] ^ other.bits[i];
        return result;
    }

    BitMaskFixed operator&(const BitMaskFixed& other) const {
        assert(size() == other.size());
        BitMaskFixed result(size());
        for (size_t i = 0; i < size(); ++i)
            result.bits[i] = bits[i] & other.bits[i];
        return result;
    }

    int size() const {
        return bits.size();
    }

    std::string to_string() const {
        std::string s;
        for (int i = size() - 1; i >= 0; i--)
            s.push_back( (bits[i] == 1) ? '1' : '0' );
        return s;
    }

    BitMaskFixed operator*(const BitMaskFixed& other) const {
        assert( size() == other.size() );

        std::vector<int> result_all_bits(2 * size());

        for (int i = 0; i < size(); ++i)
            for (int j = 0; j < size(); ++j)
                result_all_bits[i + j] += bits[i] * other.bits[j];

        int transition = 0;

        for (int i = 0; i < result_all_bits.size(); ++i) {
            int d = (transition + result_all_bits[i]);
            transition = d >> 1;
            result_all_bits[i] = d & 1;
        }

        
        return BitMaskFixed(result_all_bits).split(size());
    }

    BitMaskFixed& operator+=(const BitMaskFixed& other) {
        assert( size() == other.size() );

        int carry = 0;

        for (int i = 0; i < this->size(); ++i) {
            int d = (carry + bits[i] + other.bits[i]);
            carry = d >> 1;
            bits[i] = d & 1;
        }

        return *this;
    }

    BitMaskFixed& operator-=(const BitMaskFixed& other) {
        assert( size() == other.size() );

        auto minus_other = -other;
        *this += minus_other;
        return *this;
    }


    BitMaskFixed operator-() const {
        BitMaskFixed result = *this;
    
        result.inv();
        
        int carry = 1;
        
        for (int i = 0; i < this->size(); ++i) {
            int d = (carry + result.bits[i]);
            carry = d >> 1;
            result.bits[i] = d & 1;
        }

        return result;
    }

    BitMaskFixed operator-(const BitMaskFixed& other) const {
        assert( size() == other.size() );
        auto minus_other = -other;
        return *this + minus_other;
    }


    BitMaskFixed split(int sz) {
        assert(sz >= 0);
        std::vector<int> bits_result;
        for (int i = 0; i < sz; ++i) bits_result.push_back(bits[i]);
        return BitMaskFixed(bits_result);
    }

    BitMaskFixed split(int l, int r) {
        assert(r >= l && l >= 0 && size() >= r);
        std::vector<int> bits_result;
        for (int i = l; i < r; ++i) bits_result.push_back(bits[i]);
        return BitMaskFixed(bits_result);
    }

    bool is_null() const {
        for (int i = 0; i < size(); ++i) if (bits[i] != 0) return false;
        return true;
    }

      
    int operator[](int index) const {
        if (index >= 0) {
            assert(index < bits.size());
            return bits[index];
        } else {
            assert(size() + index >= 0);
            return bits[size() + index];
        }
    }

    int& operator[](int index) {
        if (index >= 0) {
            assert(index < bits.size());
            return bits[index];
        } else {
            assert(size() + index >= 0);
            return bits[size() + index];
        }
    }


    void resize(int sz) {
        int x = size();
        if (size() < sz)
            for (int i = x; i < sz; ++i) bits.push_back(0);
        else
            for (int i = sz; i < x; ++i) bits.pop_back();
    }
    friend std::ostream& operator<<(std::ostream& os, const BitMaskFixed& bm) {
        os << bm.to_string();
        return os;
    }

    int to_int() {
        int res = 0;
        int p = 1;
        for (int i = 0; i < bits.size(); ++i) {
            res += bits[i] * p;
            p *= 2;
        }
        res -= bits.back() * p;
        return res;
    }
    int first_one_bit() const {
        for (int i = size() - 1; i >= 0; --i) {
            if (bits[i] == 1) return i;
        }
        return -1;
    }

    int back() const {
        return bits.back();
    }
    int sign() const {
        return back();
    }

    int first() const {
        return bits[0];
    }

    BitMaskFixed& operator>>=(int x) {
        assert(x >= 0);
        bits_step(-x);
        return *this;
    }

    BitMaskFixed& operator<<=(int x) {
        assert(x >= 0);
        bits_step(x);
        return *this;
    }

    BitMaskFixed operator>>(int x) const {
        assert(x >= 0);
        auto copy_this = *this;
        copy_this.bits_step(-x);
        return copy_this;
    }

    BitMaskFixed operator<<(int x) const {
        assert(x >= 0);
        auto copy_this = *this;
        copy_this.bits_step(x);
        return copy_this;
    }

    bool all_one() const {
        for (int x : bits) if (x == 0) return false;
        return true;
    }

private:
    void bits_step(int k) {
        assert(abs(k) <= size());
        if (k > 0) {
            for (int i = size() - 1; i >= k; --i)
                bits[i] = bits[i - k];
            for (int i = 0; i < k; ++i) bits[i] = 0;
        } else {
            k = abs(k);
            for (int i = 0; i < size() - k; ++i)
                bits[i] = bits[i + k];
            for (int i = size() - 1; i >= size() - k; --i) bits[i] = 0;
        }
    }
};

