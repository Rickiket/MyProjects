Session #1
    SHOW TRANSACTION ISOLATION LEVEL; //проеряю режим изоляции
Session #2
    SHOW TRANSACTION ISOLATION LEVEL; //проеряю режим изоляции


Session #1
    BEGIN; //открываю транзакцию в 1 сессии
Session #2
    BEGIN; //открываю транзакцию во 2 сессии


Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (5) 
Session #2
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (5)


Session #1
    UPDATE pizzeria 
    SET rating = 4
    WHERE name = 'Pizza Hut';
Session #2
    UPDATE pizzeria 
    SET rating = 3.6
    WHERE name = 'Pizza Hut';


Session #1
    COMMIT; //сохраняю изменения
Session #2
    COMMIT; //сохраняю изменения


Session #1
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6) 
Session #2
    SELECT rating
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю значение (3.6)