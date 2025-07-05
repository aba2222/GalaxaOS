#ifndef __STRING_H
#define __STRING_H

#include <stdint.h>

class String {
public:
    // 默认构造函数
    String() : data(0), length(0) {}

    // 构造函数
    String(const char* str) {
        if (str) {
            length = 0;
            while (str[length] != '\0') {
                length++;
            }
            data = new char[length + 1]; // +1 是为了存储 '\0'
            for (size_t i = 0; i < length; i++) {
                data[i] = str[i];
            }
            data[length] = '\0'; // 添加字符串结束符
        } else {
            data = 0;
            length = 0;
        }
    }

    // 拷贝构造函数
    String(const String& other) {
        length = other.length;
        if (length > 0) {
            data = new char[length + 1];
            for (size_t i = 0; i < length; i++) {
                data[i] = other.data[i];
            }
            data[length] = '\0';
        } else {
            data = 0;
        }
    }

    String(String&& other) noexcept {
        data = other.data;
        length = other.length;

        other.data = nullptr;
        other.length = 0;
    }

    // 赋值操作符
    String& operator=(const String& other) {
        if (this == &other) return *this; // 避免自赋值

        // 使用临时对象处理分配，防止部分失败造成内存泄漏
        char* newData = new char[other.length + 1];
        for (size_t i = 0; i < other.length; ++i) {
            newData[i] = other.data[i];
        }
        newData[other.length] = '\0';

        delete[] data;  // 释放旧的data
        data = newData;
        length = other.length;
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            length = other.length;

            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    String operator+(const String& other) const {
        String result;
        size_t newLength = length + other.length;
        char* newData = new char[newLength + 1];

        // 复制当前字符串
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }

        // 复制其他字符串
        for (size_t i = 0; i < other.length; ++i) {
            newData[length + i] = other.data[i];
        }
        newData[newLength] = '\0';

        result.data = newData;
        result.length = newLength;

        return result;
    }
    
    bool operator==(const String& other) const {
        if (length != other.length) return false;
        for (size_t i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    // 获取字符串长度
    size_t GetLength() const {
        return length;
    }

    // 索引操作符
    char& operator[](size_t index) {
        return data[index];
    }

    const char& operator[](size_t index) const {
        return data[index];
    }

    // 析构函数
    ~String() {
        if (data) {
            delete[] data;
        }
    }

    // 获取底层数据
    const char* c_str() const {
        return data ? data : "";
    }
private:
    char* data;     // 字符串数据
    size_t length;  // 字符串长度
};

#endif