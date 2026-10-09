CREATE OR REPLACE FUNCTION func_minimum(arr NUMERIC[])
RETURNS NUMERIC
AS $$
    DECLARE
    min_val NUMERIC;
    temp NUMERIC;
BEGIN  
    FOREACH temp IN ARRAY arr
    LOOP
        IF temp IS NOT NULL THEN
            IF min_val IS NULL OR temp < min_val THEN
            min_val := temp;
            END IF;
        END IF;
    END LOOP;
    RETURN min_val;
END;
$$ LANGUAGE plpgsql;

SELECT func_minimum(VARIADIC arr => ARRAY[10.0, -1.0, 5.0, 4.4, -2]);
