CREATE OR REPLACE FUNCTION fnc_trg_person_update_audit()
RETURNS trigger AS $$
BEGIN
    INSERT INTO person_audit(name, age, gender, address, type_event, row_id)
    VALUES (OLD.name, OLD.age, OLD.gender, OLD.address, 'U', OLD.id);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_person_update_audit
AFTER UPDATE ON person
FOR EACH ROW
EXECUTE FUNCTION fnc_trg_person_update_audit();

UPDATE person SET name = 'Bulat' WHERE id = 10; 
UPDATE person SET name = 'Damir' WHERE id = 10;
