CREATE DATABASE IF NOT EXISTS farm_db;
USE farm_db;

CREATE TABLE Culture (
    Culture_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Season VARCHAR(50),
    Amount INT,
    Fertilizers VARCHAR(50)
);

CREATE TABLE Farm (
    Farm_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Area INT,
    Culture_ID INT,
    FOREIGN KEY (Culture_ID) REFERENCES Culture(Culture_ID) ON DELETE SET NULL
);

CREATE TABLE Brigade (
    Brigade_ID INT,
    Farm_ID INT,
    Name VARCHAR(50),
    Workers_amount INT,
    PRIMARY KEY (Brigade_ID, Farm_ID),
    FOREIGN KEY (Farm_ID) REFERENCES Farm(Farm_ID)
);

CREATE TABLE Transaction (
    Transaction_ID INT PRIMARY KEY,
    Transaction_Date DATE,
    Money_transfer DECIMAL(10,2),
    Transaction_type VARCHAR(50)
);

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
    FOREIGN KEY (Transaction_ID) REFERENCES Transaction(Transaction_ID) ON DELETE SET NULL
);

-- Таблица Culture
INSERT INTO Culture VALUES (1, 'Пшениця', 'Весна', 100, 'Азот');

-- Таблица Farm
INSERT INTO Farm VALUES (1, 'Ферма A', 500, 1);

-- Таблица Brigade
INSERT INTO Brigade VALUES 
(1, 1, 'Бригада Альфа', 5),
(2, 1, 'Бригада Бета', 4);

-- Таблица Transaction
INSERT INTO Transaction VALUES
(1, '2024-01-10', 10000.00, 'Покупка'),
(2, '2024-01-15', 8000.00, 'Покупка');

-- Таблица Tech
INSERT INTO Tech VALUES
(1, 1, 1, 'Трактор 1', 'B', '2024-01-11', 'В роботі', 1),
(2, 1, 1, 'Трактор 2', 'B', '2024-01-12', 'В роботі', 1),
(3, 2, 1, 'Комбайн 1', 'B', '2024-01-13', 'На ремонті', 2),
(4, 2, 1, 'Комбайн 2', 'A', '2024-01-14', 'На ремонті', 2);

SELECT b.Brigade_ID, b.Name, COUNT(t.Tech_ID) AS TechCount
FROM Tech t
JOIN Brigade b ON t.Brigade_ID = b.Brigade_ID AND t.Farm_ID = b.Farm_ID
WHERE t.Type = 'B'  -- замените на нужный тип
GROUP BY b.Brigade_ID, b.Name
HAVING COUNT(t.Tech_ID) > (
    SELECT AVG(tech_count) FROM (
        SELECT COUNT(Tech_ID) AS tech_count
        FROM Tech
        WHERE Type = 'B'  -- тот же тип
        GROUP BY Brigade_ID, Farm_ID
    ) AS avg_table
);
