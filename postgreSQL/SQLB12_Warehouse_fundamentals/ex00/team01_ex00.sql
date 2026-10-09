WITH last_currency_rate AS (
    SELECT DISTINCT ON (id) 
        id,
        name,
        rate_to_usd
        FROM currency
        ORDER BY id, updated DESC
),
user_balances AS (
    SELECT
        user_id,
        type,
        currency_id,
        SUM(money) AS volume
    FROM balance
    GROUP BY user_id, type, currency_id
)
SELECT 
    COALESCE(u.name, 'not defined') AS name,
    COALESCE(u.lastname, 'not defined') AS lastname,
    user_balances.type,
    user_balances.volume,
    COALESCE(last_currency_rate.name, 'not defined') AS currency_name,
    COALESCE(last_currency_rate.rate_to_usd, 1) AS last_rate_to_usd,
    (user_balances.volume * COALESCE(last_currency_rate.rate_to_usd, 1))::DOUBLE PRECISION AS total_volume_to_usd
FROM user_balances
LEFT JOIN "user" u ON u.id = user_balances.user_id
LEFT JOIN last_currency_rate ON last_currency_rate.id = user_balances.currency_id
ORDER BY name DESC, lastname ASC, type ASC;