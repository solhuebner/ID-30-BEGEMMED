/*
 Begemmed: http://www.team-arg.org/bgem-manual.html

 Arduboy version 1.2:  http://www.team-arg.org/bgem-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

 2015 - JO3RI

 Game License: MIT : https://opensource.org/licenses/MIT
 
 */

//determine the game
#define GAME_ID 30

#include <SPI.h>
#include <EEPROM.h>
#include "Arglib.h"
#include "menu_bitmaps.h"
#include "gems_bitmaps.h"
#include "number_bitmaps.h"
#include "background_bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_MAIN          0
#define STATE_MENU_HELP          1
#define STATE_MENU_INFO          3
#define STATE_MENU_SOUNDFX       4

//define menu choices (on main menu)
#define CHOOSE_HELP              1
#define CHOOSE_PLAY              2
#define CHOOSE_INFO              3
#define CHOOSE_CONF              4

//define game states (on main menu)
#define STATE_GAME_INIT          2
#define STATE_GAME_PLAYING       8
#define STATE_GAME_PAUSE         9
#define STATE_GAME_OVER          10

#define PLAYFIELD_WIDTH          8         // Playfield width in gems
#define PLAYFIELD_HEIGHT         8         // Playfield height in gems
#define PLAYFIELD_ZERO_X         64        // zero x-position of the playfield in pixel position on the screen
#define PLAYFIELD_ZERO_Y         0         // zero x-position of the playfield in pixel position on the screen
#define GEM_FREE                 0         // an empty place in the playfield

#define SWITCH_GEM_UP            1
#define SWITCH_GEM_DOWN          2
#define SWITCH_GEM_LEFT          3
#define SWITCH_GEM_RIGHT         4

#define NO_FLAG_ON_FIELD         0
#define FLAG_ON_FIELD            1

#define GEM_PIXELS               8

#define SCORE_SMALL              0
#define SCORE_BIG                1

#define GAME_MODE_EASY           1
#define GAME_MODE_NORMAL         2
#define GAME_MODE_HARD           3

Arduboy arduboy;
SimpleButtons buttons (arduboy);

unsigned char gameState = STATE_MENU_MAIN;
boolean soundYesNo;
boolean modeIsSelected;
boolean smokeLeftRight;

unsigned long scorePlayer;
unsigned long extraScoreForChain;
boolean giveExtraScore;

int field[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
boolean fieldFlags[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
boolean canMoveGemsDown;


int menuSelection;
int level;
int selectorXpos;
int selectorYpos;
int timer;
byte animationFrame = 0;
byte smokeFrame = 0;
boolean selectorFrame = false;
byte blinkFrame = 0;
boolean blinkTubes = false;
byte flickerPlace = 0;
byte testingSwitch;
byte gameMode;

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);
  arduboy.drawBitmap(0, 8, TEAMarg2, 128, 48, 1);
  arduboy.display();
  delay(3000);
  gameState = STATE_MENU_MAIN;
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundYesNo = true;
  arduboy.initRandomSeed();
  menuSelection = CHOOSE_PLAY;
  gameMode = GAME_MODE_NORMAL;
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  buttons.poll();
  switch (gameState)
  {
    case STATE_MENU_MAIN:
      // show the splash art
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 48, 1);
      arduboy.drawBitmap(0, 48, mainmenu_bitmaps[menuSelection - 1], 128, 16, 1);
      if (buttons.justPressed(RIGHT_BUTTON) && (menuSelection < 4))menuSelection++;
      if (buttons.justPressed(LEFT_BUTTON) && (menuSelection > 1))menuSelection--;
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
      break;
    case STATE_MENU_HELP: // QR code
      arduboy.clearDisplay();
      arduboy.drawBitmap(32, 0, qrcode_bitmap, 64, 64, 1);
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
      break;
    case STATE_MENU_INFO: // infoscreen
      arduboy.clearDisplay();
      arduboy.drawBitmap(32, 20, info_bitmap, 64, 24, 1);
      if (buttons.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
      break;
    case STATE_MENU_SOUNDFX: // soundconfig screen
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 48, WHITE);
      arduboy.drawBitmap(0, 48, soundYesNo_bitmaps[soundYesNo], 128, 16, WHITE);
      if (buttons.justPressed(RIGHT_BUTTON)) soundYesNo = true;
      if (buttons.justPressed(LEFT_BUTTON)) soundYesNo = false;
      if (buttons.justPressed(A_BUTTON | B_BUTTON))
      {
        arduboy.audio.save_on_off();
        gameState = STATE_MENU_MAIN;
      }
      if (soundYesNo == true) arduboy.audio.on();
      else arduboy.audio.off();
      break;
    case STATE_GAME_INIT:
      arduboy.clearDisplay();
      level = 0;
      scorePlayer = 0;
      initPlayfield();
      removeFlag();
      modeIsSelected = false;
      selectorXpos = 4;
      selectorYpos = 4;
      timer = 24;
      testingSwitch = 0;
      canMoveGemsDown = true;
      giveExtraScore = false;
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 48, WHITE);
      arduboy.drawBitmap(0, 48, gameOver_bitmap, 128, 16, WHITE);
      arduboy.drawBitmap(33, 52, gameMode_bitmaps[gameMode-1], 62, 8, WHITE);
      if (buttons.justPressed(RIGHT_BUTTON) && (gameMode < 3)) gameMode++;
      if (buttons.justPressed(LEFT_BUTTON) && (gameMode > 1)) gameMode--;
      if (buttons.justPressed(A_BUTTON | B_BUTTON))
      {
        arduboy.clearDisplay();
        gameState = STATE_GAME_PLAYING;
      }
      break;
    case STATE_GAME_PLAYING:
      if (arduboy.everyXFrames(45)) timer--;
      if (timer < 0)
      {
        gameState = STATE_GAME_OVER;
        delay(500);
      }
      if (arduboy.everyXFrames(600))
      {
        blinkTubes = true;
        flickerPlace = (random(0,6)*9);
      }
      if (arduboy.everyXFrames(4))
        {
          
          selectorFrame =!selectorFrame;
          blinkFrame++;
          if (blinkFrame > 12)
          {
            blinkFrame = 0;
            blinkTubes = false;
          }
          animationFrame++;
          if (animationFrame > 11) animationFrame = 0;
          
          smokeFrame++;
          if (smokeFrame > 23) 
          {
            smokeFrame = 0;
            smokeLeftRight = !smokeLeftRight;
          }
        }
      updateStage();
      removeGems();
      while (canMoveGemsDown) moveGemsDown();
      addNewGems();
      
      if (buttons.justPressed(UP_BUTTON) && (modeIsSelected == false) && (selectorYpos > 0))selectorYpos--;
      if (buttons.justPressed(DOWN_BUTTON) && (modeIsSelected == false) && (selectorYpos < PLAYFIELD_HEIGHT-1))selectorYpos++;
      if (buttons.justPressed(LEFT_BUTTON) && (modeIsSelected == false) && (selectorXpos > 0))selectorXpos--;
      if (buttons.justPressed(RIGHT_BUTTON) && (modeIsSelected == false) && (selectorXpos < PLAYFIELD_WIDTH-1))selectorXpos++;

      if (buttons.justPressed(UP_BUTTON) && (modeIsSelected == true) && (selectorYpos > 0))switchGem(SWITCH_GEM_UP);
      if (buttons.justPressed(DOWN_BUTTON) && (modeIsSelected == true) && (selectorYpos < PLAYFIELD_HEIGHT-1))switchGem(SWITCH_GEM_DOWN);
      if (buttons.justPressed(LEFT_BUTTON) && (modeIsSelected == true) && (selectorXpos > 0))switchGem(SWITCH_GEM_LEFT);
      if (buttons.justPressed(RIGHT_BUTTON) && (modeIsSelected == true) && (selectorXpos < PLAYFIELD_WIDTH-1))switchGem(SWITCH_GEM_RIGHT);

      if (buttons.justPressed(B_BUTTON))modeIsSelected = !modeIsSelected;
      if (buttons.justPressed(A_BUTTON))
      {
        arduboy.clearDisplay();
        gameState = STATE_GAME_PAUSE;
      }
      break;
    case STATE_GAME_OVER:
      arduboy.clearDisplay();
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 48, WHITE);
      arduboy.drawBitmap(0, 48, gameOver_bitmap, 128, 16, WHITE);
      drawScore(41, 52, SCORE_BIG);
      if (buttons.justPressed(A_BUTTON | B_BUTTON))
      {
        arduboy.clearDisplay();
        gameState = STATE_MENU_MAIN;
      }
      break;
    case STATE_GAME_PAUSE:
      arduboy.drawBitmap(0, 0, splashscreen_bitmap, 128, 48, WHITE);
      arduboy.drawBitmap(0, 48, pause_bitmap, 128, 16, WHITE);
      if (buttons.justPressed(A_BUTTON))
      {
        arduboy.clearDisplay();
        gameState = STATE_GAME_PLAYING;
      }
      break;
  }
  arduboy.display();
}

void updateStage()
{
  drawField();
  drawSelector();
  drawbackground();
  drawScore(11,16, SCORE_SMALL);
}

// playfield
//-----------

void initPlayfield()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      field[x][y] = random (1,5 + gameMode);
    }
  }
}

void removeFlag()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      fieldFlags[x][y] = NO_FLAG_ON_FIELD;
    }
  }
}


void drawbackground()
{
  arduboy.drawBitmap(30, 37, timerMask_bitmap, 5, 24, BLACK);
  arduboy.drawBitmap(30, 61-timer, timer_bitmap, 5, 24, WHITE);
  arduboy.drawBitmap(30, 62, timerMask2_bitmap, 1, 8, BLACK);
  arduboy.drawBitmap(0, 0, background_bitmap, 64, 64, WHITE);
  arduboy.drawBitmap(8, 44, pumpmask_bitmap, 15, 10, BLACK);
  arduboy.drawBitmap(8, 44, pump_bitmaps[animationFrame], 15, 10, WHITE);
  arduboy.drawBitmap(41, 44, gearmask_bitmap, 19, 17, BLACK);
  arduboy.drawBitmap(41, 44, gear_bitmaps[animationFrame], 19, 17, WHITE);
  arduboy.drawBitmap(4, 33, gaugeneedlemask_bitmap, 3, 3, BLACK);
  arduboy.drawBitmap(4, 33, gaugeneedle_bitmaps[animationFrame], 3, 3, WHITE);
  arduboy.drawBitmap(0, 54, smokemask_bitmap, 7, 8, BLACK);
  arduboy.drawBitmap(12, 54, smokemask_bitmap, 7, 8, BLACK);
  arduboy.drawBitmap(12*smokeLeftRight, 54, smoke_bitmaps[smokeFrame], 7, 8, WHITE);
  arduboy.drawBitmap(1, 13, bellowmask_bitmap, 7, 8, BLACK);
  arduboy.drawBitmap(1, 13, bellow_bitmaps[animationFrame], 7, 8, WHITE);
  
};

void drawField()
{
  Serial.println();
  for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
  {
    for (int x = 0 ; x < PLAYFIELD_WIDTH; x++)
    {
      // draw every tile in the playfield
      arduboy.drawBitmap((x * GEM_PIXELS) + PLAYFIELD_ZERO_X, (y * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[field[x][y]], 8, 8, WHITE);
    }
  }
}

void drawSelector()
{
  switch (modeIsSelected)
  {
    case false:
      arduboy.drawBitmap(PLAYFIELD_ZERO_X+(selectorXpos*GEM_PIXELS), selectorYpos*GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      break;
    case true:
      arduboy.drawBitmap(PLAYFIELD_ZERO_X+(selectorXpos*GEM_PIXELS)-GEM_PIXELS, selectorYpos*GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X+(selectorXpos*GEM_PIXELS)+GEM_PIXELS, selectorYpos*GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X+(selectorXpos*GEM_PIXELS), (selectorYpos*GEM_PIXELS)-GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X+(selectorXpos*GEM_PIXELS), (selectorYpos*GEM_PIXELS)+GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      break;
  }
}

void drawScore(int scoreX, int scoreY, byte sizeScore)
{
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  itoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 6 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (sizeScore)
    {
      case SCORE_SMALL:
        arduboy.drawBitmap(scoreX + (9 * i), scoreY, numberMask_bitmap, 3, 8, BLACK);
        arduboy.drawBitmap(scoreX + (9 * i), scoreY, number_bitmaps[0], 3, 8, WHITE);
        break;
      case SCORE_BIG:
        arduboy.drawBitmap(scoreX + (8 * i), scoreY, numberBig_bitmaps[0], 7, 8, WHITE);
        break;
    }
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (sizeScore)
    {
      case SCORE_SMALL:
        arduboy.drawBitmap(scoreX + (pad * 9) + (9 * i), scoreY, numberMask_bitmap, 3, 8, BLACK);
        arduboy.drawBitmap(scoreX + (pad * 9) + (9 * i), scoreY, number_bitmaps[digit], 3, 8, WHITE);
        break;
      case SCORE_BIG:
        arduboy.drawBitmap(scoreX + (pad * 8) + (8 * i), scoreY, numberBig_bitmaps[digit], 7, 8, WHITE);
        break;
    }
  }
  switch (sizeScore)
    {
      case SCORE_SMALL:
        if (blinkTubes == true) arduboy.drawBitmap(scoreX + flickerPlace, scoreY, numberBlink_bitmaps[blinkFrame], 3, 8, BLACK);
        break;
    }
}



// Testcases
// ---------

void switchGem(int directionToSwitch)
{
  int switchField [] ={0,0};
  testingSwitch = 0;
  while (testingSwitch < 2)
  {
    switch (directionToSwitch)
    {
      case SWITCH_GEM_UP:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos][selectorYpos-1];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos][selectorYpos-1] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_DOWN:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos][selectorYpos+1];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos][selectorYpos+1] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_LEFT:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos-1][selectorYpos];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos-1][selectorYpos] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_RIGHT:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos+1][selectorYpos];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos+1][selectorYpos] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
    }
    extraScoreForChain = 0;
    testingSwitch++;
    removeGems();
  }
  arduboy.drawBitmap((selectorXpos * GEM_PIXELS) + PLAYFIELD_ZERO_X, (selectorYpos * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
  arduboy.drawBitmap(((selectorXpos + 1) * GEM_PIXELS) + PLAYFIELD_ZERO_X, (selectorYpos * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
  arduboy.drawBitmap(((selectorXpos - 1) * GEM_PIXELS) + PLAYFIELD_ZERO_X, (selectorYpos * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
  arduboy.drawBitmap((selectorXpos * GEM_PIXELS) + PLAYFIELD_ZERO_X, ((selectorYpos + 1) * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
  arduboy.drawBitmap((selectorXpos * GEM_PIXELS) + PLAYFIELD_ZERO_X, ((selectorYpos - 1) * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
  testingSwitch = 0;
  if (modeIsSelected == true)arduboy.tunes.tone(330, 25);
}

void removeGems()
{
  threeInColumn();
  threeInRow();
  deleteGems();
  canMoveGemsDown = true;
}

void addNewGems()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    if (IsGemFree(column, 0))
    {
      field[column][0] = random (1,5 + gameMode);
      arduboy.drawBitmap((column * GEM_PIXELS) + PLAYFIELD_ZERO_X, 0, gemsinverted_bitmaps[0], 8, 8, BLACK);
    }
  }
}

void deleteGems()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      if (fieldFlags[x][y] == FLAG_ON_FIELD)
      {
        giveExtraScore = true;
        field[x][y] = GEM_FREE;
        scorePlayer += 5;
        timer +=4;
        if (timer > 24) timer = 24; 
        testingSwitch++;
      }
    }
  }
  if (giveExtraScore == true)
  {
    scorePlayer += extraScoreForChain;
    extraScoreForChain += 100;
    arduboy.tunes.tone(440, 100);
    delay(100);
    arduboy.tunes.tone(1047, 200);
  }
  giveExtraScore = false;
  removeFlag();
}


void threeInColumn()
{
  //check if 3 or more gems are equal in the same column
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = 0; row <PLAYFIELD_HEIGHT; row++)
    {
      if (!IsGemFree(column, row))
      {
        if (underIsSameGem(column, row))
        {
          if (underIsSameGem(column, row + 1))
          {
            fieldFlags[column][row] = FLAG_ON_FIELD;
            fieldFlags[column][row + 1] = FLAG_ON_FIELD;
            fieldFlags[column][row + 2] = FLAG_ON_FIELD;
            if (underIsSameGem(column, row + 2))
            {
              fieldFlags[column][row + 3] = FLAG_ON_FIELD;
              if (underIsSameGem(column, row + 3))
              {
                fieldFlags[column][row + 4] = FLAG_ON_FIELD;
              }
            }
          }
        }
      }
    }
  }
}


void threeInRow()
{
  //check if 4 or more gems are equal in the same row
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (int row = PLAYFIELD_HEIGHT - 1; row > -1; row--)
    {
      if (!IsGemFree(column, row))
      {
        if (rightIsSameGem(column, row))
        {
          if (rightIsSameGem(column + 1, row))
          {
            fieldFlags[column][row] = FLAG_ON_FIELD;
            fieldFlags[column + 1][row] = FLAG_ON_FIELD;
            fieldFlags[column + 2][row] = FLAG_ON_FIELD;
            if (rightIsSameGem(column + 2, row))
            {
              fieldFlags[column + 3][row] = FLAG_ON_FIELD;
              if (rightIsSameGem(column + 3, row))
              {
                fieldFlags[column + 4][row] = FLAG_ON_FIELD;
              }
            }
          } 
        }
      }
    }
  }
}

boolean IsGemFree(int array_x, int array_y)
{
  if (field [array_x][array_y] == GEM_FREE) return true;
  else return false;
}

boolean rightIsSameGem(int array_x, int array_y)
{
  if ((array_x + 1 < PLAYFIELD_WIDTH ) && (field [array_x][array_y] == field [array_x + 1][array_y])) return true;
  else return false;
}

boolean underIsSameGem(int array_x, int array_y)
{
  if ((array_y + 1 < PLAYFIELD_HEIGHT) && (field [array_x][array_y] == field [array_x][array_y + 1])) return true;
  else return false;
}

void moveGemsDown()
{
  canMoveGemsDown = false;
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (int row = PLAYFIELD_HEIGHT-1; row > 0; row--)
    {
      if (IsGemFree(column, row))
      {
        if (!IsGemFree(column, row - 1))
        {
          field [column][row] = field [column][row - 1];
          field [column][row - 1] = GEM_FREE;
          drawField();
          arduboy.drawBitmap((column * GEM_PIXELS) + PLAYFIELD_ZERO_X, (row * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[0], 8, 8, BLACK);
          arduboy.drawBitmap((column * GEM_PIXELS) + PLAYFIELD_ZERO_X, (row * GEM_PIXELS) + PLAYFIELD_ZERO_Y, gemsinverted_bitmaps[field[column][row]], 8, 8, WHITE);
          arduboy.display();
          canMoveGemsDown = true;
          //delay(30);
        }
      }
    }
  }
}




