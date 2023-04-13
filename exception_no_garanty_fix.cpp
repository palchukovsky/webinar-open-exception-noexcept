#include <iostream>
#include <string>
#include <vector>

class BankAccount {
 public:
  BankAccount(const std::string_view id,
              const unsigned int initialBalance,
              const unsigned limit)
      : m_id{id}, m_balance{initialBalance}, m_limit{limit} {}

  [[nodiscard]] const std::string &id() const { return m_id; }
  [[nodiscard]] unsigned int balance() const { return m_balance; }

  void deposit(const unsigned int amount) {
    checkDepositPossibility(amount);

    m_transactions.push_back(static_cast<int>(amount));
    m_balance += amount;
  }

  void withdraw(const unsigned int amount) {
    checkWithdrawPossibility(amount);

    m_transactions.push_back(-static_cast<int>(amount));
    m_balance -= amount;
  }

  void transfer(const unsigned int amount, BankAccount &recipient) {
    checkWithdrawPossibility(amount);
    recipient.checkDepositPossibility(amount);

    auto myTransactions = m_transactions;
    myTransactions.push_back(-static_cast<int>(amount));

    recipient.m_transactions.push_back(static_cast<int>(amount));

    m_balance -= static_cast<int>(amount);
    recipient.m_balance += static_cast<int>(amount);

    m_transactions = std::move(myTransactions);
  }

 private:
  void checkDepositPossibility(const unsigned int amount) const {
    if (m_balance + amount > m_limit) {
      throw std::runtime_error{"Account has reached its limit"};
    }
  }
  void checkWithdrawPossibility(const unsigned int amount) const {
    if (m_balance < amount) {
      throw std::runtime_error{
          "Insufficient funds to withdraw from the account"};
    }
  }

  std::string m_id;

  unsigned int m_balance;
  unsigned int m_limit;

  std::vector<int> m_transactions;
};

//-----------------------------------------------------------------------------

int main() {
  BankAccount client_1{"Client 1", 100, 150};
  BankAccount client_2{"Client 2", 100, 150};

  const auto &printBalances = [&client_1, &client_2]() {
    std::cout << client_1.id() << ": " << client_1.balance() << ";\t"
              << client_2.id() << ": " << client_2.balance() << ";\t"
              << "Total: " << client_1.balance() + client_2.balance()
              << std::endl
              << std::endl;
  };
  printBalances();

  try {
    auto amount = 30;
    std::cout << "Transferring " << amount << "..." << std::endl;
    client_1.transfer(amount, client_2);
    printBalances();

    amount = 50;
    std::cout << "Transferring " << amount << "..." << std::endl;
    client_1.transfer(amount, client_2);
    printBalances();

  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }

  std::cout << "Final balances:\n";
  printBalances();
}