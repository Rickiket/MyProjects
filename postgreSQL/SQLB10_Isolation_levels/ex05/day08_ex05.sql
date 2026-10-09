Session #1
    BEGIN; //открываю транзакцию в 1 сессии
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен read committed
Session #2
    BEGIN; //открываю транзакцию во 2 сессии
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен read committed

Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(21.9)
Session #2
    INSERT INTO pizzeria (id, name, rating)
    VALUES (10, 'Kazan Pizza', 5); //добавляю новую пиццерию
    COMMIT; //закрываю транзакцию во 2 сессии

Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе (поменялась на 26.9)
    COMMIT; //закрываю транзакцию в 1 сессии

Session #1
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(26.9)
Session #2
    SELECT SUM(rating)
    FROM pizzeria; //проверяю сумму всех рейтингов вместе(26.9)
