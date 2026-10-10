CREATE DATABASE IF NOT EXISTS farm_db;
USE farm_db;
SHOW TABLES;

-- 1. Culture
DROP TABLE IF EXISTS Culture;
CREATE TABLE Culture (
    Culture_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Season VARCHAR(50),
    Amount INT,
    Fertilizers VARCHAR(50)
);

-- 2. Farm
DROP TABLE IF EXISTS Farm;
CREATE TABLE Farm (
    Farm_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Area INT,
    Culture_ID INT,
    FOREIGN KEY (Culture_ID) REFERENCES Culture(Culture_ID) ON DELETE SET NULL
);

-- 3. Brigade
DROP TABLE IF EXISTS Brigade;
CREATE TABLE Brigade (
    Brigade_ID INT,
    Farm_ID INT,
    Name VARCHAR(50),
    Workers_amount INT,
    PRIMARY KEY (Brigade_ID, Farm_ID),
    FOREIGN KEY (Farm_ID) REFERENCES Farm(Farm_ID)
);

-- 4. Transaction
DROP TABLE IF EXISTS Transaction;
CREATE TABLE Transaction (
    Transaction_ID INT PRIMARY KEY,
    Transaction_Date DATE,
    Money_transfer DECIMAL(10,2),
    Transaction_type VARCHAR(50)
);

-- 5. Worker (с Brigadir_ID как внешний ключ на самого себя)
DROP TABLE IF EXISTS Worker;
CREATE TABLE Worker (
    Worker_ID INT,
    Brigade_ID INT,
    Farm_ID INT,
    Brigadir_ID INT,
    Name VARCHAR(50),
    Surname VARCHAR(50),
    Position VARCHAR(50),
    Rescue_Date DATE,
    Salary DECIMAL(10,2),
    Status VARCHAR(50),
    Transaction_ID INT,
    PRIMARY KEY (Worker_ID, Brigade_ID, Farm_ID),
    FOREIGN KEY (Brigade_ID, Farm_ID) REFERENCES Brigade(Brigade_ID, Farm_ID),
    FOREIGN KEY (Transaction_ID) REFERENCES Transaction(Transaction_ID) ON DELETE SET NULL,
    FOREIGN KEY (Brigadir_ID) REFERENCES Worker(Worker_ID) ON DELETE SET NULL
);

-- 6. Tech
DROP TABLE IF EXISTS Tech;
CREATE TABLE Tech (
    Tech_ID INT,
    Brigade_ID INT,
    Farm_ID INT,
    Name VARCHAR(50),
    Type VARCHAR(50),
    Purchase_Date DATE,
    Status VARCHAR(50),
    Transaction_ID INT,
    PRIMARY KEY (Tech_ID, Brigade_ID, Farm_ID),
    FOREIGN KEY (Brigade_ID, Farm_ID) REFERENCES Brigade(Brigade_ID, Farm_ID),
    FOREIGN KEY (Transaction_ID) REFERENCES Transaction(Transaction_ID) ON DELETE SET NULL
);
