SELECT DISTINCT pz.name AS pizzeria_name
FROM person_visits pv
INNER JOIN person p ON p.id = pv.person_id AND p.name = 'Andrey'
INNER JOIN pizzeria pz ON pz.id = pv.pizzeria_id
WHERE NOT EXISTS (
    SELECT 1
    FROM person_order po
    INNER JOIN menu m ON m.id = po.menu_id
    WHERE po.person_id = p.id
        AND m.pizzeria_id = pz.id
)
ORDER BY 1;