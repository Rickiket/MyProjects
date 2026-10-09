Session #1
    BEGIN; //открываю транзакцию в 1 сессии
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; //включаю другую изоляцию
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен repeatable read
Session #2
    BEGIN; //открываю транзакцию во 2 сессии
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; //включаю другую изоляцию
    SHOW TRANSACTION ISOLATION LEVEL; //проверяю что в сессис включен repeatable read


Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6) 
Session #2
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6)


Session #1
    UPDATE pizzeria 
    SET rating = 4
    WHERE name = 'Pizza Hut'; //меняю значение в 1 сессии
Session #2
    UPDATE pizzeria 
    SET rating = 3.6
    WHERE name = 'Pizza Hut'; //меняю значение во 2 сессии


Session #1
    COMMIT; //сохраняю изменения
Session #2 //вышла ошибка "could not serialize access due to concurrent update"
    COMMIT; //сохраняю изменения (ROLLBACK)


Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (4) 
Session #2
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (4)