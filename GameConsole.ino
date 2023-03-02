

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 6, XM = A2, YP = A1, YM = 7;  //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pixel_x, pixel_y;

bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  //.kbv makes sense to me
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

//Screen colors
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


int firstButtonState = 0;
int secondButtonState = 0;

double xPos = 0;
double yPos = 0;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.fillRect(xPos, yPos, 100, 100, YELLOW);
}

void loop() {
  firstButtonState = digitalRead(13);
  secondButtonState = digitalRead(12);

  double lastPos = xPos;
  if (firstButtonState == HIGH) {
    xPos += 0.1;
  }
  if (secondButtonState == HIGH) {
    xPos -= 0.1;
  }

  if (lastPos != xPos) {
    tft.fillRect(xPos, yPos, 100, 100, YELLOW);
    tft.fillRect(lastPos, yPos, 1, 100, BLACK);
  }
}
