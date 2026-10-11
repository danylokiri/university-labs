CREATE DATABASE farm_db;
GO

USE farm_db;
GO

CREATE TABLE Culture (
    Culture_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Season VARCHAR(50),
    Amount INT,
    Fertilizers VARCHAR(50)
);
GO

CREATE TABLE Farm (
    Farm_ID INT PRIMARY KEY,
    Name VARCHAR(50),
    Area INT,
    Culture_ID INT FOREIGN KEY REFERENCES Culture(Culture_ID)
);
GO

CREATE TABLE Brigade (
    Brigade_ID INT,
    Farm_ID INT,
    Name VARCHAR(50),
    Workers_amount INT,
    PRIMARY KEY (Brigade_ID, Farm_ID),
    FOREIGN KEY (Farm_ID) REFERENCES Farm(Farm_ID)
);
GO

CREATE TABLE [Transaction] (
    Transaction_ID INT PRIMARY KEY,
    Transaction_Date DATE,
    Money_transfer DECIMAL(10,2),
    Transaction_type VARCHAR(50)
);
GO

CREATE TABLE Worker (
    Worker_ID INT IDENTITY(1,1) PRIMARY KEY,
    Brigade_ID INT,
    Farm_ID INT,
    Brigadir_ID INT NULL,
    Name VARCHAR(50),
    Surname VARCHAR(50),
    Position VARCHAR(50),
    Rescue_Date DATE,
    Salary DECIMAL(10,2),
    Status VARCHAR(50),
    Transaction_ID INT NULL,
    PRIMARY KEY (Worker_ID, Brigade_ID, Farm_ID),
    FOREIGN KEY (Brigade_ID, Farm_ID) REFERENCES Brigade(Brigade_ID, Farm_ID),
    FOREIGN KEY (Transaction_ID) REFERENCES [Transaction](Transaction_ID)
);
GO

ALTER TABLE Worker
ADD CONSTRAINT UQ_WorkerID UNIQUE (Worker_ID);
GO

ALTER TABLE Worker
ADD CONSTRAINT FK_Brigadir
FOREIGN KEY (Brigadir_ID) REFERENCES Worker(Worker_ID);
GO

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
    FOREIGN KEY (Transaction_ID) REFERENCES [Transaction](Transaction_ID)
);
GO

CREATE TABLE master.dbo.Logon_Log (
    Log_ID INT IDENTITY PRIMARY KEY,
    LoginName NVARCHAR(255),
    HostName NVARCHAR(255),
    AppName NVARCHAR(255),
    LogonTime DATETIME DEFAULT GETDATE()
);
GO
