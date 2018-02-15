CREATE KEYSPACE IF NOT EXISTS Tonstudio WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 } ;

USE Tonstudio;

DROP TABLE IF EXISTS Tonstudio.audioStreams;

CREATE TABLE audioStreams ( name varchar PRIMARY KEY, typeIsRaw boolean, stream blob, project varchar);

INSERT INTO audioStreams (name, typeIsRaw, stream, varchar) VALUES ('fred', true ,bigintAsBlob(3), 'CoolesProjekt');
 
SELECT * FROM audioStreams;

TRUNCATE Tonstudio.audioStreams;