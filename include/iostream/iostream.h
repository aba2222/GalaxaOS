#ifndef IOSTREAM_H
#define IOSTREAM_H

extern "C" void printf(const char*, ...);

namespace std {

/*class istream {
public:
    istream& operator>>(char* str);
    istream& operator>>(int& num);
    istream& operator>>(unsigned int& num);
    istream& operator>>(long& num);
    istream& operator>>(unsigned long& num);
    istream& operator>>(void*& ptr);
};*/

class ostream {
public:
    ostream& operator<<(const char* str);
    ostream& operator<<(char c);
    ostream& operator<<(int num);
    ostream& operator<<(unsigned int num);
    ostream& operator<<(long num);
    ostream& operator<<(unsigned long num);
    ostream& operator<<(void* ptr);
};

extern ostream cout;

} // namespace std

#endif // IOSTREAM_H