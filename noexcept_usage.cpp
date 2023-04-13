#include <iostream>
#include <string>
#include <vector>

size_t notSafeObjectCtors = 0;
size_t notSafeObjectMoves = 0;

size_t safeObjectCtors = 0;
size_t safeObjectMoves = 0;

class RecordNotSafe {
 public:
  explicit RecordNotSafe(std::string data)
      : m_data{new std::string{std::move(data)}} {
    ++notSafeObjectCtors;

    std::cout << "NOT safe Ctor +1: " << notSafeObjectCtors
              << ", Total: " << (notSafeObjectCtors + notSafeObjectMoves)
              << std::endl;
  }

  ~RecordNotSafe() { delete m_data; }

  RecordNotSafe &operator=(const RecordNotSafe &) = delete;
  RecordNotSafe &operator=(RecordNotSafe &&rhs) = delete;

  RecordNotSafe(const RecordNotSafe &rhs) : RecordNotSafe{*rhs.m_data} {}

  RecordNotSafe(RecordNotSafe &&rhs) : m_data{rhs.m_data} {
    rhs.m_data = nullptr;

    ++notSafeObjectMoves;

    std::cout << "NOT safe Move +1: " << notSafeObjectMoves
              << ", Total: " << (notSafeObjectCtors + notSafeObjectMoves)
              << std::endl;
  }

 private:
  const std::string *m_data;
};

//-----------------------------------------------------------------------------

class RecordSafe {
 public:
  explicit RecordSafe(std::string data)
      : m_data{new std::string{std::move(data)}} {
    ++safeObjectCtors;

    std::cout << "SAFE Ctor +1: " << safeObjectCtors
              << ", Total: " << (safeObjectCtors + safeObjectMoves)
              << std::endl;
  }

  ~RecordSafe() { delete m_data; }

  RecordSafe &operator=(const RecordSafe &) = delete;
  RecordSafe &operator=(RecordSafe &&rhs) = delete;

  RecordSafe(const RecordSafe &rhs) : RecordSafe{*rhs.m_data} {}

  RecordSafe(RecordSafe &&rhs) noexcept : m_data{rhs.m_data} {
    rhs.m_data = nullptr;

    ++safeObjectMoves;

    std::cout << "SAFE Move +1: " << safeObjectMoves
              << ", Total: " << (safeObjectCtors + safeObjectMoves)
              << std::endl;
  }

 private:
  const std::string *m_data;
};

//-----------------------------------------------------------------------------

int main() {
  {
    std::vector<RecordNotSafe> notSafe;
    std::vector<RecordSafe> safe;

    for (int i = 0; i < 25; ++i) {
      auto value = std::to_string(i);

      notSafe.emplace_back(value);
      safe.emplace_back(std::move(value));
    }
  }

  std::cout << "Finally:\n"
            << "\tNOT safe: constructor " << notSafeObjectCtors << " + moves "
            << notSafeObjectMoves << " = "
            << (notSafeObjectCtors + notSafeObjectMoves)
            << "\n"
               "\tSAFE: constructor "
            << safeObjectCtors << " + moves " << safeObjectMoves << " = "
            << (safeObjectCtors + safeObjectMoves) << std::endl;

  return 0;
}