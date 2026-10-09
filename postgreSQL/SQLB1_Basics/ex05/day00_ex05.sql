SELECT
(
    SELECT name
    FROM person
    WHERE id = p.id
) AS name
FROM person p
WHERE
(
    SELECT COUNT(*)
    FROM person_order
    WHERE
    (
        person_id = p.id
        AND order_date = DATE '2022-01-07'
        AND
        (
            menu_id = 13
            OR menu_id = 14
            OR menu_id = 18
        )
    )
) > 0