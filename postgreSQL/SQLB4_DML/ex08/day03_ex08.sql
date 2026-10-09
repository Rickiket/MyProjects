INSERT INTO menu (id, pizzeria_id, pizza_name, price)
SELECT 
COALESCE(MAX(m.id), 0) + 1, 
MIN(pz.id),
'sicilian pizza', 
900
FROM menu m
RIGHT JOIN pizzeria pz ON TRUE
WHERE pz.name = 'Dominos';