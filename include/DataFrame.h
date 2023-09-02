#include"Includes.h"
#include"Series.h"
#include"Render.h"
#include"UseFunc.h"
//#include"ReadCSV.h"
#include"SortStructure.h"
#include<functional>



class DataFrame{ 
public:
    DataFrame();
    DataFrame(const DataFrame& df);
    DataFrame& operator=(const DataFrame& df);
    DataFrame(DataFrame&& df);
    DataFrame& operator=(DataFrame&& df);
    template <class T>
    Series<T> GetColumn(string name);
    template <class T>
    Series<T> GetColumn(i64 ind);
    i64 CountColumns();
    i64 CountRows();
    string GetTypeOfColumn(string name);
    string GetTypeOfColumn(i64 ind);
    template <class T>
    DataFrame GroupBy(string name, string func);
    void ReadCSV(string name);
    template <class T>
    T Locate (i64 row, i64 column);
    DataFrame Locate(i64 rb, i64 re, i64 cb, i64 ce);
    void Drop(string name);
    void Drop(i64 ind);
    template <class T>
    void AddColumn(string name, Series<T> series);
    template <class T>
    void AddColumn(Series<T> series);
    void Head(i64 n);
    void Tail(i64 n);
    template <class T>
    DataFrame SortBy(string name, bool ascend);
    template <class T>
    DataFrame Select(string name, T element, std::function<bool(T,T)> func);
    void WriteCSV(string name);
    ~DataFrame() = default;

private:
    std::vector<std::string> names;
    std::vector<std::any> data;
    i64 n_columns;
    i64 n_rows;
    std::vector<string> types;

};


DataFrame::DataFrame(){
    n_rows = 0;
    n_columns = 0;
    data = {};
    names = {};
    types = {};
}

DataFrame::DataFrame(const DataFrame& df){
    data = df.data;
    n_rows = df.n_rows;
    n_columns = df.n_columns;
    names = df.names;
    types = df.types;
}

DataFrame& DataFrame::operator=(const DataFrame& df){
    if(this == &df){
        return *this;
    }
    DataFrame tmp(df);
    std::swap(tmp.data, data);
    std::swap(tmp.n_columns, n_columns);
    std::swap(tmp.n_rows, n_rows);
    std::swap(tmp.names, names);
    std::swap(tmp.types, types);
    return *this;
}

DataFrame::DataFrame(DataFrame&& df){
    std::swap(df.data, data);
    std::swap(df.n_columns, n_columns);
    std::swap(df.n_rows, n_rows);
    std::swap(df.names, names);
    std::swap(df.types, types);
}

DataFrame& DataFrame::operator=(DataFrame&& df){
    if (this == &df){
        return *this;
    }
    DataFrame tmp(std::move(df));
    std::swap(tmp.data, data);
    std::swap(tmp.n_columns, n_columns);
    std::swap(tmp.n_rows, n_rows);
    std::swap(tmp.names, names);
    std::swap(tmp.types, types);
    return *this;
}

template <class T>
Series<T> DataFrame::GetColumn(string name){
    bool flag = false;
    i64 ind;
    for (i64 i = 0; i < n_columns; ++i){
        if (name == names[i]){
            flag = true;
            ind = i;
            break;
        }
    }

    if (!flag) {
        throw std::logic_error("No such name in DataFrame");
    }
    if (types[ind] != typeid(Series<T>).name()) {
        std::cout << "The type of the column is " << types[ind] << ". You try to get " << typeid(Series<T>).name() << '\n';
        throw std::logic_error("Wrong type");
    
    }
        return std::any_cast<Series<T>>(data[ind]);
}

template <class T>
Series<T> DataFrame::GetColumn(i64 ind){
    if (ind > n_columns){
        throw std::logic_error("Index out of range");
    }

    return std::any_cast<Series<T>>(data[ind]);     
}

template <class T>
void DataFrame::AddColumn(string name, Series<T> series){
    names.push_back(name);
    series.SetName(name);
    types.push_back(typeid(Series<T>).name());
    ++n_columns;
    if (series.GetSize() > n_rows){
        n_rows = series.GetSize();
    }
    data.push_back(series);
}

template <class T>
void DataFrame::AddColumn(Series<T> series){
    names.push_back(series.GetName());
    types.push_back(typeid(Series<T>).name());
    ++n_columns;
    if (series.GetSize() > n_rows){
        n_rows = series.GetSize();
    }
    data.push_back(series);
}

i64 DataFrame::CountColumns(){
    return n_columns;
}

i64 DataFrame::CountRows(){
    return n_rows;
}

void DataFrame::Head(i64 n){
    int space = 200 / n_columns;
    if (space % 2 != 0) {++space;}
    WriteSpaces(5);

    for (int i = 0; i < n_columns; ++i){
        int len = names[i].size();
        int ws = (space - len) / 2;
        WriteSpaces(ws); 
        std:: cout << names[i];
        WriteSpaces(ws) ;
        if (len % 2 != 0){
            std::cout << " ";
        }
    }

    std::cout << '\n';

    for (i64 i = 0; i < std::min(n_rows, n); ++i){
        int tmp = n_rows;
        int ind_ws = (5 - CountWs(i)) / 2;
        WriteSpaces(ind_ws);
        std::cout << i;
        WriteSpaces(ind_ws);
        //std::cout << "Over index " << '\n';
        for (i64 ind = 0; ind < n_columns; ++ind){
            //std::cout << types[ind] << '\n';
            if (types[ind] == typeid(Series<i8>).name()){
                i8 element = std::any_cast<Series<i8>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i16>).name()) {
                i16 element = std::any_cast<Series<i16>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i32>).name()){
                //std::cout << "INNN" << '\n';
                i32 element = std::any_cast<Series<i32>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i64>).name()){
                i64 element = std::any_cast<Series<i64>>(data[ind])[i];
                WriteInt(element, space); 
            } else if (types[ind] == typeid(Series<string>).name()) {
                string element = std::any_cast<Series<string>>(data[ind])[i];
                WriteStr(element, space);
            } else if (types[ind] == typeid(Series<bool>).name()) {
                bool element = std::any_cast<Series<bool>>(data[ind])[i];
                WriteBool(element, space);
            } else if (types[ind] == typeid(Series<float>).name()) {
                float element = std::any_cast<Series<float>>(data[ind])[i];
                WriteBF(element, space);
            } else if (types[ind] == typeid(Series<double>).name()){
                double element = std::any_cast<Series<double>>(data[ind])[i];
                WriteBF(element, space);
            } else {
                throw std::logic_error("Somehow DataFrame contains unknown type. This is impossible");
            }
        }
        std::cout << '\n';
    }
}

void DataFrame::Tail(i64 n){
    int space = 200 / n_columns;
    if (space % 2 != 0) {++space;}
    WriteSpaces(5);

    for (int i = 0; i < n_columns; ++i){
        int len = names[i].size();
        int ws = (space - len) / 2;
        WriteSpaces(ws); 
        std:: cout << names[i];
        WriteSpaces(ws) ;
        if (len % 2 != 0){
            std::cout << " ";
        }
    }

    std::cout << '\n';
    i64 zero = 0;
    for (i64 i = std::max(zero, n_rows - n); i < n_rows; ++i){
        int tmp = n_rows;
        int ind_ws = (5 - CountWs(i)) / 2;
        WriteSpaces(ind_ws);
        std::cout << i;
        WriteSpaces(ind_ws);
        //std::cout << "Over index " << '\n';
        for (i64 ind = 0; ind < n_columns; ++ind){
            //std::cout << types[ind] << '\n';
            if (types[ind] == typeid(Series<i8>).name()){
                i8 element = std::any_cast<Series<i8>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i16>).name()) {
                i16 element = std::any_cast<Series<i16>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i32>).name()){
                //std::cout << "INNN" << '\n';
                i32 element = std::any_cast<Series<i32>>(data[ind])[i];
                WriteInt(element, space);
            } else if (types[ind] == typeid(Series<i64>).name()){
                i64 element = std::any_cast<Series<i64>>(data[ind])[i];
                WriteInt(element, space); 
            } else if (types[ind] == typeid(Series<string>).name()) {
                string element = std::any_cast<Series<string>>(data[ind])[i];
                WriteStr(element, space);
            } else if (types[ind] == typeid(Series<bool>).name()) {
                bool element = std::any_cast<Series<bool>>(data[ind])[i];
                WriteBool(element, space);
            } else if (types[ind] == typeid(Series<float>).name()) {
                float element = std::any_cast<Series<float>>(data[ind])[i];
                WriteBF(element, space);
            } else if (types[ind] == typeid(Series<double>).name()){
                double element = std::any_cast<Series<double>>(data[ind])[i];
                WriteBF(element, space);
            } else {
                throw std::logic_error("Somehow DataFrame contains unknown type. This is impossible");
            }
        }
        std::cout << '\n';
    }
}

template <class T>
T DataFrame::Locate(i64 row, i64 column){
    if (row > n_rows || column > n_columns) {
        throw std::logic_error("Index out of range");
    }
    if (types[column] != typeid(Series<T>).name()) {
        std::cout << "The type of the column is " << types[column] << ". You try to get " << typeid(Series<T>).name() << '\n';
        throw std::logic_error("Wrong type");
    
    }
        return std::any_cast<Series<T>>(data[column])[row];
}
          
DataFrame DataFrame::Locate(i64 rb, i64 re, i64 cb, i64 ce){
    DataFrame df;
    for (i64 ind = cb; ind < ce; ++ind){
            if (types[ind] == typeid(Series<i8>).name()){
                Series<i8> ser = std::any_cast<Series<i8>>(data[ind]);
                Series<i8> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<i8>(res.GetName(), res);
            } else if (types[ind] == typeid(Series<i16>).name()) {
                Series<i16> ser = std::any_cast<Series<i16>>(data[ind]);
                Series<i16> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<i16>(res.GetName(), res);
                
            } else if (types[ind] == typeid(Series<i32>).name()){
                //std::cout << "INNN" << '\n';
                Series<i32> ser = std::any_cast<Series<i32>>(data[ind]);
                Series<i32> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<i32>(res.GetName(), res);
                //std::cout << df.n_rows << '\n';
                
            } else if (types[ind] == typeid(Series<i64>).name()){
                Series<i64> ser = std::any_cast<Series<i64>>(data[ind]);
                Series<i64> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<i64>(res.GetName(), res);
             
            } else if (types[ind] == typeid(Series<string>).name()) {
                Series<string> ser = std::any_cast<Series<string>>(data[ind]);
                Series<string> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<string>(res.GetName(), res);
            
            } else if (types[ind] == typeid(Series<bool>).name()) {
                Series<bool> ser = std::any_cast<Series<bool>>(data[ind]);
                Series<bool> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<bool>(res.GetName(), res);
                
            } else if (types[ind] == typeid(Series<float>).name()) {
                Series<float> ser = std::any_cast<Series<float>>(data[ind]);
                Series<float> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<float>(res.GetName(), res);
        
            } else if (types[ind] == typeid(Series<double>).name()){
                Series<double> ser = std::any_cast<Series<double>>(data[ind]);
                Series<double> res;
                res.SetName(ser.GetName());
                for (i64 j = rb; j < re; ++j){
                    res.AddElement(ser[j]);
                }
                df.AddColumn<double>(res.GetName(), res);
            } else {
                throw std::logic_error("Somehow DataFrame contains unknown type. This is impossible");
            }
    }
    return df;
}

string DataFrame::GetTypeOfColumn(string name){
    i64 ind = 0;
    for (i64 i = 0; i < n_columns; ++i){
        if (names[i] == name){
            ind = i;
            break;
        }
    }
    if (types[ind] == typeid(Series<i8>).name()){
        return "Series<int8_t>";
    } else if (types[ind] == typeid(Series<i16>).name()){
        return "Series<int16_t>";
    } else if (types[ind] == typeid(Series<i32>).name()){
        return "Series<int32_t>";
    } else if (types[ind] == typeid(Series<i64>).name()){
        return "Series<int64_t>";
    } else if (types[ind] == typeid(Series<string>).name()){
        return "Series<string>";
    } else if (types[ind] == typeid(Series<bool>).name()){
        return "Series<bool>"; 
    } else if (types[ind] == typeid(Series<float>).name()){
        return "Series<float>";
    } else if (types[ind] == typeid(Series<double>).name()){
        return "Series<double>";
    }
    
}

string DataFrame::GetTypeOfColumn(i64 ind){
    if (types[ind] == typeid(Series<i8>).name()){
        return "Series<int8_t>";
    } else if (types[ind] == typeid(Series<i16>).name()){
        return "Series<int16_t>";
    } else if (types[ind] == typeid(Series<i32>).name()){
        return "Series<int32_t>";
    } else if (types[ind] == typeid(Series<i64>).name()){
        return "Series<int64_t>";
    } else if (types[ind] == typeid(Series<string>).name()){
        return "Series<string>";
    } else if (types[ind] == typeid(Series<bool>).name()){
        return "Series<bool>"; 
    } else if (types[ind] == typeid(Series<float>).name()){
        return "Series<float>";
    } else if (types[ind] == typeid(Series<double>).name()){
        return "Series<double>";
    }
    
}

template <class T>
DataFrame DataFrame::GroupBy(string name, string func){
    i64 ind = 0;
    for (i64 i = 0; i < n_columns; ++i){
        if (names[i] == name){
            ind = i;
            break;
        }
    }

    Series<T> inv_col = std::any_cast<Series<T>>(data[ind]);
    std::vector<T> un_elements = inv_col.UniqueElements();
    Series<T> main_column(name, un_elements);
    DataFrame df;
    df.AddColumn<T>(name, main_column);
    if (func == "Count"){
        Series<T> count;
        for (i64 i = 0; i < main_column.GetSize(); ++i){
            // std::cout << main_column[i] << '\n';
            // std::cout << inv_col.Count(main_column[i]) << '\n';
            count.AddElement(inv_col.Count(main_column[i]));
        }
        df.AddColumn("Counts", count);
        return df;
    }
    for (i64 ind = 0; ind < n_columns; ++ind){
        if (names[ind] != name){
            //std::cout << "check index" << ind << '\n';
            if (types[ind] == typeid(Series<i8>).name()){
                Series<i8> series = std::any_cast<Series<i8>>(data[ind]);
                std::vector<i8> v;
                std::vector<std::vector<i8>> values(main_column.GetSize(), v);
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    i8 tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<i8>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                } else {
                std::vector<i8> res = GetResOfFunc<i8>(values, func);
                Series<i8> func_series(func, res);
                df.AddColumn<i8>(series.GetName(), func_series);
                }
            } else if (types[ind] == typeid(Series<i16>).name()) {
                Series<i16> series = std::any_cast<Series<i16>>(data[ind]);
                std::vector<i16> v;
                std::vector<std::vector<i16>> values(main_column.GetSize());
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    i16 tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<i16>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                     
                } else {            
                std::vector<i16> res = GetResOfFunc<i16>(values, func);
                Series<i16> func_series(func, res);
                df.AddColumn<i16>(series.GetName(), func_series);
                }
            } else if (types[ind] == typeid(Series<i32>).name()){
                Series<i32> series = std::any_cast<Series<i32>>(data[ind]);
                std::vector<i32> v;
                std::vector<std::vector<i32>> values(main_column.GetSize(), v);
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    i32 tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<i32>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                } else {

                //std::cout << "Values " << values[0][0] << '\n';
                std::vector<i32> res = GetResOfFunc<i32>(values, func);
                Series<i32> func_series(func, res);
                df.AddColumn<i32>(series.GetName(), func_series);
                }
            } else if (types[ind] == typeid(Series<i64>).name()){
                Series<i64> series = std::any_cast<Series<i64>>(data[ind]);
                std::vector<i64> v;
                std::vector<std::vector<i64>> values(main_column.GetSize(), v);
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    i64 tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<i64>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                     
                } else {
                std::vector<i64> res = GetResOfFunc<i64>(values, func);
                Series<i64> func_series(func, res);
                df.AddColumn<i64>(series.GetName(), func_series);
                }
            } else if (types[ind] == typeid(Series<float>).name()) {
                Series<float> series = std::any_cast<Series<float>>(data[ind]);
                std::vector<float> v;
                std::vector<std::vector<float>> values(main_column.GetSize());
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    float tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<float>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                     
                } else {             
                std::vector<float> res = GetResOfFunc<float>(values, func);
                Series<float> func_series(func, res);
                df.AddColumn<float>(series.GetName(), func_series);
                }
            } else if (types[ind] == typeid(Series<double>).name()){
                Series<double> series = std::any_cast<Series<double>>(data[ind]);
                std::vector<double> v;
                std::vector<std::vector<double>> values(main_column.GetSize(), v    );
                for (i64 i = 0; i < series.GetSize(); ++i){
                    T inv_el = inv_col[i];
                    double tar_el = series[i];
                    for (i64 j =0; j < un_elements.size(); ++j){
                        if (inv_el == un_elements[j]){
                            values[j].push_back(tar_el);
                        }
                    }
                }
                if (func == "Mean"){
                    std::vector<float> res = MeanFunc<double>(values);
                    Series<float> func_series(func, res);
                    df.AddColumn<float>(series.GetName(), func_series);
                     
                } else {
                std::vector<double> res = GetResOfFunc<double>(values, func);
                Series<double> func_series(func, res);
                df.AddColumn<double>(series.GetName(), func_series);
                }
            } 
        }
    }
    return df;
}

void DataFrame::ReadCSV(string name){
    std::vector<std::vector<string>> content = GetCSV(name);
    DataFrame df;
    for (i64 i = 0; i < content[0].size(); ++i){
        bool int_flag = false;
        bool double_flag = false;
        Series<i64> int_ser;
        Series<double> double_ser;
        Series<string> str_ser;

        if (is_number<double>(content[1][i])){
            //std:: cout << "index " << i << '\n';
            for (i64 k = 1; k < content.size(); ++k){
                if (content[k][i] != "0"){
                    int_flag = is_number<int>(content[k][i]);
                    if (!int_flag){
                        double_flag = is_number<double>(content[k][i]);
                    }
                    break;
                }
            }
        }
        
        for (i64 j = 1; j < content.size(); ++j){
            if (int_flag){
                int_ser.AddElement(GetInt(content[j][i]));
            } else if (double_flag){
                double_ser.AddElement(GetDouble(content[j][i]));
            } else {
                str_ser.AddElement(content[j][i]);
            }  
        }
        if (int_flag){
            df.AddColumn<i64>(content[0][i], int_ser);
        } else if (double_flag){
            df.AddColumn<double>(content[0][i], double_ser);
        } else {
            df.AddColumn<string>(content[0][i], str_ser);
        }
        //df.Head(df.CountRows());
    }
    *this = std::move(df);
}

void DataFrame::Drop(string name){
    bool flag = false;
    i64 ind;
    for (i64 i = 0; i < n_columns; ++i){
        if (name == names[i]){
            flag = true;
            ind = i;
            break;
        }
    }

    if (!flag) {
        throw std::logic_error("No such name in DataFrame");
    }

    std::vector<std::any> new_data;
    std::vector<string> new_names;
    std::vector<string> new_types;
    for (i64 i = 0; i < n_columns; ++i){
        if (i != ind){
            new_data.push_back(data[i]);
            new_names.push_back(names[i]);
            new_types.push_back(types[i]);
        }
    }
    --n_columns;
    std::swap(data, new_data);
    std::swap(names, new_names);
    std::swap(types, new_types);
}

void DataFrame::Drop(i64 ind){
    if (ind > n_columns - 1){
        throw std::logic_error("Index out of range\n");
    }

    std::vector<std::any> new_data;
    std::vector<string> new_names;
    std::vector<string> new_types;
    for (i64 i = 0; i < n_columns; ++i){
        if (i != ind){
            new_data.push_back(data[i]);
            new_names.push_back(names[i]);
            new_types.push_back(types[i]);
        }
    }
    --n_columns;
    std::swap(data, new_data);
    std::swap(names, new_names);
    std::swap(types, new_types);
}

template <class T>
DataFrame DataFrame::SortBy(string name, bool ascend){
    bool flag = false;
    i64 ind;
    for (i64 i = 0; i < n_columns; ++i){
        if (name == names[i]){
            flag = true;
            ind = i;
            break;
        }
    }

    if (!flag) {
        throw std::logic_error("No such name in DataFrame");
    }

    if ((names[ind] == typeid(string).name()) || (names[ind] == typeid(bool).name())) {
        throw std::logic_error("You can sort only numeric types.\n");
    }

    std::vector<SortStructure<T>> sort_vec;
    Series<T> tar_ser = std::any_cast<Series<T>>(data[ind]);

    for (i64 i = 0; i < n_rows; ++i){
        sort_vec.push_back({i, tar_ser[i]});
    }

    std::sort(sort_vec.begin(), sort_vec.end(), [](SortStructure<T>& s1, SortStructure<T>& s2){if (s1.data < s2.data){return true;} 
                                                                                         else {return false;}});

    if (!ascend){
        std::reverse(sort_vec.begin(), sort_vec.end());
    }

    DataFrame df;
    for (i64 i = 0; i < n_columns; ++i){
        if (types[i] == typeid(Series<i8>).name()){
            Series<i8> ser = std::any_cast<Series<i8>>(data[i]);
            Series<i8> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i8>(ser.GetName(), new_ser);
        } else if (types[i] == typeid(Series<i16>).name()){
            Series<i16> ser = std::any_cast<Series<i16>>(data[i]);
            Series<i16> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i16>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<i32>).name()){
            Series<i32> ser = std::any_cast<Series<i32>>(data[i]);
            Series<i32> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i32>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<i64>).name()){
            Series<i64> ser = std::any_cast<Series<i64>>(data[i]);
            Series<i64> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i64>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<string>).name()){
            Series<string> ser = std::any_cast<Series<string>>(data[i]);
            Series<string> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<string>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<bool>).name()){
            Series<bool> ser = std::any_cast<Series<bool>>(data[i]);
            Series<bool> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<bool>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<float>).name()){
            Series<float> ser = std::any_cast<Series<float>>(data[i]);
            Series<float> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<float>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<double>).name()){
            Series<double> ser = std::any_cast<Series<double>>(data[i]);
            Series<double> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<double>(ser.GetName(), new_ser);          
        }

    }
    return df;
}

template <class T>
DataFrame DataFrame::Select(string name, T element, std::function<bool(T,T)> func){
    bool flag = false;
    i64 ind;
    for (i64 i = 0; i < n_columns; ++i){
        if (name == names[i]){
            flag = true;
            ind = i;
            break;
        }
    }

    if (!flag) {
        throw std::logic_error("No such name in DataFrame");
    }

    std::vector<SortStructure<T>> sort_vec;
    Series<T> tar_ser = std::any_cast<Series<T>>(data[ind]);

    for (i64 i = 0; i < n_rows; ++i){
        if (func(element, tar_ser[i])){
            // std::cout << "el " << element << '\n';
            //std::cout << "cur el " << typeid(tar_ser[i]).name() << '\n';
            sort_vec.push_back({i, tar_ser[i]});
        }
    }

    DataFrame df;
    for (i64 i = 0; i < n_columns; ++i){
        if (types[i] == typeid(Series<i8>).name()){
            Series<i8> ser = std::any_cast<Series<i8>>(data[i]);
            Series<i8> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i8>(ser.GetName(), new_ser);
        } else if (types[i] == typeid(Series<i16>).name()){
            Series<i16> ser = std::any_cast<Series<i16>>(data[i]);
            Series<i16> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i16>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<i32>).name()){
            Series<i32> ser = std::any_cast<Series<i32>>(data[i]);
            Series<i32> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i32>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<i64>).name()){
            Series<i64> ser = std::any_cast<Series<i64>>(data[i]);
            Series<i64> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<i64>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<string>).name()){
            Series<string> ser = std::any_cast<Series<string>>(data[i]);
            Series<string> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<string>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<bool>).name()){
            Series<bool> ser = std::any_cast<Series<bool>>(data[i]);
            Series<bool> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<bool>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<float>).name()){
            Series<float> ser = std::any_cast<Series<float>>(data[i]);
            Series<float> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<float>(ser.GetName(), new_ser);          
        } else if (types[i] == typeid(Series<double>).name()){
            Series<double> ser = std::any_cast<Series<double>>(data[i]);
            Series<double> new_ser;
            for (i64 j = 0; j < sort_vec.size(); ++j){
                new_ser.AddElement(ser[sort_vec[j].index]);
            }
            df.AddColumn<double>(ser.GetName(), new_ser);          
        }

    }
    return df;
}

void DataFrame::WriteCSV(string name){
    std::ofstream myfile;
    myfile.open (name);
    for (i64 i = 0; i < names.size(); ++i){
        myfile << names[i];
        if (i < names.size() - 1){
            myfile << ',';
        }

    }
    myfile << '\n';
    for (i64 j = 0; j < n_rows; ++j){
        for (i64 i = 0; i < n_columns; ++i){
            if (types[i] == typeid(Series<i8>).name()){
                Series<i8> ser = std::any_cast<Series<i8>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }
            } else if (types[i] == typeid(Series<i16>).name()){
                Series<i16> ser = std::any_cast<Series<i16>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }        
            } else if (types[i] == typeid(Series<i32>).name()){
                Series<i32> ser = std::any_cast<Series<i32>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }     
            } else if (types[i] == typeid(Series<i64>).name()){
                Series<i64> ser = std::any_cast<Series<i64>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }         
            } else if (types[i] == typeid(Series<string>).name()){
                Series<string> ser = std::any_cast<Series<string>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }       
            } else if (types[i] == typeid(Series<bool>).name()){
                Series<bool> ser = std::any_cast<Series<bool>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }      
            } else if (types[i] == typeid(Series<float>).name()){
                Series<float> ser = std::any_cast<Series<float>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }         
            } else if (types[i] == typeid(Series<double>).name()){
                Series<double> ser = std::any_cast<Series<double>>(data[i]);
                myfile << ser[j];
                if (i < n_columns - 1){
                    myfile << ',';
                }     
            }
        }
        if (j < n_rows - 1){
            myfile << '\n';
        }
    } 
}