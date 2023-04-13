#include <iostream>
#include <system_error>
#include <vector>

enum class ErrorCode {
  NoError = 0,
  EmptyDataSource,
  DataSourcHasNegativeValue,
  DataSourceTooBig,
};

class ErrorCategory : public std::error_category {
 public:
  [[nodiscard]] const char *name() const noexcept override {
    return "AppErrors";
  }

  [[nodiscard]] std::string message(int condition) const override;
};

std::string ErrorCategory::message(const int condition) const {
  switch (static_cast<ErrorCode>(condition)) {
    case ErrorCode::NoError:
      return "No error";
    case ErrorCode::EmptyDataSource:
      return "Empty data";
    case ErrorCode::DataSourcHasNegativeValue:
      return "Data has negative value";
    case ErrorCode::DataSourceTooBig:
      return "Data is too big to calculate";
    default:
      return "Unknown error";
  }
}

std::error_condition makeErrorCondition(const ErrorCode errorCode) {
  static const ErrorCategory errorCategory;
  return std::error_condition{static_cast<int>(errorCode), errorCategory};
}

//-----------------------------------------------------------------------------

double calc_average(const std::vector<int> &data, std::error_condition &err);

int main() {
  std::error_condition err = makeErrorCondition(ErrorCode::NoError);

  const std::vector<int> sourceWithData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const std::vector<int> sourceEmpty;
  const std::vector<int> sourceWithNegative = {1, 2, 3, 4, -5, 6, 7, 8, 9, 10};

  auto result = calc_average(sourceWithData, err);
  if (err) {
    std::cerr << R"(Error with "sourceWithData": )" << err.message()
              << std::endl;
  } else {
    std::cout << R"("sourceWithData" result: )" << result << std::endl;
  }

  result = calc_average(sourceEmpty, err);
  if (err) {
    std::cerr << R"(Error with "sourceEmpty": )" << err.message() << std::endl;
  } else {
    std::cout << R"("sourceEmpty" result: )" << result << std::endl;
  }

  result = calc_average(sourceWithNegative, err);
  if (err) {
    std::cerr << R"(Error with "sourceWithNegative": )" << err.message()
              << std::endl;
  } else {
    std::cout << R"("sourceWithNegative" result: )" << result << std::endl;
  }

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------

double calc_average(const std::vector<int> &data, std::error_condition &err) {
  if (data.empty()) {
    err = makeErrorCondition(ErrorCode::EmptyDataSource);
    return {};
  }

  if (data.size() > 100) {
    err = makeErrorCondition(ErrorCode::DataSourceTooBig);
    return {};
  }

  double result = 0;
  for (const auto &i : data) {
    if (i < 0) {
      err = makeErrorCondition(ErrorCode::DataSourcHasNegativeValue);
      return {};
    }

    result += i;
  }

  return result;
}
