#include"Includes.h"
//#include"Series.h"

template <class T>
float mean(std::vector<T> v){
    if(v.empty()){
        throw std::logic_error("Can't find the Mean of the emptiness.");
    }

    float sum = 0;
    for (i64 i = 0; i < v.size(); ++i){
        sum += v[i];
    }
    return sum / v.size();
}

template <class T>
T max(std::vector<T> v){
    if(v.empty()){
        throw std::logic_error("Can't find the Max of the emptiness.");
    }
    T max = v[0];
    for (i64 i = 0; i < v.size(); ++i){
        if (v[i] > max){
            max = v[i];
        }
    }
    return max;
}

template <class T>
T min(std::vector<T> v){
    if(v.empty()){
        throw std::logic_error("Can't find the Max of the emptiness.");
    }
    T min = v[0];
    for (i64 i = 0; i < v.size(); ++i){
        if (v[i] < min){
            min = v[i];
        }
    }
    return min;
}

template <class T>
T sum(std::vector<T> v){
    if(v.empty()){
        throw std::logic_error("Can't find the Sum of the emptiness.");
    }
    T sum = 0;
    for (i64 i = 0; i < v.size(); ++i){
        sum += v[i];

    }
    return sum;
}

template <class T>
std::vector<float> MeanFunc(std::vector<std::vector<T>> values){
    std::vector<float> res;
    for (i64 i = 0; i < values.size(); ++i){
        res.push_back(mean<T>(values[i])); 
    }
    return res;
}


template <class T>
std::vector<T> GetResOfFunc(std::vector<std::vector<T>> values, string func){
    std::vector<T> res;
    for (i64 i = 0; i < values.size(); ++i){
        if (func == "Max"){
            res.push_back(max<T>(values[i]));
        }
        if (func == "Min"){
            res.push_back(min<T>(values[i]));
        }
        if (func == "Sum"){
            res.push_back(sum<T>(values[i]));
        }

    }
    return res;
    
}