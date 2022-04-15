SELECT name FROM songs;
SELECT name FROM songs ORDER BY tempo;
SELECT name FROM songs ORDER BY duration_ms DESC LIMIT 5;
SELECT name FROM songs WHERE danceability > .75 AND energy > .75 AND valence > .75;
SELECT AVG(energy) FROM songs;
SELECT S.name FROM songs S JOIN artists A on S.artist_id = A.id WHERE A.name = 'Post Malone';
SELECT AVG(energy) FROM songs S JOIN artists A on S.artist_id = A.id WHERE A.name = 'Drake';
SELECT name FROM songs WHERE name like '%feat.%';
