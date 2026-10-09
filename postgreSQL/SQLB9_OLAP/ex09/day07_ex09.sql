SELECT
    p.address,
    ROUND(MAX(p.age) - (MIN(p.age)::numeric / MAX(p.age)), 2) AS formula,
    ROUND(AVG(p.age), 2) AS average_age,
    (MAX(p.age) - (MIN(p.age)::numeric / MAX(p.age))) > AVG(p.age) AS comparison
FROM person p
GROUP BY p.address
ORDER BY p.address ASC;