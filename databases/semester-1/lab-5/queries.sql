
CREATE TABLE Brigade
(
	Brigade_ID	  INTEGER  NOT NULL ,
	Name		  VARCHAR2(20)  NULL ,
	Workers_amount	  VARCHAR2(20)  NULL ,
	Farm_ID		  INTEGER  NOT NULL 
);



CREATE UNIQUE INDEX XPKBrigade ON Brigade
(Brigade_ID  ASC,Farm_ID  ASC);



ALTER TABLE Brigade
	ADD CONSTRAINT  XPKBrigade PRIMARY KEY (Brigade_ID,Farm_ID);



CREATE TABLE Culture
(
	Name		  CHAR(18)  NULL ,
	Season		  VARCHAR2(20)  NULL ,
	Amount		  INTEGER  NULL ,
	Culture_ID	  INTEGER  NOT NULL ,
	Fertilizrs	  VARCHAR2(20)  NULL 
);



CREATE UNIQUE INDEX XPKCulture ON Culture
(Culture_ID  ASC);



ALTER TABLE Culture
	ADD CONSTRAINT  XPKCulture PRIMARY KEY (Culture_ID);



CREATE TABLE Farm
(
	Farm_ID		  INTEGER  NOT NULL ,
	Name		  VARCHAR2(20)  NULL ,
	Area		  INTEGER  NULL ,
	Culture_ID	  INTEGER  NULL 
);



CREATE UNIQUE INDEX XPKFarm ON Farm
(Farm_ID  ASC);



ALTER TABLE Farm
	ADD CONSTRAINT  XPKFarm PRIMARY KEY (Farm_ID);



CREATE TABLE Tech
(
	Tech_ID		  INTEGER  NOT NULL ,
	Name		  VARCHAR2(20)  NULL ,
	Type		  VARCHAR2(20)  NULL ,
	Purchase_Date	  DATE  NULL ,
	Status		  CHAR(18)  NULL ,
	Brigade_ID	  INTEGER  NOT NULL ,
	Transaction_ID	  INTEGER  NULL ,
	Farm_ID		  INTEGER  NOT NULL 
);



CREATE UNIQUE INDEX XPKTech ON Tech
(Tech_ID  ASC,Brigade_ID  ASC,Farm_ID  ASC);



ALTER TABLE Tech
	ADD CONSTRAINT  XPKTech PRIMARY KEY (Tech_ID,Brigade_ID,Farm_ID);



CREATE TABLE Transaction
(
	Transaction_ID	  INTEGER  NOT NULL ,
	Transaction_Date  DATE  NULL ,
	Money_transfer	  INTEGER  NULL ,
	Transaction_type  VARCHAR2(20)  NULL 
);



CREATE UNIQUE INDEX XPKTransaction ON Transaction
(Transaction_ID  ASC);



ALTER TABLE Transaction
	ADD CONSTRAINT  XPKTransaction PRIMARY KEY (Transaction_ID);



CREATE TABLE Worker
(
	Worker_ID	  INTEGER  NOT NULL ,
	Brigadir_ID	  INTEGER  NULL ,
	Name		  VARCHAR2(20)  NULL ,
	Surname		  VARCHAR2(20)  NULL ,
	Position	  VARCHAR2(20)  NULL ,
	Rescue_Date	  DATE  NULL ,
	Sallary		  INTEGER  NULL ,
	Status		  VARCHAR2(20)  NULL ,
	Brigade_ID	  INTEGER  NOT NULL ,
	Transaction_ID	  INTEGER  NULL ,
	Farm_ID		  INTEGER  NOT NULL 
);



CREATE UNIQUE INDEX XPKWorker ON Worker
(Worker_ID  ASC,Brigade_ID  ASC,Farm_ID  ASC);



ALTER TABLE Worker
	ADD CONSTRAINT  XPKWorker PRIMARY KEY (Worker_ID,Brigade_ID,Farm_ID);



ALTER TABLE Brigade
	ADD (CONSTRAINT  R_23 FOREIGN KEY (Farm_ID) REFERENCES Farm(Farm_ID));



ALTER TABLE Farm
	ADD (CONSTRAINT  R_22 FOREIGN KEY (Culture_ID) REFERENCES Culture(Culture_ID) ON DELETE SET NULL);



ALTER TABLE Tech
	ADD (CONSTRAINT  R_12 FOREIGN KEY (Brigade_ID,Farm_ID) REFERENCES Brigade(Brigade_ID,Farm_ID));



ALTER TABLE Tech
	ADD (CONSTRAINT  R_18 FOREIGN KEY (Transaction_ID) REFERENCES Transaction(Transaction_ID) ON DELETE SET NULL);



ALTER TABLE Worker
	ADD (CONSTRAINT  R_11 FOREIGN KEY (Brigade_ID,Farm_ID) REFERENCES Brigade(Brigade_ID,Farm_ID));



ALTER TABLE Worker
	ADD (CONSTRAINT  R_17 FOREIGN KEY (Transaction_ID) REFERENCES Transaction(Transaction_ID) ON DELETE SET NULL);



ALTER TABLE Worker
	ADD (CONSTRAINT  R_20 FOREIGN KEY (Brigadir_ID,Brigade_ID,Farm_ID) REFERENCES Worker(Worker_ID,Brigade_ID,Farm_ID) ON DELETE SET NULL);



CREATE  TRIGGER tI_Brigade BEFORE INSERT ON Brigade for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- INSERT trigger on Brigade 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Farm R/23 Brigade on child insert restrict */
    /* ERWIN_RELATION:CHECKSUM="0000ecc9", PARENT_OWNER="", PARENT_TABLE="Farm"
    CHILD_OWNER="", CHILD_TABLE="Brigade"
    P2C_VERB_PHRASE="R/23", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_23", FK_COLUMNS="Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Farm
      WHERE
        /* %JoinFKPK(:%New,Farm," = "," AND") */
        :new.Farm_ID = Farm.Farm_ID;
    IF (
      /* %NotnullFK(:%New," IS NOT NULL AND") */
      
      NUMROWS = 0
    )
    THEN
      raise_application_error(
        -20002,
        'Cannot insert Brigade because Farm does not exist.'
      );
    END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tD_Brigade AFTER DELETE ON Brigade for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- DELETE trigger on Brigade 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Brigade R/11 Worker on parent delete restrict */
    /* ERWIN_RELATION:CHECKSUM="0001e629", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/11", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_11", FK_COLUMNS="Brigade_ID""Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Worker
      WHERE
        /*  %JoinFKPK(Worker,:%Old," = "," AND") */
        Worker.Brigade_ID = :old.Brigade_ID AND
        Worker.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN
      raise_application_error(
        -20001,
        'Cannot delete Brigade because Worker exists.'
      );
    END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Brigade R/12 Tech on parent delete restrict */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/12", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_12", FK_COLUMNS="Brigade_ID""Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Tech
      WHERE
        /*  %JoinFKPK(Tech,:%Old," = "," AND") */
        Tech.Brigade_ID = :old.Brigade_ID AND
        Tech.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN
      raise_application_error(
        -20001,
        'Cannot delete Brigade because Tech exists.'
      );
    END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Brigade AFTER UPDATE ON Brigade for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Brigade 
DECLARE NUMROWS INTEGER;
BEGIN
  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Brigade R/11 Worker on parent update restrict */
  /* ERWIN_RELATION:CHECKSUM="00036108", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/11", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_11", FK_COLUMNS="Brigade_ID""Farm_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Brigade.Brigade_ID <> Brigade.Brigade_ID OR 
    Brigade.Farm_ID <> Brigade.Farm_ID
  THEN
    SELECT count(*) INTO NUMROWS
      FROM Worker
      WHERE
        /*  %JoinFKPK(Worker,:%Old," = "," AND") */
        Worker.Brigade_ID = :old.Brigade_ID AND
        Worker.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN 
      raise_application_error(
        -20005,
        'Cannot update Brigade because Worker exists.'
      );
    END IF;
  END IF;

  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Brigade R/12 Tech on parent update restrict */
  /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/12", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_12", FK_COLUMNS="Brigade_ID""Farm_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Brigade.Brigade_ID <> Brigade.Brigade_ID OR 
    Brigade.Farm_ID <> Brigade.Farm_ID
  THEN
    SELECT count(*) INTO NUMROWS
      FROM Tech
      WHERE
        /*  %JoinFKPK(Tech,:%Old," = "," AND") */
        Tech.Brigade_ID = :old.Brigade_ID AND
        Tech.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN 
      raise_application_error(
        -20005,
        'Cannot update Brigade because Tech exists.'
      );
    END IF;
  END IF;

  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Farm R/23 Brigade on child update restrict */
  /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Farm"
    CHILD_OWNER="", CHILD_TABLE="Brigade"
    P2C_VERB_PHRASE="R/23", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_23", FK_COLUMNS="Farm_ID" */
  SELECT count(*) INTO NUMROWS
    FROM Farm
    WHERE
      /* %JoinFKPK(:%New,Farm," = "," AND") */
      :new.Farm_ID = Farm.Farm_ID;
  IF (
    /* %NotnullFK(:%New," IS NOT NULL AND") */
    
    NUMROWS = 0
  )
  THEN
    raise_application_error(
      -20007,
      'Cannot update Brigade because Farm does not exist.'
    );
  END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/


CREATE  TRIGGER tD_Culture AFTER DELETE ON Culture for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- DELETE trigger on Culture 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Culture R/22 Farm on parent delete set null */
    /* ERWIN_RELATION:CHECKSUM="0000b50a", PARENT_OWNER="", PARENT_TABLE="Culture"
    CHILD_OWNER="", CHILD_TABLE="Farm"
    P2C_VERB_PHRASE="R/22", C2P_VERB_PHRASE="входить в", 
    FK_CONSTRAINT="R_22", FK_COLUMNS="Culture_ID" */
    UPDATE Farm
      SET
        /* %SetFK(Farm,NULL) */
        Farm.Culture_ID = NULL
      WHERE
        /* %JoinFKPK(Farm,:%Old," = "," AND") */
        Farm.Culture_ID = :old.Culture_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Culture AFTER UPDATE ON Culture for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Culture 
DECLARE NUMROWS INTEGER;
BEGIN
  /* Culture R/22 Farm on parent update set null */
  /* ERWIN_RELATION:CHECKSUM="0000da00", PARENT_OWNER="", PARENT_TABLE="Culture"
    CHILD_OWNER="", CHILD_TABLE="Farm"
    P2C_VERB_PHRASE="R/22", C2P_VERB_PHRASE="входить в", 
    FK_CONSTRAINT="R_22", FK_COLUMNS="Culture_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Culture.Culture_ID <> Culture.Culture_ID
  THEN
    UPDATE Farm
      SET
        /* %SetFK(Farm,NULL) */
        Farm.Culture_ID = NULL
      WHERE
        /* %JoinFKPK(Farm,:%Old," = ",",") */
        Farm.Culture_ID = :old.Culture_ID;
  END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/


CREATE  TRIGGER tI_Farm BEFORE INSERT ON Farm for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- INSERT trigger on Farm 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Culture R/22 Farm on child insert set null */
    /* ERWIN_RELATION:CHECKSUM="0000e8d2", PARENT_OWNER="", PARENT_TABLE="Culture"
    CHILD_OWNER="", CHILD_TABLE="Farm"
    P2C_VERB_PHRASE="R/22", C2P_VERB_PHRASE="входить в", 
    FK_CONSTRAINT="R_22", FK_COLUMNS="Culture_ID" */
    UPDATE Farm
      SET
        /* %SetFK(Farm,NULL) */
        Farm.Culture_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Culture
            WHERE
              /* %JoinFKPK(:%New,Culture," = "," AND") */
              :new.Culture_ID = Culture.Culture_ID
        ) 
        /* %JoinPKPK(Farm,:%New," = "," AND") */
         and Farm.Farm_ID = Farm.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tD_Farm AFTER DELETE ON Farm for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- DELETE trigger on Farm 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Farm R/23 Brigade on parent delete restrict */
    /* ERWIN_RELATION:CHECKSUM="0000db90", PARENT_OWNER="", PARENT_TABLE="Farm"
    CHILD_OWNER="", CHILD_TABLE="Brigade"
    P2C_VERB_PHRASE="R/23", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_23", FK_COLUMNS="Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Brigade
      WHERE
        /*  %JoinFKPK(Brigade,:%Old," = "," AND") */
        Brigade.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN
      raise_application_error(
        -20001,
        'Cannot delete Farm because Brigade exists.'
      );
    END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Farm AFTER UPDATE ON Farm for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Farm 
DECLARE NUMROWS INTEGER;
BEGIN
  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Farm R/23 Brigade on parent update restrict */
  /* ERWIN_RELATION:CHECKSUM="000202ae", PARENT_OWNER="", PARENT_TABLE="Farm"
    CHILD_OWNER="", CHILD_TABLE="Brigade"
    P2C_VERB_PHRASE="R/23", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_23", FK_COLUMNS="Farm_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Farm.Farm_ID <> Farm.Farm_ID
  THEN
    SELECT count(*) INTO NUMROWS
      FROM Brigade
      WHERE
        /*  %JoinFKPK(Brigade,:%Old," = "," AND") */
        Brigade.Farm_ID = :old.Farm_ID;
    IF (NUMROWS > 0)
    THEN 
      raise_application_error(
        -20005,
        'Cannot update Farm because Brigade exists.'
      );
    END IF;
  END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Culture R/22 Farm on child update set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Culture"
    CHILD_OWNER="", CHILD_TABLE="Farm"
    P2C_VERB_PHRASE="R/22", C2P_VERB_PHRASE="входить в", 
    FK_CONSTRAINT="R_22", FK_COLUMNS="Culture_ID" */
    UPDATE Farm
      SET
        /* %SetFK(Farm,NULL) */
        Farm.Culture_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Culture
            WHERE
              /* %JoinFKPK(:%New,Culture," = "," AND") */
              :new.Culture_ID = Culture.Culture_ID
        ) 
        /* %JoinPKPK(Farm,:%New," = "," AND") */
         and Farm.Farm_ID = Farm.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/


CREATE  TRIGGER tI_Tech BEFORE INSERT ON Tech for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- INSERT trigger on Tech 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Brigade R/12 Tech on child insert restrict */
    /* ERWIN_RELATION:CHECKSUM="000231fc", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/12", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_12", FK_COLUMNS="Brigade_ID""Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Brigade
      WHERE
        /* %JoinFKPK(:%New,Brigade," = "," AND") */
        :new.Brigade_ID = Brigade.Brigade_ID AND
        :new.Farm_ID = Brigade.Farm_ID;
    IF (
      /* %NotnullFK(:%New," IS NOT NULL AND") */
      
      NUMROWS = 0
    )
    THEN
      raise_application_error(
        -20002,
        'Cannot insert Tech because Brigade does not exist.'
      );
    END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/18 Tech on child insert set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/18", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_18", FK_COLUMNS="Transaction_ID" */
    UPDATE Tech
      SET
        /* %SetFK(Tech,NULL) */
        Tech.Transaction_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Transaction
            WHERE
              /* %JoinFKPK(:%New,Transaction," = "," AND") */
              :new.Transaction_ID = Transaction.Transaction_ID
        ) 
        /* %JoinPKPK(Tech,:%New," = "," AND") */
         and Tech.Tech_ID = Tech.Tech_ID AND
        Tech.Brigade_ID = Tech.Brigade_ID AND
        Tech.Farm_ID = Tech.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Tech AFTER UPDATE ON Tech for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Tech 
DECLARE NUMROWS INTEGER;
BEGIN
  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Brigade R/12 Tech on child update restrict */
  /* ERWIN_RELATION:CHECKSUM="00023519", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/12", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_12", FK_COLUMNS="Brigade_ID""Farm_ID" */
  SELECT count(*) INTO NUMROWS
    FROM Brigade
    WHERE
      /* %JoinFKPK(:%New,Brigade," = "," AND") */
      :new.Brigade_ID = Brigade.Brigade_ID AND
      :new.Farm_ID = Brigade.Farm_ID;
  IF (
    /* %NotnullFK(:%New," IS NOT NULL AND") */
    
    NUMROWS = 0
  )
  THEN
    raise_application_error(
      -20007,
      'Cannot update Tech because Brigade does not exist.'
    );
  END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/18 Tech on child update set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/18", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_18", FK_COLUMNS="Transaction_ID" */
    UPDATE Tech
      SET
        /* %SetFK(Tech,NULL) */
        Tech.Transaction_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Transaction
            WHERE
              /* %JoinFKPK(:%New,Transaction," = "," AND") */
              :new.Transaction_ID = Transaction.Transaction_ID
        ) 
        /* %JoinPKPK(Tech,:%New," = "," AND") */
         and Tech.Tech_ID = Tech.Tech_ID AND
        Tech.Brigade_ID = Tech.Brigade_ID AND
        Tech.Farm_ID = Tech.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/


CREATE  TRIGGER tD_Transaction AFTER DELETE ON Transaction for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- DELETE trigger on Transaction 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/17 Worker on parent delete set null */
    /* ERWIN_RELATION:CHECKSUM="00019920", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/17", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_17", FK_COLUMNS="Transaction_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Transaction_ID = NULL
      WHERE
        /* %JoinFKPK(Worker,:%Old," = "," AND") */
        Worker.Transaction_ID = :old.Transaction_ID;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/18 Tech on parent delete set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/18", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_18", FK_COLUMNS="Transaction_ID" */
    UPDATE Tech
      SET
        /* %SetFK(Tech,NULL) */
        Tech.Transaction_ID = NULL
      WHERE
        /* %JoinFKPK(Tech,:%Old," = "," AND") */
        Tech.Transaction_ID = :old.Transaction_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Transaction AFTER UPDATE ON Transaction for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Transaction 
DECLARE NUMROWS INTEGER;
BEGIN
  /* Transaction R/17 Worker on parent update set null */
  /* ERWIN_RELATION:CHECKSUM="0001d400", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/17", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_17", FK_COLUMNS="Transaction_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Transaction.Transaction_ID <> Transaction.Transaction_ID
  THEN
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Transaction_ID = NULL
      WHERE
        /* %JoinFKPK(Worker,:%Old," = ",",") */
        Worker.Transaction_ID = :old.Transaction_ID;
  END IF;

  /* Transaction R/18 Tech on parent update set null */
  /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Tech"
    P2C_VERB_PHRASE="R/18", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_18", FK_COLUMNS="Transaction_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Transaction.Transaction_ID <> Transaction.Transaction_ID
  THEN
    UPDATE Tech
      SET
        /* %SetFK(Tech,NULL) */
        Tech.Transaction_ID = NULL
      WHERE
        /* %JoinFKPK(Tech,:%Old," = ",",") */
        Tech.Transaction_ID = :old.Transaction_ID;
  END IF;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/


CREATE  TRIGGER tI_Worker BEFORE INSERT ON Worker for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- INSERT trigger on Worker 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Brigade R/11 Worker on child insert restrict */
    /* ERWIN_RELATION:CHECKSUM="000389dc", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/11", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_11", FK_COLUMNS="Brigade_ID""Farm_ID" */
    SELECT count(*) INTO NUMROWS
      FROM Brigade
      WHERE
        /* %JoinFKPK(:%New,Brigade," = "," AND") */
        :new.Brigade_ID = Brigade.Brigade_ID AND
        :new.Farm_ID = Brigade.Farm_ID;
    IF (
      /* %NotnullFK(:%New," IS NOT NULL AND") */
      
      NUMROWS = 0
    )
    THEN
      raise_application_error(
        -20002,
        'Cannot insert Worker because Brigade does not exist.'
      );
    END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/17 Worker on child insert set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/17", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_17", FK_COLUMNS="Transaction_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Transaction_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Transaction
            WHERE
              /* %JoinFKPK(:%New,Transaction," = "," AND") */
              :new.Transaction_ID = Transaction.Transaction_ID
        ) 
        /* %JoinPKPK(Worker,:%New," = "," AND") */
         and Worker.Worker_ID = Worker.Worker_ID AND
        Worker.Brigade_ID = Worker.Brigade_ID AND
        Worker.Farm_ID = Worker.Farm_ID;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Worker R/20 Worker on child insert set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Worker"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/20", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_20", FK_COLUMNS="Brigadir_ID""Brigade_ID""Farm_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Brigadir_ID = NULL,
        Worker.Brigade_ID = NULL,
        Worker.Farm_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Worker
            WHERE
              /* %JoinFKPK(:%New,Worker," = "," AND") */
              :new.Brigadir_ID = Worker.Worker_ID AND
              :new.Brigade_ID = Worker.Brigade_ID AND
              :new.Farm_ID = Worker.Farm_ID
        ) 
        /* %JoinPKPK(Worker,:%New," = "," AND") */
         and Worker.Worker_ID = Worker.Worker_ID AND
        Worker.Brigade_ID = Worker.Brigade_ID AND
        Worker.Farm_ID = Worker.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tD_Worker AFTER DELETE ON Worker for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- DELETE trigger on Worker 
DECLARE NUMROWS INTEGER;
BEGIN
    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Worker R/20 Worker on parent delete set null */
    /* ERWIN_RELATION:CHECKSUM="0000fb19", PARENT_OWNER="", PARENT_TABLE="Worker"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/20", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_20", FK_COLUMNS="Brigadir_ID""Brigade_ID""Farm_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Brigadir_ID = NULL,
        Worker.Brigade_ID = NULL,
        Worker.Farm_ID = NULL
      WHERE
        /* %JoinFKPK(Worker,:%Old," = "," AND") */
        Worker.Brigadir_ID = :old.Worker_ID AND
        Worker.Brigade_ID = :old.Brigade_ID AND
        Worker.Farm_ID = :old.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

CREATE  TRIGGER tU_Worker AFTER UPDATE ON Worker for each row
-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
-- UPDATE trigger on Worker 
DECLARE NUMROWS INTEGER;
BEGIN
  /* Worker R/20 Worker on parent update set null */
  /* ERWIN_RELATION:CHECKSUM="0004e2c3", PARENT_OWNER="", PARENT_TABLE="Worker"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/20", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_20", FK_COLUMNS="Brigadir_ID""Brigade_ID""Farm_ID" */
  IF
    /* %JoinPKPK(:%Old,:%New," <> "," OR ") */
    Worker.Worker_ID <> Worker.Worker_ID OR 
    Worker.Brigade_ID <> Worker.Brigade_ID OR 
    Worker.Farm_ID <> Worker.Farm_ID
  THEN
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Brigadir_ID = NULL,
        Worker.Brigade_ID = NULL,
        Worker.Farm_ID = NULL
      WHERE
        /* %JoinFKPK(Worker,:%Old," = ",",") */
        Worker.Brigadir_ID = :old.Worker_ID AND
        Worker.Brigade_ID = :old.Brigade_ID AND
        Worker.Farm_ID = :old.Farm_ID;
  END IF;

  /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
  /* Brigade R/11 Worker on child update restrict */
  /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Brigade"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/11", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_11", FK_COLUMNS="Brigade_ID""Farm_ID" */
  SELECT count(*) INTO NUMROWS
    FROM Brigade
    WHERE
      /* %JoinFKPK(:%New,Brigade," = "," AND") */
      :new.Brigade_ID = Brigade.Brigade_ID AND
      :new.Farm_ID = Brigade.Farm_ID;
  IF (
    /* %NotnullFK(:%New," IS NOT NULL AND") */
    
    NUMROWS = 0
  )
  THEN
    raise_application_error(
      -20007,
      'Cannot update Worker because Brigade does not exist.'
    );
  END IF;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Transaction R/17 Worker on child update set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Transaction"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/17", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_17", FK_COLUMNS="Transaction_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Transaction_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Transaction
            WHERE
              /* %JoinFKPK(:%New,Transaction," = "," AND") */
              :new.Transaction_ID = Transaction.Transaction_ID
        ) 
        /* %JoinPKPK(Worker,:%New," = "," AND") */
         and Worker.Worker_ID = Worker.Worker_ID AND
        Worker.Brigade_ID = Worker.Brigade_ID AND
        Worker.Farm_ID = Worker.Farm_ID;

    /* ERwin Builtin 5 но€бр€ 2024 г. 3:58:19 */
    /* Worker R/20 Worker on child update set null */
    /* ERWIN_RELATION:CHECKSUM="00000000", PARENT_OWNER="", PARENT_TABLE="Worker"
    CHILD_OWNER="", CHILD_TABLE="Worker"
    P2C_VERB_PHRASE="R/20", C2P_VERB_PHRASE="", 
    FK_CONSTRAINT="R_20", FK_COLUMNS="Brigadir_ID""Brigade_ID""Farm_ID" */
    UPDATE Worker
      SET
        /* %SetFK(Worker,NULL) */
        Worker.Brigadir_ID = NULL,
        Worker.Brigade_ID = NULL,
        Worker.Farm_ID = NULL
      WHERE
        NOT EXISTS (
          SELECT * FROM Worker
            WHERE
              /* %JoinFKPK(:%New,Worker," = "," AND") */
              :new.Brigadir_ID = Worker.Worker_ID AND
              :new.Brigade_ID = Worker.Brigade_ID AND
              :new.Farm_ID = Worker.Farm_ID
        ) 
        /* %JoinPKPK(Worker,:%New," = "," AND") */
         and Worker.Worker_ID = Worker.Worker_ID AND
        Worker.Brigade_ID = Worker.Brigade_ID AND
        Worker.Farm_ID = Worker.Farm_ID;


-- ERwin Builtin 5 но€бр€ 2024 г. 3:58:19
END;
/

