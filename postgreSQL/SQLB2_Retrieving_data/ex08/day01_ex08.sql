SELECT
po.order_date,
person.name || ' (age:' || person.age || ')' AS person_information
FROM 
(
    SELECT 
    person_id AS id,
    order_date
    FROM person_order
) AS po
NATURAL JOIN person
ORDER BY order_date ASC, person_information ASC;