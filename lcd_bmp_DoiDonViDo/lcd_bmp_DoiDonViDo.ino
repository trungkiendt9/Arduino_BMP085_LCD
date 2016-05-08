//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
/*******************************************************

  This program will test the LCD panel and the buttons
  Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_BMP085 bmp;
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
unsigned int id = 0;                       // Chi so do vi do trong mang don vi
unsigned int dot = 0;                       // So sau dau thap phan
#define btnRIGHT  100
#define btnUP     101
#define btnDOWN   102
#define btnLEFT   103
#define btnSELECT 104
#define btnNONE   105
#define Pa   0
#define hPa  1
#define kPa  2
#define bar  3
#define mmHg 4
#define atm  5
#define mbar 6
//Khai bao mang ki tu cac don vi do ap suat
char* Unit[]={"Pa  ", "hPa ", "kPa ", "bar ","mmHg", "atm ","mbar"}; // 7 don vi do
// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  delay(150);
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  // if (adc_key_in < 50)   return btnRIGHT;
  // if (adc_key_in < 250)  return btnUP;
  // if (adc_key_in < 450)  return btnDOWN;
  // if (adc_key_in < 650)  return btnLEFT;
  // if (adc_key_in < 850)  return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:

  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}
//Ham hien thi nhiet do len LCD
void HienThiNhietDo()
{
  lcd.setCursor(0, 0);
  lcd.print("T= "); // print a simple message
  lcd.setCursor(3, 0);
  lcd.print(bmp.readTemperature());
  lcd.setCursor(8, 0);
  lcd.print(char(223));
  lcd.setCursor(9, 0);
  lcd.print("C"); 
}
//Ham hien thi ap suat len LCD
int HienThiApSuat(unsigned int id, unsigned int dot)
{
  lcd.setCursor(0, 1);
  lcd.print("P= "); // print a simple message
  lcd.setCursor(3, 1);
  lcd.print(ConvertPressure(id), dot);
  lcd.setCursor(12, 1);
  lcd.print(Unit[id]);
}
// Ham chuyen doi giua cac don vi do
float ConvertPressure(unsigned int id)
{
  switch (id)
  {
    case Pa:
    {
      dot = 0;
      return float(bmp.readPressure());
    }
    case hPa:
    {
      dot = 2;
      return float(bmp.readPressure())/float(100);
    }
    case kPa:
    {
      dot = 3;
      return float(bmp.readPressure())/float(1000);
    }
    case bar:
    {
      dot = 5;
      return float(bmp.readPressure())/float(100000);
    }
    case mmHg:
    {
      dot = 5;
      return float(bmp.readPressure())*0.00750061683;
    }
    case atm:
    {
      dot = 7;
      return float(bmp.readPressure())*0.00000986923267;
    }
    case mbar:
    {
      dot = 2;
      return float(bmp.readPressure())/float(100);
    }
  }
  return float(bmp.readPressure());
}
// Ham chon lua va doi don vi do
void SetupUnit()
{
  lcd.setCursor(0,1);
  lcd.print("UP DOWN SELECT");
  lcd.setCursor(0,0);
  lcd.print("Setup unit:");
  lcd.setCursor(11,0);
  lcd.print(Unit[id]);
  lcd.blink();
  do
  {
    lcd_key = read_LCD_buttons();  // read the buttons
    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
      case btnUP:
        {
          id = id + 1;
          id = id%7;                // Quay vong cho don vi
          lcd.setCursor(11,0);
          lcd.print(Unit[id]);
          break;
        }
      case btnDOWN:
        {
          if (id == 0)
          {
            id = 6;
          }
          else
          {
            id = id - 1;
            id = id%7;                // Quay vong cho don vi
          }
            lcd.setCursor(11,0);
            lcd.print(Unit[id]);
          break;
        }
      case btnNONE:
        {
        }
    }
  } while (lcd_key != btnSELECT);
  lcd.setCursor(0,0);
  lcd.print("UnitChoosen:");
  lcd.print(Unit[id]);
  lcd.setCursor(0,1);
  lcd.print(" UNIT SELECTED  ");
  delay(3000);
}
void setup()
{
  lcd.begin(16, 2);              // start the library
  Serial.begin(9600);
  Serial.print(sizeof(Unit));
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  SetupUnit();                    // Setup don vi do
  lcd.clear();
}

void loop()
{
  Serial.println(bmp.readPressure());
  Serial.println(ConvertPressure(id));
  lcd_key = read_LCD_buttons();
  if (lcd_key == btnLEFT)
  {
    setup();
  }
  HienThiNhietDo();
  HienThiApSuat(id, dot);
  delay(1000);
}