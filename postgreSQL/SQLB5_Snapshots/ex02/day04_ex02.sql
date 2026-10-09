CREATE VIEW v_generated_dates AS
SELECT g_d.generated_date::date
FROM generate_series(DATE '2022-01-01', DATE '2022-01-31', INTERVAL '1 day') AS g_d(generated_date)
ORDER BY g_d.generated_date;