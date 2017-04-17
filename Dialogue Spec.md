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
```

Example:
```
	speaker "Raymond"
	content "Hello!"
	lchrimg "char/raymond/left_side"
	rchrimg "char/npc1/right_side"
	setactive 0
	playvox "vox/raymond/hello.wav"
	waittillvoxend
	content "..."
	wait 120
```