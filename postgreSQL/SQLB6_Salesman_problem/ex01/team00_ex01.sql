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
ORDER BY total_cost,tour;