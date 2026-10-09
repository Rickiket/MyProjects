SELECT pz.name AS pizzeria_name
FROM pizzeria pz
INNER JOIN person_visits pv
    ON pv.pizzeria_id = pz.id
INNER JOIN person p
    ON p.id = pv.person_id
GROUP BY pz.name
HAVING SUM(CASE WHEN p.gender = 'male' THEN 1 ELSE 0 END)
        >
        SUM(CASE WHEN p.gender = 'female' THEN 1 ELSE 0 END)

UNION ALL

SELECT pz.name
FROM pizzeria pz
INNER JOIN person_visits pv
    ON pv.pizzeria_id = pz.id
INNER JOIN person p
    ON p.id = pv.person_id
GROUP BY pz.name
HAVING SUM(CASE WHEN p.gender = 'male' THEN 1 ELSE 0 END)
        <
        SUM(CASE WHEN p.gender = 'female' THEN 1 ELSE 0 END)
ORDER BY 1;