#include "lib.h"
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <list>

using namespace std;

template<typename T1, typename T2, typename T3, typename T4>
T1 split(T1 begin_it, T1 end_it, T2 out_it, T3 split_charq, T4 tabu_fun)
{
    while(begin_it != end_it)
    {
        auto string_end (find(begin_it, end_it, split_charq));
        *out_it++ = tabu_fun(begin_it, string_end);
        if(string_end == end_it) {return end_it;}
        begin_it = next(string_end);
    }
    return begin_it;
}
int main()
{
        auto new_string ([](auto x_it, auto y_it){ return string(x_it, y_it);});
        auto compare ([](auto x_it, auto y_it){ return x_it.size() < y_it.size();});
        list<string> lit;
        for(string line; getline(cin, line);)
        {
        split(begin(line), end(line), back_inserter(lit), '\t', new_string);
        }
        lit.erase(unique(lit.begin(), lit.end()), lit.end());
        lit.sort(compare); 
        lit.remove_if([](auto x){ return x.size() < 6; });
        for(const auto & x : lit) cout << x << "\n";
    return 0;
}
