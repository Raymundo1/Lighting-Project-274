# Project: Lighting

#### 1. Accessories: 
* LCD Screen 
* JoyStick 
* Arduino Mega Board (AMG)
* several wires 
* potentiometer 
* pushbutton

**Wiring instructions:** (we have pack it with a picture)

Firstly: 
* 5V(board) \<—\> Positive power bus 
* GND(board) \<—\> Negative power bus

**Part 1** (LCD Screen) : 
From up to down (connection) 
* GND \<—\> Negative power bus\
* VCC \<—\> Positive power bus 
* RESET \<—\> Arduino Pin 8 
* D/C \<—\> Arduino Pin 7 
* CARD\_CS \<—\> Arduino Pin 5 
* TFT\_CS \<—\> Arduino Pin 6 
* MOSI \<—\> Digital 51 
* SCK \<—\> Digital 52 
* MISO \<—\> Digital 50 
* LITE \<—\> Positive power bus

**Part 2** (JoyStick) : 
From up to down (connection) 
* VCC \<—\> Positive power bus 
* VERT \<—\> Pin A0 
* HORZ \<—\> Pin A1 
* SEL \<—\> Arduino Pin 9 
* GND \<—\> Negative power bus

**Part 3** (Potentiometer) : 
* Left leg \<—\> Positive power bus 
* Middle leg \<—\> Analog A3 
* Right leg \<—\> Negative power bus

**Part 4** (Pushbutton) :
* One leg \<—\> Arduino Pin 10 
* Another leg \<—\> Negative power bus

#### 2. The process of the running code 
* unzip the file 
* connect your MEGA board with computer by using the data line 
* open the terminal and type “make upload” to upload your code to board 
* see the condition of LCD Screen 
* play the game by instruction

#### 3. The feature of the game (very very important) 
* At first(start
face), there are two options(use JoyStick to choose) 
* You should press
the JoyStick to enter the face 
* if you choose “SCORE” it will turn to
score\_face which store your the most three highest score after you
playing the game 
* if you choose “PLAY” it will turn to game\_face.
(initial life : 2)

there are few fancy features that will appear in game: 
1. Press pushbutton that plane will shoot. 
2. Press the JoyStick that the game will pause and you can press again the game will continue. 
3. When your score get \"40 \* i\"(i is positive integer) it will appear yellow block (and if you can shoot it, your will get one more life ( the life you have is showed under the Screen). 
4. At specific period, all the barriers in one row will shift left one uint. 
5. With the score increasing ,the moving speed of barrier will increase. 
6. You can’t shoot more than 15 barriers in one same(x-axis) position. if you did, you will lose one life

##### When your life is “0” ,it will change to fail\_face and after a little time it will shows rating\_face And you can use the potentiometer to rate this game (least 1 most 3) under the Rating you can see the average rating score.

### NOTE: we will pack the face picture with README that user can more easily understand the game
