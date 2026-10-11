CREATE TABLE LogTable (
    Log_ID INT IDENTITY PRIMARY KEY,
    Message NVARCHAR(255),
    CreatedAt DATETIME DEFAULT GETDATE()
);
GO

CREATE TRIGGER trg_AfterInsert_Worker
ON Worker
AFTER INSERT
AS
BEGIN
    INSERT INTO LogTable (Message)
    SELECT 'New worker inserted: ' + Name + ' ' + Surname
    FROM inserted;
END;
GO

CREATE TRIGGER trg_AfterUpdate_TechStatus
ON Tech
AFTER UPDATE
AS
BEGIN
    INSERT INTO LogTable (Message)
    SELECT 'Tech ID ' + CAST(i.Tech_ID AS NVARCHAR) + ' status changed from ' + d.Status + ' to ' + i.Status
    FROM inserted i
    JOIN deleted d ON i.Tech_ID = d.Tech_ID AND i.Brigade_ID = d.Brigade_ID AND i.Farm_ID = d.Farm_ID
    WHERE ISNULL(i.Status, '') <> ISNULL(d.Status, '');
END;
GO

CREATE TRIGGER trg_InsteadOfDelete_Transaction
ON [Transaction]
INSTEAD OF DELETE
AS
BEGIN
    UPDATE t
    SET Money_transfer = 0
    FROM [Transaction] t
    JOIN deleted d ON t.Transaction_ID = d.Transaction_ID;
END;
GO

CREATE TRIGGER trg_InsteadOfInsert_Farm
ON Farm
INSTEAD OF INSERT
AS
BEGIN
    INSERT INTO Farm (Farm_ID, Name, Area, Culture_ID)
    SELECT 
        Farm_ID,
        Name,
        CASE WHEN Area < 10 THEN 10 ELSE Area END,
        Culture_ID
    FROM inserted;
END;
GO

--всі перевірки в query

CREATE TABLE DDL_Log (
    Log_ID INT IDENTITY PRIMARY KEY,
    EventType NVARCHAR(100),
    ObjectName NVARCHAR(255),
    SqlCommand NVARCHAR(MAX),
    EventTime DATETIME DEFAULT GETDATE()
);
GO

CREATE TRIGGER trg_DDL_Log
ON DATABASE
FOR CREATE_TABLE, DROP_TABLE, ALTER_TABLE
AS
BEGIN
    DECLARE @EventData XML = EVENTDATA();

    INSERT INTO DDL_Log (EventType, ObjectName, SqlCommand)
    VALUES (
        @EventData.value('(/EVENT_INSTANCE/EventType)[1]', 'NVARCHAR(100)'),
        @EventData.value('(/EVENT_INSTANCE/ObjectName)[1]', 'NVARCHAR(255)'),
        @EventData.value('(/EVENT_INSTANCE/TSQLCommand/CommandText)[1]', 'NVARCHAR(MAX)')
    );
END;
GO


CREATE TRIGGER trg_Logon_Audit
ON ALL SERVER
FOR LOGON
AS
BEGIN
    BEGIN TRY
        DECLARE @LoginName NVARCHAR(255) = ORIGINAL_LOGIN();
        DECLARE @HostName NVARCHAR(255) = HOST_NAME();
        DECLARE @AppName NVARCHAR(255) = APP_NAME();

        IF @LoginName NOT LIKE 'NT SERVICE\%' AND @LoginName NOT LIKE 'NT AUTHORITY\%' 
        BEGIN
            IF EXISTS (
                SELECT 1 FROM master.sys.tables 
                WHERE name = 'Logon_Log' AND SCHEMA_NAME(schema_id) = 'dbo'
            )
            BEGIN
                INSERT INTO master.dbo.Logon_Log (LoginName, HostName, AppName)
                VALUES (@LoginName, @HostName, @AppName);
            END
        END
    END TRY
    BEGIN CATCH
    END CATCH
END;
GO

