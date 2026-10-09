WITH cte AS
(
    SELECT gs::date
    FROM generate_series(DATE '2022-01-01', DATE '2022-01-10', INTERVAL '1 day') AS gs
)

SELECT cte.gs AS missing_date
FROM cte
LEFT JOIN person_visits pv
    ON pv.visit_date = cte.gs
    AND (pv.person_id = 1 OR pv.person_id = 2)
WHERE person_id IS NULL
ORDER BY 1;