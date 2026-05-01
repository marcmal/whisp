export module whisp.util;

export namespace whisp::util
{
using Byte = unsigned char;

template <class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overload(Ts...) -> overload<Ts...>;

}