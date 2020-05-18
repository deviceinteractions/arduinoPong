#include <LedControl.h>
pinMode(2, INPUT);
pinMode(3, INPUT);
int DIN = 12;
int CS =  11;
int CLK = 10;
byte displayImage[8]=     {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int ballX=3; //X position
int ballY=7; //Y position - top
int SpeedX=0; // no X movement, can be -1, 0 or 1
int speedY=-1; // downward Y movement, can be -1, 0 or 1
int paddleX = 4; // X position of center of paddle - can be 1 to 6. Paddle width is 3 
int score = 0;
void setup(){
 lc.shutdown(0,false);       // Keep MAX72XX awake
 lc.setIntensity(0,15);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display
}

void loop(){
    ballX=ballX+speedX;
    ballY=ballY+speedY;
    if(ballX==0 || ballX==7){
        speedX = speedX *-1; // bouncing off walls in horizontal direction
    }
    // bouncing off walls in vertical direction
    if(ballY==7){
        speedY = speedY *-1; // bouncing off the ceiling
    }
    if(ballY==0 && ballX>=(paddleX-1) && ballX<=(paddleX+1))){
        speedY = speedY *-1; // bouncing off the paddle
        score++;    // player earns a point
    }

    if(ballY==0 && ballX<(paddleX-1) && ballX>(paddleX+1)){
        // going past the paddle - player is out
        Serial.println(); Serial.print("Score: "); Serial.println(score);
        displayImage={0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00}; // show a line
        renderByte(displayImage);
        while(1); // Freeze
    }
    displayImage={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // clearing the image
    addPixel(ballX, ballY); // adding the ball position to image
    addPixel(paddleX - 1, 0); addPixel(paddleX, 0); addPixel(paddleX + 1, 0);
    // adding paddle position to image
    renderByte(displayImage); // show the generated image
    if(digitalRead(2) && paddleX>0) {
        paddleX = paddleX-1; // move paddle left
    }

    if(digitalRead(3) && paddleX<7) {
        paddleX = paddleX+1; // move paddle right
    }
}

addPixel(int xVal, int yVal){
    int newRowval = 2^(7-xVal);
    int rowVal = displayImage[7-y];
    displayImage[7-y] = rowVal || newRowval;    // inserting a 1 at the required pixel
} 

void renderByte(byte image [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,image[i]);
  }
}
