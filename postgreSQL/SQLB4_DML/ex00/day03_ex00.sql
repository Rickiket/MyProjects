SELECT DISTINCT
m.pizza_name,
m.price,
pz.name AS pizzeria_name,
pv.visit_date
FROM menu m
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id
INNER JOIN person_visits pv
    ON pv.pizzeria_id = pz.id
WHERE m.price BETWEEN 800 AND 1000
    AND EXISTS (
        SELECT 1
        FROM person
        WHERE pv.person_id = id
        AND name = 'Kate'
    )
ORDER BY 1,2,3;