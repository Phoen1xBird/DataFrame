#include"Includes.h"

template <class T>
struct SortStructure{
    i64 index;
    T data;
    SortStructure(i64 i, T value){
        index = i;
        data = value;
    }
};
