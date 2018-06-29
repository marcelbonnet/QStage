CREATE TABLE musicas( musica_id INTEGER NOT NULL PRIMARY KEY, titulo STRING NOT NULL, html TEXT, programa TEXT);
CREATE TABLE playlist_musicas(fk_musica INTEGER NOT NULL, fk_playlist INTEGER NOT NULL, ordem INTEGER, FOREIGN KEY (fk_musica) REFERENCES musicas(id), foreign key (fk_playlist) references playlist(playlist_id) );
CREATE TABLE playlist(id integer not null primary key, titulo STRING NOT NULL , data_apresentacao DATE);
