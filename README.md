# QStage

It is a program to send SYSEX MIDI Messages to my Roland Keyboard. Since it is the first program I wrote in C++, it was basically focused on my needs and as an opportunity to educate myself on C++. I chose to use Qt Framework, and almost nothing from stdlib.

## QStage Features

	- saves a list of musics as Roland configurations and lyrics/chords
	- displays the selected music (lyrics, chords, or whatever was saved as HTML markup)
	- connects to a jack midi port and send SYSEX MIDI messages to configure Roland's "PERFORM" Mode.
	- display all the Roland parameters as widgets on the screen, so the keyboard may be modified in realtime and all changes saved to a file


