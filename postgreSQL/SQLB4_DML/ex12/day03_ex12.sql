INSERT INTO person_order (id, person_id, menu_id, order_date)
SELECT
(SELECT COALESCE(MAX(id), 0) FROM person_order) + new_id.id,
p.id,
m_id.menu_id,
DATE '2022-02-25'
FROM generate_series(1, (SELECT COUNT(*) FROM person), 1) AS new_id(id)
INNER JOIN person p ON new_id.id = (
                                    SELECT COUNT(*)
                                    FROM person p2
                                    WHERE p2.id <= p.id
                                    )
INNER JOIN (
            SELECT MIN(m.id)
            FROM menu m
            WHERE EXISTS (
                            SELECT 1
                            FROM pizzeria pz
                            WHERE m.pizzeria_id = pz.id
                            AND pz.name = 'Dominos'
                        )
            AND m.pizza_name = 'greek pizza'
            ) AS m_id(menu_id) ON TRUE;