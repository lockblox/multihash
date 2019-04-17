#pragma once
#include <iterator>

namespace multihash {

template <typename T>
constexpr T* do_pointer_from(T* p) noexcept {
  return p;
}

template <typename ContiguousIterator>
constexpr auto pointer_from(ContiguousIterator i) noexcept(
    noexcept(do_pointer_from(i)))
    -> decltype(do_pointer_from(i))  // necessary for SFINAE
{
  return do_pointer_from(i);
}

namespace detail {
template <typename, typename = void>
struct contiguous_iterator_impl : std::false_type {};

// Uses void_t from n3911
template <typename I>
struct contiguous_iterator_impl<
    I, std::void_t<decltype(pointer_from(std::declval<I>()))>>
    : std::true_type {};
}  // namespace detail

template <typename I>
using is_contiguous_iterator =
    typename detail::contiguous_iterator_impl<I>::type;
}  // namespace multihash