CREATE TABLE Department (
    Dept_ID INT PRIMARY KEY,
    Dept_Name VARCHAR(100)
);
GO

CREATE TABLE Employee (
    Emp_ID INT PRIMARY KEY,
    Name VARCHAR(100),
    Dept_ID INT,
    FOREIGN KEY (Dept_ID) REFERENCES Department(Dept_ID)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
GO

CREATE TABLE Equipment (
    Equip_ID INT PRIMARY KEY,
    Emp_ID INT,
    Equipment_Name VARCHAR(100),
    FOREIGN KEY (Emp_ID) REFERENCES Employee(Emp_ID)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
GO


DROP TABLE Equipment;
DROP TABLE Employee;
DROP TABLE Department;


ALTER TABLE Worker
ADD Tech_Type NVARCHAR(50) NULL;

WITH Types AS (
    SELECT DISTINCT Type FROM Tech
),
Randomized AS (
    SELECT 
        w.Worker_ID,
        t.Type,
        ROW_NUMBER() OVER (ORDER BY NEWID()) AS rn
    FROM Worker w
    CROSS JOIN Types t
)

UPDATE w
SET Tech_Type = r.Type
FROM Worker w
JOIN (
    SELECT Worker_ID, Type,
           ROW_NUMBER() OVER (PARTITION BY Worker_ID ORDER BY NEWID()) AS rnk
    FROM Randomized
) r ON w.Worker_ID = r.Worker_ID
WHERE r.rnk = 1 AND w.Worker_ID <= 80;


--lab5 indeksi
CREATE NONCLUSTERED INDEX IX_Worker_Position
ON Worker(Position);
GO

CREATE UNIQUE INDEX IX_Transaction_TypeAmount
ON [Transaction](Transaction_type, Money_transfer);
GO

CREATE NONCLUSTERED INDEX IX_Worker_Salary
ON Worker(Salary)
INCLUDE(Name, Surname);
GO

CREATE NONCLUSTERED INDEX IX_Worker_ActiveStatus
ON Worker(Status)
WHERE Status = 'Active';
GO

--4 opt
CREATE NONCLUSTERED INDEX IX_Farm_Culture_Area
ON Farm (Culture_ID, Area);

CREATE NONCLUSTERED INDEX IX_Tech_Type_BrigadeFarm
ON Tech (Type, Brigade_ID, Farm_ID);

CREATE NONCLUSTERED INDEX IX_Culture_Name_Fertilizers
ON Culture (Name, Fertilizers);

CREATE NONCLUSTERED INDEX IX_Farm_CultureID
ON Farm (Culture_ID, Farm_ID);

CREATE NONCLUSTERED INDEX IX_Worker_Join
ON Worker (Worker_ID, Brigade_ID, Farm_ID);


CREATE SEQUENCE Seq_Tech_ID
    START WITH 1000
    INCREMENT BY 1;
GO
