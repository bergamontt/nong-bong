CREATE TABLE currency (
  code TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  minor_unit INTEGER NOT NULL DEFAULT 100
);

CREATE TABLE exchange_rate (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  base_currency TEXT NOT NULL REFERENCES currency(code),
  target_currency TEXT NOT NULL REFERENCES currency(code),
  rate REAL NOT NULL,
  UNIQUE (base_currency, target_currency)
);

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

CREATE TABLE card_design (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  author TEXT,
  image_ref TEXT NOT NULL
);

CREATE TABLE card (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  user_id INTEGER NOT NULL REFERENCES user(id),
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