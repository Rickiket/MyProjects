Session #1
    BEGIN; //открываю транзакцию в 1 сессии
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; //включаю другую изоляцию
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен repeatable read
Session #2
    BEGIN; //открываю транзакцию во 2 сессии
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; //включаю другую изоляцию
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен repeatable read

Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(26.9)
Session #2
    INSERT INTO pizzeria (id, name, rating)
    VALUES (11, 'Kazan Pizza 2', 4); //добавляю новую пиццерию
    COMMIT; //закрываю транзакцию во 2 сессии


Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе (осталось 26.9)
    COMMIT; //закрываю транзакцию в 1 сессии

Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(поменялось на 30.9)
Session #2
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(30.9)