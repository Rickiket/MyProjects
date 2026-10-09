SELECT missing_date::date
FROM generate_series(DATE '2022-01-01', DATE '2022-01-10', INTERVAL '1 day') AS missing_date
LEFT JOIN person_visits
    ON visit_date = missing_date
    AND (person_id = 1 OR person_id = 2)
WHERE person_id IS NULL
ORDER BY 1;