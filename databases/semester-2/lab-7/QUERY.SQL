-- 1
SELECT * 
FROM [Culture] 
WHERE [Amount] > 30;

-- 2
SELECT * 
FROM [Farm] 
WHERE [Area] > 100;

-- 3
SELECT * 
FROM [Worker] 
WHERE [Status] = 'Active';

-- 4
SELECT * 
FROM [Transaction] 
WHERE [Transaction_type] = 'Salary';

-- 5
SELECT * 
FROM [Tech] 
WHERE [Status] = 'Working';

-- 6
SELECT * 
FROM [Culture] 
WHERE [Season] = 'Spring' 
  AND [Amount] > 25;

-- 7
SELECT * 
FROM [Farm] 
WHERE [Area] > 100 
   OR [Culture_ID] < 10;

-- 8
SELECT * 
FROM [Worker] 
WHERE [Status] = 'Active' 
  AND [Salary] > 10000;

-- 9
SELECT * 
FROM [Tech] 
WHERE [Status] <> 'Working';

-- 10
SELECT * 
FROM [Transaction] 
WHERE [Transaction_type] = 'Salary' 
   OR [Money_transfer] > 30000;

-- 11
SELECT * 
FROM [Culture] 
WHERE [Name] LIKE 'C%';

-- 12
SELECT * 
FROM [Farm] 
WHERE [Farm_ID] LIKE '%1';

-- 13
SELECT * 
FROM [Worker] 
WHERE [Surname] LIKE 'Surname1_';

-- 14
SELECT * 
FROM [Tech] 
WHERE [Name] LIKE '%tor';

-- 15
SELECT * 
FROM [Transaction] 
WHERE [Transaction_type] LIKE 'S%y';

-- 16
SELECT f.[Name] AS [FarmName], c.[Name] AS [CultureName]
FROM [Farm] AS f
JOIN [Culture] AS c 
  ON f.[Culture_ID] = c.[Culture_ID];

-- 17
SELECT b.[Name] AS [BrigadeName], f.[Name] AS [FarmName]
FROM [Brigade] AS b
JOIN [Farm] AS f 
  ON b.[Farm_ID] = f.[Farm_ID];

-- 18
SELECT w.[Name], w.[Surname], b.[Name] AS [BrigadeName]
FROM [Worker] AS w
JOIN [Brigade] AS b 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID];

-- 19
SELECT t.[Name] AS [TechName], b.[Name] AS [BrigadeName]
FROM [Tech] AS t
JOIN [Brigade] AS b 
  ON t.[Brigade_ID] = b.[Brigade_ID]
 AND t.[Farm_ID]    = b.[Farm_ID];

-- 20
SELECT w.[Name], w.[Surname], tr.[Transaction_type], tr.[Money_transfer]
FROM [Worker] AS w
JOIN [Transaction] AS tr 
  ON w.[Transaction_ID] = tr.[Transaction_ID];

-- 21
SELECT f.[Name] AS [FarmName], c.[Name] AS [CultureName]
FROM [Farm] AS f
LEFT JOIN [Culture] AS c 
  ON f.[Culture_ID] = c.[Culture_ID];

-- 22
SELECT w.[Name], w.[Surname], tr.[Transaction_type]
FROM [Worker] AS w
LEFT JOIN [Transaction] AS tr 
  ON w.[Transaction_ID] = tr.[Transaction_ID];

-- 23
SELECT t.[Name] AS [TechName], b.[Name] AS [BrigadeName]
FROM [Brigade] AS b
RIGHT JOIN [Tech] AS t 
  ON t.[Brigade_ID] = b.[Brigade_ID]
 AND t.[Farm_ID]    = b.[Farm_ID];

-- 24
SELECT b.[Name] AS [BrigadeName], w.[Name] AS [WorkerName]
FROM [Brigade] AS b
LEFT JOIN [Worker] AS w 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID];

-- 25
SELECT c.[Name] AS [CultureName], f.[Name] AS [FarmName]
FROM [Culture] AS c
RIGHT JOIN [Farm] AS f 
  ON f.[Culture_ID] = c.[Culture_ID];

-- 26
SELECT * 
FROM [Worker]
WHERE [Salary] > (
    SELECT AVG([Salary]) 
    FROM [Worker]
);

-- 27
SELECT * 
FROM [Farm]
WHERE [Culture_ID] IN (
    SELECT [Culture_ID] 
    FROM [Culture] 
    WHERE [Season] = 'Spring'
);

-- 28
SELECT * 
FROM [Transaction]
WHERE [Money_transfer] > (
    SELECT MAX([Money_transfer]) 
    FROM [Transaction] 
    WHERE [Transaction_type] = 'Salary'
);

-- 29
SELECT [Name], [Area] 
FROM [Farm]
WHERE [Farm_ID] IN (
    SELECT [Farm_ID] 
    FROM [Brigade] 
    WHERE [Workers_amount] > 20
);

-- 30
SELECT * 
FROM [Tech]
WHERE [Transaction_ID] IN (
    SELECT [Transaction_ID] 
    FROM [Transaction] 
    WHERE [Transaction_type] = 'Repair'
);

-- 31
SELECT f.[Culture_ID], COUNT(*) AS [FarmCount]
FROM [Farm] AS f
GROUP BY f.[Culture_ID]
HAVING COUNT(*) > 1;

-- 32
SELECT w.[Position], COUNT(*) AS [WorkerCount]
FROM [Worker] AS w
GROUP BY w.[Position]
HAVING COUNT(*) > 10;

-- 33
SELECT b.[Farm_ID], AVG(b.[Workers_amount]) AS [AvgWorkers]
FROM [Brigade] AS b
GROUP BY b.[Farm_ID]
HAVING AVG(b.[Workers_amount]) > 15;

-- 34
SELECT t.[Status], COUNT(*) AS [TechCount]
FROM [Tech] AS t
GROUP BY t.[Status]
HAVING COUNT(*) >= 5;

-- 35
SELECT c.[Fertilizers], COUNT(f.[Farm_ID]) AS [FarmCount]
FROM [Culture] AS c
JOIN [Farm] AS f 
  ON c.[Culture_ID] = f.[Culture_ID]
GROUP BY c.[Fertilizers]
HAVING COUNT(f.[Farm_ID]) > 2;

-- 36
SELECT c.[Season], COUNT(f.[Farm_ID]) AS [FarmCount]
FROM [Culture] AS c
JOIN [Farm] AS f 
  ON f.[Culture_ID] = c.[Culture_ID]
GROUP BY c.[Season]
HAVING COUNT(f.[Farm_ID]) >= 1;

-- 37
SELECT w.[Name], w.[Surname], w.[Salary]
FROM [Worker] AS w
JOIN [Brigade] AS b 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID]
WHERE w.[Salary] > (
    SELECT AVG([Salary]) 
    FROM [Worker]
);

-- 38
SELECT t.[Name] AS [TechName], t.[Status], tr.[Money_transfer]
FROM [Tech] AS t
JOIN [Transaction] AS tr 
  ON t.[Transaction_ID] = tr.[Transaction_ID]
WHERE tr.[Money_transfer] > 20000;

-- 39
SELECT f.[Name] AS [FarmName], AVG(w.[Salary]) AS [AvgSalary]
FROM [Farm] AS f
JOIN [Brigade] AS b 
  ON f.[Farm_ID] = b.[Farm_ID]
JOIN [Worker] AS w 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID]
GROUP BY f.[Farm_ID], f.[Name]
HAVING AVG(w.[Salary]) > 10000;

-- 40
SELECT c.[Name] AS [CultureName], COUNT(f.[Farm_ID]) AS [FarmCount]
FROM [Culture] AS c
JOIN [Farm] AS f 
  ON f.[Culture_ID] = c.[Culture_ID]
WHERE f.[Area] > 100
GROUP BY c.[Culture_ID], c.[Name]
HAVING COUNT(f.[Farm_ID]) >= 2;

-- 41
SELECT w.[Name], w.[Surname], b.[Name] AS [BrigadeName]
FROM [Worker] AS w
JOIN [Brigade] AS b 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID]
WHERE w.[Status] = 'Active';

-- 42
SELECT f.[Name] AS [FarmName], c.[Name] AS [CultureName]
FROM [Farm] AS f
JOIN [Culture] AS c 
  ON f.[Culture_ID] = c.[Culture_ID]
WHERE c.[Season] = 'Spring';

-- 43
SELECT t.[Name] AS [TechName], tr.[Money_transfer]
FROM [Tech] AS t
JOIN [Transaction] AS tr 
  ON t.[Transaction_ID] = tr.[Transaction_ID]
WHERE tr.[Money_transfer] > 20000;

-- 44
SELECT b.[Brigade_ID], MIN(b.[Name]) AS [BrigadeName], COUNT(w.[Worker_ID]) AS [WorkerCount]
FROM [Brigade] AS b
JOIN [Worker] AS w 
  ON w.[Brigade_ID] = b.[Brigade_ID]
 AND w.[Farm_ID]    = b.[Farm_ID]
WHERE w.[Salary] > 10000
GROUP BY b.[Brigade_ID];

-- 45
SELECT w.[Name], w.[Surname], tr.[Transaction_type]
FROM [Worker] AS w
JOIN [Transaction] AS tr 
  ON w.[Transaction_ID] = tr.[Transaction_ID]
WHERE tr.[Transaction_type] = 'Salary';



--LAB3

SELECT 
  w.Name        AS WorkerName,
  w.Surname     AS WorkerSurname,
  f.Name        AS FarmName,
  c.Name        AS CultureName
FROM Worker   AS w
JOIN Brigade  AS b ON w.Brigade_ID = b.Brigade_ID
                  AND w.Farm_ID    = b.Farm_ID
JOIN Farm     AS f ON b.Farm_ID    = f.Farm_ID
JOIN Culture  AS c ON f.Culture_ID = c.Culture_ID;
GO

SELECT 
  c.Name           AS CultureName,
  AVG(f.Area)      AS AvgAreaByCulture
FROM Culture AS c
JOIN Farm    AS f 
  ON c.Culture_ID = f.Culture_ID
GROUP BY 
  c.Culture_ID, 
  c.Name
HAVING 
  AVG(f.Area) > (
    SELECT AVG(Area) 
    FROM Farm
  );


SELECT 
  w.Name        AS EntityName,
  b.Name        AS BrigadeName,
  f.Name        AS FarmName,
  'Worker'      AS EntityType
FROM Worker   AS w
JOIN Brigade  AS b ON w.Brigade_ID = b.Brigade_ID
                  AND w.Farm_ID    = b.Farm_ID
JOIN Farm     AS f ON b.Farm_ID    = f.Farm_ID
WHERE w.Status = 'Active'

UNION

SELECT 
  t.Name        AS EntityName,
  b.Name        AS BrigadeName,
  f.Name        AS FarmName,
  'Tech'        AS EntityType
FROM Tech     AS t
JOIN Brigade  AS b ON t.Brigade_ID = b.Brigade_ID
                  AND t.Farm_ID    = b.Farm_ID
JOIN Farm     AS f ON b.Farm_ID    = f.Farm_ID
WHERE t.Status = 'Working';
GO


--2


SELECT * FROM Department;
SELECT * FROM Employee;
SELECT * FROM Equipment;

DELETE FROM Department WHERE Dept_ID = 1;

UPDATE Department SET Dept_ID = 101 WHERE Dept_ID = 2;


SELECT * FROM Worker
ORDER BY Salary ASC;

SELECT * FROM Farm
ORDER BY Area DESC;

SELECT * FROM Tech
ORDER BY Purchase_Date DESC;

SELECT * FROM Culture
ORDER BY Amount DESC;

SELECT * FROM [Transaction]
ORDER BY Money_transfer DESC;

SELECT * FROM Worker
ORDER BY Surname ASC, Name ASC;


SELECT * FROM Worker
WHERE Salary > 10000 AND Status = 'Active';

SELECT * FROM Farm
WHERE Area >= 150 OR Culture_ID < 3;

SELECT * FROM Tech
WHERE NOT Status = 'Working';

SELECT * FROM [Transaction]
WHERE Money_transfer <> 0;

SELECT * FROM Culture
WHERE Season = 'Spring' AND Amount <= 30;

SELECT * FROM Worker
WHERE Position = 'Tractor driver' OR Position = 'Agronomist';

--lab5


SELECT
    i.name AS IndexName,
    i.type_desc AS IndexType,
    i.is_unique AS IsUnique,
    ps.avg_fragmentation_in_percent AS FragmentationPercent,
    t.name AS TableName,
    ps.page_count AS PageCount
FROM sys.indexes i
JOIN sys.dm_db_index_physical_stats(DB_ID(), NULL, NULL, NULL, 'LIMITED') ps
    ON i.object_id = ps.object_id AND i.index_id = ps.index_id
JOIN sys.tables t
    ON i.object_id = t.object_id
WHERE i.name IS NOT NULL
ORDER BY FragmentationPercent DESC;


SELECT * FROM Worker WHERE Name = 'Ivan' AND Surname = 'petr';
SELECT * FROM Culture WHERE Culture_ID = 1;
SELECT * FROM Tech WHERE Tech_ID = 3;

SELECT * FROM Tech
ORDER BY Tech_ID DESC;

SELECT COUNT(*) AS TotalWorkers FROM Worker;


--lab7 перевірка
INSERT INTO Worker (Worker_ID, Brigade_ID, Farm_ID, Name, Surname, Position, Rescue_Date, Salary, Status)
VALUES (999, 1, 1, 'Test', 'User', 'Engineer', GETDATE(), 12000, 'Active');

SELECT * FROM LogTable;

UPDATE Tech
SET Status = 'Working'
WHERE Tech_ID = 1 AND Brigade_ID = 1 AND Farm_ID = 1;

UPDATE Tech
SET Status = 'Broken'
WHERE Tech_ID = 1 AND Brigade_ID = 1 AND Farm_ID = 1;

SELECT * FROM LogTable;

SELECT Tech_ID, Brigade_ID, Farm_ID, Status
FROM Tech
WHERE Tech_ID = 1 AND Brigade_ID = 1 AND Farm_ID = 1;

SELECT * FROM [Transaction] WHERE Transaction_ID = 1;
DELETE FROM [Transaction] WHERE Transaction_ID = 1;
SELECT * FROM [Transaction] WHERE Transaction_ID = 1;

INSERT INTO Farm (Farm_ID, Name, Area, Culture_ID)
VALUES (9999, 'Test Farm', 5, 1);
SELECT * FROM Farm WHERE Farm_ID = 9999;

CREATE TABLE TestDDL (ID INT);
DROP TABLE TestDDL;
ALTER TABLE Farm ADD TestField INT NULL;
SELECT * FROM DDL_Log;

SELECT * FROM master.dbo.Logon_Log ORDER BY LogonTime DESC;


