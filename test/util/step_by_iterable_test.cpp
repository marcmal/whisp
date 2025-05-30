
#include "util/step_by_iterable.hpp"
#include <gtest/gtest.h>
#include <ranges>

namespace whisp
{

namespace
{
template <typename It, std::size_t N, typename Expected>
concept StepByDerefReturns = std::same_as<decltype(*std::declval<step_by::Iterator<It, N>>()), Expected>;

static_assert(StepByDerefReturns<std::vector<int>::iterator, 4, int&>);
static_assert(StepByDerefReturns<std::vector<int>::const_iterator, 4, const int&>);

template <typename It, std::size_t N, typename Expected>
concept StepByBaseReturns = std::same_as<decltype(std::declval<step_by::Iterator<It, N>>().base()), Expected>;
static_assert(StepByBaseReturns<std::vector<int>::iterator, 4, std::vector<int>::iterator>);
static_assert(StepByBaseReturns<std::vector<int>::const_iterator, 4, std::vector<int>::const_iterator>);

template <typename It, typename Sent, std::size_t N, typename Expected>
concept StepByCursorNextReturns = std::same_as<decltype(std::declval<step_by::Cursor<It, Sent, N>>().next()), Expected>;
static_assert(StepByCursorNextReturns<std::vector<int>::iterator, std::vector<int>::iterator, 4, std::optional<int*>>);
static_assert(StepByCursorNextReturns<std::vector<int>::const_iterator,
                                      std::vector<int>::const_iterator,
                                      4,
                                      std::optional<const int*>>);

template <typename T, std::size_t N, typename ExpectedIt>
concept StepByIterableBeginReturns =
    std::same_as<decltype(step_by::makeIterable<N>(std::declval<T&>()).begin()), ExpectedIt>;

static_assert(StepByIterableBeginReturns<std::vector<int>, 4, step_by::Iterator<std::vector<int>::iterator, 4>>);
static_assert(
    StepByIterableBeginReturns<const std::vector<int>, 4, step_by::Iterator<std::vector<int>::const_iterator, 4>>);

template <typename T, std::size_t N, typename ExpectedSent>
concept StepByIterableEndReturns =
    std::same_as<decltype(step_by::makeIterable<N>(std::declval<T&>()).end()), ExpectedSent>;

static_assert(StepByIterableEndReturns<std::vector<int>, 4, step_by::Sentinel<std::vector<int>::iterator>>);
static_assert(StepByIterableEndReturns<const std::vector<int>, 4, step_by::Sentinel<std::vector<int>::const_iterator>>);

template <typename T, std::size_t N, typename ExpectedCursor>
concept StepByIterableCursorNextReturns =
    std::same_as<decltype(step_by::makeIterable<N>(std::declval<T&>()).cursor().next()), ExpectedCursor>;
static_assert(StepByIterableCursorNextReturns<std::vector<int>, 4, std::optional<int*>>);
static_assert(StepByIterableCursorNextReturns<const std::vector<int>, 4, std::optional<const int*>>);
}

TEST(StepByIterableTest, IterateOverStepByIterable)
{
    const std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const auto stepIterable = step_by::makeIterable<2>(vec);

    std::vector<int> result{};
    result.reserve(vec.size());

    for (const auto val : stepIterable)
    {
        result.push_back(val);
    }

    const std::vector<int> expected{10, 30, 50};
    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, ModifyElementsWhileIteratingOverStepByIterable)
{
    std::vector<int> vec{10, 20, 30, 40, 50, 60};
    auto stepIterable = step_by::makeIterable<1>(vec);

    for (auto&& val : stepIterable)
    {
        val += 10;
    }

    const std::vector<int> result = stepIterable | std::ranges::to<std::vector>();
    const std::vector<int> expected{20, 30, 40, 50, 60, 70};
    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, ConvertVectorToStepByIterableAndBackward)
{
    const std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const auto stepIterable = step_by::makeIterable<2>(vec);

    const std::vector<int> result = stepIterable | std::ranges::to<std::vector>();
    const std::vector<int> expected{10, 30, 50};

    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, ConvertSpanToStepByIterableAndBackward)
{
    const std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const std::span<const int> span{vec.begin(), vec.end()};

    const auto stepIterable = step_by::makeIterable<3>(span);

    const std::vector<int> result = stepIterable | std::ranges::to<std::vector>();
    const std::vector<int> expected{10, 40};

    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, ConvertViewToStepByIterableAndBackToVector)
{
    const std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const auto view = vec | std::views::transform([](const auto val) { return val * val; });
    const auto stepIterable = step_by::makeIterable<4>(view);

    const std::vector<int> result = stepIterable | std::ranges::to<std::vector>();
    const std::vector<int> expected{100, 2500};

    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, IterateUsingCursor)
{
    const std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const auto stepIterable = step_by::makeIterable<2>(vec);
    auto cursor = stepIterable.cursor();

    auto val = cursor.next();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val.value(), 10);

    val = cursor.next();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val.value(), 30);

    val = cursor.next();
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(*val.value(), 50);

    val = cursor.next();
    ASSERT_FALSE(val.has_value());
}

TEST(StepByIterableTest, ModifyElementsUsingCursor)
{
    std::vector<int> vec{10, 20, 30, 40, 50, 60};
    const auto stepIterable = step_by::makeIterable<2>(vec);
    auto cursor = stepIterable.cursor();

    auto val = cursor.next();
    ASSERT_TRUE(val.has_value());
    *val.value() += 10;

    val = cursor.next();
    ASSERT_TRUE(val.has_value());
    *val.value() += 10;

    val = cursor.next();
    ASSERT_TRUE(val.has_value());
    *val.value() += 10;

    val = cursor.next();
    ASSERT_FALSE(val.has_value());

    const std::vector<int> result = stepIterable | std::ranges::to<std::vector>();
    const std::vector<int> expected{20, 40, 60};
    EXPECT_EQ(result, expected);
}

TEST(StepByIterableTest, CalculateSize)
{
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
        step_by::Iterable<decltype(v), 1> iterable(v);
        EXPECT_EQ(iterable.size(), 7);
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
        step_by::Iterable<decltype(v), 2> iterable(v);
        EXPECT_EQ(iterable.size(), 4);
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
        step_by::Iterable<decltype(v), 3> iterable(v);
        EXPECT_EQ(iterable.size(), 3);
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
        step_by::Iterable<decltype(v), 4> iterable(v);
        EXPECT_EQ(iterable.size(), 2);
    }
}

}