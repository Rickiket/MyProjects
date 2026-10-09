SELECT
    p.name,
    temp.count_of_visits
FROM(
        SELECT
            pv.person_id,
            COUNT(*) AS count_of_visits
        FROM person_visits pv
        GROUP BY pv.person_id
        ORDER BY count_of_visits DESC
        FETCH FIRST 4 ROWS ONLY
    ) AS temp
INNER JOIN person p
    ON p.id = temp.person_id
ORDER BY temp.count_of_visits DESC, p.name ASC;