--------WAVmaker v.2.0--------

Code Author: David Nelson

Compatibility: Windows Desktop

------------------------------

I.   Overview
II.  How To Use The WAVmaker
III. Tips
IV.  Changes

NOTE: This program requires a basic understanding of music theory.


I.   Overview

	WAVmaker lets you compose and play songs through the console. 
	The program creates a 16-bit WAV file with a sample rate of 44100 
	frames/sec. 

II.  How To Use The WAVmaker

	The program will prompt you for further instruction. 
	WAVMaker's prompts, << , are detailed below.
 
 
	Time Signature	4 --> how many beats per measure
		        _
			
			4 --> note with a length of one beat  
  
	<< Which note represents a beat?

		Accepts: 1, 2, 4, 8, 16, 32, ... (2^n)

				1: Whole Note 4: Quarter Note
				2: Half Note  8: Eighth Note

	The tempo is the beats per minute.

	<< What is the tempo?

		Accepts: Any Number > 0
	
 	<< Enter a note.

		Example User Input: c42s1a75d1t

		Parsing

			c4:  The octave that the note belongs to. The program 
			     uses scientific pitch notation (c0 - c9). In this 
			     example, the note is part of the fifth octave with 
			     a "C" pitch.

			2:   The type of note. In this example, the "2" 
			     represents a half note.

			s1:  Signifies a sharp note. "b" is used for flats. In 
			     this example, the note is a C sharp (s2 would be C 
			     double sharp).

			a75: Note volume. Value ranges from 0-100. Default is 
			     set at 100.

			d1:  Signifies a dotted note (d2 is a double dotted 
			     note).

			 t:  Note will be tied to the next note.

		NOTE: These segments can be inputted in any order and can be 
		      omitted as well. However, there is one restriction.

			1. The octave must be entered first, followed by the 
			   note type. Neither of these can be omitted.

	Other User Commands

		ch:   Change the chord setting to ON. Notes are added to a 
		      chord. Input ch again to change the chord setting to OFF.

		n:    Start a new chord. This command only works when the chord 
		      setting is set to ON.

		show: Display all measures.

		q:    Signals to the program that the user has finished 
		      composing. The program will generate a WAV file, 
		      music.wav, in the working directory's "Songs" folder.

		NOTE: music.wav can be moved from its location after closing 
		      WAVmaker. If it is not moved, the file will be 
		      overwritten the next time WAVmaker is used.

	WAVmaker comes with twenty example tracks. Each track is a popular 
	nursery rhyme. 

	<< Play example song? (1-20)  		 
		      
		Accepts: 1-20, show, show(1-20), q

		1:     Plays track 1.
		show:  Displays all track names.
		show1: Displays track 1 name.
		q:     Exit WAVmaker.
	  
III. Tips

	Shortcut

		-If you possess the sheet music for the song you want to 
		 compose in WAVmaker, open Notepad and type the instructions in
		 a column. Copy and paste the instructions into WAVmaker.

			e44b1
			f44t
			f44t
			g48d1
			...

	There is something wrong with my song. It should not sound like that! 

		-The complex waveforms that are produced by musical instruments can 
		 vary considerably from the sine waves modelled by this 
		 program. Inharmonicity is why a note played by a piano makes a 
		 different sound than the same note played on a guitar, or a 
		 violin, or in our case, a synthesizer. Many of the partials 
		 that combine to make these complex waveforms are not exact 
		 multiples of the fundamental frequency. With that being said, 
		 the flute is one of the closest instruments to producing an 
		 actual sine wave. The quality of your song should improve 
		 drastically by switching over to sheet music that is meant to 
		 be played a flute. Transposing the sheet music you have is 
		 another option.    
		
	Clipping

		-The sound will be distorted if the chord's volume exceeds the 
		 maximum volume. This is known as "clipping" and can be 
		 prevented by carefully setting the volume of each chord note.

			chord: f44a33, a44a33, c44a33

	Rests

		-Simply set the volume to zero to create a rest. 

IV.  Changes

	Version 2.0
	
	+the user can now set the bpm (beats per measure)
	+the song is no longer limited to one measure; measure length is 
	 enforced
	+measures are displayed on the console
	+chords have been implemented
	+WAVmaker can play songs
	+twenty example tracks are included with the program
	
	Version 1.0

	+create a single measure with four notes; bpm, beats per minute, is 
	 fixed
	+program generates a WAV file