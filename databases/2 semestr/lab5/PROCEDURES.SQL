EXEC sp_help 'Worker';
EXEC sp_spaceused 'Farm';
EXEC sp_helpindex 'Culture';

CREATE PROCEDURE #TempWorkerCount
AS
BEGIN
    SELECT COUNT(*) AS WorkerCount FROM Worker;
END;
GO

CREATE PROCEDURE #TempFarmSummary
AS
BEGIN
    SELECT Culture_ID, COUNT(*) AS FarmsCount, SUM(Area) AS TotalArea
    FROM Farm
    GROUP BY Culture_ID;
END;
GO

CREATE PROCEDURE #TempTopBrigades
AS
BEGIN
    SELECT TOP 5 Brigade_ID, COUNT(*) AS TechCount
    FROM Tech
    GROUP BY Brigade_ID
    ORDER BY TechCount DESC;
END;
GO

EXEC #TempWorkerCount;
EXEC #TempFarmSummary;
EXEC #TempTopBrigades;

DROP PROCEDURE IF EXISTS AddNewWorker;
GO

CREATE PROCEDURE AddNewWorker
    @Worker_ID INT,
    @Name NVARCHAR(50),
    @Surname NVARCHAR(50),
    @Farm_ID INT,
    @Brigade_ID INT
AS
BEGIN
    BEGIN TRANSACTION;

    BEGIN TRY
        IF NOT EXISTS (SELECT 1 FROM Farm WHERE Farm_ID = @Farm_ID)
        BEGIN
            RAISERROR('Farm not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        IF NOT EXISTS (SELECT 1 FROM Brigade WHERE Brigade_ID = @Brigade_ID AND Farm_ID = @Farm_ID)
        BEGIN
            RAISERROR('Brigade not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        INSERT INTO Worker(Worker_ID, Name, Surname, Farm_ID, Brigade_ID)
        VALUES (@Worker_ID, @Name, @Surname, @Farm_ID, @Brigade_ID);

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        THROW;
    END CATCH
END;
GO


CREATE PROCEDURE UpdateFertilizerForCulture
    @Culture_ID INT,
    @NewFertilizer NVARCHAR(50)
AS
BEGIN
    BEGIN TRANSACTION;
    BEGIN TRY
        UPDATE Culture
        SET Fertilizers = @NewFertilizer
        WHERE Culture_ID = @Culture_ID;

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        THROW;
    END CATCH
END;
GO

CREATE PROCEDURE RemoveTechByID
    @Tech_ID INT
AS
BEGIN
    BEGIN TRANSACTION;
    BEGIN TRY
        IF NOT EXISTS (SELECT 1 FROM Tech WHERE Tech_ID = @Tech_ID)
        BEGIN
            RAISERROR('Tech not found.', 16, 1);
            ROLLBACK TRANSACTION;
            RETURN;
        END

        DELETE FROM Tech WHERE Tech_ID = @Tech_ID;
        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        THROW;
    END CATCH
END;
GO

EXEC AddNewWorker 
    @Worker_ID = 157, 
    @Name = 'Ivan', 
    @Surname = 'petr', 
    @Farm_ID = 1, 
    @Brigade_ID = 1;

EXEC UpdateFertilizerForCulture 
    @Culture_ID = 1, 
    @NewFertilizer = 'Organic';

EXEC RemoveTechByID @Tech_ID = 3;


CREATE PROCEDURE InsertTestTech
    @Count INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @CurrentMaxID INT = ISNULL((SELECT MAX(Tech_ID) FROM Tech), 100000);

    ;WITH Numbers AS (
        SELECT TOP (@Count) ROW_NUMBER() OVER (ORDER BY (SELECT NULL)) AS N
        FROM sys.all_objects
    ),
    RandomBrigades AS (
        SELECT 
            ROW_NUMBER() OVER (ORDER BY NEWID()) AS RowNum,
            Brigade_ID,
            Farm_ID
        FROM Brigade
    )
    INSERT INTO Tech (Tech_ID, Name, Farm_ID, Brigade_ID)
    SELECT 
        @CurrentMaxID + n.N,
        CONCAT('Tech', @CurrentMaxID + n.N),
        b.Farm_ID,
        b.Brigade_ID
    FROM Numbers n
    CROSS APPLY (
        SELECT TOP 1 Brigade_ID, Farm_ID 
        FROM RandomBrigades
        WHERE RowNum = ((ABS(CHECKSUM(NEWID(), n.N)) % (SELECT COUNT(*) FROM Brigade)) + 1)
    ) b;
END;
GO

EXEC InsertTestTech @Count = 50;


CREATE PROCEDURE InsertTechWithSequenceProc
    @Name NVARCHAR(50),
    @Farm_ID INT,
    @Brigade_ID INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @InsertedID INT;

    SET @InsertedID = NEXT VALUE FOR Seq_Tech_ID;

    INSERT INTO Tech (Tech_ID, Name, Farm_ID, Brigade_ID)
    VALUES (@InsertedID, @Name, @Farm_ID, @Brigade_ID);

    SELECT @InsertedID AS InsertedID;
END;
GO


EXEC InsertTechWithSequenceProc 
    @Name = 'New1', 
    @Farm_ID = 1, 
    @Brigade_ID = 1;

	
