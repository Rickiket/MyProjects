CREATE SEQUENCE seq_person_discounts START 1;

SELECT setval(
    'seq_person_discounts',
    (SELECT COUNT(*) FROM person_discounts) + 1,
    false
);

ALTER TABLE person_discounts
ALTER COLUMN id SET DEFAULT nextval('seq_person_discounts');

ALTER SEQUENCE seq_person_discounts
OWNED BY person_discounts.id;