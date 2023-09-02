#include"Includes.h"

void WriteSpaces(int n){
    for (int i = 0; i < n; ++i){
        std::cout << ' ';
    }     
}


int CountWs(i64 n){
    if (n == 0) {
        return 1;
    }
    int counter = 0;
    if (n < 0) {
        ++counter;
    }
    while (n != 0){
        n = n / 10;
        //std::cout << "n " << n << '\n';
        ++counter;
        //std::cout << "Counter " << counter << '\n';
    }

    return counter;
}


void WriteElement(int ws, i64 element){
    WriteSpaces(ws);
    std::cout << element;
    WriteSpaces(ws);
}

void WriteElement(int ws, string element){
    WriteSpaces(ws);
    std::cout << element;
    WriteSpaces(ws);
}

void WriteElement(int ws, bool element){
    WriteSpaces(ws);
    std::cout << element;
    WriteSpaces(ws);
}



void WriteInt(i64 element, int space){
    int counter = CountWs(element);            
    int ws = (space - counter) / 2;
    WriteElement(ws, element);
    if (counter %  2 != 0) {
        std::cout << " ";
    }
}

void WriteStr(string element, int space){
    int ws = (space - element.size()) / 2;
    WriteElement(ws, element);
    if (element.size() % 2 != 0){
        std::cout << " ";
    }
}


void WriteBool(bool element, int space){
    int ws = (space - 1) / 2;
    WriteElement(ws, element);
    std::cout << " ";
}

void WriteBF(float element, int space){
    int c = int(element);
    int counter = CountWs(c) + 3;
    int ws = (space - counter) / 2;
    WriteSpaces(ws);
    printf("%.2f", element);
    WriteSpaces(ws);
    if (counter % 2 != 0) {
        std:: cout << " ";
    }
}

void WriteBF(double element, int space){
    int c = int(element);
    int counter = CountWs(c) + 3;
    int ws = (space - counter) / 2;
    WriteSpaces(ws);
    printf("%.2f", element);
    WriteSpaces(ws);
    if (counter % 2 != 0) {
        std:: cout << " ";
    }
}