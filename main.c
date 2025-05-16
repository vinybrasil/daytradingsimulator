#include "raylib.h"

#define NUM_FRAMES 1200
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define BASEOFFSET 30
#define TEXTSCREENHEIGHT 150
#include <math.h>
#include <stdlib.h>
#include <time.h>

float GetRandomNormalValue(float mean, float stddev) {
  float x, y, s;

  do {
    x = 2.0f * GetRandomValue(0, RAND_MAX) / (float)RAND_MAX - 1.0f;
    y = 2.0f * GetRandomValue(0, RAND_MAX) / (float)RAND_MAX - 1.0f;
    s = x * x + y * y;
  } while (s >= 1.0f || s == 0.0f);

  s = sqrtf(-2.0f * logf(s) / s);
  return mean + stddev * x * s;
}

void drawball(int x, int y, Color color) { DrawCircle(x, y, 5, color); }

void startscreenscreen() {
  DrawRectangle(0, 100, 800, 200, BLACK);

  // DrawText(TextFormat("DAY TRADING SIMULATOR"), 40, 180, 30, GREEN);
  DrawText(TextFormat("DAY TRADING SIMULATOR"), 180, TEXTSCREENHEIGHT, 30,
           WHITE);

  DrawRectangle(200, TEXTSCREENHEIGHT + 40 + 30, 100, 40, GREEN);
  DrawText(TextFormat("PLAY"), 210, TEXTSCREENHEIGHT + 75, 30, WHITE);

  DrawRectangle(310, TEXTSCREENHEIGHT + 40 + 30, 240, 40, GREEN);
  DrawText(TextFormat("HOW TO PLAY"), 320, TEXTSCREENHEIGHT + 75, 30, WHITE);
}

void howtoplayscreen() {
  DrawRectangle(0, 100, 800, 200, BLACK);

  // DrawText(TextFormat("DAY TRADING SIMULATOR"), 40, 180, 30, GREEN);
  // DrawText(TextFormat("DAY TRADING SIMULATOR"), 180, TEXTSCREENHEIGHT, 30,
  //          WHITE);

  // DrawRectangle(200, TEXTSCREENHEIGHT + 40 + 30, 100, 40, GREEN);
  // DrawText(TextFormat("PLAY"), 210, TEXTSCREENHEIGHT + 75, 30, WHITE);

  DrawText(
      TextFormat("You can buy or sell the stock. \nIf you buy and then sell, "
                 "the profit is the price of the buy minus\nthe price of the "
                 "sell. \nIf you sell and then buy, the profit is the negative "
                 "of price of the buy\nminus the price of the sell.\nThe total "
                 "profit is the sum of all the trades."),
      40, 150, 20, WHITE);

  DrawRectangle(585, TEXTSCREENHEIGHT + 40 + 55, 140, 40, RED);
  DrawText(TextFormat("<BACK"), 580 + 25, TEXTSCREENHEIGHT + 40 + 60, 30,
           WHITE);
}

void endscreenscreen(double total_position) {
  DrawRectangle(0, 100, 800, 200, BLACK);

  Color cor;
  const char *text;

  if (total_position >= 0) {

    cor = GREEN;
    text = TextFormat("TOTAL PROFIT: +%0.4f", total_position);

  } else {

    cor = RED;

    text = TextFormat("TOTAL PROFIT: %0.4f", total_position);
  }

  DrawText(TextFormat("GAME OVER"), 300, TEXTSCREENHEIGHT, 30, WHITE);

  DrawText(text, 180, TEXTSCREENHEIGHT + 30, 30, cor);

  DrawRectangle(270, TEXTSCREENHEIGHT + 40 + 30, 240, 40, GREEN);
  DrawText(TextFormat("PLAY AGAIN"), 300, TEXTSCREENHEIGHT + 75, 30, WHITE);
}

int scale(double x) {
  double screenheight_scaled = (double)SCREENHEIGHT;
  return SCREENHEIGHT -
         (int)(screenheight_scaled * ((x - 98.5) / (101.5 - 98.5)));
}

void DrawGridManual(int width, int height, int cellSize, Color color) {
  // Draw vertical lines
  for (int x = 0; x <= width; x += cellSize) {
    DrawLine(x, 0, x, height, color);
  }

  // Draw horizontal lines
  for (int y = 0; y <= height; y += cellSize) {
    DrawLine(0, y, width, y, color);
  }
}

void showtotalposition(double total_position, int offset_text_y_table,
                       int font_size) {

  if (total_position > 0) {
    DrawText(TextFormat("Total profit: %0.4f", total_position), 600,
             BASEOFFSET + 2 * offset_text_y_table, font_size, DARKGREEN);
  } else if (total_position == 0) {
    DrawText(TextFormat("Total profit: %0.4f", total_position), 600,
             BASEOFFSET + 2 * offset_text_y_table, font_size, DARKGRAY);
  } else {
    DrawText(TextFormat("Total profit: %0.4f", total_position), 600,
             BASEOFFSET + 2 * offset_text_y_table, font_size, RED);
  }
}

void showlastposition(double last_position, int offset_text_y_table,
                      int font_size) {

  if (last_position > 0) {
    DrawText(TextFormat("Last profit: %0.4f", last_position), 600,
             BASEOFFSET + 3 * offset_text_y_table, font_size, DARKGREEN);
  } else if (last_position == 0) {
    DrawText(TextFormat("Last profit: %0.4f", last_position), 600,
             BASEOFFSET + 3 * offset_text_y_table, font_size, DARKGRAY);
  } else {
    DrawText(TextFormat("Last profit: %0.4f", last_position), 600,
             BASEOFFSET + 3 * offset_text_y_table, font_size, RED);
  }
}

void showcurrentposition(double current_position, int offset_text_y_table,
                         int font_size) {
  if (current_position > 0) {
    DrawText(TextFormat("Current profit: %0.4f", current_position), 600,
             BASEOFFSET + 4 * offset_text_y_table, font_size, DARKGREEN);
  } else if (current_position == 0) {
    DrawText(TextFormat("Current profit: %0.4f", current_position), 600,
             BASEOFFSET + 4 * offset_text_y_table, font_size, DARKGRAY);
  } else {
    DrawText(TextFormat("Current profit: %0.4f", current_position), 600,
             BASEOFFSET + 4 * offset_text_y_table, font_size, RED);
  }
}

int main() {

  //   const int SCREENWIDTH = 800;
  //   const int SCREENHEIGHT = 450;

  unsigned int seed = (unsigned int)time(NULL);
  SetRandomSeed(seed);

  typedef enum {
    SCREEN_START,
    SCREEN_HOWTOPLAY,
    SCREEN_GAME,
  } GameScreen;

  int t = 0;
  int posx = 0;
  int posy;

  int posy_p0;
  int posy_p1;

  bool game_end = false;
  const int end_screen = 600 - 20;
  const int frame_per_value = 5; // a cada dois frame muda o valor
  const int total_values_needed = end_screen;
  const int offset_text_y_table = 30;
  double price = 100.0;
  double prices[total_values_needed];

  const double dt = 1.0 / (252.0 * total_values_needed);

  double increment_1;
  double increment_2;
  double increment_3;
  double increment_4;

  prices[0] = price;

  double max_price = -10000.0;
  double min_price = 10000.0;

  double mu;
  double randValue;
  double sigma = 0.3;

  bool btnBuyAction = false;
  bool btnSellAction = false;
  bool btnPlayAgainAction = false;
  bool btnPlayAction = false;
  bool btnHowToPlayAction = false;
  bool btnbackaction = false;

  int timebought[30];
  double pricesbought[30];
  int qnt_buys = 0;

  int timesells[30];
  double pricesells[30];
  int qnt_sells = 0;

  bool currentlyBought = false;
  bool currentlySelled = false;

  double priceBuy = 0;
  double priceSell = 0;

  double total_position = 0.0;
  double current_position = 0.0;
  double last_position = 0.0;

  double realtimeprofit;
  const int offset_text_line = 10;

  Vector2 mousePoint = {0.0f, 0.0f};

  int height = 60;
  Rectangle btnBuyBounds = {600, 300, 170, height};
  Rectangle btnSellBounds = {600, 300 + height, 170, height};
  Rectangle btnPlayAgainBounds = {270, TEXTSCREENHEIGHT + 40 + 30, 240, 40};
  Rectangle btnPlay = {200, TEXTSCREENHEIGHT + 40 + 30, 100, 40};
  Rectangle btnHowToPlay = {310, TEXTSCREENHEIGHT + 40 + 30, 240, 40};
  Rectangle btnback = {585, TEXTSCREENHEIGHT + 40 + 55, 140, 40};

  int offset_text_x = 50;
  int offset_text_y = 20;

  int font_size = 18;

  for (int i = 1; i < end_screen; i++) {
    mu = GetRandomNormalValue(0.0, 0.5);
    randValue = GetRandomNormalValue(0.0, 1.0);

    increment_1 = mu - (0.5 * sigma * sigma);
    increment_2 = increment_1 * dt;
    increment_3 = increment_2 + sigma * randValue * sqrt(dt);
    increment_4 = exp(increment_3);

    prices[i] = prices[i - 1] * increment_4;

    if (prices[i] > max_price) {
      max_price = prices[i];
    }

    if (prices[i] < min_price) {
      min_price = prices[i];
    }
  }

  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Day trading Simulator");

  SetTargetFPS(NUM_FRAMES);

  GameScreen currentScreen = SCREEN_START;

  while (!WindowShouldClose()) {

    mousePoint = GetMousePosition();
    btnBuyAction = false;
    btnSellAction = false;
    btnPlayAgainAction = false;
    btnHowToPlayAction = false;

    if (CheckCollisionPointRec(mousePoint, btnBuyBounds)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnBuyState = 2;
      //   else
      //     btnBuyState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnBuyAction = true;
      }
    }
    //  else
    //   btnBuyState = 0;

    if (CheckCollisionPointRec(mousePoint, btnSellBounds)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnSellState = 2;
      //   else
      //     btnSellState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnSellAction = true;
      }
    }
    // else
    //   btnSellState = 0;

    if (CheckCollisionPointRec(mousePoint, btnPlayAgainBounds)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnSellState = 2;
      //   else
      //     btnSellState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnPlayAgainAction = true;
      }
    }

    if (CheckCollisionPointRec(mousePoint, btnPlay)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnSellState = 2;
      //   else
      //     btnSellState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnPlayAction = true;
      }
    }

    if (CheckCollisionPointRec(mousePoint, btnHowToPlay)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnSellState = 2;
      //   else
      //     btnSellState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnHowToPlayAction = true;
      }
    }

    if (CheckCollisionPointRec(mousePoint, btnback) &&
        (currentScreen == SCREEN_HOWTOPLAY)) {

      //   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      //     btnSellState = 2;
      //   else
      //     btnSellState = 1;

      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btnbackaction = true;
      }
    }
    if (btnBuyAction && (!currentlyBought)) {

      pricesbought[qnt_buys] = price;
      timebought[qnt_buys] = posx;
      qnt_buys += 1;
    }

    if (btnBuyAction && (!currentlyBought) && (!currentlySelled)) {
      priceBuy = price;
      currentlyBought = true;
    }

    if (currentlyBought) {
      current_position = price - priceBuy;
    }

    if (btnSellAction && (!currentlySelled)) {

      pricesells[qnt_sells] = price;
      timesells[qnt_sells] = posx;
      qnt_sells += 1;
    }

    // iniciar operação vendida
    if (btnSellAction && (!currentlySelled) && (!currentlyBought)) {
      priceSell = price;
      currentlySelled = true;
    }

    // estado corrente da operação vendida
    if (currentlySelled) {
      current_position = priceSell - price;
    }

    // fechar operação comprada
    if (btnSellAction && currentlyBought) {
      currentlyBought = false;
      priceSell = price;
      total_position += current_position;
      last_position = current_position;
      current_position = 0.0;
    }

    // fechar operação vendida
    if (btnBuyAction && currentlySelled) {
      currentlySelled = false;
      priceBuy = price;
      total_position += current_position;
      last_position = current_position;
      current_position = 0.0;
    }

    if (currentScreen == SCREEN_START) {
      // Check for button press to move to next screen
      // btnbackaction = false;
      if (btnPlayAction) {
        currentScreen = SCREEN_GAME;
        btnPlayAction = true;
      }

      if (btnHowToPlayAction) {
        currentScreen = SCREEN_HOWTOPLAY;
        // btnPlayAction = true;
      }
    }

    if (currentScreen == SCREEN_HOWTOPLAY) {
      if (btnbackaction) {
        currentScreen = SCREEN_START;
        btnbackaction = false;
      }
    }

    BeginDrawing();

    switch (currentScreen) {
    case SCREEN_START:

      ClearBackground(RAYWHITE);
      startscreenscreen();
      break;

    case SCREEN_HOWTOPLAY:

      ClearBackground(RAYWHITE);
      howtoplayscreen();
      break;

    case SCREEN_GAME:

      ClearBackground(RAYWHITE);

      DrawGridManual(SCREENWIDTH, SCREENHEIGHT, 20, LIGHTGRAY);

      posy = scale(price);

      DrawRectangle(580, 0, 220, SCREENHEIGHT, WHITE);

      for (int i = 1; i < posx; i++) {

        posy_p1 = scale(prices[i]);

        posy_p0 = scale(prices[i - 1]);

        DrawLine(i - 1, posy_p0, i, posy_p1, BLUE);
      }

      drawball(posx, posy, BLACK);

      drawball(0, GetScreenHeight() / 2, BLACK);

      DrawText(TextFormat("Price: %0.4f", prices[posx]), 600, BASEOFFSET,
               font_size, DARKGRAY);

      DrawText(TextFormat("seed: %i", seed), 600,
               BASEOFFSET + 1 * offset_text_y_table, font_size, DARKGRAY);

      DrawRectangle(btnBuyBounds.x, btnBuyBounds.y, btnBuyBounds.width,
                    btnBuyBounds.height, GREEN);

      DrawRectangle(btnSellBounds.x, btnSellBounds.y, btnSellBounds.width,
                    btnSellBounds.height, RED);

      DrawText("BUY", btnBuyBounds.x + offset_text_x,
               btnBuyBounds.y + offset_text_y, 20, WHITE);
      DrawText("SELL", btnSellBounds.x + offset_text_x,
               btnSellBounds.y + offset_text_y, 20, WHITE);

      DrawText(TextFormat("100"), 0, GetScreenHeight() / 2 - 20, 12, BLACK);
      DrawText(TextFormat("%0.4f", price), posx, posy - 20, 12, BLACK);

      showtotalposition(total_position, offset_text_y_table, font_size);

      showlastposition(last_position, offset_text_y_table, font_size);

      showcurrentposition(current_position, offset_text_y_table, font_size);

      if (qnt_buys == qnt_sells) {
        for (int i = 0; i < qnt_buys; i++) {
          DrawLine(timebought[i], scale(pricesbought[i]), timesells[i],
                   scale(pricesells[i]), BLACK);

          realtimeprofit = -(pricesbought[i] - pricesells[i]);

          if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2.0) -
                         offset_text_line,
                     20, DARKGREEN);
          } else {
            DrawText(TextFormat("%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2.0) -
                         offset_text_line,
                     20, RED);
          }
        }
      }

      if (qnt_buys > 0) {
        for (int j = 0; j < qnt_buys; j++) {
          DrawCircle(timebought[j], scale(pricesbought[j]), 5, GREEN);
        }
      }

      if (qnt_sells > 0) {
        for (int k = 0; k < qnt_sells; k++) {
          DrawCircle(timesells[k], scale(pricesells[k]), 5, RED);
        }
      }

      if (qnt_buys > qnt_sells) {
        for (int i = 0; i < qnt_sells; i++) {
          DrawLine(timebought[i], scale(pricesbought[i]), timesells[i],
                   scale(pricesells[i]), BLACK);

          realtimeprofit = -(pricesbought[i] - pricesells[i]);

          if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2) -
                         offset_text_line,
                     20, DARKGREEN);
          } else {
            DrawText(TextFormat("%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2) -
                         offset_text_line,

                     20, RED);
          }
        }

        // realtimeprofit = -(pricesbought[qnt_buys - 1] - price);

        realtimeprofit = -(pricesbought[qnt_buys - 1] - price);

        DrawLine(timebought[qnt_buys - 1],

                 scale(pricesbought[qnt_buys - 1]),

                 posx, posy, BLACK);

        if (realtimeprofit >= 0) {
          DrawText(TextFormat("+%0.4f", realtimeprofit),
                   (posx + timebought[qnt_buys - 1]) / 2,
                   scale((pricesbought[qnt_buys - 1] + price) / 2) -
                       offset_text_line,
                   20, DARKGREEN);
        } else {
          DrawText(TextFormat("%0.4f", realtimeprofit),
                   (posx + timebought[qnt_buys - 1]) / 2,
                   scale((pricesbought[qnt_buys - 1] + price) / 2) -
                       offset_text_line,
                   20, RED);
        }
      }

      if (qnt_buys < qnt_sells) {
        for (int i = 0; i < qnt_buys; i++) {
          DrawLine(timebought[i], scale(pricesbought[i]), timesells[i],
                   scale(pricesells[i]), BLACK);

          realtimeprofit = -(pricesbought[i] - pricesells[i]);

          if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2) -
                         offset_text_line,
                     20, DARKGREEN);
          } else {
            DrawText(TextFormat("%0.4f", realtimeprofit),
                     (timesells[i] + timebought[i]) / 2,
                     scale((pricesbought[i] + pricesells[i]) / 2) -
                         offset_text_line,
                     20, RED);
          }
        }
        DrawLine(timesells[qnt_sells - 1], scale(pricesells[qnt_sells - 1]),
                 posx, posy, BLACK);

        realtimeprofit = -(price - pricesells[qnt_sells - 1]);

        if (realtimeprofit >= 0) {
          DrawText(TextFormat("+%0.4f", realtimeprofit),
                   (timesells[qnt_sells - 1] + posx) / 2,
                   scale((price + pricesells[qnt_sells - 1]) / 2) -
                       offset_text_line,

                   20, DARKGREEN);
        } else {
          DrawText(TextFormat("%0.4f", realtimeprofit),
                   (timesells[qnt_sells - 1] + posx) / 2,
                   scale((price + pricesells[qnt_sells - 1]) / 2) -
                       offset_text_line,

                   20, RED);
        }
      }

      if (game_end) {

        endscreenscreen(total_position);

        if (btnPlayAgainAction) {
          t = 0;
          price = 100.0;
          prices[0] = 100.0;

          total_position = 0;
          current_position = 0;
          last_position = 0;

          priceBuy = 0.0;
          priceSell = 0.0;

          posx = 0;

          currentlyBought = false;
          currentlySelled = false;

          qnt_buys = 0;
          qnt_sells = 0;
          game_end = false;

          btnPlayAgainAction = false;
        }
      }

      break;
    }

    EndDrawing();

    if (btnPlayAction) {
      t += 1;

      if (!game_end) {
        if (t % frame_per_value == 0) {
          posx += 1;
          price = prices[posx];
        }
      }

      if (posx == end_screen) {
        game_end = true;
      }
    }
  }

  CloseWindow();
  return 0;
}
