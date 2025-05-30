#include <iterator>
#include <optional>
#include <ranges>
#include <utility>

namespace step_by
{
namespace detail
{
template <std::size_t Step>
concept ValidStep = Step > 0;
}

template <typename It, std::size_t Step>
    requires std::forward_iterator<It> && detail::ValidStep<Step>
class Iterator
{
  public:
    using value_type = std::iter_value_t<It>;
    using difference_type = std::iter_difference_t<It>;
    using iterator_category = typename std::iterator_traits<It>::iterator_category;
    using iterator_concept = std::forward_iterator_tag;

    Iterator() = default;
    Iterator(It current, It end) : current{current}, end{end}
    {
    }

    decltype(auto) operator*() const
    {
        return *current;
    }

    Iterator& operator++()
    {
        std::ranges::advance(current, Step, end);
        return *this;
    }

    Iterator operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    It base() const
    {
        return current;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs)
    {
        return lhs.current == rhs.current;
    }

  private:
    It current;
    It end;
};

template <typename Sent>
class Sentinel
{
  public:
    Sentinel() = default;
    explicit Sentinel(Sent end) : end{end}
    {
    }

    template <std::forward_iterator It, std::size_t Step>
    friend bool operator==(const Iterator<It, Step>& iter, const Sentinel& sent)
    {
        return iter.base() == sent.end;
    }

    template <std::forward_iterator It, std::size_t Step>
    friend bool operator==(const Sentinel& sent, const Iterator<It, Step>& iter)
    {
        return iter == sent;
    }

    template <std::forward_iterator It, std::size_t Step>
    friend bool operator!=(const Iterator<It, Step>& iter, const Sentinel& sent)
    {
        return !(iter == sent);
    }

    template <std::forward_iterator It, std::size_t Step>
    friend bool operator!=(const Sentinel& sent, const Iterator<It, Step>& iter)
    {
        return !(iter == sent);
    }

  private:
    Sent end;
};

template <typename It, typename Sent, std::size_t Step>
    requires detail::ValidStep<Step>
class Cursor
{
  public:
    using value_type = std::iter_value_t<It>;
    using reference_type = std::iter_reference_t<It>;
    using pointer_type = std::add_pointer_t<reference_type>;

    Cursor(It current, Sent end) : current{current}, end{end}
    {
    }

    std::optional<pointer_type> next()
    {
        if (current == end)
        {
            return std::nullopt;
        }

        auto* result = std::to_address(current);
        std::ranges::advance(current, Step, end);
        return result;
    }

  private:
    It current;
    Sent end;
};

template <typename Range, std::size_t Step>
    requires std::ranges::forward_range<Range>
class Iterable
{
  public:
    Iterable() = default;
    explicit Iterable(Range base) : base{std::move(base)}
    {
    }

    auto begin()
    {
        return Iterator<std::ranges::iterator_t<Range>, Step>(std::ranges::begin(base), std::ranges::end(base));
    }

    auto end()
    {
        return Sentinel<std::ranges::sentinel_t<Range>>(std::ranges::end(base));
    }

    auto begin() const
    {
        return Iterator<std::ranges::iterator_t<const Range>, Step>(std::ranges::begin(base), std::ranges::end(base));
    }

    auto end() const
    {
        return Sentinel<std::ranges::sentinel_t<const Range>>(std::ranges::end(base));
    }

    auto cursor() const
    {
        return Cursor<std::ranges::iterator_t<const Range>, std::ranges::sentinel_t<const Range>, Step>(
            std::ranges::begin(base), std::ranges::end(base));
    }

    auto size() const
        requires std::ranges::sized_range<Range>
    {
        return (std::ranges::size(base) + Step - 1) / Step;
    }

  private:
    Range base;
};

template <std::size_t Step, std::ranges::range Range>
auto makeIterable(Range&& r)
{
    return Iterable<std::views::all_t<Range>, Step>(std::views::all(std::forward<Range>(r)));
}

}
