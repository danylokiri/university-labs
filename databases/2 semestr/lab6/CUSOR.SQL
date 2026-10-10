BEGIN TRANSACTION;
UPDATE Worker 
SET Salary = Salary + 1000 
WHERE Worker_ID = 5;

IF (SELECT COUNT(*) FROM Farm WHERE Area > 1000) = 0
    ROLLBACK;
ELSE
    COMMIT;
	

BEGIN TRANSACTION;
UPDATE Worker 
SET Salary = Salary + 500 
WHERE Worker_ID = -1; 

IF @@ERROR <> 0
    ROLLBACK;
ELSE
    COMMIT;


BEGIN TRANSACTION;
BEGIN TRY
    UPDATE Worker 
    SET Salary = 'wrong'
    WHERE Worker_ID = 5;

    COMMIT;
END TRY
BEGIN CATCH
    ROLLBACK;
    PRINT ERROR_MESSAGE();
END CATCH;


BEGIN TRANSACTION;
BEGIN TRY
    DECLARE @CurrentMax INT = (SELECT MAX(Worker_ID) FROM Worker);
    DECLARE @StartID INT = 
        CASE 
            WHEN @CurrentMax IS NULL THEN 100000
            WHEN @CurrentMax < 100000 THEN 100000
            ELSE @CurrentMax + 1
        END;

    DECLARE @RowCount INT = 10000 + ABS(CHECKSUM(NEWID())) % 90001;

    SELECT DISTINCT Position   INTO #Positions FROM Worker WHERE Position IS NOT NULL;
    SELECT DISTINCT Status     INTO #Statuses  FROM Worker WHERE Status IS NOT NULL;
    SELECT DISTINCT Tech_Type  INTO #Types     FROM Worker WHERE Tech_Type IS NOT NULL;

    ;WITH E1(N) AS (SELECT 1 UNION ALL SELECT 1),
         E2(N) AS (SELECT 1 FROM E1 a, E1 b),       -- 4
         E3(N) AS (SELECT 1 FROM E2 a, E2 b),       -- 16
         E4(N) AS (SELECT 1 FROM E3 a, E3 b),       -- 256
         E5(N) AS (SELECT 1 FROM E4 a, E4 b),       -- 65,536
         E6(N) AS (SELECT 1 FROM E5 a, E2 b),       -- 262,144
         E7(N) AS (SELECT 1 FROM E6 a, E2 b),       -- 1,048,576
         Numbers AS (
            SELECT TOP (@RowCount) ROW_NUMBER() OVER (ORDER BY (SELECT NULL)) AS N
            FROM E7
         ),
         Brigades AS (
            SELECT Brigade_ID, Farm_ID FROM Brigade
         ),
         IndexedBrigades AS (
            SELECT B.*, ROW_NUMBER() OVER (ORDER BY NEWID()) AS rn
            FROM Brigades B
         )

    INSERT INTO Worker (
        Worker_ID, Brigade_ID, Farm_ID, Brigadir_ID,
        Name, Surname, Position, Rescue_Date, Salary,
        Status, Transaction_ID, Tech_Type
    )
    SELECT 
        @StartID + N.N,
        B.Brigade_ID,
        B.Farm_ID,
        B.Brigade_ID,
        CONCAT('Name', @StartID + N.N),
        CONCAT('Surname', @StartID + N.N),
        (SELECT TOP 1 Position FROM #Positions ORDER BY NEWID()),
        DATEADD(DAY, -ABS(CHECKSUM(NEWID())) % 1800, GETDATE()),
        5000 + ABS(CHECKSUM(NEWID())) % 15000,
        (SELECT TOP 1 Status FROM #Statuses ORDER BY NEWID()),
        NULL,
        (SELECT TOP 1 Tech_Type FROM #Types ORDER BY NEWID())
    FROM Numbers N
    CROSS APPLY (
        SELECT TOP 1 Brigade_ID, Farm_ID
        FROM IndexedBrigades
        WHERE rn = ((N.N - 1) % (SELECT COUNT(*) FROM Brigades)) + 1
    ) B;

    DROP TABLE #Positions;
    DROP TABLE #Statuses;
    DROP TABLE #Types;

    COMMIT;
END TRY
BEGIN CATCH
    ROLLBACK;
    PRINT ERROR_MESSAGE();
END CATCH;

SELECT COUNT(*) AS TotalWorkers FROM Worker;


ALTER PROCEDURE AddNewWorker
    @Worker_ID INT,
    @Name NVARCHAR(50),
    @Surname NVARCHAR(50),
    @Farm_ID INT,
    @Brigade_ID INT
AS
BEGIN
    DECLARE @StartTime DATETIME2 = SYSDATETIME();
    PRINT 'Start Time: ' + CONVERT(NVARCHAR, @StartTime);
    BEGIN TRANSACTION;

    BEGIN TRY
        IF NOT EXISTS (SELECT 1 FROM Farm WHERE Farm_ID = @Farm_ID)
        BEGIN
            RAISERROR('Farm not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        IF NOT EXISTS (
            SELECT 1 FROM Brigade 
            WHERE Brigade_ID = @Brigade_ID AND Farm_ID = @Farm_ID
        )
        BEGIN
            RAISERROR('Brigade not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        INSERT INTO Worker(Worker_ID, Name, Surname, Farm_ID, Brigade_ID)
        VALUES (@Worker_ID, @Name, @Surname, @Farm_ID, @Brigade_ID);

        COMMIT TRANSACTION;

        SELECT TOP 10
            W.Worker_ID,
            W.Name,
            W.Surname,
            B.Brigade_ID,
            B.Farm_ID,
            F.Name AS FarmName,
            W.Salary,
            W.Rescue_Date
        FROM Worker W
        INNER JOIN Brigade B ON W.Brigade_ID = B.Brigade_ID
        INNER JOIN Farm F ON W.Farm_ID = F.Farm_ID
        WHERE W.Salary > 8000 AND W.Position IS NOT NULL
        ORDER BY W.Rescue_Date DESC;

    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Error: ' + ERROR_MESSAGE();
    END CATCH

    DECLARE @EndTime DATETIME2 = SYSDATETIME();
    PRINT 'End Time: ' + CONVERT(NVARCHAR, @EndTime);
    PRINT 'Execution Duration (ms): ' + CONVERT(NVARCHAR, DATEDIFF(MILLISECOND, @StartTime, @EndTime));
END;




CREATE NONCLUSTERED INDEX IX_Worker_Salary_Position_RescueDate
ON Worker (Salary, Position, Rescue_Date DESC)
INCLUDE (Worker_ID, Name, Surname, Brigade_ID, Farm_ID);



ALTER PROCEDURE AddNewWorker
    @Worker_ID INT,
    @Name NVARCHAR(50),
    @Surname NVARCHAR(50),
    @Farm_ID INT,
    @Brigade_ID INT
AS
BEGIN
    DECLARE @StartTime DATETIME2 = SYSDATETIME();
    PRINT 'Start Time: ' + CONVERT(NVARCHAR, @StartTime);

    BEGIN TRANSACTION;

    BEGIN TRY
        IF NOT EXISTS (SELECT 1 FROM Farm WHERE Farm_ID = @Farm_ID)
        BEGIN
            RAISERROR('Farm not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        IF NOT EXISTS (
            SELECT 1 FROM Brigade 
            WHERE Brigade_ID = @Brigade_ID AND Farm_ID = @Farm_ID
        )
        BEGIN
            RAISERROR('Brigade not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        INSERT INTO Worker(Worker_ID, Name, Surname, Farm_ID, Brigade_ID)
        VALUES (@Worker_ID, @Name, @Surname, @Farm_ID, @Brigade_ID);

        COMMIT TRANSACTION;

        DECLARE @WID INT,
                @WName NVARCHAR(50),
                @WSurname NVARCHAR(50),
                @BID INT,
                @FID INT,
                @FName NVARCHAR(100),
                @Salary MONEY,
                @RescueDate DATE;

        DECLARE WorkerCursor CURSOR FOR
            SELECT TOP 10
                W.Worker_ID,
                W.Name,
                W.Surname,
                B.Brigade_ID,
                B.Farm_ID,
                F.Name,
                W.Salary,
                W.Rescue_Date
            FROM Worker W
            INNER JOIN Brigade B ON W.Brigade_ID = B.Brigade_ID
            INNER JOIN Farm F ON W.Farm_ID = F.Farm_ID
            WHERE W.Salary > 8000 AND W.Position IS NOT NULL
            ORDER BY W.Rescue_Date DESC;

        OPEN WorkerCursor;
        FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;

        WHILE @@FETCH_STATUS = 0
        BEGIN
            PRINT 'Worker_ID: ' + CAST(@WID AS NVARCHAR) + 
                  ', Name: ' + @WName +
                  ', Surname: ' + @WSurname +
                  ', Brigade_ID: ' + CAST(@BID AS NVARCHAR) +
                  ', Farm_ID: ' + CAST(@FID AS NVARCHAR) +
                  ', FarmName: ' + @FName +
                  ', Salary: ' + CAST(@Salary AS NVARCHAR) +
                  ', Rescue_Date: ' + CONVERT(NVARCHAR, @RescueDate);

            FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;
        END

        CLOSE WorkerCursor;
        DEALLOCATE WorkerCursor;

    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Error: ' + ERROR_MESSAGE();
    END CATCH

    DECLARE @EndTime DATETIME2 = SYSDATETIME();
    PRINT 'End Time: ' + CONVERT(NVARCHAR, @EndTime);
    PRINT 'Execution Duration (ms): ' + CONVERT(NVARCHAR, DATEDIFF(MILLISECOND, @StartTime, @EndTime));
END;



ALTER PROCEDURE AddNewWorker
    @Worker_ID INT,
    @Name NVARCHAR(50),
    @Surname NVARCHAR(50),
    @Farm_ID INT,
    @Brigade_ID INT
AS
BEGIN
    DECLARE @StartTime DATETIME2 = SYSDATETIME();
    PRINT 'Start Time (вся процедура): ' + CONVERT(NVARCHAR, @StartTime);

    BEGIN TRANSACTION;

    BEGIN TRY
        IF NOT EXISTS (SELECT 1 FROM Farm WHERE Farm_ID = @Farm_ID)
        BEGIN
            RAISERROR('Farm not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        IF NOT EXISTS (
            SELECT 1 FROM Brigade 
            WHERE Brigade_ID = @Brigade_ID AND Farm_ID = @Farm_ID
        )
        BEGIN
            RAISERROR('Brigade not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        INSERT INTO Worker(Worker_ID, Name, Surname, Farm_ID, Brigade_ID)
        VALUES (@Worker_ID, @Name, @Surname, @Farm_ID, @Brigade_ID);

        COMMIT TRANSACTION;

        DECLARE @WID INT,
                @WName NVARCHAR(50),
                @WSurname NVARCHAR(50),
                @BID INT,
                @FID INT,
                @FName NVARCHAR(100),
                @Salary MONEY,
                @RescueDate DATE;

        IF CURSOR_STATUS('global', 'WorkerCursor') >= -1
            CLOSE WorkerCursor;

        DECLARE WorkerCursor CURSOR GLOBAL FOR
            SELECT TOP 10
                W.Worker_ID,
                W.Name,
                W.Surname,
                B.Brigade_ID,
                B.Farm_ID,
                F.Name,
                W.Salary,
                W.Rescue_Date
            FROM Worker W
            INNER JOIN Brigade B ON W.Brigade_ID = B.Brigade_ID
            INNER JOIN Farm F ON W.Farm_ID = F.Farm_ID
            WHERE W.Salary > 8000 AND W.Position IS NOT NULL
            ORDER BY W.Rescue_Date DESC;

--1 cursor
        DECLARE @Start1 DATETIME2 = SYSDATETIME();

        OPEN WorkerCursor;
        FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;

        PRINT 'Cursor Pass 1';
        WHILE @@FETCH_STATUS = 0
        BEGIN
            PRINT 'Worker_ID: ' + CAST(@WID AS NVARCHAR) + 
                  ', Name: ' + @WName +
                  ', Surname: ' + @WSurname +
                  ', Brigade_ID: ' + CAST(@BID AS NVARCHAR) +
                  ', Farm_ID: ' + CAST(@FID AS NVARCHAR) +
                  ', FarmName: ' + @FName +
                  ', Salary: ' + CAST(@Salary AS NVARCHAR) +
                  ', Rescue_Date: ' + CONVERT(NVARCHAR, @RescueDate);

            FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;
        END
        DECLARE @End1 DATETIME2 = SYSDATETIME();
        PRINT 'Pass 1 Duration (ms): ' + CAST(DATEDIFF(MILLISECOND, @Start1, @End1) AS NVARCHAR);
        CLOSE WorkerCursor;

---2 cursor
        DECLARE @Start2 DATETIME2 = SYSDATETIME();

        OPEN WorkerCursor;
        FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;

        PRINT 'Cursor Pass 2';
        WHILE @@FETCH_STATUS = 0
        BEGIN
            PRINT 'Worker_ID: ' + CAST(@WID AS NVARCHAR) + 
                  ', Name: ' + @WName +
                  ', Surname: ' + @WSurname +
                  ', Brigade_ID: ' + CAST(@BID AS NVARCHAR) +
                  ', Farm_ID: ' + CAST(@FID AS NVARCHAR) +
                  ', FarmName: ' + @FName +
                  ', Salary: ' + CAST(@Salary AS NVARCHAR) +
                  ', Rescue_Date: ' + CONVERT(NVARCHAR, @RescueDate);

            FETCH NEXT FROM WorkerCursor INTO @WID, @WName, @WSurname, @BID, @FID, @FName, @Salary, @RescueDate;
        END
        DECLARE @End2 DATETIME2 = SYSDATETIME();
        PRINT 'Pass 2 Duration (ms): ' + CAST(DATEDIFF(MILLISECOND, @Start2, @End2) AS NVARCHAR);
        CLOSE WorkerCursor;

        DEALLOCATE WorkerCursor;

    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Error: ' + ERROR_MESSAGE();
    END CATCH

    DECLARE @EndTime DATETIME2 = SYSDATETIME();
    PRINT 'End Time for all: ' + CONVERT(NVARCHAR, @EndTime);
    PRINT 'Execution Duration Total (ms): ' + CONVERT(NVARCHAR, DATEDIFF(MILLISECOND, @StartTime, @EndTime));
END;




EXEC AddNewWorker 
    @Worker_ID = 200009, 
    @Name = 'Test', 
    @Surname = 'Worker', 
    @Farm_ID = 1, 
    @Brigade_ID = 1;
