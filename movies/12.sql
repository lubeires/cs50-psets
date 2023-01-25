SELECT title
FROM movies
WHERE id IN
(
    SELECT movies.id
    FROM movies
    JOIN stars
    ON movies.id = stars.movie_id
    JOIN people
    ON stars.person_id = people.id
    WHERE people.name = "Johnny Depp"
)
AND id IN
(
    SELECT movies.id
    FROM movies
    JOIN stars
    ON movies.id = stars.movie_id
    JOIN people
    ON stars.person_id = people.id
    WHERE people.name = "Helena Bonham Carter"
);