SELECT SUM(songs.energy)/COUNT(*)
AS drake_average_energy
FROM songs
JOIN artists
ON songs.artist_id = artists.id
WHERE artists.name = "Drake";