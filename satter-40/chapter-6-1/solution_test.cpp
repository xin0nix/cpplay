#include <gtest/gtest.h>

#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

namespace detail {

template <typename T, typename Enable = void>
struct is_iterator : std::false_type {};

template <typename T>
struct is_iterator<
    T, std::void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type {};

template <typename T> void destroy(T *p) { p->~T(); }

// template <typename FwdIter> // Uncomment for example with PP
template <typename FwdIter,
          typename Enable = std::enable_if_t<is_iterator<FwdIter>::value>>
void destroy(FwdIter first, FwdIter last) {
  while (first != last) {
    if constexpr (std::is_pointer_v<FwdIter>) {
      // called for the ducks and the duckList
      ::detail::destroy(first);
    } else {
      // called for the backup
      ::detail::destroy(&(*first));
    }
    ++first;
  }
}
} // namespace detail

struct Duck {
  std::string name;
};

TEST(DestroyTest, Basic1) {
  std::vector<Duck> ducks{{"Alla"}, {"Violetta"}};
  std::vector<Duck> backup{{"Alla"}, {"Violetta"}};
  std::list<Duck> duckList{{"Alla"}, {"Violetta"}};
  ::detail::destroy(ducks.begin(), ducks.end());
  ::detail::destroy(backup.data(), backup.data() + backup.size());
  ::detail::destroy(duckList.begin(), duckList.end());
  struct PP {
    [[gnu::noinline]] PP &operator*() { return *this; }
    [[gnu::noinline]] PP *operator&() { return this; }
    [[gnu::noinline]] bool operator!=(const PP &) { return false; }
    [[gnu::noinline]] PP &operator++() { return *this; }
  } pp;
  // Must not compile
  // ::detail::destroy(pp, pp); // Uncomment for example with PP
}
