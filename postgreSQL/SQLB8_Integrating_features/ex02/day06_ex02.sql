SELECT
    p.name,
    m.pizza_name,
    m.price,
    (m.price * (1-pd.discount / 100))::BIGINT AS discount_price,
    pz.name AS pizzeria_name
FROM person_order po
INNER JOIN menu m
    ON m.id = po.menu_id
INNER JOIN pizzeria pz
    ON pz.id = m.pizzeria_id
INNER JOIN person p
    ON p.id = po.person_id
LEFT JOIN person_discounts pd
    ON pd.person_id = p.id
    AND pd.pizzeria_id = pz.id
ORDER BY p.name, m.pizza_name;