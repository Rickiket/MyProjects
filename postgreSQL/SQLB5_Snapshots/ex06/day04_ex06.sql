CREATE MATERIALIZED VIEW mv_dmitriy_visits_and_eats AS
SELECT pz.name
FROM pizzeria pz
WHERE EXISTS (
    SELECT 1
    FROM person_visits pv
    INNER JOIN person p ON p.id = pv.person_id
    WHERE pv.pizzeria_id = pz.id
    AND visit_date = DATE '2022-01-08'
    AND p.name = 'Dmitriy'
)
AND EXISTS (
    SELECT 1
    FROM menu m
    WHERE m.pizzeria_id = pz.id
    AND m.price < 800
);