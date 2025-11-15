
# 💸 Nong Bong

Educational desktop application for flexible management of your bank account and card transactions



![C++](https://img.shields.io/badge/C%2B%2B-20-AAA694) ![Qt](https://img.shields.io/badge/Qt-6.10-EBF8B8) ![SOCI](https://img.shields.io/badge/SOCI-v4.1.2-ffe787) ![doctest](https://img.shields.io/badge/doctest-v2.4.10-db7c26) ![sqlite](https://img.shields.io/badge/sqlite-3.5.0-559cad)




## 📜 Features

- User-friendly graphical interface
- The ability to open different types of accounts (debit and credit) in different currencies (hryvnia, euros, dollars)
- Replenishing your card with cash
- Withdrawing money from your account
- Selecting a card design
- Ability to view transaction history
- Automatic regular transfers between accounts, payment for services


## 🗂️ Test Accounts

You can register your own account in the application, but if necessary, you can use existing test accounts:

+380 (11) 111-1111: nazar123

    Account cards and its pins:

    5375410012345678 - 1234
    5375410087654321 - 4321
    4149600098765432 - 5678

+380 (22) 222-2222 iryna123

    Account cards and its pins:

    4893470011223344 - 1111

+380 (33) 333-3333 daria123

    Account cards and its pins:

    4893470099887766 - 2222
    4149600066554433 - 9999

## 🛠️ Run Locally

To run the program, you need to have the latest version of Qt installed and the PATH variable configured.

Clone the project

```bash
  git clone https://github.com/bergamontt/nong-bong.git
  cd nong-bong
```

Create build folder

```bash
  mkdir build
  cd build
```

Build with cmake
```bash
  cmake ..
  cmake --build .
```

