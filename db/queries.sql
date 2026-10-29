-- 1. 10 последних показаний
SELECT * FROM readings ORDER BY id DESC LIMIT 10;

-- 2. минимум, среднее и максимум по каждому датчику
SELECT sensor, MIN(value) AS min_v, ROUND(AVG(value), 2) AS avg_v, MAX(value) AS max_v
FROM readings GROUP BY sensor;

-- 3. все события по порядку времени
SELECT * FROM events ORDER BY t_s, id;

-- 4. количество записей в каждой таблице
SELECT 'readings' AS tbl, COUNT(*) AS cnt FROM readings
UNION ALL
SELECT 'events', COUNT(*) FROM events;
