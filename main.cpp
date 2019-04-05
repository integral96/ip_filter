//#include <cassert>
#include "lib.h"
#include <vector>
//#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <list>

using namespace std;

template<typename T1, typename T2, typename T3, typename T4>
T1 split(T1 begin_it, T1 end_it, T2 out_it, T3 split_charq, T4 tabu_fun)
{
    ///////////////////задаем начальный и конечный итератор и итератор вывода, а также char q и правило
    while(begin_it != end_it)
    {
        ////////////////////////////находим перескакиваем и итерируем
        auto string_end (find(begin_it, end_it, split_charq));
        *out_it++ = tabu_fun(begin_it, string_end);
        if(string_end == end_it) {return end_it;}
        begin_it = next(string_end);
    }
    return begin_it;
}

int main()
{
        ///////////////////лямбда для новой строки
        auto new_string ([](auto x_it, auto y_it)
        {
            return string(x_it, y_it);
        });
        list<string> lit;
        //////////////////////////////////////Считываем в лист
        for(string line; getline(cin, line);)
        {
        split(begin(line), end(line), back_inserter(lit), '\t', new_string);
        }
        //////////////////////////////////////сортируем и удаляем не нужное
        lit.remove_if([](auto x){ return x.size() < 5; });
        lit.sort(); 
        lit.erase(unique(lit.begin(), lit.end()), lit.end());
        ///////////////////////////////////выводим
        copy(begin(lit), end(lit), ostream_iterator<string> {cout, "\n"} );
    //cout << "HELLLO" << endl;
   return 0;
}