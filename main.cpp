#include "lib.h"
#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <list>

using namespace std;

// создаем тюпл
template<size_t K, typename V>
struct init {
    V value;
    constexpr init() : value() {}
    template<typename T>
    explicit constexpr init(T&& value_) : value(static_cast<T&&>(value_)) {}
};
template<size_t K, typename V>
constexpr const V& _get(const init<K, V>& x) {
    return x.value;
}
template<size_t K, typename V>
constexpr V& _get(init<K, V>& x) {
    return x.value;
}
template<size_t K, typename V>
constexpr V&& _get(init<K, V>&& x) {
    return static_cast<V&&>(x.value);
}
template<typename K, typename ...Args>
struct my_tuple_impl;
template<size_t ...K, typename ...Args>
struct my_tuple_impl<index_sequence<K...>, Args...> : init<K, Args>... {
    static constexpr size_t size_ = sizeof...(Args);
    template<typename ...Args1>
    explicit constexpr my_tuple_impl(Args1&&... args) : init<K, Args>(static_cast<Args1&&>(args))... {}
};
template<typename ...Args>
struct my_tuple : my_tuple_impl<make_index_sequence<sizeof...(Args)>, Args...> {
    using base = my_tuple_impl<make_index_sequence<sizeof...(Args)>, Args...>;
    template<typename ...Args1, enable_if_t<base::size_ == sizeof...(Args1), bool> = true>
    explicit constexpr my_tuple(Args1&&... args) : base(static_cast<Args1&&>(args)...) {}
};
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(const my_tuple<Args...>& value) {
    return _get<K>(value);
}
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(my_tuple<Args...>& value) {
    return _get<K>(value);
}
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(my_tuple<Args...>&& value) {
    return _get<K>(static_cast<my_tuple<Args...>&&>(value));
}
// создаем мейк тюпл
template<typename V>
struct unwrap_ {
    using type = V;
};
template<typename V>
struct unwrap_<reference_wrapper<V>> {
    using type = V&;
};
template<typename V>
using unwrap_t = typename unwrap_<decay_t<V>>::type;
template<typename ...Args>
constexpr my_tuple<unwrap_t<Args>...> make_my_tuple(Args&&... value) {
    return my_tuple<unwrap_t<Args>...>(forward<Args>(value)...);
}
// создаем цикл для вывода тюпл
template<size_t K, size_t I, typename Closure>
enable_if_t<(I == K)> _loop_impl(Closure&) {}
template<size_t K, size_t I, typename Closure>
enable_if_t<(I < K)> _loop_impl(Closure& closure) {
    closure.template apply<I>();
    _loop_impl<K, I + 1>(closure);
}
template<size_t K, typename Closure>
void _loop(Closure& closure) {
    _loop_impl<K, 0>(closure);
}
template<typename My_Tuple>
struct print_tuple {
    constexpr print_tuple(const My_Tuple& tup_) : tup(tup_) {}
    template<size_t I>
    void apply() {
        cout << "\t" << my_get<I>(tup) << "\n";
    }
private:
    const My_Tuple& tup;
};
// создаем простой класс
struct Class_A {
    friend ostream& operator << (ostream& os, const Class_A& ) {
        os << "Class A";
        return os;
    }
};
// создаем айпи фильтр
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
    cout << "Вывод тюпла:\n";
    string str1 = "Arnold Schwarzenegger";
    string str2 = "Sylvester Stallone";
    my_tuple<int, string, double, string, Class_A> tuple_1(17, ref(str1), .02155, str2, Class_A());
    print_tuple closure1(tuple_1);
    _loop<5>(closure1);
    cout << "Вывод make тюпла:\n";
    auto tuple_2 = make_my_tuple(str1 + str2, 2255, cref(str1), Class_A(), move(str2));
    print_tuple closure2(tuple_2);
    _loop<5>(closure2);
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
    cout << "Вывод айпи фильтра:\n";
    for(const auto & x : lit) cout << x << "\n";

    return 0;
}

