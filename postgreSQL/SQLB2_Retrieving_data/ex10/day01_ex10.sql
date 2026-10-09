SELECT
p.name AS person_name,
m.pizza_name,
pz.name AS pizzeria_name
FROM person_order po
INNER JOIN person p
    ON p.id = po.person_id
INNER JOIN menu m
    ON m.id = po.menu_id
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id
ORDER BY 1, 2, 3;