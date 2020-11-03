#include <iostream>
#include <cstring>

class String {
private:
    size_t str_size = 0;
    size_t capacity = 1;
    const int array_length_factor = 2; // множитель для размеров строк
    char* str = new char[capacity];

    void createString() {
        str = new char[capacity];
        str[0] = '\0';
    }

    bool willExpand() const {
        return str_size == capacity;
    }

    bool willShrink() const {
        if (str_size < size_t(capacity / array_length_factor)) {
            return true;
        } else {
            return false;
        }
    }

    void deleteString() {
        delete[] str;
    }

    void expandCapacity() { // for "long" push_back, get more memory
        capacity = str_size * array_length_factor + 1;
    }

    void shrinkCapacity() { // for "long" pop_back, cut memory
        capacity = int(capacity / 2) + 1;
    }

    char* bufcpy() {
        char* buf;
        buf = new char[str_size];
        std::copy(str, str + str_size, buf);
        return buf;
    }

public:
    void push_back(const char c) { // fix end
        if (willExpand()) {
            char* buf = bufcpy();
            deleteString();
            expandCapacity();
            createString();
            std::copy(buf, buf + str_size, str);
        }
        str[str_size++] = c;
//        str[str_size] = '\0';
    }

    void pop_back() {
        if (willShrink()) {
            str_size--;
            char* buf = bufcpy();
            deleteString();
            shrinkCapacity();
            createString();
            std::copy(buf, buf + str_size + 1, str); // + 1 is for \0
        } else {
            str_size--;
            str[str_size] = '\0';
        }
    }

    char& front();
    char& front() const;
    char& back();
    char& back() const;
    bool empty() const;
    void clear();
    int find(String& substring);
    int rfind(String& substring);
    String substr(int start, int count);
    size_t length() const;

    // Operators overloading

    friend std::ostream& operator<< (std::ostream &out, const String& str);
    friend std::istream& operator>> (std::istream &inp, const String& str);

    String& operator+=(const String& to_add) { // += strings
        size_t old_size = str_size;
        str_size += to_add.str_size;
        expandCapacity();
        std::copy(to_add.str, to_add.str + to_add.str_size, str + old_size);
        return *this;
    }

    String& operator+=(const char to_add) { // += chars
        push_back(to_add);
        return *this;
    }

    String& operator+=(const char* to_add) { // += C strings
        size_t to_add_len = strlen(to_add);
        size_t old_size = str_size;
        str_size += to_add_len;
        expandCapacity();
        std::copy(to_add, to_add + to_add_len, str + old_size);
        return *this;
    }

    String& operator=(const String& new_string) {
        deleteString();
        capacity = new_string.capacity;
        str_size = new_string.str_size;
        createString();
        std::copy(new_string.str, new_string.str + new_string.str_size + 1, str); // + 1 for /0
        return *this; // разыменовали указатель this
    }

    String& operator=(const char* cstr) {
        deleteString();
        str_size = strlen(cstr);
        expandCapacity();
        createString();
        std::copy(cstr, cstr + str_size, str);
        return *this; // разыменовали указатель this
    }

    String& operator=(const char ch) {
        deleteString();
        str_size = 1;
        expandCapacity();
        createString();
        memset(str, ch, 1);
        return *this; // разыменовали указатель this
    }

    char operator[](const int i) {
        if (i < 0) // a[-1] returns last char
            return str[str_size - i*(-1)]; // some pythonic here
        else
            return str[i];
    }

    char operator[](const int i) const {
        if (i < 0) // a[-1] returns last char
            return str[str_size - i*(-1)]; // some pythonic here
        else
            return str[i];
    }

    String () {
        char* str = new char[capacity];
        str[0] = '\0';
    }; // конструктор по умолчанию.
    String (int n, char c);
    String (const char* cstring);
    String (const String &copy_str); // конструктор копирования
    String (char cchar);

    ~String() {
        deleteString();
    }
};

String operator+(const String& first, const String& second) {
    String copy = first; // RVO
    copy += second;
    return copy;
}

std::ostream& operator<< (std::ostream &out, const String& str) {
    for (int i = 0; i < (int)str.str_size; i++) {
        out << str[i];
    }
    return out;
}

std::istream& operator>> (std::istream &inp, String& str) {
    char buf;
    while (inp >> buf){
        str.push_back(buf);
    }
    return inp;
}

char& String::front() { // возвращает указатель на первый элемент
    return str[0];
}

char& String::front() const { // возвращает указатель на первый элемент
    return str[0];
}

char& String::back() { // возвращает указатель на последний элемент
    return str[str_size-1];
}

char& String::back() const { // возвращает указатель на последний элемент
    return str[str_size-1];
}

bool String::empty() const {
    return (str_size == 0);
}

size_t String::length() const {
    return str_size;
}

void String::clear() {
    deleteString();
    str_size = 0;
    capacity = 0;
    expandCapacity();
    createString();
}

int String::find(String& substring) {
    int pointer = 0;
    int index = str_size;
    if (substring.str_size == 0)
        return 0;
    for (int i = 0; i < (int)str_size; i++) {
        if (pointer == (int)substring.str_size)
            break;
        if (str[i] == substring[pointer]) {
            pointer += 1;
            index = i;
        } else {
            pointer = 0;
            index = str_size;
        }
    }
    return (index != (int)str_size) ? index - (int)substring.str_size + 1 : index;
}

int String::rfind(String& substring) {
    int var = (int)substring.str_size - 1;
    int pointer = var;
    int index = str_size;
    if (substring.str_size == 0)
        return 0;
    for (int i = (int)(str_size - 1); i >= 0; i--) {
        if (pointer < 0)
            break;
        if (str[i] == substring[pointer]) {
            pointer -= 1;
            index = i;
        } else {
            pointer = var;
            index = str_size;
        }
    }
    return (index != (int)str_size) ? index - var : index;
}

String String::substr(int start, int count) {
    char* buf = new char[count];
    std::copy(str + start, str + start + count, buf);
    String copy = buf;
    return copy;
}

String::String(int n, char c) { // set constructor
    str_size = (size_t)n;
    expandCapacity();
    createString();
    memset(str, c, n);
}

String::String (const char* cstring) : String(strlen(cstring), '0'){  // C strings constructor
    std::copy(cstring, cstring + str_size, str);
}

String::String (const String &copy_str) { // copy constructor
    str_size = copy_str.str_size;
    capacity = copy_str.capacity;
    createString();
    std::copy(copy_str.str, copy_str.str + str_size, str);
}

String::String (const char cchar) : String(1, cchar) { } // char constructor

int main() {
    String s;
    String s2 = 'b';
    s.push_back('b');
    s.push_back('a');
    String s3 = s;
    return 0;
}
