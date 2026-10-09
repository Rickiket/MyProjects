SELECT 
menu.pizza_name, pizzeria.name AS pizzeria_name
FROM person_order
INNER JOIN menu
    ON  menu.id = person_order.menu_id
INNER JOIN person
    ON person.id = person_order.person_id
INNER JOIN pizzeria
    ON pizzeria.id = menu.pizzeria_id
WHERE person.name IN ('Denis', 'Anna')
ORDER BY 1, 2;