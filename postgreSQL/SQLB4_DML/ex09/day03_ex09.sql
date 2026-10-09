INSERT INTO person_visits (id, person_id, pizzeria_id, visit_date)
SELECT
(
    SELECT COALESCE(MAX(id), 0 )
    FROM person_visits
) + ROW_NUMBER() OVER (ORDER BY p.id),
p.id,
pz.id,
DATE '2022-02-24'
FROM person p
INNER JOIN pizzeria pz ON pz.name = 'Dominos'
WHERE p.name IN ('Denis', 'Irina');