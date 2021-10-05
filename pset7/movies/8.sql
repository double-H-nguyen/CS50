-- write a SQL query to list the names of all people who starred in Toy Story
SELECT people.name
FROM stars
    JOIN people ON stars.person_id = people.id
    JOIN movies ON stars.movie_id = movies.id
WHERE movies.title = "Toy Story";