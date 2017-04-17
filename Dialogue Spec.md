List of global variables:
```
	choice (int) = last choice segment's result
```

List of instructions:
```
	speaker <string>[name of speaker]
		Set the speaker's name
	content <string>[content of dialogue]
		Set the dialogue content
	playvox <string>[path to vox]
		Play a voice clip (will animate active speaker)
	lchrimg <string>[path to image sequence of left character]
		Set the image sequence for left speaker
	rchrimg <string>[path to image sequence of right character]
		Set the image sequence for right speaker
	setactive <int>[ 0 - left speaker speaking ; 1 - right speaker speaking ; -1 - no speaker speaking]
		Set the active speaker (used for VOX and lipsync)
	wait <int>[amount of frames to wait (60 = 1s)]
		Waits a specified amount of frames
	waittillvoxend
		Waits until the currently playing VOX is finished
	waittillkey
		Waits until player presses Enter
	waittillkeyorvox
		Waits until VOX finishes playing or player presses enter
	jmpif <string>[expression]
		Skips next line if expression returns true
	goto <string>[label]
		Goes to label
	jmp <int>[lines]
		Jumps X lines
	setvar <int>[var] <any>[value]
		Sets a value to a var
	setchoicecount <int>[count]
		Sets the choice count for next choice segment
	choice <int>[id] <string>[label]
		Sets the label for a specific choice
	execchoice
		Executes a choice segment, sets the result to choice
```

Example:
```
begin:
	speaker "Raymond"
	content "Hello!"
	lchrimg "char/raymond/left_side"
	rchrimg "char/npc1/right_side"
	setactive 0
	playvox "vox/raymond/hello.wav"
	waittillvoxend
	content "..."
	wait 120
	setchoicecount 4
	choice 1 "Wow, you are quite quiet"
	choice 2 "Why aren't you saying anything?"
	choice 3 "I think I'm starting to dive into deep insanity"
	choice 4 "Please help"
	execchoice
	jmpif "choice != 1"
	goto option1
	jmpif "choice != 2"
	goto option2
	jmpif "choice != 3"
	goto option3
	jmpif "choice != 4"
	goto option4
	goto part2

option1:
	speaker "That NPC"
	setactive 1
	content "..."
	wait 160
	goto part2
option2:
	speaker "That NPC"
	setactive 1
	content "Cause I hate you"
	wait 160
	goto part2
option3:
	speaker "That NPC"
	setactive 1
	content "Good for you"
	wait 160
	goto part2
option4:
	speaker "That NPC"
	setactive 1
	content "No"
	wait 160
	goto part2

part2:
	speaker "Raymond"
	setactive 0
	content "It was fun talking to you"
	playvox "vox/raymond/itwasfun.wav"
	waittillvoxend
```