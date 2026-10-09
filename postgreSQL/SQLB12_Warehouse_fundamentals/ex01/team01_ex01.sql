begin ;

insert into currency values (100, 'EUR', 0.85, '2022-01-01 13:29');
insert into currency values (100, 'EUR', 0.79, '2022-01-08 13:29');

WITH currency_name_only AS (
    SELECT DISTINCT id, name
    FROM currency
)

SELECT
    COALESCE(u.name, 'not defined') as name,
    COALESCE(u.lastname, 'not defined') as lastname,
    cno.name AS currency_name,
    (b.money * COALESCE(
        (SELECT c1.rate_to_usd
         FROM currency c1
         WHERE c1.id = b.currency_id
            AND c1.updated <= b.updated
         ORDER BY updated DESC
         FETCH FIRST 1 ROW ONLY)
    ,   
        (SELECT c2.rate_to_usd
         FROM currency c2
         WHERE c2.id = b.currency_id
            AND c2.updated > b.updated
         ORDER BY updated ASC
         FETCH FIRST 1 ROW ONLY)
    ))::NUMERIC(10,3)::DOUBLE PRECISION AS currency_in_usd
FROM balance b
INNER JOIN currency_name_only cno ON cno.id = b.currency_id
LEFT JOIN "user" u ON u.id = b.user_id
ORDER BY 
    COALESCE(u.name, 'not defined') DESC,
    COALESCE(u.lastname, 'not defined') ASC, 
    cno.name ASC;

rollback ;



