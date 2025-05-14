Developed with Unreal Engine and Visual Studio Community 2022.

The demo level is called M_Submission, is the only one on the Levels folder and its the default level for the project (editor and game).


Bowling scoring rules taken from:
- https://en.wikipedia.org/wiki/Tenpin_bowling#Traditional_scoring
- https://www.wikihow.com/Score-Bowling

Rules overview:
- Numbers represent fallen pins 
- - is used when the ball falls into the lateral lanes and hits nothing (value is 0)
- f is fault and is used when the player steps beyond the limit line in the lane (value is 0)
- / is a spare - you were able to drop all 10 pins in your second throw in your current turn (value is 10 + the value of next throw)
- x is a strike - you were able to dropp all 10 pins in a single throw at the beginning of your turn (value is 10 + the value of next 2 throws)

Restrictions:
- First slot per turn can be a number, -, f, F or x, X
- Second slot per turn can be a number, -, f, F or /
- Second slot and third slot cannot be filled up if previous slots have no values (within the same turn column)

Possible improvements (not done because of time concerns):
- The system is able to check order within the same turn - you cannot input the second or third value if the previous ones are not set. This doesn't work on the general scoreboard yet: you currently can input values in any position.
- The next values retrieval logic in ScoreBoard.cpp is considering special cases for the last and 1-before-last positions. I think it could be streamlined.
- On the last field 
- A reset button to cleanup board.

When were blueprints used and why:
- The level blueprint was used only to spawn the score board widget blueprint and enable the mouse pointer
- There is a widget blueprint for the name field. It has no functionality whatsoever.
- There is a blueprint that extends the C++ class for the individual score. Used to arrange graphically the widgets.
- There is also a blueprint that extends the C++ class for the final score. Also used to arrange graphically the widgets.
- The last widget blueprint 