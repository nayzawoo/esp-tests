//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

#include <M5StickC.h>

//ULTLORES
//        #define GRIDX 30
//        #define GRIDY 20
//        #define CELLXY 4

//LORES
//#define GRIDX = 60
// #define GRIDY = 40
// #define CELLXY = 2
int viewx = 120;
int viewy = 80;
//HIRES
#define GRIDX 120
#define GRIDY 80
#define CELLXY 1

#define TFT_GREY 0x1010
#define TFT_ORANGE 0xFFA5
int runt = 0;
int res = 1;

int rcolor = random(0xFFFF);

//Number of generations
#define NUMGEN 999
int gens = NUMGEN;
uint16_t genCount = 0;

//Current grid
uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
uint8_t newgrid[GRIDX][GRIDY];

double vbat = 0.0;
int discharge, charge;
double temp = 0.0;
double bat_p = 0.0;

void setup()
{

  pinMode(M5_LED, OUTPUT);

  digitalWrite(M5_LED, LOW);

  //Set up the display
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(TFT_RED);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(0, 0);
  M5.Axp.EnableCoulombcounter();

  extern const unsigned char gImage_002[];

  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.drawBitmap(0, 0, 80, 160, (uint16_t *)gImage_002);

  delay(2000);

  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.println(F(" LITTLE BITS  "));

  delay(6000);
  M5.Lcd.fillScreen(WHITE);
}

// menu select resolution
void setres()
{
  extern const unsigned char gImage_002[];
  M5.Lcd.setRotation(0);
  rcolor == random(0xFFFF);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawBitmap(0, 0, 80, 160, (uint16_t *)gImage_002);
  M5.Lcd.setRotation(3);
  while (digitalRead(M5_BUTTON_HOME) == HIGH)
  {
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.setTextColor(BLACK, WHITE);
    M5.Lcd.println(F(" LITTLE BITS  "));
    M5.Lcd.setCursor(5, 60);
    M5.Lcd.println(F(" set Res"));
    M5.Lcd.setCursor(120, 60);
    M5.Lcd.println(res);
    if (digitalRead(M5_BUTTON_RST) == LOW)
    {
      res++;
      rcolor == random(0xFFFF);
    }
    if (res == 5)
    {
      res = 1;
    }
    CELLXY == res;
    if (res == 1)
    {
      viewx = 120;
      viewy = 80;
      gens = 999;
    }
    if (res == 2)
    {
      viewx = 60;
      viewy = 40;
      gens = 999;
    }
    if (res == 3)
    {
      viewx = 40;
      viewy = 30;
      gens = 800;
    }
    if (res == 4)
    {
      viewx = 30;
      viewy = 20;
      gens = 500;
    }
    delay(50);
  }
  M5.Lcd.fillScreen(BLACK);
}

// Check the Moore neighborhood
int getNumberOfNeighbors(int x, int y)
{
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

//Compute the CA. Basically everything related to CA starts here
void computeCA()
{
  for (int16_t x = 1; x < viewx; x++)
  {
    for (int16_t y = 1; y < viewy; y++)
    {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)
        newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3 || neighbors == 7))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0)
        newgrid[x][y] = 0;
    }
  }
}

//Draws the grid on the display
void drawGrid(void)
{

  int color = 0x0000;
  for (int16_t x = 1; x < 120 - 1; x++)
  {
    for (int16_t y = 1; y < 80 - 1; y++)
    {
      if ((grid[x][y]) != (newgrid[x][y]))
      {
        if (newgrid[x][y] == 1)
          color = rcolor; //random(0xFFFF);
        else
          color = 0;
        M5.Lcd.fillRect(res * x, res * y, res, res, color);
      }
    }
  }
}

//Initialise Grid
void initGrid(void)
{
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawRect(0, 0, 120, 80, random(0xFFFF));
  for (int16_t x = 0; x < viewx; x++)
  {
    for (int16_t y = 0; y < viewy; y++)
    {
      newgrid[x][y] = 0;

      if (x == 0 || x == viewx - 1 || y == 0 || y == viewy - 1)
      {
        grid[x][y] = 0;
      }
      else
      {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }
    }
  }
  runt++;
}

void statusSys()
{
  M5.Lcd.setRotation(0);
  extern const unsigned char gImage_002[];
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.drawBitmap(0, 0, 80, 160, (uint16_t *)gImage_002);

  M5.Lcd.setTextColor(BLACK, WHITE);
  while (digitalRead(M5_BUTTON_HOME) == HIGH)
  {

    vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
    temp = -144.7 + M5.Axp.GetTempData() * 0.1;
    bat_p = M5.Axp.GetPowerbatData() * 1.1 * 0.5 / 1000;
    M5.Lcd.setRotation(3);
    M5.Lcd.setCursor(0, 15, 1);
    M5.Lcd.printf("batt:%.3fV\r\n", vbat);    //battery voltage
    M5.Lcd.printf("temp:%.1fC\r\n", temp);    //axp192 inside temp
    M5.Lcd.printf("power:%.3fmW\r\n", bat_p); //battery power
    delay(500);
  }
  digitalWrite(M5_LED, LOW);
  delay(200);
  setres();
}



void loop()
{

  int GEN_DELAY = analogRead(60) / 400;
  //Display a simple splash screen

  initGrid();

  genCount = NUMGEN;

  drawGrid();

  //Compute generations
  for (int gen = 0; gen < gens; gen++)
  {
    if (digitalRead(M5_BUTTON_HOME) == LOW)
    {
      gen = 0;
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(TFT_WHITE, RED);
      M5.Lcd.setCursor(35, 27);
      M5.Lcd.println(F("  NUKE  "));
      delay(200);
      rcolor = random(0xFFFF);
      runt--;
      initGrid();
      int GEN_DELAY = 20;
    }
    if (digitalRead(M5_BUTTON_RST) == LOW)
    {
      setres();
      M5.Lcd.setRotation(3);
    }
    vbat = M5.Axp.GetVbatData() * 1.1 / 1000;

    M5.Lcd.setTextColor(TFT_WHITE, BLACK);
    M5.Lcd.setCursor(132, 5);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("GEN");
    M5.Lcd.setCursor(122, 20);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(gen);
    M5.Lcd.setCursor(132, 45);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println("RUN");
    M5.Lcd.setCursor(122, 60);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println(runt);
    M5.Lcd.setCursor(0, 70);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(BLACK, RED);
    M5.Lcd.printf("%.3fV\r\n", vbat);
    if (vbat <= 3.8)
    {
      M5.Lcd.setCursor(20, 30);
      M5.Lcd.setTextSize(3);
      M5.Lcd.printf("FEED ME");
    }
    digitalWrite(M5_LED, HIGH);
    delay(GEN_DELAY);

    computeCA();
    drawGrid();

    for (int16_t x = 1; x < viewx - 1; x++)
    {
      for (int16_t y = 1; y < viewy - 1; y++)
      {
        grid[x][y] = newgrid[x][y];
      }
    }
  }
}
