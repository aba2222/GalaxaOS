#include <iostream/iostream.h>

namespace std {
istream cin;

/*istream& istream::operator>>(char* str) {
    scanf("%s", str);
    return *this;
}
istream& istream::operator>>(int& num) {
    scanf("%d", &num);
    return *this;
}
istream& istream::operator>>(unsigned int& num) {
    scanf("%u", &num);
    return *this;
}
istream& istream::operator>>(long& num) {
    scanf("%ld", &num);
    return *this;
}
istream& istream::operator>>(unsigned long& num) {
    scanf("%lu", &num);
    return *this;
}
istream& istream::operator>>(void*& ptr) {
    scanf("%p", &ptr);
    return *this;
}*/


ostream cout;

ostream& ostream::operator<<(const char* str) {
    printf("%s", str);
    return *this;
}

ostream& ostream::operator<<(char c) {
    printf("%c", c);
    return *this;
}

ostream& ostream::operator<<(int num) {
    printf("%d", num);
    return *this;
}

ostream& ostream::operator<<(unsigned int num) {
    printf("%u", num);
    return *this;
}

ostream& ostream::operator<<(long num) {
    printf("%ld", num);
    return *this;
}

ostream& ostream::operator<<(unsigned long num) {
    printf("%lu", num);
    return *this;
}

ostream& ostream::operator<<(void* ptr) {
    printf("%p", ptr);
    return *this;
}

} // namespace std
