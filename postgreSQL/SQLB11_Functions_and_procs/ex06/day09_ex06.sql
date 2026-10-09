CREATE OR REPLACE FUNCTION fnc_person_visits_and_eats_on_date(
    pperson VARCHAR DEFAULT 'Dmitriy',
    pprice NUMERIC DEFAULT 500,
    pdate DATE DEFAULT '2022-01-08'
)
RETURNS SETOF VARCHAR
AS $$
BEGIN
RETURN QUERY
    SELECT DISTINCT pz.name
    FROM person_visits pv
    INNER JOIN person p
        ON p.id = pv.person_id
    INNER JOIN pizzeria pz
        ON pz.id = pv.pizzeria_id
    INNER JOIN menu m
        ON pz.id = m.pizzeria_id
    WHERE p.name = pperson
        AND m.price < pprice
        AND pv.visit_date = pdate;
END;
$$ LANGUAGE plpgsql;

select *  
from fnc_person_visits_and_eats_on_date(pprice := 800);

select *  
from fnc_person_visits_and_eats_on_date(pperson := 'Anna',pprice := 1300,pdate := '2022-01-01');
