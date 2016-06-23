#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"

void stateMenuIntro()
{
  globalCounter++;
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 10, TEAMarg, i);
  sprites.drawSelfMasked(43, 50, TEAM_argPart5, 0);
  if (globalCounter > 180)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  sprites.drawSelfMasked(0, 48, mainMenu, menuSelection - 2);
  sprites.drawSelfMasked(64, 48, mainMenu, menuSelection - 2+6);
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5))menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2))menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(32, 32*i, qrcode, i);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  arduboy.drawBitmap(32, 20, info_bitmap, 64, 24, 1);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}



void stateMenuSoundfx()
{
  arduboy.clearDisplay();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  sprites.drawSelfMasked(0, 48, mainMenu, 4 + arduboy.audio.enabled());
  sprites.drawSelfMasked(64, 48, mainMenu, 6 + 4 + arduboy.audio.enabled());
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();;
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


#endif
