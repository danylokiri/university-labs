USE farm_db;

-- 1. Вивести всіх працівників
SELECT * FROM worker;

-- 2. Вивести тільки ім’я та посаду працівників
SELECT Name, Position FROM worker;

-- 3. Вивести всіх, хто працює зараз
SELECT * FROM worker
WHERE Status = 'Працює';

-- 4. Вивести всіх бригадирів
SELECT * FROM worker
WHERE Position = 'Бригадир';

-- 5. Показати працівників із зарплатою більше 8000
SELECT * FROM worker
WHERE Salary > 8000;

-- 6. Показати назви ферм та їх площу
SELECT Name, Area FROM farm;

-- 7. Показати ферми з площею більше 130
SELECT * FROM farm
WHERE Area > 130;

-- 8. Вивести всі транзакції типу "Зарплата"
SELECT * FROM transaction
WHERE Transaction_type = 'Зарплата';

-- 9. Вивести техніку у статусі "В роботі"
SELECT * FROM tech
WHERE Status = 'В роботі';

-- 10. З’єднати працівників з транзакціями (JOIN)
SELECT w.Name, w.Surname, w.Position, t.Money_transfer, t.Transaction_Date
FROM worker w
JOIN transaction t ON w.Transaction_ID = t.Transaction_ID;
