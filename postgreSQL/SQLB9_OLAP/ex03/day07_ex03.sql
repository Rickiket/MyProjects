WITH order_cte AS (
    SELECT
        pz.id AS pz_id,
        pz.name AS pz_name,
        COUNT(*) AS order_count
    FROM person_order po
    INNER JOIN menu m
        ON m.id = po.menu_id
    INNER JOIN pizzeria pz
        ON pz.id = m.pizzeria_id
    GROUP BY pz.id, pz.name
),
visit_cte AS (
    SELECT
        pz.id AS pz_id,
        pz.name AS pz_name,
        COUNT(*) AS visit_count
    FROM person_visits pv
    INNER JOIN pizzeria pz
        ON pz.id = pv.pizzeria_id
    GROUP BY pz.id, pz.name
)

SELECT
    COALESCE(order_cte.pz_name, visit_cte.pz_name) AS name,
    COALESCE(order_cte.order_count, 0) + COALESCE(visit_cte.visit_count, 0) AS total_count
FROM order_cte
FULL JOIN visit_cte
    ON visit_cte.pz_id = order_cte.pz_id
ORDER BY total_count DESC, name ASC;