SELECT
m.pizza_name, p.name AS pizzeria_name, m.price
FROM menu m
INNER JOIN pizzeria p
    ON p.id = m.pizzeria_id
WHERE m.pizza_name IN ('mushroom pizza', 'pepperoni pizza')
ORDER BY 1, 2;