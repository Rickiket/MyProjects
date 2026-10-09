SELECT DISTINCT p.name
FROM person_order po
INNER JOIN person p
    ON p.id = po.person_id
INNER JOIN menu m
    ON m.id = po.menu_id
WHERE m.pizza_name IN ('mushroom pizza', 'pepperoni pizza')
    AND p.gender = 'male'
    AND p.address IN ('Moscow', 'Samara')
ORDER BY 1 DESC;