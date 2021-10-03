CREATE TABLE transactions(
  id INTEGER PRIMARY KEY NOT NULL,
  user_id INTEGER NOT NULL,
  transaction_type TEXT NOT NULL,
  date DATETIME NOT NULL,
  symbol TEXT NOT NULL,
  price NUMERIC NOT NULL,
  shares INTEGER NOT NULL,
  FOREIGN KEY(user_id) REFERENCES users(id)
);