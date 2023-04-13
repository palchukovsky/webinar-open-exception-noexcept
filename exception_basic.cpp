#include <exception>
#include <iostream>
#include <vector>

/*
  Previous example used:

  enum class ErrorCode {
    NoError = 0,
    EmptyDataSource,
    DataSourcHasNegativeValue,
    DataSourceTooBig,
  };
*/

class Exception : public std::exception {
 public:
  [[nodiscard]] const char *what() const noexcept override {
    return "App general error";
  }
};

class EmptyDataSourceException : public Exception {
 public:
  [[nodiscard]] const char *what() const noexcept override {
    return "Data source is emtpy";
  }
};

class DataSourcHasNegativeValueException : public Exception {
 public:
  [[nodiscard]] const char *what() const noexcept override {
    return "Data source has negative value";
  }
};

//-----------------------------------------------------------------------------

double calc_average(const std::vector<int> &data);  // no any additional args!

int main() {
  const std::vector<int> sourceWithData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const std::vector<int> sourceEmpty;
  const std::vector<int> sourceWithNegative = {1, 2, 3, 4, -5, 6, 7, 8, 9, 10};

  try {
    const auto result = calc_average(sourceWithData);
    std::cout << R"("sourceWithData" result: )" << result << std::endl;
  } catch (const Exception &ex) {
    std::cerr << R"(Error with "sourceWithData": )" << ex.what() << std::endl;
  }

  try {
    const auto result = calc_average(sourceEmpty);
    std::cout << R"("sourceEmpty" result: )" << result << std::endl;
  } catch (const Exception &ex) {
    std::cerr << R"(Error with "sourceEmpty": )" << ex.what() << std::endl;
  }

  try {
    const auto result = calc_average(sourceWithNegative);
    std::cout << R"("sourceWithNegative" result: )" << result << std::endl;
  } catch (const Exception &ex) {
    std::cerr << R"(Error with "sourceWithNegative": )" << ex.what()
              << std::endl;
  }

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------

double calc_average(const std::vector<int> &data) {
  if (data.empty()) {
    throw EmptyDataSourceException();
  }

  if (data.size() > 100) {
    throw Exception();
  }

  double result = 0;
  for (const auto &i : data) {
    if (i < 0) {
      throw DataSourcHasNegativeValueException();
    }

    result += i;
  }

  return result;
}
