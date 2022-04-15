-- CREATE TABLE movies (
--                     id INTEGER,
--                     title TEXT NOT NULL,
--                     year NUMERIC,
--                     PRIMARY KEY(id)
--                 );
-- CREATE TABLE stars (
--                 movie_id INTEGER NOT NULL,
--                 person_id INTEGER NOT NULL,
--                 FOREIGN KEY(movie_id) REFERENCES movies(id),
--                 FOREIGN KEY(person_id) REFERENCES people(id)
--             );
-- CREATE TABLE directors (
--                 movie_id INTEGER NOT NULL,
--                 person_id INTEGER NOT NULL,
--                 FOREIGN KEY(movie_id) REFERENCES movies(id),
--                 FOREIGN KEY(person_id) REFERENCES people(id)
--             );
-- CREATE TABLE ratings (
--                 movie_id INTEGER NOT NULL,
--                 rating REAL NOT NULL,
--                 votes INTEGER NOT NULL,
--                 FOREIGN KEY(movie_id) REFERENCES movies(id)
--             );
-- CREATE TABLE people (
--                 id INTEGER,
--                 name TEXT NOT NULL,
--                 birth NUMERIC,
--                 PRIMARY KEY(id)
--             );

SELECT title FROM movies WHERE year = 2008;
SELECT birth FROM people WHERE name = 'Emma Stone';
SELECT title FROM movies WHERE year >= 2018 ORDER BY title;
SELECT COUNT(*) FROM movies M JOIN ratings R ON M.id = R.movie_id WHERE R.rating = 10.0;
SELECT title, year FROM movies WHERE title LIKE 'Harry Potter%' ORDER BY year;
SELECT AVG(R.rating) FROM movies M JOIN ratings R ON M.id = R.movie_id WHERE M.year = 2012;
SELECT M.title, R.rating FROM movies M JOIN ratings R ON M.id = R.movie_id WHERE year = 2010 ORDER BY R.rating DESC, M.title;
SELECT P.name FROM people P JOIN stars S ON P.id = S.person_id JOIN movies M ON M.id = S.movie_id WHERE title = 'Toy Story';
SELECT DISTINCT P.name FROM people P JOIN stars S ON P.id = S.person_id JOIN movies M ON M.id = S.movie_id WHERE year = 2004 ORDER BY P.birth;
SELECT DISTINCT P.name FROM people P JOIN directors D ON P.id = D.person_id JOIN ratings R ON R.movie_id = D.movie_id WHERE R.rating >= 9.0;
SELECT M.title FROM movies M JOIN stars S ON M.id = S.movie_id JOIN people P ON P.id = S.person_id JOIN ratings R ON M.id = R.movie_id WHERE P.name = 'Chadwick Boseman' ORDER BY R.rating DESC LIMIT 5;
SELECT title FROM movies WHERE id IN ( SELECT S.movie_id FROM stars S JOIN people P ON P.id = S.person_id WHERE P.name = 'Johnny Depp' INTERSECT SELECT S.movie_id FROM stars S JOIN people P ON P.id = S.person_id WHERE P.name = 'Helena Bonham Carter' );
SELECT P.name FROM people P JOIN stars S ON P.id = S.person_id WHERE P.name != 'Kevin Bacon' AND S.movie_id IN (SELECT S.movie_id FROM people P JOIN stars S ON P.id = S.person_id WHERE P.name = 'Kevin Bacon' AND birth = '1958');
