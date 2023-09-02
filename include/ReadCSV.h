#include<fstream>
#include<sstream>
#include"Includes.h"

std::vector<std::vector<string>> GetCSV(string fname)
{
 	std::vector<std::vector<string>> content;
	std::vector<string> row;
	string line, word;
 
	std::fstream file (fname, std::ios::in);
	if(file.is_open())
	{
		while(std::getline(file, line))
		{
			row.clear();
 
			std::stringstream str(line);
 
			while(std::getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}
	else
		throw std::logic_error("Could not open the file\n");

	return content;
}


template<typename Numeric>
bool is_number(const std::string& s)
{
    Numeric n;
    return((std::istringstream(s) >> n >> std::ws).eof());
}


i64 GetInt(const std::string& s) {
    std::istringstream istrm(s);
  i64 value = 0;
  char c = istrm.peek();
    // (c >= '0') && (c <= '9')
  while(isdigit(c)) {               
    c = istrm.get();
    value = value * 10 + (c - '0');
    c = istrm.peek();
  }

  return value;
}

double GetDouble(const std::string& s) {
    std::istringstream istrm(s);
  double value = 0;
  char c = istrm.peek();
  double fic_n = 0;
  bool dot_flag = false;
  int i = 10;
         
  while((isdigit(c)) || (c == '.')) {
    c = istrm.get();
    if (c == '.'){
        dot_flag = true;
        c = istrm.peek();
        continue;
    }
    if (dot_flag){
        value = value  + (c - '0' + fic_n) / (i);
        i = i * 10;
    } else {
        value = value * 10 + (c - '0');
    }                 
    c = istrm.peek();
  }  

  return value;
}