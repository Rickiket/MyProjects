CREATE UNIQUE INDEX idx_menu_unique
ON menu USING btree (pizzeria_id, pizza_name);

EXPLAIN ANALYZE
SELECT *
FROM menu m
WHERE m.pizzeria_id = 1
    AND m.pizza_name = 'cheese pizza';

SET enable_seqscan = OFF;
EXPLAIN ANALYZE
SELECT *
FROM menu m
WHERE m.pizzeria_id = 1
    AND m.pizza_name = 'cheese pizza';
SET enable_seqscan = ON;