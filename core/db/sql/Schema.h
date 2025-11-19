#pragma once

namespace schema_sql
{
    inline const auto createCurrency =
        R"(
            CREATE TABLE currency (
                code TEXT PRIMARY KEY,
                name TEXT NOT NULL,
                minor_unit INTEGER NOT NULL DEFAULT 100
            );
        )";

    inline const auto createExchangeRate =
        R"(
            CREATE TABLE exchange_rate (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                base_currency TEXT NOT NULL REFERENCES currency(code),
                target_currency TEXT NOT NULL REFERENCES currency(code),
                rate REAL NOT NULL,
                UNIQUE (base_currency, target_currency)
            );
        )";

    inline const auto createUser =
        R"(
            CREATE TABLE user (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                first_name TEXT NOT NULL,
                last_name TEXT NOT NULL,
                email TEXT UNIQUE,
                phone TEXT NOT NULL UNIQUE,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                password_hash TEXT NOT NULL,
                status TEXT NOT NULL DEFAULT 'active' CHECK (status IN ('active','blocked','deleted')),
                failed_login_count INTEGER NOT NULL DEFAULT 0,
                blocked_until DATETIME
            );
        )";

    inline const auto createCardDesign =
        R"(
            CREATE TABLE card_design (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                author TEXT,
                image_ref TEXT NOT NULL
            );
        )";

    inline const auto createCard =
        R"(
            CREATE TABLE card (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER NOT NULL REFERENCES users(id),
                card_number TEXT NOT NULL UNIQUE,
                allow_credit INTEGER NOT NULL DEFAULT 0,
                credit_limit INTEGER,
                currency_code TEXT NOT NULL REFERENCES currency(code),
                balance INTEGER NOT NULL DEFAULT 0,
                daily_limit INTEGER NOT NULL DEFAULT 0,
                design_id INTEGER REFERENCES card_design(id),
                pin_hash TEXT NOT NULL,
                status TEXT NOT NULL DEFAULT 'active' CHECK (status IN ('active','blocked','deleted')),
                failed_access_count INTEGER NOT NULL DEFAULT 0,
                blocked_until DATETIME
            );
        )";

    inline const auto createScheduledTransfer =
        R"(
            CREATE TABLE scheduled_transfer (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                from_card_id INTEGER NOT NULL REFERENCES card(id),
                to_card_id INTEGER NOT NULL REFERENCES card(id),
                amount INTEGER NOT NULL CHECK (amount > 0),
                currency_code TEXT NOT NULL REFERENCES currency(code),
                frequency TEXT NOT NULL CHECK (frequency IN ('daily','weekly','monthly')),
                next_run DATETIME,
                active INTEGER NOT NULL DEFAULT 1,
                description TEXT NOT NULL,
                comment TEXT
            );
        )";

    inline const auto createBankTransaction =
        R"(
            CREATE TABLE bank_transaction (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
                type TEXT NOT NULL CHECK (type IN ('deposit','withdrawal','transfer','payment')),
                from_card_id INTEGER REFERENCES card(id),
                to_card_id INTEGER REFERENCES card(id),
                amount INTEGER NOT NULL CHECK (amount > 0),
                currency_code TEXT NOT NULL REFERENCES currency(code),
                description TEXT NOT NULL,
                comment TEXT,
                status TEXT NOT NULL DEFAULT 'completed' CHECK (status IN ('completed','failed'))
            );
        )";
}