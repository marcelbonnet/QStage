-- DB QStage
CREATE TABLE IF NOT EXISTS  musicas( musica_id INTEGER NOT NULL PRIMARY KEY, titulo STRING NOT NULL, html TEXT, programa TEXT, artista STRING);
CREATE TABLE IF NOT EXISTS  playlist_musicas(fk_musica INTEGER NOT NULL, fk_playlist INTEGER NOT NULL, ordem INTEGER, FOREIGN KEY (fk_musica) REFERENCES musicas(id), foreign key (fk_playlist) references playlist(playlist_id) );
CREATE TABLE IF NOT EXISTS  playlist(id integer not null primary key, titulo STRING NOT NULL , data_apresentacao DATE);
CREATE TABLE IF NOT EXISTS  modelos(id INTEGER PRIMARY KEY, nome STRING NOT NULL, modelo TEXT NOT NULL, tipo STRING NOT NULL);
CREATE TABLE IF NOT EXISTS waveforms(id INTEGER PRIMARY KEY, nome STRING NOT NULL, groupType INTEGER NOT NULL, groupId INTEGER NOT NULL, waveGroup STRING NOT NULL, oneShot INTEGER NOT NULL, number INTEGER NOT NULL);
