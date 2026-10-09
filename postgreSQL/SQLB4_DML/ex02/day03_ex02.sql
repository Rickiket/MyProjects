SELECT
m.pizza_name,
m.price,
pz.name AS pizzeria_name
FROM menu m
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id
WHERE NOT EXISTS (
    SELECT 1
    FROM person_order
    WHERE menu_id = m.id
)
ORDER BY 1,2;