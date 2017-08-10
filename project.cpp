/**
  Last name: Chen   First name: Xinlei
  Section: LBL A2(60696)
  Partner: Hanwen Yang
  Project Title:  Lighting
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <math.h>

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

//Joystick pins:
#define JOYSTICK_VERT   0  //Analog input A0 - vertical
#define JOYSTICK_HORIZ  1  //Analog input A1 - horizontal
#define JOYSTICK_BUTTON 9  //Digital input pin 9 for the button

//Pushbutton pin
#define PUSHBUTTON  10 //Digital input pin 10 for the button

//Potentiometer
#define RATING_DIAL  3 //Analog input A3

/**The global varialbles we need*/  
int mode = 0;      //the mode to change face
int high_score[3]; //store the most three highest score
int n = 0;         //the number-th of row of barrier
int number = 0;    //the number-th of the bullet

int score = 0;     //store current score
int old_score;

uint8_t m = 0;     // the times of you shoot in same position(x-axis)

uint8_t life = 2;  //store current score 
uint8_t old_life;

uint8_t judge = 0; //use judge to determine whether the game is over


uint8_t preserve_x;//store the current position of x-axis
uint8_t preserve_y;//store the current position of y-axis

uint16_t times = 0;//change the frequency of functions
uint16_t old_times = 0;

float rating = 0;  //store the user's ratings
float num = 0;     //store how many users has rated the game


typedef struct{
	bool exist;    //judge 
	int barrier_y;
	uint8_t type;  //judge whether barrier is normal or has some features
} Barrier; 

Barrier barrier[12][8];

typedef struct{
	int bullet_x; // store the cursor_x of every bullet
	int bullet_y; // store the cursor_y of every bullet
	bool exist;
} Bullet;

Bullet bullet[15];

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//to display the score
void score_face(){
    
    int selection2 = 1;

    tft.fillScreen(0);
	tft.setCursor(0,0);
	tft.setTextWrap(false);
    
    tft.setCursor(50,40);
	tft.setTextColor(0xF800);//RED
	tft.setTextSize(1);
    tft.print("RANK");
    delay(150);
    tft.fillRect(50,40,50,50,0x0000);
    
    tft.setCursor(40,40);
	tft.setTextColor(0xF800);//RED
	tft.setTextSize(2);
    tft.print("RANK");
    delay(150);
    tft.fillRect(40,40,50,50,0x0000);

    tft.setCursor(30,35);
	tft.setTextColor(0xF800);//RED
	tft.setTextSize(3);
    tft.print("RANK");

    tft.setCursor(35,70);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("FIRST    ");
    tft.print(high_score[0]);

    tft.setCursor(35,90);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SECOND   ");
    tft.print(high_score[1]);

    tft.setCursor(35,110);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("THIRD    ");
    tft.print(high_score[2]);

    while (selection2 == 1) {
    selection2 = digitalRead(JOYSTICK_BUTTON);
    }

    if( selection2 == 0){ return; }

}
//^-------------------------MODE3---------------------------- ^                                                         |

// initial face
void start_face(){
	int select;
	    select = 1;
	int init_joystick_vert;
	    init_joystick_vert = analogRead(JOYSTICK_VERT);
	int vertical;
	int dif;    

	tft.fillScreen(0);
	tft.setCursor(0,0);
	tft.setTextWrap(false);

	tft.setCursor(15,40);
	tft.setTextColor(0xF800);//RED
	tft.setTextSize(2);
    tft.print("LIGHTING");

    tft.drawRect(45,80,40,12,0xFFFF);
    tft.setCursor(55,82);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PLAY");

    tft.drawRect(45,110,40,12,0xFFFF);
    tft.setCursor(52,112);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SCORE");

    while(select == 1) {
    	select = digitalRead(JOYSTICK_BUTTON);
    	vertical = analogRead(JOYSTICK_VERT);
    	dif = vertical - init_joystick_vert;
        
        //decrease
    	if(dif < -50 ){                     
          tft.fillRect(46,81,38,10,0xFFFF);
          tft.setCursor(55,82);
          tft.setTextColor(0x0000);
          tft.print("PLAY");

          tft.fillRect(46,111,38,10,0x0000);
          tft.setCursor(52,112);
          tft.setTextColor(0xFFFF);
          tft.print("SCORE");
          
          mode = 2;//mode 2 is game surface
    	}

    	if(dif > 50){
    	  tft.fillRect(46,81,38,10,0x0000);
          tft.setCursor(55,82);
          tft.setTextColor(0xFFFF);
          tft.print("PLAY");
          
          tft.fillRect(46,111,38,10,0xFFFF);
          tft.setCursor(52,112);
          tft.setTextColor(0x0000);
          tft.print("SCORE");

          mode = 3;//mode 3 is score surface; 
         }

         delay(50);
    }

    if(select == 0){
    	return ;
    }

}
//--------------------MODE1--------------------------
//free barrier
void free_barr(){
	for(int i = 0; i < 12; i++){
	    for(int j = 0; j < 8; j++){
        barrier[i][j].exist = 0;
		barrier[i][j].barrier_y = 0;
		barrier[i][j].type = 0;
	}
  }
}

//free bullet
void free_bullet(){
	for(int i = 0; i < 15; i++){
		bullet[i].exist = 0;
		bullet[i].bullet_x = 0;
		bullet[i].bullet_y = 0;
	} 
}

//free the single barrier
void clean_barrier(int row ,int column){
   	
   	barrier[row][column].exist = 0;
   	barrier[row][column].barrier_y = 0;
   	barrier[row][column].type = 0;
}

//free the single bullet
void clean_bullet(int order){
	bullet[order].exist = 0;
	bullet[order].bullet_x = 0;
	bullet[order].bullet_y = 0;
}

//initialize the screen
void init_screen(){

	tft.fillScreen(0);
	tft.setCursor(0,0);
	tft.setTextWrap(false);

	tft.fillRect(0, 145, 128, 15, 0xFFFF);

	tft.setCursor(80,149);
	tft.setTextSize(1);
	tft.setTextColor(0x0000);
	tft.print("Life: ");
	tft.print(life);

	tft.setCursor(5,149);
	tft.setTextSize(1);
	tft.setTextColor(0x0000);
	tft.print("Score: ");
	tft.print(score);

}

//generate the barrier row by row
void gen_barrier(){
    uint16_t ran;
    uint16_t barr;


    for(int i = 0; i < 8; i++){
    	ran = random(1,65535);

    	barr = (ran << i) >> 15;

    	if( barr == 1 ){
    	tft.fillRect(5 + (12*i) + (3*i), 0 , 12, 12, 0xF800);
    	barrier[n][i].exist = 1;
      }
    }
        
    n = n + 1;
    if( n == 12 ){ n = 0;}
 }


//move (downward) all the exist barriers
void barr_move(){
     
    gen_barrier(); //generate the barrier randomly

    for(int i = 0; i < 12; i++){
      for(int j = 0; j < 8; j++ ){
    	  if(barrier[i][j].exist == 1){
    			
    	  if(barrier[i][j].barrier_y < 144 ){      
    		  tft.fillRect(5 + 15*j,barrier[i][j].barrier_y
    				            ,12,12,0x0000);
    	   }
    			
    	  barrier[i][j].barrier_y += 12; //move down one unit 
    			
    	  if (barrier[i][j].barrier_y > 132 ){
           clean_barrier(i,j);
          }
                
          else {
                //the condition of special barrier (life-block barrier)
                if (barrier[i][j].barrier_y < 124 &&   
                	      barrier[i][j].type == 1){

                	tft.fillRect(5 + 15*j,barrier[i][j].barrier_y
                	,12,12,0xFFE0);
                }
                //the condition of normal barrier
                else if (barrier[i][j].barrier_y < 124 &&
                	      barrier[i][j].type == 0){
                tft.fillRect(5 + 15*j,barrier[i][j].barrier_y
                	,12,12,0xF800);}
                }
            }
    	}
    }

    delay(0);
}

//horizontally move all the barrier in one row at every specific period
void horiz_barrmv(){
	uint8_t row;

    row = random(2,7);
    
    delay(200);

    if(barrier[row][0].exist == 1){
    	tft.fillRect(5,barrier[row][0].barrier_y,12,12,0x0000);
    	barrier[row][0].exist = 0;
    	barrier[row][0].barrier_y = 0;
    }
    
    bool a = true; //judge if exist a life-block barrier
    for(int k = 1; k < 8; ++k){
    	if( barrier[row][k].exist == 1 && barrier[row][k].type == 1){
    		 a = false;
    		 break;
    	}
    }
    
    //the main code of move function
    for(int i = 1; i < 8; ++i){
      if(barrier[row][i].exist == 1 && barrier[row][i].barrier_y < 120
      	 && a ) {
         
         barrier[row][i-1].barrier_y = barrier[row][i].barrier_y;
         tft.fillRect(5+(i-1)*15,barrier[row][i-1].barrier_y,12,12,0xF800);
    	 tft.fillRect(5+(i)*15,barrier[row][i].barrier_y,12,12,0x0000);
         barrier[row][i-1].exist = 1;

         clean_barrier(row,i);
       }
   }
   delay(200);
}

//upward move the bullet which plane shoot
void bullet_move(){
   for(int i = 0; i < 15; i++){
   	if(bullet[i].exist == 1 ){
   		tft.fillRect(bullet[i].bullet_x,bullet[i].bullet_y,6,6,0x0000);
   		bullet[i].bullet_y -= 8;
   		
   		if(bullet[i].bullet_y < 0){
   		  bullet[i].bullet_x = 128;}

   		else if(bullet[i].bullet_y >= 0){
   		tft.fillRect(bullet[i].bullet_x,bullet[i].bullet_y,6,6,0x07FF);}
   	}
  }
}

//the funtion of plane create the bullet
void shoot(){
	
    tft.fillRect(preserve_x - 3, preserve_y - 8, 6, 6, 0x07FF);
    bullet[number].bullet_x = preserve_x - 3;
    bullet[number].bullet_y = preserve_y - 8;
    bullet[number].exist = 1;

    number = number + 1;
    if( number ==  15) { number = 0; } 

    old_times = times;
}

//the funtion of the bullet beat the barrier
void barrier_shoot(){
    uint8_t k;

	for(int i = 0; i < 15; i++){
		if(bullet[i].exist == 1){

          k = (bullet[i].bullet_x - 5) / 15;
          
          for(int j = 0; j < 12; j++){
            if(  (barrier[j][k].exist == 1)
              && (barrier[j][k].barrier_y <= bullet[i].bullet_y) 
              && (bullet[i].bullet_y <= (barrier[j][k].barrier_y + 12)) ){
              
              tft.fillRect(5+15*k,barrier[j][k].barrier_y,12,12,0x0000);
              tft.fillRect(bullet[i].bullet_x,bullet[i].bullet_y
                	,6,6,0x0000);
            
            //the condition of bullet beats the life-block barrier  
            if( barrier[j][k].type == 1) { life = life + 1;}               
 
                clean_barrier(j,k);
                clean_bullet(i);
                score++;
            }
          }
        }
    }
}

//change the score when the score changes
void change_score(){
    
    if( old_score != score ){     
       tft.fillRect(45, 149,20,10,0xFFFF);

	   tft.setCursor(5,149);
	   tft.setTextSize(1);
	   tft.setTextColor(0x0000);
	
	   tft.print("Score: ");
	   tft.print(score);
  }
}

//change the life when the life changes
void change_life(){
    
    if( old_life != life){
      tft.fillRect(110,149,16,10,0xFFFF);

      tft.setCursor(115,149);
      tft.setTextSize(1);
	  tft.setTextColor(0x0000);
	
	  tft.print(life);
    }
}

//the face when plane be beaten and life is equal to zero
void fail_screen(){
    
    init_screen();
    
    tft.setCursor(5,149);
	tft.setTextSize(1);
	tft.setTextColor(0x0000);
    tft.print("Score: ");
	tft.print(score);

	tft.setCursor(40,40);
    tft.setTextSize(2);
    tft.setTextColor(0xF800);
    tft.print("OVER");
}


//the function of plane be beaten by barrier 
void ruin_plane(){

   uint8_t k;

   k = (preserve_x - 5) / 15;

   for(int i = 0; i < 12; i++){
   	 if(  (barrier[i][k].exist == 1) 
      && (barrier[i][k].barrier_y <= preserve_y) 
      && (preserve_y <= (barrier[i][k].barrier_y + 12)) ){
      	
      	life = life - 1;
        
        if(life > 0){
        	judge = 2;
        	break;
        }  
        
        if( life == 0){
        fail_screen();
        judge = 1;
        delay(3000);
        life = 2;
        break;
        }
       }
     }
} 

//the function of barrier become life-bolck barrier 
//which can increase one life if plane beat this special barrier
void life_block(){
	uint8_t row;
	uint8_t column;

	row = 10;
	column = random(1,6);
    
    for( int i = column; i < 7; i++){
	if( barrier[row][i].exist == 1){
		//yellow
       tft.fillRect(5+i*15,barrier[row][i].barrier_y,12,12,0xFFE0);
       barrier[row][i].type = 1; //type 1 is increase life
       break;
	}
  }
 score++;
}

//the main fuction of game
void game(){

   free_barr();
   free_bullet();
  
   score = 0; //initialize the score 
    
   init_screen();

   int16_t cursor_x = 71; int16_t cursor_y = 128;
   int16_t old_cursor_x;  int16_t old_cursor_y;
   int16_t init_joystick_vert; int16_t init_joystick_horiz;
   init_joystick_vert = analogRead(JOYSTICK_VERT);
   init_joystick_horiz = analogRead(JOYSTICK_HORIZ);
   
    while(true){
   	int16_t vertical, horizontal;
   	int16_t delta_vert, delta_horiz;
   	uint8_t selection;
   	uint8_t joy_selection;
	
	selection = digitalRead(PUSHBUTTON);

   	vertical = analogRead(JOYSTICK_VERT);
   	horizontal = analogRead(JOYSTICK_HORIZ);
   	joy_selection = digitalRead(JOYSTICK_BUTTON);

   	//compute the change of joystick
   	delta_vert = vertical - init_joystick_vert;
   	delta_horiz = horizontal - init_joystick_horiz;

   	//create the old_cursor
   	old_cursor_x = cursor_x;
    old_cursor_y = cursor_y;

    //change of cursor
    if( delta_horiz > 450)  { cursor_x = cursor_x + 15;}
    if( delta_horiz < -450) { cursor_x = cursor_x - 15;}

    if( cursor_x > 120)    { cursor_x = 116;}
    if( cursor_x < 5)      { cursor_x = 11; }
    
    cursor_y = cursor_y + delta_vert/350;
    
    //preserve the condition of cursor of x and y
    preserve_x = cursor_x;
    preserve_y = cursor_y;

    //to draw the cursor
    tft.fillTriangle(cursor_x,cursor_y,cursor_x - 6,cursor_y + 12,
    	   cursor_x + 6,cursor_y + 12, 0x07FF);

    //redraw a tiny patch of the map to replace the old cursor
    if((abs(delta_vert) > 10) || (abs(delta_horiz) > 10)){
       tft.fillTriangle(old_cursor_x,old_cursor_y,old_cursor_x - 6,
       	old_cursor_y + 12,old_cursor_x + 6,old_cursor_y + 12,0x0000);
    }
    
    //distirct the boundary of plane moving
    if( cursor_y < 0 )     { cursor_y = 0;}
    if( cursor_y > 130)    { cursor_y = 130;}
    
    if(abs(delta_horiz) > 300){ delay(50);}

    old_score = score;

    old_life = life;
    
    //use the times to change the fluency of funtions' running
    times++;
    if( times > 130000 ){ times = 0; } 
    
    //the funtion of pause when game is on
    if(joy_selection == 0){ 
       joy_selection = 1;
       
       delay(500);
       
       tft.fillRect(40,50,40,20,0xFFFF);
       tft.setCursor(45,55);
       tft.setTextColor(0x0000);
       tft.print("Pause");
       
       while( joy_selection == 1){
           joy_selection = digitalRead(JOYSTICK_BUTTON);
       }
       
       tft.fillRect(40,50,40,20,0x0000);
       delay(500);
    }
    
    //make shoot be more smooth
    if( (selection == 0) && (abs(times - old_times) > 100) ){
        shoot();
    }
    
    if( times % 20 == 0){ bullet_move(); }

    barrier_shoot();
    
    ruin_plane();
    
    //create the life-block barrier
    if( score % 40 == 0 && score != 0){ life_block(); }
    
    //horizontally moving of barrier
    if(times %  2156  == 0){ horiz_barrmv(); }
    
    //use portion to change the moving speed of barriers when 
    //users' score meet any specific standard 
    uint8_t portion = 450;
    if( score < 100 && score > 50) { portion = 400;}
    if( score > 100 && score < 150) { portion = 350;}
    if( score > 150) { portion = 300;}

    if( times % portion == 0){
    	barr_move();
    }	

    change_score();
    change_life();

    //stay the same X-position all the time 
    //and get more than 20 points by this way will loss life
    if( old_cursor_x != cursor_x){ 
        m = 0;
    }
    
    if( (old_cursor_x == cursor_x) && (score - old_score != 0)){
          m = m + 1;
       if( m == 15) { 
    	   old_life = life;
    	   life = life - 1;
    	   change_life(); 
    	   m = 0;
      }
      if( life == 0){
        fail_screen();
        judge = 1;
        delay(3000);
        life = 2;
      }
    }
    
 
    if( judge == 1){ return; }
    if( judge == 2){ 
      free_barr();
      free_bullet();
      init_screen();
      n = 0;
      judge = 0;
     }
   }
}

//the face of rating
void rate_face(){
    tft.fillScreen(0);
	tft.setCursor(0,0);
	tft.setTextWrap(false);

	tft.drawRect(30,50,12,12,0xFFFF);
	tft.drawRect(60,50,12,12,0xFFFF);
	tft.drawRect(90,50,12,12,0xFFFF);

	tft.setCursor(40,100);
	tft.setTextColor(0xF800);
	tft.setTextSize(2);
	tft.print("RATE");
	
	tft.setCursor(50,120);
	tft.setTextSize(1);
	tft.setTextColor(0xFFFF);
	tft.print(rating);
    
    int rate;
    float ranum;

    int selection3 = 1;

    while( selection3 == 1){
    selection3 = digitalRead(JOYSTICK_BUTTON);	
    rate = analogRead(RATING_DIAL);	
    
    if( rate < 350){
    	tft.fillRect(32,52,8,8,0xFFFF);
    	tft.fillRect(62,52,8,8,0x0000);
    	tft.fillRect(92,52,8,8,0x0000);
        ranum = 1;
    }

    if( rate > 350 && rate < 700){
    	tft.fillRect(32,52,8,8,0xFFFF);
    	tft.fillRect(62,52,8,8,0xFFFF);
    	tft.fillRect(92,52,8,8,0x0000);
    	ranum = 2;
    }

    if( rate > 700){
    	tft.fillRect(32,52,8,8,0xFFFF);
    	tft.fillRect(62,52,8,8,0xFFFF);
    	tft.fillRect(92,52,8,8,0xFFFF);
    	ranum = 3;
    }
    }
    
    num = num + 1;
    rating = (rating*(num - 1) + ranum) / num;
    
}

//list the score in score_face
void score_list(){
	int store;
	store = score;

	if( store > high_score[0]){
		high_score[2] = high_score[1];
		high_score[1] = high_score[0];
		high_score[0] = store;
	}

	if((store > high_score[1]) && (store < high_score[0])){
		high_score[2] = high_score[1];
		high_score[1] = store;
	}

	if((store > high_score[2])&& (store < high_score[1])){
		high_score[2] = store;
	}

}
//^---------------------MODE2-------------------------^
int main(){
	init();

	pinMode(JOYSTICK_BUTTON,INPUT);
	digitalWrite(JOYSTICK_BUTTON,HIGH);

	pinMode(PUSHBUTTON,INPUT);
	digitalWrite(PUSHBUTTON,HIGH);

	pinMode(RATING_DIAL,OUTPUT);


	Serial.begin(9600);

	tft.initR(INITR_BLACKTAB);

	high_score[0] = 0;
    high_score[1] = 0;
    high_score[2] = 0;


    while(true){
    
    start_face();
    
    if( mode == 2 ){
    	game();
    	judge = 0;
        score_list();
        rate_face();
    }

    if( mode == 3){
    	score_face();
    }
  
  }
    
	return 0;
}

