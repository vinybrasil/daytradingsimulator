#include "raylib.h"

#include "raymath.h"
#include "rlgl.h"
#include <complex.h>
#include <math.h>

#include <stdlib.h>

#define NUM_FRAMES 130

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

int main() {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "day trading simulator");

  SetTargetFPS(NUM_FRAMES);

  int posx = 0;
  int posy = GetScreenHeight() / 2;
  int randValue;
  int price = GetScreenHeight() / 2;

  int prices[NUM_FRAMES * screenWidth];

  int height = 60;
  int end_screen = 600 - 20;
  Rectangle btnBuyBounds = {600, 300, 170, height};
  Rectangle btnSellBounds = {600, 300 + height, 170, height};

  bool gameEnded = false;

  //   int btnBuyState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
  //   int btnSellState = 0;
  bool btnBuyAction = false;
  bool btnSellAction = false;

  Vector2 mousePoint = {0.0f, 0.0f};

  int offset_text_y_table = 20;
  int offset_text_x = 50;
  int offset_text_y = 20;
  int offset_text_line = 40;

  int t = 0;
  prices[0] = GetScreenHeight() / 2;
  int total_position = 0;
  int current_position = 0;
  int last_position = 0;
  int priceBuy = 0;
  int priceSell = 0;

  int realtimeprofit;

  bool currentlyBought = false;
  bool currentlySelled = false;

  int timebought[30];
  int pricesbought[30];
  int qnt_buys = 0;

  int timesells[30];
  int pricesells[30];
  int qnt_sells = 0;

  while (!WindowShouldClose()) {

    mousePoint = GetMousePosition();
    btnBuyAction = false;
    btnSellAction = false;

    posy = GetScreenHeight() - price;


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

    if (btnBuyAction && (!currentlyBought)) { // Selled

      pricesbought[qnt_buys] = price;
      timebought[qnt_buys] = posx;
      qnt_buys += 1;
    }

    if (btnBuyAction && (!currentlyBought) && (!currentlySelled)) {
      priceBuy = price;
      currentlyBought = true;
      // drawbuy = true;
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
      current_position = 0;
    }

    // fechar operação vendida
    if (btnBuyAction && currentlySelled) {
      currentlySelled = false;
      priceBuy = price;
      total_position += current_position;
      last_position = current_position;
      current_position = 0;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    // DrawGrid(100, 3.0);

    DrawGridManual(800, 450, 20, LIGHTGRAY);
    DrawRectangle(580, 0, 220, 450, WHITE);

    DrawLine(end_screen, 450, end_screen, 0, BLACK);

    for (int i = 1; i < posx; i++) {
      DrawLine(i - 1, GetScreenHeight() - prices[i - 1], i,
               GetScreenHeight() - prices[i], BLUE);
    }

    DrawCircle(0, GetScreenHeight() / 2, 5, BLACK);

    DrawCircle(posx, posy, 5, BLACK); // MAROON

    if (qnt_buys > 0) {
      for (int j = 0; j < qnt_buys; j++) {
        DrawCircle(timebought[j], GetScreenHeight() - pricesbought[j], 5,
                   GREEN);
      }
    }

    if (qnt_sells > 0) {
      for (int k = 0; k < qnt_sells; k++) {
        DrawCircle(timesells[k], GetScreenHeight() - pricesells[k], 5, RED);
      }
    }

    if (qnt_buys == qnt_sells) {
      for (int i = 0; i < qnt_buys; i++) {
        DrawLine(timebought[i], GetScreenHeight() - pricesbought[i],
                 timesells[i], GetScreenHeight() - pricesells[i], BLACK);

        realtimeprofit = -(pricesbought[i] - pricesells[i]);

        if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%i", realtimeprofit),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, DARKGREEN);
        } else {
            DrawText(TextFormat("%i", realtimeprofit),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, RED);         
        }

      }
    }

    if (qnt_buys > qnt_sells) {
      for (int i = 0; i < qnt_sells; i++) {
        DrawLine(timebought[i], GetScreenHeight() - pricesbought[i],
                 timesells[i], GetScreenHeight() - pricesells[i], BLACK);

        realtimeprofit = -(pricesbought[i] - pricesells[i]);

        if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%i", realtimeprofit),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, DARKGREEN);
        } else {
            DrawText(TextFormat("%i", realtimeprofit),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, RED);
        }

      }

      realtimeprofit = -(pricesbought[qnt_buys - 1] - price);

      DrawLine(timebought[qnt_buys - 1],
               GetScreenHeight() - pricesbought[qnt_buys - 1], posx, posy,
               BLACK);

      if (realtimeprofit >= 0) {
        DrawText(TextFormat("+%i", realtimeprofit),
        (posx + timebought[qnt_buys - 1]) / 2,
        GetScreenHeight() - ((pricesbought[qnt_buys - 1] + price) / 2) -
        offset_text_line,

        20, DARKGREEN);
      } else {
        DrawText(TextFormat("%i", realtimeprofit),
        (posx + timebought[qnt_buys - 1]) / 2,
        GetScreenHeight() - ((pricesbought[qnt_buys - 1] + price) / 2) -
        offset_text_line,

        20, RED);  
      }

    }

    if (qnt_buys < qnt_sells) {
      for (int i = 0; i < qnt_buys; i++) {
        DrawLine(timebought[i], GetScreenHeight() - pricesbought[i],
                 timesells[i], GetScreenHeight() - pricesells[i], BLACK);

        realtimeprofit = -(pricesbought[i] - pricesells[i]);

        if (realtimeprofit >= 0) {
            DrawText(TextFormat("+%i", realtimeprofit ),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, DARKGREEN);
        } else {
            DrawText(TextFormat("%i", realtimeprofit ),
            (timesells[i] + timebought[i]) / 2,
            GetScreenHeight() - ((pricesbought[i] + pricesells[i]) / 2) -
            offset_text_line,

            20, RED);
        }

      }
      DrawLine(timesells[qnt_sells - 1],
               GetScreenHeight() - pricesells[qnt_sells - 1], posx, posy,
               BLACK);


      realtimeprofit = -(price - pricesells[qnt_sells - 1]);

      if (realtimeprofit >= 0){
        DrawText(TextFormat("+%i", realtimeprofit ),
        (timesells[qnt_sells - 1] + posx) / 2,
        GetScreenHeight() - ((price + pricesells[qnt_sells - 1]) / 2) -
        offset_text_line,

        20, DARKGREEN);
      } else {
        DrawText(TextFormat("%i", realtimeprofit ),
        (timesells[qnt_sells - 1] + posx) / 2,
        GetScreenHeight() - ((price + pricesells[qnt_sells - 1]) / 2) -
        offset_text_line,

        20, RED);  
      }

    }

    DrawText(TextFormat("250"), 0, GetScreenHeight() / 2 - 20, 12, BLACK);
    DrawText(TextFormat("%i", price), posx, posy - 20, 12, BLACK);

    DrawText(TextFormat("Current price: %i", price), 600, 50, 20, DARKGRAY);
    DrawText(TextFormat("Std dev: %0.2f", 5.), 600, 50 + offset_text_y_table,
             20, DARKGRAY);

    if (total_position > 0) {
      DrawText(TextFormat("Total profit: %i", total_position), 600,
               50 + 2 * offset_text_y_table, 20, GREEN);
    } else if (total_position == 0) {
      DrawText(TextFormat("Total profit: %i", total_position), 600,
               50 + 2 * offset_text_y_table, 20, DARKGRAY);
    } else {
      DrawText(TextFormat("Total profit: %i", total_position), 600,
               50 + 2 * offset_text_y_table, 20, RED);
    }

    if (last_position > 0) {
      DrawText(TextFormat("Last profit: %i", last_position), 600,
               50 + 3 * offset_text_y_table, 20, GREEN);
    } else if (last_position == 0) {
      DrawText(TextFormat("Last profit: %i", last_position), 600,
               50 + 3 * offset_text_y_table, 20, DARKGRAY);
    } else {
      DrawText(TextFormat("Last profit: %i", last_position), 600,
               50 + 3 * offset_text_y_table, 20, RED);
    }

    if (current_position > 0) {
      DrawText(TextFormat("Current profit: %i", current_position), 600,
               50 + 4 * offset_text_y_table, 20, GREEN);
    } else if (current_position == 0) {
      DrawText(TextFormat("Current profit: %i", current_position), 600,
               50 + 4 * offset_text_y_table, 20, DARKGRAY);
    } else {
      DrawText(TextFormat("Current profit: %i", current_position), 600,
               50 + 4 * offset_text_y_table, 20, RED);
    }

    DrawText(TextFormat("Last buy price: %i", priceBuy), 600,
             50 + 5 * offset_text_y_table, 20, DARKGRAY);

    DrawText(TextFormat("Last sell price: %i", priceSell), 600,
             50 + 6 * offset_text_y_table, 20, DARKGRAY);

    DrawRectangle(btnBuyBounds.x, btnBuyBounds.y, btnBuyBounds.width,
                  btnBuyBounds.height, GREEN);

    DrawRectangle(btnSellBounds.x, btnSellBounds.y, btnSellBounds.width,
                  btnSellBounds.height, RED);

    DrawText("BUY", btnBuyBounds.x + offset_text_x,
             btnBuyBounds.y + offset_text_y, 20, WHITE);
    DrawText("SELL", btnSellBounds.x + offset_text_x,
             btnSellBounds.y + offset_text_y, 20, WHITE);

    if (gameEnded) {
      DrawRectangle(0, 100, screenWidth, 200, BLACK);
      if (total_position >= 0) {
        DrawText(TextFormat("GAME OVER\nTOTAL PROFIT: +%i\nTry again? [Y|N]",
                            total_position),
                 40, 180, 30, GREEN);
      } else {
        DrawText(TextFormat("GAME OVER\nTOTAL PROFIT: %i\nTry again? [Y|N]",
                            total_position),
                 40, 180, 30, RED);
      }

      //reseta o jogo
      if (IsKeyPressed(KEY_Y)) {
        t = 0;
        prices[0] = GetScreenHeight() / 2;
        total_position = 0;
        current_position = 0;
        last_position = 0;
        priceBuy = 0;
        priceSell = 0;
        posx = 0;
        price = GetScreenHeight() / 2;
        currentlyBought = false;
        currentlySelled = false;
        qnt_buys = 0;
        qnt_sells = 0;
        gameEnded = false;
      }
    }

    EndDrawing();

    if (!gameEnded) {

      t += 1;

      if (t % 5 == 0) {
        posx += 1;

        randValue = GetRandomNormalValue(0.0, 5.);

        price += randValue;

        prices[posx] = price;

        // randValue = GetRandomNormalValue(0.0,1);
        // double mu = 1;
        // double sigma = 0.2;

        // int increment = (int) (exp( (mu- 0.5*pow(sigma,2.0) + sigma*randValue)));
        // price = prices[posx - 1] * increment; //* exp((mu - 0.5*pow(sigma,2.0)) + sigma*randValue);
        // prices[posx] = price;
        
        //S[t+1] <- S[t] * exp((mu - 0.5*sigma^2)*dt + sigma*dW[t])  

        
      }
    }

    if (posx == end_screen) {
      gameEnded = true;
    }
  }

  CloseWindow();
  return 0;
}