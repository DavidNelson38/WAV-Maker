--------WAV maker v.0.1--------

Code Author: David Nelson

-------------------------------

I.   Overview
II.  How To Use The WAV maker
III. Current Version
IV.  Future Versions

NOTE: This program requires a basic understanding of music theory.


I.   Overview

	Program creates a 16-bit WAV file with a sample rate of 44100 
	frames/sec. 

II.  How to Use The WAV maker

	Time Signature	4 --> how many beats per measure
		        _
			
			4 --> note with length equal to one beat  
  
	<< Which note represents a beat?

		Accepts: 1, 2, 4, 8, 16, 32, ... (2^n)

				1: Whole Note 4: Quarter Note
				2: Half Note  8: Eighth Note

	The tempo is the beats per minute.

	<< What is the tempo?

		Accepts: Any number > 0
	
 	<< Enter a note.

		Example User Input: c42s1a75d1t

		Parsing

			c4:  the octave that the note belongs to. The program 
			     uses scientific pitch notation (c0 - c9). In this 
			     example, the note is part of the fifth octave with 
			     a "C" pitch.

			2:   the type of note. In this example, the "2" 
			     represents a half note.

			s1:  signifies a sharp note. "b" is used for flats. In 
			     this example, the note is a C sharp (s2 would be C 
			     double sharp).

			a75: note's amplitude. Value ranges from 0-100. Default 
			     is set at 100.

			d1:  signifies a dotted note (d2 is a double dotted 
			     note).

			 t:  note will be tied to the next note.

		NOTE: these instructions can be input in any order and can be 
		      omitted as well. However, there is one restriction.

			1. The octave must be entered first, followed by the 
			   note type. Neither of these can be omitted.

	Upon completion, the program will generate a WAV file in the current 
	directory. When opened in the appropriate player, the file will play 
	the notes in succession.

III. Current Version

	-Although the program asks for the beats per measure, it will always 
	 accept four notes
	-Program ends when four notes have been entered 

IV.  Future Versions

	-Add Comments to Source Code
	-Force the combined note length to equal the measure's length
	-Create multiple measures
	-Functionality to delete or replace notes
	-Display notes in command window
	-Implement chords 