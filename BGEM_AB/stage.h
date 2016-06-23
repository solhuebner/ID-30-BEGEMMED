#ifndef STAGE_H
#define STAGE_H

#include <Arduino.h>
#include "globals.h"

// playfield
//-----------

void initPlayfield()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      field[x][y] = random (1, 5 + gameMode);
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
  arduboy.drawBitmap(30, 61 - timer, timer_bitmap, 5, 24, WHITE);
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
  arduboy.drawBitmap(12 * smokeLeftRight, 54, smoke_bitmaps[smokeFrame], 7, 8, WHITE);
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
      arduboy.drawBitmap(PLAYFIELD_ZERO_X + (selectorXpos * GEM_PIXELS), selectorYpos * GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      break;
    case true:
      arduboy.drawBitmap(PLAYFIELD_ZERO_X + (selectorXpos * GEM_PIXELS) - GEM_PIXELS, selectorYpos * GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X + (selectorXpos * GEM_PIXELS) + GEM_PIXELS, selectorYpos * GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X + (selectorXpos * GEM_PIXELS), (selectorYpos * GEM_PIXELS) - GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      arduboy.drawBitmap(PLAYFIELD_ZERO_X + (selectorXpos * GEM_PIXELS), (selectorYpos * GEM_PIXELS) + GEM_PIXELS, selector_bitmaps[selectorFrame], 8, 8, BLACK);
      break;
  }
}

void drawScore(int scoreX, int scoreY, byte sizeScore)
{
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  ltoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 6 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (sizeScore)
    {
      case SCORE_SMALL:
        sprites.drawSelfMasked(scoreX + (9 * i), scoreY, numbersSmall, 0);
        break;
      case SCORE_BIG:
        sprites.drawSelfMasked(scoreX + (8 * i), scoreY, numbersBig, 0);
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
        sprites.drawSelfMasked(scoreX + (pad * 9) + (9 * i), scoreY, numbersSmall, digit);
        break;
      case SCORE_BIG:
        sprites.drawSelfMasked(scoreX + (pad * 8) + (8 * i), scoreY, numbersBig, 0);
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



void threeInColumn()
{
  //check if 3 or more gems are equal in the same column
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = 0; row < PLAYFIELD_HEIGHT; row++)
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



void addNewGems()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    if (IsGemFree(column, 0))
    {
      field[column][0] = random (1, 5 + gameMode);
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
        timer += 4;
        if (timer > 24) timer = 24;
        testingSwitch++;
      }
    }
  }
  if (giveExtraScore == true)
  {
    scorePlayer += extraScoreForChain;
    extraScoreForChain += 100;
    arduboy.audio.tone(440, 100);
    delay(100);
    arduboy.audio.tone(1047, 200);
  }
  giveExtraScore = false;
  removeFlag();
}


void removeGems()
{
  threeInColumn();
  threeInRow();
  deleteGems();
  canMoveGemsDown = true;
}


void moveGemsDown()
{
  canMoveGemsDown = false;
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (int row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
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

void switchGem(int directionToSwitch)
{
  int switchField [] = {0, 0};
  testingSwitch = 0;
  while (testingSwitch < 2)
  {
    switch (directionToSwitch)
    {
      case SWITCH_GEM_UP:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos][selectorYpos - 1];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos][selectorYpos - 1] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_DOWN:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos][selectorYpos + 1];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos][selectorYpos + 1] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_LEFT:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos - 1][selectorYpos];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos - 1][selectorYpos] = switchField [0];
        modeIsSelected = !modeIsSelected;
        break;
      case SWITCH_GEM_RIGHT:
        switchField [0] = field [selectorXpos][selectorYpos];
        switchField [1] = field [selectorXpos + 1][selectorYpos];
        field [selectorXpos][selectorYpos] = switchField [1];
        field [selectorXpos + 1][selectorYpos] = switchField [0];
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
  if (modeIsSelected == true)arduboy.audio.tone(330, 25);
}

void updateStage()
{
  drawField();
  drawSelector();
  drawbackground();
  drawScore(11, 17, SCORE_SMALL);
}

#endif
