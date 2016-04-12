/*
  This is a working PONG-game for Arduino, and it's ment to be used with the Nootropic Desing's Video Experimenter.
  Made by Nanohenry10-9 10.3.2016
*/

#include <TVout.h> //Include some libraries
#include <fontALL.h>

#define pot1Pin 0 //Define some stuff
#define pot2Pin 1

#define padMinPos  5
#define padMaxPos  90

#define ballStrPosX  55
#define ballStrPosY  47

#define selTopPos 48
#define selBotPos 58

int pot1Val = 0; //Declare some variables
int pot2Val = 0;

int ballPosX = ballStrPosX;
int ballPosY = ballStrPosY;

int ballXMov = 1;
int ballYMov = 1;

int pad1Pos = 0;
int pad2Pos = 0;

int lScore = 0;
int rScore = 0;

int mode = 0;

int selPos = selTopPos;

TVout TV; //Initialize the TVout library

void setup() {
  TV.begin(PAL, 120, 96); //Begin VE
  pinMode(pot1Pin, INPUT); //Set the pins
  pinMode(pot2Pin, INPUT);
  TV.select_font(font6x8); //Set font (needed)
  menu(); //The menu
}

void loop() {
  drawGame(); //Draw the game area (i.e. the net)

  pot1Val = analogRead(pot1Pin); //Get potentiometer values
  pot2Val = analogRead(pot2Pin);

  pot1Val = map(pot1Val, 0, 1023, padMinPos, padMaxPos); //Make potentiometer values valid for the game
  pot2Val = map(pot2Val, 0, 1023, padMinPos, padMaxPos);

  pad1Pos = pot1Val; //Potentiometer 1 value = left paddle position

  if (mode == 1) { //Check if mode is "single player" (VS computer)
    pad2Pos = ballPosY; //Ball's vertical position = right paddle position
  } else {
    pad2Pos = pot2Val; //Potentiometer 2 value = right paddle position
  }

  drawPaddles(pad1Pos, pad2Pos); //Draw the paddles

  drawBall(ballPosX, ballPosY); //Draw the ball

  updateBallPos(); //Update ball position (so it actually moves, so it doesn't stay still)

  printScore(lScore, rScore); //Show the score

  if (lScore == 7 || rScore == 7) { //If left score or right score is seven
    gameOver(); //Game over screen
  }

  delay(100); //Wait 100ms to not to make the ball go superfast
}

void menu() {
  TV.clear_screen(); //Clear the screen

  pot1Val = analogRead(pot1Pin); //Get potentiometer 1 value

  if (pot1Val < 5 || pot1Val > 1020) { //If potentiometer 1 value is less than five or bigger than 1020
    TV.clear_screen(); //Clear the screen
    TV.draw_rect(0, 0, 110, 95, WHITE); //Draw the square around screen
    TV.print(17, 42, "Please center"); //Print some stuff (the alert to center potentiometer 1)
    TV.print(11, 56, "potentiometer 1");

    do {
      pot1Val = analogRead(pot1Pin);
    } while (pot1Val < 5 || pot1Val > 1020); //While potentiometer 1 value is less than five or bigger than 1020

    TV.clear_screen(); //Clear the screen
  }

  TV.draw_rect(0, 0, 110, 95, WHITE); //Draw the square around screen

  do {
    pot1Val = analogRead(pot1Pin); //Get potentiometer 1 value

    TV.draw_rect(0, 0, 110, 95, WHITE); //Draw the square around screen

    selPos = pot1Val;
  
    selPos = map(selPos, 0, 1023, selTopPos, selBotPos); //Map the value to be doog for the menu

    TV.print(20, selPos, ">"); //Print the menu's options
    TV.print(26, selTopPos, "VS computer");
    TV.print(26, selBotPos, "2 players");

  } while (pot1Val > 5 && pot1Val < 1020); //Check if potentiometer 1 value is bigger than five and less than 1020

  if (pot1Val < 10) { //If potentiometer 1 value is less than ten
    mode = 1; //Mode = "single player" (VS computer)
  } else {
    mode = 2; //Mode = "multiplayer" (2 players)
  }

  TV.clear_screen(); //Clear the screen
}

void drawGame() {
  TV.clear_screen(); //Clear the screen
  TV.draw_rect(0, 0, 110, 95, WHITE); //Draw the square around screen
  int i = 1; //Create a veriable for the while-loop (used almost like the for-loop in this case)
  while (i <= 95) { //The while-loop (more information above)
    TV.set_pixel(55, i, WHITE); //"Net" between players
    i += 2; //Increase i by two
  }
}

void drawPaddles(int pad1Pos, int pad2Pos) {
  if (pad1Pos > padMaxPos) { //If paddle 1 position is bigger than the maximium paddle position value
    pad1Pos = padMaxPos; //Paddle 1 position = the maximium paddle position value
  }
  if (pad1Pos < padMinPos) { //If paddle 1 position is less than the minimium paddle position value
    pad1Pos = padMinPos; //Paddle 1 position = the minimium paddle position value
  }
  if (pad2Pos > padMaxPos) { //If paddle 2 position is bigger than the maximium paddle position value
    pad2Pos = padMaxPos; //Paddle 2 position = the maximium paddle position value
  }
  if (pad2Pos < padMinPos) { //If paddle 2 position is less than the minimium paddle position value
    pad2Pos = padMinPos; //Paddle 2 position = the minimium paddle position value
  }

  TV.draw_line(2, (pad1Pos - 5), 2, (pad1Pos + 5), WHITE); //Draw left paddle
  TV.draw_line(108, (pad2Pos - 5), 108, (pad2Pos + 5), WHITE); //Draw right paddle
}

void drawBall(int x, int y) {
  TV.set_pixel(x, y, WHITE); //Draw the ball (a pixel)
}

void updateBallPos() {
  if (ballPosX == 107) { //If ball is touching right wall

    if (ballPosY > (pad2Pos - 6) && ballPosY < (pad2Pos + 6)) { //Check if the ball hit the right paddle
      ballXMov = -1; //Make the ball move to the opposite direction on the X axis
    }
  }
  if (ballPosX == 3) { //If the ball is touching left wall

    if (ballPosY > (pad1Pos - 6) && ballPosY < (pad1Pos + 6)) { //Check if the ball hit the right paddle
      ballXMov = 1; //Make the ball move to the opposite direction on the X axis
    }
  }

  if (ballPosX == -1 || ballPosX == 112) { //If the ball is off the screen
    if (ballPosX > 50) {
      lScore = (lScore + 1); //Increase left player's score by one
    } else {
      rScore = (rScore + 1); //Increase rigth player's/computer's score by one
    }
    ballPosX = ballStrPosX; //Reset the ball's coordinates
    ballPosY = ballStrPosY;
    ballXMov = 1; //"Normalize" the ball
    ballYMov = 1;
  }

  if (ballPosY >= 94) { //If the ball is touching top wall/roof
    ballYMov = -1; //Make the ball go to the opposite direction is the Y axis
  }
  if (ballPosY <= 1) { //If the ball is touching bottom wall/floor
    ballYMov = 1; //Make the ball go to the opposite direction is the Y axis
  }

  if (ballXMov == 1) { //"Move" the ball
    ballPosX += 1;
  } else {
    ballPosX -= 1;
  }
  if (ballYMov == 1) {
    ballPosY += 1;
  } else {
    ballPosY -= 1;
  }
}

void gameOver() {
  delay(1000); //Wait 1s to be fun-looking
  TV.print(29, 20, "GAME OVER"); //Print "GAME-OVER"

  int i = 1; //Declare i for a loop
  while (i <= 95) { //Do the loop
    TV.set_pixel(55, i, WHITE); //"Net" between players
    i += 2; //Increase i by two
  }

  delay(2500); //Wait 2500ms

  lScore = 0; //Reset the scores
  rScore = 0;

  menu(); //The menu
}

void printScore(int q, int w) {
  TV.select_font(font8x8); //Select the biggest font
  TV.print(40, 5, q); //Print the score
  TV.print(63, 5, w);
  TV.select_font(font6x8); //Select the "normal" font back
}











