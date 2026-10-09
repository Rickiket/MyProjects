SELECT m.id AS menu_id
FROM menu m
WHERE NOT EXISTS (
    SELECT 1
    FROM person_order
    WHERE menu_id = m.id
)
ORDER BY 1;