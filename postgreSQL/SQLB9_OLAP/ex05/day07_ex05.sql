SELECT DISTINCT p.name
FROM person_order po
INNER JOIN person p
    ON p.id = po.person_id
ORDER BY name;