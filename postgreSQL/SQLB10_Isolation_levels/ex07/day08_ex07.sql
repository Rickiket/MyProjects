Session #1
    BEGIN; //открываю транзакцию в 1 сессии
Session #2
    BEGIN; //открываю транзакцию во 2 сессии

Session #1
    UPDATE pizzeria
    SET rating = 5
    WHERE id = 1; //меняю значение в таблице, где id = 1
Session #2
    UPDATE pizzeria
    SET rating = 5
    WHERE id = 2; //меняю значение в таблице, где id = 2

Session #1
    UPDATE pizzeria
    SET rating = 5
    WHERE id = 2; //меняю значение в таблице, где id = 2
    //ждет commit от 2 сессии
Session #2
    UPDATE pizzeria
    SET rating = 5
    WHERE id = 1; //меняю значение в таблице, где id = 1
    //deadlock detected

Session #1
    COMMIT; //закрываю транзакцию в 1 сессии
Session #2
    BEGIN; //закрываю транзакцию во 2 сессии
    //ROLLBACK