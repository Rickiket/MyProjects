CREATE VIEW v_symmetric_union AS
(
    SELECT pv1.person_id
    FROM person_visits pv1
    WHERE visit_date = DATE '2022-01-02'
)
UNION
(
    SELECT pv2.person_id
    FROM person_visits pv2
    WHERE visit_date = DATE '2022-01-06'
)
ORDER BY 1;