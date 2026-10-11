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
