SELECT
pv.visit_date AS action_date,
p.name AS person_name
FROM person_visits pv
INNER JOIN person p
    ON p.id = pv.person_id
WHERE EXISTS 
(
    SELECT 1
    FROM person_order po
    WHERE po.order_date = pv.visit_date
    AND po.person_id = pv.person_id
)
ORDER BY action_date ASC, person_name DESC;