
#include <iostream>
#include <optional>

class Way {
 public:
  Way(const int startX, const int startY) noexcept
      : m_startX(startX), m_startY(startY) {}

  [[nodiscard]] int getStartX() const noexcept {
    // Always available, so could be noexcept!
    return m_startX;
  }

  [[nodiscard]] int getStartY() const noexcept {
    // Always available, so could be noexcept!
    return m_startY;
  }

  [[nodiscard]] int getFinishX() const {
    if (!m_finishX.has_value()) {
      throw std::runtime_error{"Finish X-coordinate is not set"};
    }

    return m_finishX.value();
  }

  [[nodiscard]] int getFinishY() const {
    if (!m_finishY.has_value()) {
      throw std::runtime_error{"Finish X-coordinate is not set"};
    }

    return m_finishY.value();
  }

  void setFinish(const int x, const int y) {
    m_finishX = x;
    m_finishY = y;  // what about exception safety here?
  }

 private:
  int m_startX;
  int m_startY;

  std::optional<int> m_finishX;
  std::optional<int> m_finishY;
};

//-----------------------------------------------------------------------------

int main() {
  try {
    Way way{1, 2};

    std::cout << "Start: " << way.getStartX() << " / " << way.getStartY()
              << std::endl;
    way.setFinish(10, 20);
    std::cout << "Finish: " << way.getFinishX() << " / " << way.getFinishY()
              << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }
  return 0;
}