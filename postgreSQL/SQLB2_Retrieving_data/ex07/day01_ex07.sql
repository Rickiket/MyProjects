SELECT po.order_date,
p.name || ' (age:' || p.age || ')' AS person_information
FROM person_order po
INNER JOIN person p
    ON p.id = po.person_id
ORDER BY order_date ASC, person_information ASC;