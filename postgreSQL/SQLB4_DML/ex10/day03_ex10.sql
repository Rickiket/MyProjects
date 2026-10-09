INSERT INTO person_order (id, person_id, menu_id, order_date)
SELECT
(
    SELECT COALESCE(MAX(id), 0)
    FROM person_order
) + ROW_NUMBER() OVER (ORDER BY p.id),
p.id,
m.id,
DATE '2022-02-24'
FROM person p
INNER JOIN menu m ON m.pizza_name = 'sicilian pizza'
WHERE p.name IN ('Denis', 'Irina');