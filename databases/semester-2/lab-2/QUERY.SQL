SELECT * FROM Culture WHERE Amount > 30;

SELECT * FROM Farm WHERE Area > 100;

SELECT * FROM Worker WHERE Status = 'Активний';

SELECT * FROM Transaction WHERE Transaction_type = 'Зарплата';

SELECT * FROM Tech WHERE Status = 'Робочий';



SELECT * FROM Culture WHERE Season = 'Весна' AND Amount > 25;

SELECT * FROM Farm WHERE Area > 100 OR Culture_ID < 10;

SELECT * FROM Worker WHERE Status = 'Активний' AND Salary > 10000;

SELECT * FROM Tech WHERE NOT Status = 'Робочий';

SELECT * FROM Transaction WHERE Transaction_type = 'Зарплата' OR Money_transfer > 30000;


SELECT * FROM Culture WHERE Name LIKE 'К%';

SELECT * FROM Farm WHERE Farm_ID LIKE '%1';

SELECT * FROM Worker WHERE Surname LIKE 'Прізвище1_';

SELECT * FROM Tech WHERE Name LIKE '%тор';

SELECT * FROM Transaction WHERE Transaction_type LIKE 'З%а';


SELECT f.Name AS FarmName, c.Name AS CultureName
FROM Farm f
JOIN Culture c ON f.Culture_ID = c.Culture_ID;

SELECT b.Name AS BrigadeName, f.Name AS FarmName
FROM Brigade b
JOIN Farm f ON b.Farm_ID = f.Farm_ID;

SELECT w.Name, w.Surname, b.Name AS BrigadeName
FROM Worker w
JOIN Brigade b ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID;

SELECT t.Name AS TechName, b.Name AS BrigadeName
FROM Tech t
JOIN Brigade b ON t.Brigade_ID = b.Brigade_ID AND t.Farm_ID = b.Farm_ID;

SELECT w.Name, w.Surname, tr.Transaction_type, tr.Money_transfer
FROM Worker w
JOIN Transaction tr ON w.Transaction_ID = tr.Transaction_ID;


SELECT f.Name AS FarmName, c.Name AS CultureName
FROM Farm f
LEFT JOIN Culture c ON f.Culture_ID = c.Culture_ID;

SELECT w.Name, w.Surname, tr.Transaction_type
FROM Worker w
LEFT JOIN Transaction tr ON w.Transaction_ID = tr.Transaction_ID;

SELECT t.Name AS TechName, b.Name AS BrigadeName
FROM Brigade b
RIGHT JOIN Tech t ON t.Brigade_ID = b.Brigade_ID AND t.Farm_ID = b.Farm_ID;

SELECT b.Name AS BrigadeName, w.Name AS WorkerName
FROM Brigade b
LEFT JOIN Worker w ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID;

SELECT c.Name AS CultureName, f.Name AS FarmName
FROM Culture c
RIGHT JOIN Farm f ON f.Culture_ID = c.Culture_ID;


SELECT * FROM Worker
WHERE Salary > (SELECT AVG(Salary) FROM Worker);

SELECT * FROM Farm
WHERE Culture_ID IN (SELECT Culture_ID FROM Culture WHERE Season = 'Весна');

SELECT * FROM Transaction
WHERE Money_transfer > (SELECT MAX(Money_transfer) FROM Transaction WHERE Transaction_type = 'Зарплата');

SELECT Name, Area FROM Farm
WHERE Farm_ID IN (SELECT Farm_ID FROM Brigade WHERE Workers_amount > 20);

SELECT * FROM Tech
WHERE Transaction_ID IN (SELECT Transaction_ID FROM Transaction WHERE Transaction_type = 'Ремонт');


SELECT f.Culture_ID, COUNT(*) AS FarmCount
FROM Farm f
GROUP BY f.Culture_ID
HAVING COUNT(*) > 1;

SELECT w.Position, COUNT(*) AS WorkerCount
FROM Worker w
GROUP BY w.Position
HAVING WorkerCount > 10;

SELECT b.Farm_ID, AVG(b.Workers_amount) AS AvgWorkers
FROM Brigade b
GROUP BY b.Farm_ID
HAVING AvgWorkers > 15;

SELECT t.Status, COUNT(*) AS TechCount
FROM Tech t
GROUP BY t.Status
HAVING COUNT(*) >= 5;

SELECT c.Fertilizers, COUNT(f.Farm_ID) AS FarmCount
FROM Culture c
JOIN Farm f ON c.Culture_ID = f.Culture_ID
GROUP BY c.Fertilizers
HAVING FarmCount > 2;


SELECT c.Season, COUNT(f.Farm_ID) AS FarmCount
FROM Culture c
JOIN Farm f ON f.Culture_ID = c.Culture_ID
GROUP BY c.Season
HAVING FarmCount >= 1;

SELECT w.Name, w.Surname, w.Salary
FROM Worker w
JOIN Brigade b ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID
WHERE w.Salary > (SELECT AVG(Salary) FROM Worker);

SELECT t.Name AS TechName, t.Status, tr.Money_transfer
FROM Tech t
JOIN Transaction tr ON t.Transaction_ID = tr.Transaction_ID
WHERE tr.Money_transfer > 20000;

SELECT f.Name AS FarmName, AVG(w.Salary) AS AvgSalary
FROM Farm f
JOIN Brigade b ON f.Farm_ID = b.Farm_ID
JOIN Worker w ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID
GROUP BY f.Farm_ID
HAVING AvgSalary > 10000;

SELECT c.Name AS CultureName, COUNT(f.Farm_ID) AS FarmCount
FROM Culture c
JOIN Farm f ON f.Culture_ID = c.Culture_ID
WHERE f.Area > 100
GROUP BY c.Culture_ID
HAVING FarmCount >= 2;


SELECT w.Name, w.Surname, b.Name AS BrigadeName
FROM Worker w
JOIN Brigade b ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID
WHERE w.Status = 'Активний';

SELECT f.Name AS FarmName, c.Name AS CultureName
FROM Farm f
JOIN Culture c ON f.Culture_ID = c.Culture_ID
WHERE c.Season = 'Весна';

SELECT t.Name AS TechName, tr.Money_transfer
FROM Tech t
JOIN Transaction tr ON t.Transaction_ID = tr.Transaction_ID
WHERE tr.Money_transfer > 20000;

SELECT MIN(b.Name) AS BrigadeName, COUNT(w.Worker_ID) AS WorkerCount
FROM Brigade b
JOIN Worker w ON w.Brigade_ID = b.Brigade_ID AND w.Farm_ID = b.Farm_ID
WHERE w.Salary > 10000
GROUP BY b.Brigade_ID;

SELECT w.Name, w.Surname, tr.Transaction_type
FROM Worker w
JOIN Transaction tr ON w.Transaction_ID = tr.Transaction_ID
WHERE tr.Transaction_type = 'Зарплата';