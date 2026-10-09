CREATE TABLE cities (
    id INT PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

CREATE TABLE distances (
    from_city INT,
    to_city INT,
    distance INT,
    PRIMARY KEY (from_city, to_city)
);

INSERT INTO cities (id, name) VALUES
(1, 'a'),
(2, 'b'),
(3, 'c'),
(4, 'd');

INSERT INTO distances (from_city, to_city, distance) VALUES
(1, 2, 10),
(1, 3, 15),
(1, 4, 20),
(2, 3, 35),
(2, 4, 25),
(3, 4, 30);

SELECT *
FROM cities;

SELECT *
FROM distances;


CREATE VIEW symmetric_distances AS
SELECT from_city, to_city, distance FROM distances
UNION ALL
SELECT to_city, from_city, distance FROM distances;

DROP VIEW IF EXISTS symmetric_distances;

WITH RECURSIVE tsp AS (
    SELECT 
        ARRAY[1] AS path,
        1 AS last_city,
        0 AS total_distance
    UNION ALL
    SELECT 
        tsp.path || d.to_city,
        d.to_city,
        tsp.total_distance + d.distance
    FROM tsp
    JOIN symmetric_distances d
        ON d.from_city = tsp.last_city
    WHERE NOT (d.to_city = ANY(tsp.path))
      AND array_length(tsp.path, 1) < 4
),
complete_routes AS (
    SELECT 
        tsp.path || 1 AS route,
        tsp.total_distance + d.distance AS total_distance
    FROM tsp
    JOIN symmetric_distances d
        ON d.from_city = tsp.last_city
       AND d.to_city = 1
    WHERE array_length(tsp.path, 1) = 4
)
SELECT
    total_distance AS total_cost,
    ARRAY(
        SELECT c.name
        FROM unnest(route) WITH ORDINALITY AS t(city_id, ord)
        JOIN cities c
            ON c.id = t.city_id
        ORDER BY t.ord
    ) AS tour
FROM complete_routes
WHERE total_distance = (SELECT MIN(total_distance) FROM complete_routes)
ORDER BY tour;