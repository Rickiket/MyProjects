INSERT INTO person_visits (id, person_id, pizzeria_id, visit_date)
SELECT
(SELECT COALESCE(MAX(id), 0) FROM person_visits) + ROW_NUMBER() OVER (),
p.id,
pz.id,
DATE '2022-01-08'
FROM person p
INNER JOIN pizzeria pz ON pz.name = 'DoDo Pizza'
WHERE p.name = 'Dmitriy'
ON CONFLICT DO NOTHING;

REFRESH MATERIALIZED VIEW mv_dmitriy_visits_and_eats;