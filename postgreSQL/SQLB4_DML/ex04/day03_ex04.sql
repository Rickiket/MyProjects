SELECT pz.name AS pizzeria_name
FROM person_order po
INNER JOIN menu m ON m.id = po.menu_id
INNER JOIN person p ON p.id = po.person_id
INNER JOIN pizzeria pz ON pz.id = m.pizzeria_id
GROUP BY pz.name
HAVING SUM(CASE WHEN p.gender = 'male' THEN 1 ELSE 0 END) > 0
    AND SUM(CASE WHEN p.gender = 'female' THEN 1 ELSE 0 END) = 0

UNION

SELECT pz.name
FROM person_order po
INNER JOIN menu m ON m.id = po.menu_id
INNER JOIN person p ON p.id = po.person_id
INNER JOIN pizzeria pz ON pz.id = m.pizzeria_id
GROUP BY pz.name
HAVING SUM(CASE WHEN p.gender = 'female' THEN 1 ELSE 0 END) > 0
    AND SUM(CASE WHEN p.gender = 'male' THEN 1 ELSE 0 END) = 0
ORDER BY 1;