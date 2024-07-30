#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <cstring>
#include <utility>
template <class T, int length = 10>
struct Array {
    T node[length];
    bool busy[length];
    int size;

    Array() { 
        memset(busy, 0, sizeof(busy)); 
    }

    void Clear() {
        size = 0;
        memset(busy, 0, sizeof(busy));
    }
    bool Empty() { 
        return size == 0; 
    }

    bool Full() { 
        return size == length; 
    }

    int Allocate() {
        for (int i = 0; i < length; i++) {
            if (!busy[i]) {
                size++;
                busy[i] = true;
                return i;
            }
        }
        return -1;
    }

    int Length() { 
        return length; 
    }

    void Erase(int pos) {
        busy[pos] = 0;
        size--;
    }

    T& operator[](int pos) { 
        return node[pos]; 
    }
};
#endif