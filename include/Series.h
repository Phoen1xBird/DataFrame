#include"Includes.h"
#include"ReadCSV.h"

template <class T>

class Series{
public:
    Series();
    Series(string nm, std::vector<T> elements);
    Series(const Series& rhs);
    Series& operator=(const Series& rhs);
    Series(Series&& rhs);
    Series& operator=(Series&& rhs);
    void AddElement(T element);
    void AddSeries(std::vector<T> series);
    void DeleteElement();
    T& operator[](i64 index);
    Series operator + (const Series& rhs);
    Series operator - (const Series& rhs);
    Series operator / (const Series& rhs);
    Series operator * (const Series& rhs);
    Series operator*(const T& rhs);
    Series operator+(const T& rhs);
    Series operator-(const T& rhs);
    Series operator/(const T& rhs);
    float Mean();
    T Max();
    T Min();
    void print();
    i64 GetSize();
    void SetName(string nm);
    string GetName();
    i64 Count(const T var);
    std::vector<T> UniqueElements();
    Series<double> ToDouble();
    ~Series();
protected:
    void ReallocAtLeast(i64 n);
    string name;
    i64 size;
    i64 reserved;
    T * data;

};


template <class T>
Series<double> Series<T>::ToDouble(){
    // if (typeid(T).name() == typeid(string).name() || typeid(T).name() == typeid(bool).name()){
    //     throw std::logic_error("It's is possible to make double only from numeric types.");
    // }
    Series<double> res;
    // if (typeid(T) == typeid(string)){
    //     res.SetName(name);
    //     for (i64 i = 0; i < size; ++i){
    //         if (is_number<double>(data[i])){
    //             res.AddElement(GetDouble(data[i]));
    //         } else {
    //             throw std::logic_error("Series<srting> contains non-numeric items");
    //         }
    //     }
    //     return res;
    // } 


    for (i64 i = 0; i < size; ++i){
        res.AddElement(data[i] + 0.0);
    }
    res.SetName(name);
    return res;
    
}

template <class T>
Series<T>::Series(){
    size = 0;
    reserved = 2;
    data = new T[2];
    name = "";
}

template<class T>
Series<T>::Series(string nm, std::vector<T> elements){
    name = nm;
    data = new T[elements.size()];
    for (int i = 0; i < elements.size(); ++i){
        data[i] = elements[i];
    }
    size = elements.size();
    reserved = size;
}

template <class T>
void Series<T>::ReallocAtLeast(i64 n){
    if(n <= reserved){
        return;
    }

    T * new_data = new T[n];
    for (int i = 0; i < size; ++i){
        //std::cout << "Here" << '\n';
        new_data[i] = data[i];
    }
    //std::cout << new_data[0] << '\n';
    delete[] data;
    data = new_data;
    reserved = n; 

}

template <class T>
void Series<T>::AddElement(T element){
    if (size == reserved){
        ReallocAtLeast(2 * reserved);
    }
    data[size] = element;
    ++size;

}

template <class T>
void Series<T>::DeleteElement(){
    --size;
}

template <class T>
Series<T>::Series(const Series<T>& rhs){
    name = rhs.name;
    size = rhs.size;
    reserved = rhs.reserved;
    data = new T[rhs.size];
    for (i64 i=0; i < rhs.size; ++i) {
        data[i] = rhs.data[i];
    }

}

template <class T>
Series<T>& Series<T>::operator=(const Series<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Series<T> tmp(rhs);
    std::swap(tmp.name, name);
    std::swap(tmp.data, data);

    return *this;
}

template <class T>
Series<T>::Series(Series<T>&& rhs){
    data = nullptr;
    std::swap(rhs.data, data);
    std::swap(rhs.name, name);
    std::swap(rhs.size, size);
    std::swap(rhs.reserved, reserved);
    //std::cout << "Done" << '\n';
}

template <class T>
Series<T>& Series<T>::operator=(Series<T>&& rhs){
    if (this == &rhs){
        return *this;
    }
    Series<T> tmp(std::move(rhs));
    std::swap(rhs.data, data);
    std::swap(rhs.name, name);
    std::swap(rhs.size, size);
    std::swap(rhs.reserved, reserved);
    return *this;
}

template<class T>
Series<T> Series<T>::operator+(const Series<T>& rhs){
    if (size != rhs.size){
        throw std::logic_error("Different sizes of serieses.");
    }
    Series<T> res(rhs);
    for (int i=0; i < size; ++i) {
        res[i] = res[i] + data[i];
    }
    return res;
}

template <class T>
void Series<T>::print(){
    if (typeid(T) == typeid(i8)){
        std::cout << "Series<i8> ";
    } else if (typeid(T) == typeid(i16)){
        std::cout << "Series<i16> ";
    } else if (typeid(T) == typeid(i32)){
        std::cout << "Series<i32> ";
    } else if (typeid(T) == typeid(i64)){
        std::cout << "Series<i64> ";
    } else if (typeid(T) == typeid(string)){
        std::cout << "Series<std::string> ";
    } else if (typeid(T) == typeid(bool)){
        std::cout << "Series<bool> ";
    } else if (typeid(T) == typeid(float)){
        std::cout << "Series<float> ";
    } else if (typeid(T) == typeid(double)){
        std::cout << "Series<double> ";
    } else {
        throw std::logic_error ("Unknown type.");
    }

    std::cout << name << ": ";
    
    if (size == 0){
        std::cout << "Empty Series" << '\n';
    } else {
        for (i64 i = 0; i < size - 1; ++i){
            std::cout << data[i] << ", ";
        }
        std::cout << data[size - 1] << '\n';
    }
}

template<class T>
Series<T> Series<T>::operator-(const Series<T>& rhs){
    if (size != rhs.size){
        throw std::logic_error("Different sizes of serieses.");
    }
    Series<T> res(rhs);
    for (i64 i=0; i < size; ++i) {
        res[i] = (res[i] - data[i]) * (-1); 
    }
    return res;
}

template<class T>
Series<T> Series<T>::operator*(const Series<T>& rhs){
    if (size != rhs.size){
        throw std::logic_error("Different sizes of serieses.");
    }
    Series<T> res(rhs);
    for (i64 i=0; i < size; ++i) {
        res[i] = (res[i] * data[i]); 
    }
    return res;
}

template<class T>
Series<T> Series<T>::operator/(const Series<T>& rhs){
    if (size != rhs.size){
        throw std::logic_error("Different sizes of serieses.");
    }
    Series<T> res(rhs);
    for (i64 i=0; i < size; ++i) {
        res[i] = data[i] / res[i]; 
    }
    return res;
}

template <class T>
Series<T> Series<T>::operator*(const T& rhs){
    Series<T> res = *this;
    for (i64 i = 0; i < size; ++i){
        res[i] = res[i] * rhs;
    }
    return res;
}

template <class T>
Series<T> Series<T>::operator+(const T& rhs){
    Series<T> res = *this;
    for (i64 i = 0; i < size; ++i){
        res[i] = res[i] + rhs;
    }
    return res;
}

template <class T>
Series<T> Series<T>::operator-(const T& rhs){
    Series<T> res = *this;
    for (i64 i = 0; i < size; ++i){
        res[i] = res[i] - rhs;
    }
    return res;
}

template <class T>
Series<T> Series<T>::operator/(const T& rhs){
    Series<T> res = *this;
    for (i64 i = 0; i < size; ++i){
        res[i] = res[i] / rhs;
    }
    return res;
}

template <class T>
T& Series<T>::operator[](i64 index){
    if (index >= size){
        throw std::logic_error("Index out of range");
    } else {
        return data[index];
    }

}

template <class T>
Series<T>::~Series(){
    delete[] data;
}


template <class T>
float Series<T>::Mean(){
        if (size == 0) {
            throw std::logic_error("Can't find Mean() in empty series");
        } else {
            float sum;
            for (i64 i = 0; i < size; ++i) {
                sum += data[i];
            }
            //std::cout << sum << '\n';
            return sum / size;
        }
    }

template <class T>
T Series<T>::Max(){
    if (size == 0) {
        throw std::logic_error("Can't find Max() in empty series");
    } else {
        T max = data[0];
        for (i64 i = 0; i < size; ++i) {
            if (data[i] > max) {
                max = data[i];
            }
        }
            
        return max;
    }
}

template <class T>
T Series<T>::Min(){
    if (size == 0) {
        throw std::logic_error("Can't find Min() in empty series");
    } else {
        T min = data[0];
        for (i64 i = 0; i < size; ++i) {
            if (data[i] < min) {
                min = data[i];
            }
        }
            
        return min;
    }
}

template <class T>
void Series<T>::AddSeries(std::vector<T> series){
        if (typeid(Series<T>) != typeid(*this)) {
            throw std::logic_error("You are not able to add the Series of different type.");
        }

        for (i64 i = 0; i < series.size(); ++i){
            this->AddElement(series[i]);
        }
    }

template <class T>
i64 Series<T>::Count(const T var){
    if (size == 0){
        throw std::logic_error("Can't find Count() in empty series");
    } else {
        i64 count = 0;
        for (i64 i = 0; i < size; ++i) {
            if (data[i] == var) {
                ++count;
            }
        }

        return count; 
    }
}

template <class T>
void Series<T>::SetName(string nm){
    name = nm;
}

template <class T>
string Series<T>::GetName(){
    return name;
}

template <class T>
i64 Series<T>::GetSize(){
    return size;
}

template <class T>
std::vector<T> Series<T>::UniqueElements(){
    std::vector<T> res;
    for (i64 i = 0; i < size; ++i){
        if (std::find(res.begin(), res.end(), data[i]) == res.end()){
            res.push_back(data[i]);
        }
    }
    return res;
}
