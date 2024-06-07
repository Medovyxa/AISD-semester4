#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib> 
#include <ctime>   

#define N 100

template <typename K, typename T>
struct Pair {
    K key;
    T value;
    bool filled; 
};

template <typename K, typename T>
class MyUnorderedMap {
private:
    Pair<K, T> table[N];
    int size;

    int hashFunction(const K& key) {
        const double A = 0.6180339887;
        double fractionalPart = modf(key * A, &key);
        return static_cast<int>(N * fractionalPart);
    }

public:
    MyUnorderedMap() : size(0) {
        for (int i = 0; i < N; ++i) {
            table[i].filled = false;
        }
    }

    MyUnorderedMap(const MyUnorderedMap& other) {
        size = other.size;
        for (int i = 0; i < N; ++i) {
            table[i] = other.table[i];
        }
    }

    ~MyUnorderedMap() {}

    MyUnorderedMap& operator=(const MyUnorderedMap& other) {
        if (this != &other) {
            size = other.size;
            for (int i = 0; i < N; ++i) {
                table[i] = other.table[i];
            }
        }
        return *this;
    }

    void print() {
        for (int i = 0; i < N; ++i) {
            if (table[i].filled) {
                std::cout << "Roman: " << table[i].key << ", Arabic: " << table[i].value << std::endl;
            }
        }
    }

    void insert(const K& key, const T& value) {
        int index = hashFunction(key) % N;
        int startIndex = index;

        while (table[index].filled) {
            if (table[index].key == key) {
                return;
            }
            index = (index + 1) % N;
            if (index == startIndex) {
                throw std::overflow_error("Hash table is full");
            }
        }

        table[index].key = key;
        table[index].value = value;
        table[index].filled = true;
        size++;
    }

    T* search(const K& key) {
        int index = hashFunction(key) % N;
        while (table[index].filled) {
            if (table[index].key == key) {
                return &table[index].value;
            }
            index = (index + 1) % N;
        }
        return nullptr;
    }

    bool erase(const K& key) {
        int index = hashFunction(key) % N;
        while (table[index].filled) {
            if (table[index].key == key) {
                table[index].filled = false;
                size--;
                return true;
            }
            index = (index + 1) % N;
        }
        return false;
    }

    int count(const K& key) {
        int index = hashFunction(key) % N;
        int count = 0;
        while (table[index].filled) {
            if (table[index].key == key) {
                count++;
            }
            index = (index + 1) % N;
        }
        return count;
    }

    void fillWithRandomValues() {
        srand(time(0));
        for (int i = 0; i < N; ++i) {
            int randomKey = rand() % 1000;
            int randomValue = rand() % 1000;
            insert(randomKey, randomValue);
        }
    }

    void insert_or_assign(const K& key, const T& value) {
        int index = hashFunction(key) % N;
        while (table[index].filled) {
            if (table[index].key == key) {
                table[index].value = value;
                return;
            }
            index = (index + 1) % N;
        }
        table[index].key = key;
        table[index].value = value;
        table[index].filled = true;
        size++;
    }

    bool contains(const K& key) {
        int index = hashFunction(key) % N;
        while (table[index].filled) {
            if (table[index].key == key) {
                return true;
            }
            index = (index + 1) % N;
        }
        return false;
    }
};


int romanToDecimal(const std::string& roman) {
    std::unordered_map<char, int> romanValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };

    int result = 0;
    int prevValue = 0;
    for (int i = roman.size() - 1; i >= 0; --i) {
        int value = romanValues[roman[i]];
        if (value < prevValue) {
            result -= value;
        }
        else {
            result += value;
        }
        prevValue = value;
    }
    return result;
}


int main() {

    std::string romanNumber;
    std::cout << "Enter a Roman number (type 'exit' to stop): ";
    while (std::cin >> romanNumber && romanNumber != "exit") {

        int arabicNumber = romanToDecimal(romanNumber);
        std::cout << romanNumber << " is " << arabicNumber << " in Arabic." << std::endl;
        std::cout << "Enter a Roman number (type 'exit' to stop): ";
    }

    return 0;
}