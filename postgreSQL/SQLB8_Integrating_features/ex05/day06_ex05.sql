COMMENT ON TABLE person_discounts IS
'Таблица для хранения скидок пользователей в пиццериях';

COMMENT ON COLUMN person_discounts.id IS
'Идентификатор записи';
COMMENT ON COLUMN person_discounts.person_id IS
'Идентификатор юзера у которога есть скидка';
COMMENT ON COLUMN person_discounts.pizzeria_id IS
'Индетификатор пиццерии, в которой эта скидка';
COMMENT ON COLUMN person_discounts.discount IS
'Размер скидки в %';