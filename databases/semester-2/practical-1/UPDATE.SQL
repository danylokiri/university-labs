USE farm_db;

--  1. Повысить зарплату Иванову на 10%
UPDATE worker
SET Salary = Salary * 1.10
WHERE Worker_ID = 1 AND Brigade_ID = 1 AND Farm_ID = 1;

--  2. Обновить статус Шевченка на "Відпустка"
UPDATE worker
SET Status = 'Відпустка'
WHERE Worker_ID = 2 AND Brigade_ID = 1 AND Farm_ID = 1;

--  3. Изменить площадь фермы "Ферма Південна"
UPDATE farm
SET Area = 175
WHERE Farm_ID = 1;

--  4. Переименовать культуру "Соняшник" в "Соняшник гібрид"
UPDATE culture
SET Name = 'Соняшник гібрид'
WHERE Culture_ID = 3;

--  5. Обновить статус техники по ключам
UPDATE tech
SET Status = 'В ремонті завершено'
WHERE Tech_ID = 2 AND Brigade_ID = 2 AND Farm_ID = 2;

--  6. Удалить работника Дмитра Лисенка
DELETE FROM worker
WHERE Worker_ID = 4 AND Brigade_ID = 2 AND Farm_ID = 2;

--  7. Удалить транзакцию "Закупка техніки"
DELETE FROM transaction
WHERE Transaction_ID = 1;