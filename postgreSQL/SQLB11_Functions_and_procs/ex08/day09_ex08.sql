CREATE OR REPLACE FUNCTION fnc_fibonacci(pstop INTEGER DEFAULT 10)
RETURNS SETOF INTEGER
AS $$
    DECLARE
        first_val INTEGER := 0;
        second_val INTEGER := 1;
        temp INTEGER;
    BEGIN
        WHILE first_val < pstop LOOP
            RETURN NEXT first_val;
            temp := first_val + second_val;
            first_val := second_val;
            second_val := temp;
        END LOOP;
    END;
$$ LANGUAGE plpgsql;

select * from fnc_fibonacci(100);

select * from fnc_fibonacci();
