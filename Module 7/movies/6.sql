SELECT substr(AVG(rating) * 1.0, 1, instr(AVG(rating) * 1.0, '.') + 2)
FROM ratings
WHERE(
    SELECT id
    FROM movies
    WHERE year = 2012
)