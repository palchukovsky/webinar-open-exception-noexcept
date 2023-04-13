#include <iostream>
#include <stdexcept>
#include <type_traits>

void callWithoutNoexcept() {
  std::cout << "I DON'T have noexcept!" << std::endl;
}

void callWithNoexcept() noexcept {
  std::cout << "I HAVE noexcept!" << std::endl;
}

//-----------------------------------------------------------------------------

template <typename Func>
void checkNoexcept(const Func& func) {
  if constexpr (std::is_nothrow_invocable_v<Func>) {
    std::cout << "Function is noexcept: ";
  } else {
    std::cout << "Function is not noexcept ";
  }

  func();
}

//-----------------------------------------------------------------------------

int main() {
  checkNoexcept(callWithoutNoexcept);
  checkNoexcept(callWithNoexcept);

  return 0;
}
