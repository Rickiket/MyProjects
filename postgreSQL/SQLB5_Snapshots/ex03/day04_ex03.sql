SELECT v_dates.generated_date AS missing_date
FROM v_generated_dates v_dates
WHERE NOT EXISTS (
    SELECT 1
    FROM person_visits pv
    WHERE pv.visit_date = v_dates.generated_date
)
ORDER BY 1;