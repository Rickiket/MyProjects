Session #1
    BEGIN; //захожу в режим транзакции

    UPDATE pizzeria 
    SET rating = 5
    WHERE name = 'Pizza Hut'; //меняю рейтинг на 5

    SELECT *
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю изменения в 1 сессии


Session #2
    SELECT *
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю есть ли изменения во 2 сессии(нету)


Session #1
    COMMIT; //сохраняю изменения


Session #2
    SELECT *
    FROM pizzeria
    WHERE name = 'Pizza Hut'; //проверяю есть ли изменения во 2 сессии (есть)
