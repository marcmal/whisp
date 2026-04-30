export module whisp.util;

export namespace whisp::util
{
using Byte = unsigned char;
}

export template <class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

export template <class... Ts>
overload(Ts...) -> overload<Ts...>;
