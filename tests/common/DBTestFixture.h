#pragma once
#include "DataSource.h"
#include <fstream>
#include <sstream>
#include <soci/sqlite3/soci-sqlite3.h>

class DBTestFixture
{
public:
    DBTestFixture();
    soci::connection_pool& pool();
private:
    DataSource _ds;
    static void createSchema(soci::session& sql);
};

inline DBTestFixture::DBTestFixture()
    :  _ds(":memory:", soci::sqlite3, 1)
{
    soci::session& sql = _ds.pool().at(0);
    createSchema(sql);
}

inline void DBTestFixture::createSchema(soci::session& sql)
{
    sql << R"(
        CREATE TABLE currency (
            code TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            minor_unit INTEGER NOT NULL DEFAULT 100
        );
    )";

    sql << R"(
        CREATE TABLE exchange_rate (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            base_currency TEXT NOT NULL REFERENCES currency(code),
            target_currency TEXT NOT NULL REFERENCES currency(code),
            rate REAL NOT NULL,
            UNIQUE (base_currency, target_currency)
        );
    )";

    sql << R"(
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

    sql << R"(
        CREATE TABLE card_design (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            author TEXT,
            image_ref TEXT NOT NULL
        );
    )";

    sql << R"(
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

    sql << R"(
        CREATE TABLE scheduled_transfer (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            from_card_id INTEGER NOT NULL REFERENCES card(id),
            to_card_id INTEGER NOT NULL REFERENCES card(id),
            amount INTEGER NOT NULL CHECK (amount > 0),
            currency_code TEXT NOT NULL REFERENCES currency(code),
            frequency TEXT NOT NULL CHECK (frequency IN ('daily','weekly','monthly')),
            next_tun DATETIME,
            active INTEGER NOT NULL DEFAULT 1,
            description TEXT NOT NULL,
            comment TEXT
        );
    )";

    sql << R"(
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

inline soci::connection_pool& DBTestFixture::pool()
{
    return _ds.pool();
}
