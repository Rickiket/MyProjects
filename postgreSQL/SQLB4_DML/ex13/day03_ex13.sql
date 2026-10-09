DELETE FROM person_order
WHERE order_date = DATE '2022-02-25';

DELETE FROM menu
WHERE pizza_name = 'greek pizza'
AND pizzeria_id = (SELECT MIN(id)
                   FROM pizzeria
                   WHERE name = 'Dominos'
                  );