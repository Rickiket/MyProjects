SELECT pizza_name
FROM
(
    SELECT pizza_name FROM menu
    UNION
    SELECT pizza_name FROM menu
) AS pizza_name
ORDER BY pizza_name DESC;