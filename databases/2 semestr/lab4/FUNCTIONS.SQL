CREATE FUNCTION dbo.GetFullName
(
    @FirstName NVARCHAR(50),
    @LastName NVARCHAR(50)
)
RETURNS NVARCHAR(100)
AS
BEGIN
    RETURN @LastName + ' ' + @FirstName
END
GO

CREATE FUNCTION dbo.GetSalaryLevel(@Salary DECIMAL(10,2))
RETURNS NVARCHAR(20)
AS
BEGIN
    DECLARE @Level NVARCHAR(20)
    IF @Salary >= 15000
        SET @Level = 'High'
    ELSE IF @Salary >= 10000
        SET @Level = 'Medium'
    ELSE
        SET @Level = 'Low'
    RETURN @Level
END
GO

CREATE FUNCTION dbo.GetTechStatusLabel(@Status NVARCHAR(50))
RETURNS NVARCHAR(50)
AS
BEGIN
    DECLARE @Label NVARCHAR(50)
    IF @Status = 'Working'
        SET @Label = 'Operational'
    ELSE IF @Status = 'Broken'
        SET @Label = 'Not Operational'
    ELSE IF @Status = 'UnderRepair'
        SET @Label = 'Maintenance'
    ELSE
        SET @Label = 'Unknown'
    RETURN @Label
END
GO

SELECT dbo.GetFullName(Name, Surname) AS FullName FROM Worker; 
SELECT Name, Surname, Salary, dbo.GetSalaryLevel(Salary) AS SalaryLevel FROM Worker;
SELECT Name, Status, dbo.GetTechStatusLabel(Status) AS StatusLabel FROM Tech;


CREATE FUNCTION dbo.fn_WorkersByStatus (@Status NVARCHAR(50))
RETURNS TABLE
AS
RETURN
    SELECT Worker_ID, Name, Surname, Position, Salary
    FROM Worker
    WHERE Status = @Status;
GO

CREATE FUNCTION dbo.fn_TechDetailsByState (@State NVARCHAR(50))
RETURNS TABLE
AS
RETURN
    SELECT 
        Tech_ID,
        Name,
        Type,
        Status,
        Purchase_Date,
        DATEDIFF(YEAR, Purchase_Date, GETDATE()) AS AgeInYears
    FROM Tech
    WHERE Status = @State;
GO


CREATE FUNCTION dbo.fn_HighSalaryWorkers()
RETURNS TABLE
AS
RETURN
    SELECT Worker_ID, Name, Surname, Salary
    FROM Worker
    WHERE Salary > (SELECT AVG(Salary) FROM Worker);
GO

SELECT * FROM dbo.fn_WorkersByStatus('Active');
SELECT * FROM dbo.fn_TechDetailsByState('Broken');
SELECT * FROM dbo.fn_HighSalaryWorkers();


CREATE FUNCTION dbo.fn_WorkersWithBonus()
RETURNS @Result TABLE (
    Worker_ID INT,
    Name NVARCHAR(50),
    Surname NVARCHAR(50),
    Salary DECIMAL(10,2),
    Bonus DECIMAL(10,2)
)
AS
BEGIN
    INSERT INTO @Result
    SELECT 
        Worker_ID, Name, Surname, Salary,
        Salary * 0.10 AS Bonus
    FROM Worker
    WHERE Salary * 0.10 > 1500;

    RETURN
END
GO

CREATE FUNCTION dbo.fn_TechAgeFilter(@MinYears INT)
RETURNS @Result TABLE (
    Tech_ID INT,
    Name NVARCHAR(50),
    Purchase_Date DATE,
    Age INT
)
AS
BEGIN
    INSERT INTO @Result
    SELECT 
        Tech_ID,
        Name,
        Purchase_Date,
        DATEDIFF(YEAR, Purchase_Date, GETDATE()) AS Age
    FROM Tech
    WHERE DATEDIFF(YEAR, Purchase_Date, GETDATE()) >= @MinYears;

    RETURN
END
GO

CREATE FUNCTION dbo.fn_TransactionsOverThreshold(@MinAmount DECIMAL(10,2))
RETURNS @Result TABLE (
    Transaction_ID INT,
    Transaction_type NVARCHAR(50),
    Money_transfer DECIMAL(10,2)
)
AS
BEGIN
    INSERT INTO @Result
    SELECT 
        Transaction_ID,
        Transaction_type,
        Money_transfer
    FROM [Transaction]
    WHERE Money_transfer > @MinAmount;

    RETURN
END
GO


SELECT * FROM dbo.fn_WorkersWithBonus();
SELECT * FROM dbo.fn_TechAgeFilter(2);
SELECT * FROM dbo.fn_TransactionsOverThreshold(30000);


CREATE FUNCTION dbo.fn_CultureAreaStats()
RETURNS TABLE
AS
RETURN
    SELECT 
        c.Name AS CultureName,
        c.Season,
        COUNT(f.Farm_ID) AS FarmsCount,
        SUM(f.Area) AS TotalArea
    FROM Culture c
    JOIN Farm f ON c.Culture_ID = f.Culture_ID
    GROUP BY c.Name, c.Season;
GO

CREATE FUNCTION dbo.fn_BrigadeTechSkills
(
    @BrigadeID INT,
    @FarmID INT
)
RETURNS @Result TABLE (
    WorkerName NVARCHAR(100),
    TechType NVARCHAR(50),
    TechAvailableInBrigade BIT,
    TechExistsInOtherBrigades BIT
)
AS
BEGIN
    INSERT INTO @Result
    SELECT 
        w.Name + ' ' + w.Surname AS WorkerName,
        w.Tech_Type,
        CASE 
            WHEN EXISTS (
                SELECT 1 FROM Tech t
                WHERE t.Brigade_ID = @BrigadeID 
                  AND t.Farm_ID = @FarmID 
                  AND t.Type = w.Tech_Type
            ) THEN 1 ELSE 0
        END AS TechAvailableInBrigade,
        CASE 
            WHEN EXISTS (
                SELECT 1 FROM Tech t
                WHERE NOT (t.Brigade_ID = @BrigadeID AND t.Farm_ID = @FarmID)
                  AND t.Type = w.Tech_Type
            ) THEN 1 ELSE 0
        END AS TechExistsInOtherBrigades
    FROM Worker w
    WHERE w.Brigade_ID = @BrigadeID 
      AND w.Farm_ID = @FarmID
      AND w.Tech_Type IS NOT NULL;

    RETURN;
END
GO

CREATE FUNCTION dbo.fn_BrigadeMaxYieldCulture
(
    @FarmID INT,
    @Season NVARCHAR(20)
)
RETURNS @Result TABLE (
    CultureName NVARCHAR(50),
    Yield FLOAT,
    Area INT,
    Amount INT,
    IsPlantedNow BIT
)
AS
BEGIN
    DECLARE @MaxYieldCultureID INT

    SELECT TOP 1 @MaxYieldCultureID = c.Culture_ID
    FROM Farm f
    JOIN Culture c ON f.Culture_ID = c.Culture_ID
    WHERE f.Farm_ID = @FarmID
    ORDER BY CAST(c.Amount AS FLOAT) / NULLIF(f.Area, 0) DESC;

    INSERT INTO @Result
    SELECT 
        c.Name,
        CAST(c.Amount AS FLOAT) / NULLIF(f.Area, 0) AS Yield,
        f.Area,
        c.Amount,
        CASE WHEN c.Season = @Season THEN 1 ELSE 0 END AS IsPlantedNow
    FROM Farm f
    JOIN Culture c ON f.Culture_ID = c.Culture_ID
    WHERE f.Farm_ID = @FarmID AND c.Culture_ID = @MaxYieldCultureID;

    RETURN;
END
GO

CREATE FUNCTION dbo.fn_BrigadesWithAboveAvgTech(@Type NVARCHAR(50))
RETURNS TABLE
AS
RETURN
    WITH TechPerBrigade AS (
        SELECT Brigade_ID, Farm_ID, COUNT(*) AS TechCount
        FROM Tech
        WHERE Type = @Type
        GROUP BY Brigade_ID, Farm_ID
    ),
    AvgCount AS (
        SELECT AVG(CAST(TechCount AS FLOAT)) AS AvgTech FROM TechPerBrigade
    )
    SELECT tpb.*
    FROM TechPerBrigade tpb
    CROSS JOIN AvgCount ac
    WHERE tpb.TechCount > ac.AvgTech;
GO

CREATE FUNCTION dbo.fn_FarmFertilizerCheck()
RETURNS @Result TABLE (
    Farm_ID INT,
    Culture_Name NVARCHAR(50),
    Area INT,
    Fertilizers NVARCHAR(50),
    BrigadierSurname NVARCHAR(50),
    Warning NVARCHAR(100)
)
AS
BEGIN
    INSERT INTO @Result
    SELECT 
        f.Farm_ID,
        c.Name,
        f.Area,
        c.Fertilizers,
        bdr.Surname,
        'Fertilizer does not match culture' AS Warning
    FROM Farm f
    JOIN Culture c ON f.Culture_ID = c.Culture_ID
    JOIN Brigade b ON b.Farm_ID = f.Farm_ID
    JOIN Worker bdr ON bdr.Worker_ID = b.Brigade_ID 
                   AND bdr.Brigade_ID = b.Brigade_ID 
                   AND bdr.Farm_ID = b.Farm_ID
    WHERE 
        (c.Name = 'Wheat' AND c.Fertilizers <> 'Nitrogen') OR
        (c.Name = 'Barley' AND c.Fertilizers <> 'Organic') OR
        (c.Name = 'Corn' AND c.Fertilizers <> 'Complex') OR
        (c.Name = 'Sunflower' AND c.Fertilizers <> 'Potassium') OR
        (c.Name = 'Potato' AND c.Fertilizers <> 'Nitrogen');

    RETURN;
END
GO


SELECT * FROM dbo.fn_CultureAreaStats();
SELECT * FROM dbo.fn_BrigadeTechSkills(1, 1);
SELECT * FROM dbo.fn_BrigadeMaxYieldCulture(1, 'Summer');
SELECT * FROM dbo.fn_BrigadesWithAboveAvgTech('Type 2');
SELECT * FROM dbo.fn_FarmFertilizerCheck();
