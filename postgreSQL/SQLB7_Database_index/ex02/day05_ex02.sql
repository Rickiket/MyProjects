CREATE INDEX id_person_name
ON person USING btree (UPPER(name));

EXPLAIN ANALYZE
SELECT name
FROM person
WHERE UPPER(name) = 'ANNA';

SET enable_seqscan = OFF;
EXPLAIN ANALYZE
SELECT name
FROM person
WHERE UPPER(name) = 'ANNA';
SET enable_seqscan = ON;