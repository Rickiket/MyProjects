SELECT DISTINCT pz.name AS pizzeria_name
FROM person_visits pv
INNER JOIN person p
    ON p.id = pv.person_id
INNER JOIN pizzeria pz
    ON pz.id = pv.pizzeria_id
INNER JOIN menu m
    ON m.pizzeria_id = pz.id
WHERE pv.visit_date = DATE '2022-01-08'
    AND m.price < 800
    AND p.name = 'Dmitriy';