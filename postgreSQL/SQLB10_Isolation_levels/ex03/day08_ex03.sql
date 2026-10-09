Session #1
    BEGIN; //открываю транзакцию в 1 сессии
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен read committed
Session #2
    BEGIN; //открываю транзакцию во 2 сессии
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен read committed

Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (4) 
Session #2
    UPDATE pizzeria 
    SET rating = 3.6
    WHERE name = 'Pizza Hut'; //меняю значение во 2 сессии
    COMMIT; //закрываю транзакцию во 2 сессии

Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (изменилось на 3.6)
    COMMIT; //закрываю транзакцию в 1 сессии
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6) 
Session #2
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6)
