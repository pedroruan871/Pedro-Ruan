SELECT ROUND(AVG(energy), 3)
FROM songs
WHERE artist_id =
(
    SELECT id
    FROM artists
    WHERE name = "Drake"
)