#include <LedControl.h>

int DIN = 12;
int CS =  11;
int CLK = 10;

byte displayImage[8]=     {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// considering a 2d co-ordinate system with origin (0,0) at bottom left corner
int ballX=3; //X position
int ballY=7; //Y position - top
int speedX=0; // no X movement, can be -1, 0 or 1
int speedY=-1; // downward Y movement, can be -1, 0 or 1
int paddleX = 4; // X position of center of paddle - can be 1 to 6. Paddle width is 3 
int score = 0;

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
 pinMode (2, INPUT);
 pinMode (3, INPUT);
 lc.shutdown(0,false);       // Keep MAX72XX awake
 lc.setIntensity(0,15);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display
}

void loop(){
    // update ball position
    ballX=ballX+speedX;
    ballY=ballY+speedY;
 
    // check for ball interaction with walls
    if(ballX==0 || ballX==7){
        speedX = speedX *-1; // bouncing off walls in horizontal direction
    }
 
    // bouncing off ceiling
    if(ballY==7){
        speedY = speedY *-1; // bouncing off the ceiling
    }
    
    // bouncing off the paddle
    if(ballY==0 && ballX>=(paddleX-1) && ballX<=(paddleX+1)){
        speedY = speedY *-1; 
        score++;    // player earns a point
    }
    
    // going past the paddle
    if(ballY==0 && ballX<(paddleX-1) && ballX>(paddleX+1)){
        // going past the paddle - player is out
        Serial.println(); Serial.print("Score: "); Serial.println(score);
        for(int i=0; i<8; i++){
          displayImage[i] = 0x00;
        } 
        displayImage[3] = 0xFF; displayImage[4] = 0xFF; // show a line
        renderByte(displayImage);
        while(1); // Freeze
    }
    // clearing the image variable
    for(int i=0; i<8; i++){
      displayImage[i] = 0x00;
    }
    
    // generating new image
    addPixel(ballX, ballY); // adding the ball position to image
    addPixel(paddleX - 1, 0); addPixel(paddleX, 0); addPixel(paddleX + 1, 0);
    // adding paddle position to image
    renderByte(displayImage); // show the generated image
    
    // handling paddle control
    if(digitalRead(2) && paddleX>0) {
        paddleX = paddleX-1; // move paddle left
    }
    
    if(digitalRead(3) && paddleX<7) {
        paddleX = paddleX+1; // move paddle right
    }
 
    delay(200);
}

void addPixel(int xVal, int yVal){
    int newRowval = 2^(7-xVal);
    int rowVal = displayImage[7-yVal];
    displayImage[7-yVal] = rowVal || newRowval;    // inserting a 1 at the required pixel
} 

void renderByte(byte image [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,image[i]);
  }
}
