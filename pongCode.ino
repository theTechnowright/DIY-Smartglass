

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define UP_BUTTON 2
#define DOWN_BUTTON 3

const unsigned long PADDLE_RATE = 33;
const unsigned long BALL_RATE = 35;
const uint8_t PADDLE_HEIGHT = 10;
int MAX_SCORE = 2;

int CPU_SCORE = 0;
int PLAYER_SCORE = 0;
// On MEGA, MOSI is 51, CLK is 52
// MOSI is Data pin on display breakout
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


uint8_t ball_x = 64, ball_y = 32;
uint8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;

unsigned long paddle_update;
const uint8_t CPU_X = 12;
uint8_t cpu_y = 16;

const uint8_t PLAYER_X = 115;
uint8_t player_y = 16;


void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
    // Display the splash screen (we're legally required to do so)
    
    unsigned long start = millis();

    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);

    display.clearDisplay();
    drawCourt();

    while(millis() - start < 2000);

    display.display();

    ball_update = millis();
    paddle_update = ball_update;
}

void drawGame() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(23,0);
  display.println("-Score-");
  display.setCursor(115,0);
  display.print(PLAYER_SCORE);
  display.setCursor(5,0);
  display.print(CPU_SCORE);
    bool update = false;
    unsigned long time = millis();

    static bool up_state = false;
    static bool down_state = false;
    
    up_state |= (digitalRead(UP_BUTTON) == LOW);
    down_state |= (digitalRead(DOWN_BUTTON) == LOW);

    if(time > ball_update) {
        uint8_t new_x = ball_x + ball_dir_x;
        uint8_t new_y = ball_y + ball_dir_y;

        // Check if we hit the vertical walls
        if(new_x == 0) {
            PLAYER_SCORE++;
            display.clearDisplay();
            drawCourt();
            ball_dir_x = -ball_dir_x;
            new_x =30;
        }
         if(new_x == 127) {
            CPU_SCORE++;
            display.clearDisplay();
            drawCourt();
            ball_dir_x = -ball_dir_x;
            new_x =90;
        }

        // Check if we hit the horizontal walls.
        if(new_y == 17 || new_y == 63) {
            ball_dir_y = -ball_dir_y;
            new_y += ball_dir_y + ball_dir_y;
        }

        // Check if we hit the CPU paddle
        if(new_x == CPU_X && new_y >= cpu_y && new_y <= cpu_y + PADDLE_HEIGHT) {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        // Check if we hit the player paddle
        if(new_x == PLAYER_X
           && new_y >= player_y
           && new_y <= player_y + PADDLE_HEIGHT)
        {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        display.drawPixel(ball_x, ball_y, BLACK);
        display.drawPixel(new_x, new_y, WHITE);
        ball_x = new_x;
        ball_y = new_y;

        ball_update += BALL_RATE;

        update = true;
    }

    if(time > paddle_update) {
        paddle_update += PADDLE_RATE;

        // CPU paddle
        display.drawFastVLine(CPU_X, cpu_y, PADDLE_HEIGHT, BLACK);
        const uint8_t half_paddle = PADDLE_HEIGHT >> 1;
        if(cpu_y + half_paddle > ball_y) {
            cpu_y -= 1;
        }
        if(cpu_y + half_paddle < ball_y) {
            cpu_y += 1;
        }
        if(cpu_y < 18) cpu_y = 18;
        if(cpu_y + PADDLE_HEIGHT > 63) cpu_y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(CPU_X, cpu_y, PADDLE_HEIGHT, WHITE);

        // Player paddle
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, BLACK);
        if(up_state) {
            player_y -= 1;
        }
        if(down_state) {
            player_y += 1;
        }
        up_state = down_state = false;
        if(player_y < 18) player_y = 18;
        if(player_y + PADDLE_HEIGHT > 63) player_y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, WHITE);

        update = true;
    }

    if(update)
        display.display();
}


void drawCourt() {
    display.drawRect(0, 16, 128, 48, WHITE);
}

void loop(){
    drawGame();
    if (PLAYER_SCORE >= MAX_SCORE){
      display.clearDisplay();
      display.setCursor(15,30);
      display.println("YOU WIN!");
      display.setCursor(40,19);
      display.setTextSize(1);
      display.println("YOUR SCORE:");
      display.setCursor(95,19);
      display.print(PLAYER_SCORE);
      display.setCursor(18,50);
      display.setTextSize(1);
      display.println("Wait for a moment");
      PLAYER_SCORE = 0;
      CPU_SCORE = -1;
      delay(1000);
      
    }
      if (CPU_SCORE >= MAX_SCORE){
      display.clearDisplay();
      display.setCursor(15,30);
      display.println("YOU LOSE!");
      display.setCursor(30,19);
      display.setTextSize(1);
      display.println("YOUR SCORE:");
      display.setCursor(95,19);
      display.print(PLAYER_SCORE);
      display.setCursor(18,50);
      display.setTextSize(1);
      display.println("Wait for a moment");
      PLAYER_SCORE = 0;
      CPU_SCORE = -1; 
      delay(1000);
     
    }
}

