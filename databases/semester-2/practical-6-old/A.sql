CREATE DATABASE IF NOT EXISTS supply_db;
USE supply_db;

CREATE TABLE S (
    S_id CHAR(5) PRIMARY KEY,
    SNAME VARCHAR(20),
    STATUS INT,
    CITY VARCHAR(15)
);

CREATE TABLE P (
    P_id CHAR(5) PRIMARY KEY,
    PNAME VARCHAR(20),
    COLOR VARCHAR(10),
    WEIGHT INT,
    CITY VARCHAR(15)
);

CREATE TABLE J (
    J_id CHAR(4) PRIMARY KEY,
    JNAME VARCHAR(20),
    CITY VARCHAR(15)
);

CREATE TABLE SPJ (
    S_id CHAR(5),
    P_id CHAR(5),
    J_id CHAR(4),
    QTY INT,
    PRIMARY KEY (S_id, P_id, J_id),
    FOREIGN KEY (S_id) REFERENCES S(S_id),
    FOREIGN KEY (P_id) REFERENCES P(P_id),
    FOREIGN KEY (J_id) REFERENCES J(J_id)
);

-- S: Постачальники
INSERT INTO S VALUES
('S1', 'Шевченко', 20, 'Київ'),
('S2', 'Франко', 15, 'Львів'),
('S3', 'Костенко', 25, 'Одеса'),
('S4', 'Сосюра', 20, 'Харків'),
('S5', 'Ліна', 25, 'Дніпро');

-- P: Товари
INSERT INTO P VALUES
('P1', 'Кухня', 'Червоний', 200, 'Київ'),
('P2', 'Спальня', 'Білий', 150, 'Львів'),
('P3', 'Стілець', 'Метал', 20, 'Харків'),
('P4', 'Диван', 'Синій', 90, 'Одеса'),
('P5', 'Диван', 'Зелений', 80, 'Дніпро');

-- J: Проєкти
INSERT INTO J VALUES
('J1', 'Школи', 'Львів'),
('J2', 'Офіси', 'Київ'),
('J3', 'Пошта', 'Харків'),
('J4', 'Магазини', 'Одеса'),
('J5', 'Заводи', 'Дніпро');

-- SPJ: Постачання
INSERT INTO SPJ VALUES
('S1', 'P1', 'J1', 300),
('S1', 'P1', 'J4', 200),
('S2', 'P1', 'J1', 400),
('S2', 'P3', 'J3', 200),
('S3', 'P3', 'J5', 100),
('S4', 'P1', 'J1', 120),
('S5', 'P4', 'J2', 60);

-- 1. Повна інформація про всі проекти в Дніпрі
SELECT * FROM J WHERE CITY = 'Дніпро';

-- 2. Поєднання “ім'я проекту — місто проекту”
SELECT JNAME, CITY FROM J;

-- 3. Імена товарів, ім'я яких складає 5 символів
SELECT PNAME FROM P WHERE CHAR_LENGTH(PNAME) = 5;

-- 4. Міста постачальників, статус яких дорівнює 20
SELECT CITY FROM S WHERE STATUS = 20;

-- 5. Імена постачальників з Харкова
SELECT SNAME FROM S WHERE CITY = 'Харків';

-- 6. Імена товарів та їх вага, які постачаються постачальниками S2 та Шевченко
SELECT DISTINCT P.PNAME, P.WEIGHT
FROM S
JOIN SPJ ON S.S_id = SPJ.S_id
JOIN P ON SPJ.P_id = P.P_id
WHERE S.S_id = 'S2' OR S.SNAME = 'Шевченко';

-- 7. Імена товарів, що постачаються постачальником зі статусом >15 та ім'ям, що починається на 'С'
SELECT DISTINCT P.PNAME
FROM S
JOIN SPJ ON S.S_id = SPJ.S_id
JOIN P ON SPJ.P_id = P.P_id
WHERE S.STATUS > 15 AND S.SNAME LIKE 'С%';

-- 8. Номери проектів, в які постачаються товари з Харкова або постачальником з Харкова
SELECT DISTINCT SPJ.J_id
FROM SPJ
JOIN P ON SPJ.P_id = P.P_id
WHERE P.CITY = 'Харків'
UNION
SELECT DISTINCT SPJ.J_id
FROM SPJ
JOIN S ON SPJ.S_id = S.S_id
WHERE S.CITY = 'Харків';

-- 9. Імена товарів, які поставляються Шевченком у кількості більше 200
SELECT DISTINCT P.PNAME
FROM S
JOIN SPJ ON S.S_id = SPJ.S_id
JOIN P ON SPJ.P_id = P.P_id
WHERE S.SNAME = 'Шевченко' AND SPJ.QTY > 200;