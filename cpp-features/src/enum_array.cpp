#include <trace.hpp>

#include <array>
#include <type_traits>
#include <string>

#define VALUES = one,two,three,four
using namespace std;
using namespace std::string_literals;

#if (__cplusplus < 201700)
#include "cpp14_stubs.hpp"
#endif

namespace my {
enum class Value : size_t { one, two, three, four, end };

[[maybe_unused]]
#if (__cplusplus >= 201700)
constexpr std::array Values{Value::one, Value::two, Value::three, Value::four};
#else
constexpr auto Values = make_array(Value::one, Value::two, Value::three, Value::four);
#endif

std::ostream& operator<<(std::ostream& os, my::Value v) {
#if (__cplusplus >= 201700)
    constexpr std::array valueNames{"one", "two", "three", "four"};
#else
    constexpr auto valueNames = make_array("one", "two", "three", "four");
#endif

    static_assert(valueNames.size() >= Values.size());
    return os << valueNames[static_cast<std::underlying_type_t<Value>>(v)];
}

} // namespace my

template <my::Value>
constexpr auto toN(my::Value v) -> std::underlying_type<decltype(v)>::type {
    return static_cast<std::underlying_type<decltype(v)>::type>(v);
}



//template <std::size_t... Is>
//void runAll(std::index_sequence<Is...>)
//{
//    using dummy = int[];
//    static_cast<void>(dummy{ 0, (bar<PARAMS[Is]>(), 0)... });
//
//    // (bar<PARAMS[Is]>(), ...); since C++1z
//}


#include <array>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <utility>


// debugging aid
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}

// convert array into a tuple
template<typename Array, std::size_t... I>
constexpr auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
constexpr auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}

// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

template <class ... Ts>
void foo(Ts && ... inputs)
{
    int i = 0;
    ([&] {		// Do things in your "loop" lambda
        if (i++) std::cout << ", ";
        std::cout << inputs;
    } (), ...);
    std::cout << std::endl;
}
template<size_t... inputs>
void bar(std::integer_sequence<size_t, inputs...> seq)
{
    TraceF;
    std::cout << "The sequence of size " << seq.size() << ": ";
    ((std::cout << inputs << ' '), ...);
    std::cout << '\n';
}

/*
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}
*/

template <my::Value v>
void testFoo() {
    log_trace << v;
}

template <int v>
void testFoo() {
    log_trace << "int: " << v;
}

template<typename T, T... values>
void runAll_14(std::integer_sequence<T, values...> seq)
{
//    [[maybe_unused]]
    using unused = int[];
    (void)unused{ 0, (testFoo<values>(), 0)... };
}

template<typename T, T... values>
void runAll(std::integer_sequence<T, values...> seq)
{
    ((testFoo<values>()), ...);
}

template<my::Value ... values>
void runAll()
{
    log_trace << __PRETTY_FUNCTION__;
    ((testFoo<values>()), ...);
}

template<typename Array, typename T, T... values>
void runAll(Array a, std::integer_sequence<T, values...> seq)
{
    ([&] {		// Do things in your "loop" lambda
        constexpr my::Value v = std::get<values>(my::Values);
//        constexpr my::Value v = std::get<values>(a); // error: ‘a’ is not a constant expression
        testFoo<v>();
    } (), ...);
}

//template <class Array>
//void runAll(Array a) {
//    log_trace << __PRETTY_FUNCTION__;
//    constexpr size_t sz = a.size(); //  sizeof(Array)/sizeof(std::remove_reference<Array>::type
////    constexpr size_t sz = my::Values.size(); //  sizeof(Array)/sizeof(std::remove_reference<Array>::type
//    runAll(a, std::make_index_sequence<sz>{});
//}


int main() {
//    print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
//    print_sequence(std::make_integer_sequence<int, 20>{});
//    print_sequence(std::make_index_sequence<10>{});
//    print_sequence(std::index_sequence_for<float, std::iostream, char>{});

    constexpr std::array<int, 4> array = {1, 2, 3, 4};

    // convert an array into a tuple
//    constexpr auto tup = a2t(array);
    constexpr std::tuple<int, int, int, int> tup = a2t(array);
    static_assert(std::is_same_v<decltype(tup), const std::tuple<int, int, int, int>>, "");

    [[maybe_unused]] auto a3 = std::get<3>(array);
    [[maybe_unused]] auto t3 = std::get<3>(tup);

    // print it to cout
    std::cout << "The tuple: " << tup << '\n';
//    foo(std::make_index_sequence<10>{});
    foo(0,1,2,3);
    bar(std::make_index_sequence<10>{});

//    testFoo<my::Value::one>();
//    runAll(std::integer_sequence<my::Value, my::Value::one, my::Value::two, my::Value::three>{});
    runAll(std::make_integer_sequence<int, 4>{});
//    runAll(my::Values);
    runAll(my::Values, std::make_index_sequence<my::Values.size()>{});

    runAll<my::Value::one, my::Value::two, my::Value::three>();
}