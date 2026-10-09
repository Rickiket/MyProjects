SELECT p.name
FROM person p
INNER JOIN person_order po
    ON po.person_id = p.id
INNER JOIN menu m
    ON m.id = po.menu_id
WHERE m.pizza_name IN ('cheese pizza', 'pepperoni pizza')
    AND p.gender = 'female'
GROUP BY p.name
HAVING COUNT(DISTINCT m.pizza_name) = 2
ORDER BY 1;




--SELECT p.name
--FROM person p
--WHERE p.gender = 'female'
--    AND EXISTS (
--        SELECT 1
--        FROM person_order po
--        INNER JOIN menu m ON po.menu_id = m.id
--        WHERE po.person_id = p.id
--            AND m.pizza_name = 'cheese pizza'
--    )
--    AND EXISTS (
--        SELECT 1
--        FROM person_order po
--        INNER JOIN menu m ON po.menu_id = m.id
--        WHERE po.person_id = p.id
--            AND m.pizza_name = 'pepperoni pizza'
--    )
--ORDER BY 1;