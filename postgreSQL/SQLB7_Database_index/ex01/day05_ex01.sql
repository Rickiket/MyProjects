EXPLAIN ANALYZE
SELECT
    m.pizza_name,
    pz.name AS pizzeria_name
FROM menu m
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id;

SET enable_seqscan = OFF;

EXPLAIN ANALYZE
SELECT
    m.pizza_name,
    pz.name AS pizzeria_name
FROM menu m
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id;

SET enable_seqscan = ON;