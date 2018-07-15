-- DB QStage
CREATE TABLE musicas( musica_id INTEGER NOT NULL PRIMARY KEY, titulo STRING NOT NULL, html TEXT, programa TEXT, artista STRING);
CREATE TABLE playlist_musicas(fk_musica INTEGER NOT NULL, fk_playlist INTEGER NOT NULL, ordem INTEGER, FOREIGN KEY (fk_musica) REFERENCES musicas(id), foreign key (fk_playlist) references playlist(playlist_id) );
CREATE TABLE playlist(id integer not null primary key, titulo STRING NOT NULL , data_apresentacao DATE);
CREATE TABLE modelos(id INTEGER PRIMARY KEY, nome STRING NOT NULL, modelo TEXT NOT NULL, tipo STRING NOT NULL);
-- DUMP DE DADOS
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
-- CREATE TABLE modelos(id INTEGER PRIMARY KEY, nome STRING NOT NULL, modelo TEXT NOT NULL, tipo STRING NOT NULL);
INSERT INTO "modelos" VALUES(2,'Nova Música','<html><head><meta="utf-8"><style>body{color:#fff; background-color: #000; } h1, h2 { color:orange; }</style></head><body><h1>Teste</h1><div><p>testando</p></div></body></html>','default');
INSERT INTO "modelos" VALUES(3,'Modelo 2','<body><h1>Modelo #2</h2><iframe src="http://localhost"></body>','Teste');
COMMIT;
