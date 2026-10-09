-- SELECT
-- t_1.pizza_name,
-- t_1.pizzeria_name AS pizza_name_1,
-- t_2.pizzeria_name AS pizza_name_2,
-- t_1.price
-- FROM (
--     SELECT 
--     m.pizza_name,
--     pz.name AS pizzeria_name,
--     m.price,
--     m.pizzeria_id
--     FROM menu m
--     INNER JOIN pizzeria pz ON pz.id = m.pizzeria_id
-- ) AS t_1
-- INNER JOIN 
-- (
--     SELECT 
--     m.pizza_name,
--     pz.name AS pizzeria_name,
--     m.price,
--     m.pizzeria_id
--     FROM menu m
--     INNER JOIN pizzeria pz ON pz.id = m.pizzeria_id
-- ) t_2 ON t_2.pizza_name = t_1.pizza_name
--       AND t_2.price = t_1.price
--       AND t_2.pizzeria_id < t_1.pizzeria_id
--     ORDER BY 1;

SELECT
self_menu.pizza_name,
pz_1.name AS pizzeria_name_1,
pz_2.name AS pizzeria_name_2,
self_menu.price
FROM (
    SELECT
    m1.pizza_name,
    m1.pizzeria_id AS pizzeria_id_1,
    m2.pizzeria_id AS pizzeria_id_2,
    m1.price
    FROM menu m1
    INNER JOIN menu m2
        ON m2.price = m1.price
        AND m2.pizza_name = m1.pizza_name
        AND m2.pizzeria_id < m1.pizzeria_id
) AS self_menu
INNER JOIN pizzeria pz_1 ON pz_1.id = self_menu.pizzeria_id_1
INNER JOIN pizzeria pz_2 ON pz_2.id = self_menu.pizzeria_id_2
ORDER BY 1;