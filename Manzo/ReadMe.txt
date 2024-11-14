<Summary>
* Name of game : MANZO

* Version : Milestone_1.ver

* Developers, team, authors
  + Seohyun Min : Producer
  + Won Kim : Art Leader
  + Seokhwa Hong : QA Leader
  + Gyuwon Na : Technical Leader
  + Jiyeop Kang : Level Designer

* Where to find an updated version of the code, such as a website or GitHub repo : https://github.com/Rudy-Castan-DigiPen-Teaching/gam-200-final-project-sangrin.git

* Small description of game
DEEP DIVE:​ TO THE SEA FOLLOWING UNIDENTIFIED MESSAGE. ​
WITH THE RHYTHM GUIDED BY THE SEA​

* License : All content © 2024 DigiPen(USA) Corporation, all rights reserved.

<Before Starting>
* Links and short installation instructions for necessary programs/IDE’s/libraries
 Before building our .sln file, if you’ve never installed Visual Studio on your computer, please check here (https://visualstudio.microsoft.com/ko/downloads/). Click the download button for community. However, since we have all the necessary .dll files, .lib files, and assets in the assets or external folder, you won’t need to download any additional programs to play the game.

<Installation Instructions>
- Extract `Manzo` folder and double click.
- Double click `Manzo` folder.
- Double click `Manzo.sln` file.

(Release Mode)
- Check configuration to be set `Release` and `x64`.

(Debug Mode)
- Check configuration to be set `Debug` and `x64`.

<Run/Build Code>
(Debug Mode)
- Click `Build - Build Solution` or type keyboard `Ctrl + Shift + B`.
- After Build succeeded, check `Manzo/Manzo/x64/Debug` folder.
- Paste `assets' folders into this path if it doesn't included.
- Double click `Manzo.exe` file.

(Release Mode)
- Click `Build - Build Solution` or type keyboard `Ctrl + Shift + B`.
- After Build succeeded, check `Manzo/Manzo/x64/Release` folder.
- Paste `assets' folder into this path if it doesn't included.
- Double click `Manzo.exe` file.

You can run on directly at solution file type keyboard 'Ctrl + F5' either mode.

Play:
* Any expected windows/visuals/audio
 Please check if the background images for Mode 1 and Mode 2, the basic music (metronome audio sample), the ship, the fish (in Mode 1 only), and the ImGui window are all displaying.

* Instruction of Imgui Window
  + FPS : fps of our game
  + Delta Time : Delta time depends on engine's chrono time 
  + Device, Camera, World Coordinate for mouse points
  + Is On Beat : If this section set on true, then if you click the mouse left button when this section is set true, you can move

* Instruction of Keys & Buttons
  + MOUSE_BUTTON_LEFT : main keys for moving the ship just to click (double click is fine)
  + KEY_TAB : Hide or Show the collision lines when you run at debug mode
  + KEY_Q : Load next game state (change mode from sea into player's house)
  + KEY_W : Reload the game state
  + KEY_A : Print out ‘pick slot 1’
  + KEY_S : Print out ‘pick slot 2’
  + KEY_D : Print out ‘pick slot 3’
  + KEY_Z : Print out skill
  + KEY_NUM1 : Print out ‘picked Net’
  + KEY_NUM2 : Print out ‘picked Light’
  + KEY_NUM3 : Print out ‘picked Empty’
  + KEY_M : Game Clear
  + KEY_N : Game Over

* Gameplay Instruction
 Avoid reefs and eat fish by moving the ship with 'MOUSE_LEFT_BUTTON' in sync with the incoming beat. (For Milestone 1, collision detection is only shown and does not affect gameplay.)

* Expected final goal/result/boss
 We don’t have final goal/result/boss at this time, just if you(ship) collided with fish, it will disappear. Also fish will disappear if it goes outside of spawn window.

<FAQ/Common Mistakes>
 There is an issue when attempting to stop the program. If you run the .exe file, you should exit using the console window. If you're running it from a solution file, use either the console window or the stop button to close the program.

<Feedback>
Discord - handal1842 (Seohyun Min)
