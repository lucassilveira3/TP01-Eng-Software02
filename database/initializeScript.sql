CREATE TABLE Products (
    id INTEGER PRIMARY KEY NOT NULL,
    name TEXT NOT NULL,
    price REAL NOT NULL CHECK(price >= 0),
    amount INTEGER CHECK(amount >= 0) NOT NULL
);

CREATE TABLE Sells (
    id INTEGER PRIMARY KEY NOT NULL,
    date TIMESTAMP NOT NULL,
    total REAL NOT NULL CHECK(total >= 0)
);

CREATE TABLE SellProducts (
    sellId INT NOT NULL,
    productId INT NOT NULL,
    amount INT NOT NULL CHECK(amount >= 0),
    PRIMARY KEY (sellId, productID),
    FOREIGN KEY (sellId) REFERENCES Sells(id),
    FOREIGN KEY (productId) REFERENCES Products(id)
);

