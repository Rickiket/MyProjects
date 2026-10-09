UPDATE menu
SET price = (price * 0.9)::int
WHERE pizza_name = 'greek pizza';
