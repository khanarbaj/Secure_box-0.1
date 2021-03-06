/*The fingerprint sensor is supported by "Adafruit_Fingerprint.h" created by Adafruit company */

#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<HardwareSerial.h>
#include <Keypad.h>
#include <ctype.h>
#include<EEPROM.h>

#define DELAY 100
#define EEPROM_SIZE 7

LiquidCrystal_I2C lcd(0x27, 21, 22);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char a;
char admin_id;
int _admin_id;
char password[6];
char user_pass[6];
char get_key;
int i = 0;
int detect_id;
int id;


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

//initialize an instance of class NewKeypad
Keypad m_keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup() {

  lcd.begin();
  Serial.begin(9600);
  lcd.backlight();
  EEPROM.begin(EEPROM_SIZE);

  finger.begin(57600);
  delay(5);

  lcd.print("Elektrobotics");
  lcd.setCursor(0, 1);
  lcd.print("-------------");
  delay(3000);
  lcd.clear();

  if (finger.verifyPassword()) {
    lcd.setCursor(0, 0);
    lcd.print("fingerprint OK");
    delay(2000);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("fingerprint NOT FOUND");
    while (1) {
      delay(1);
    }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");



  if (finger.templateCount == 0)
  {
    admin_register();

  }
}

/*************** FUNCTION FOR ADMIN REGISTRATION ******************/
void admin_register()
{

  char confirm;//

  while (1)
  {
    lcd.clear();
    lcd.print("Register Admin");
    lcd.setCursor(0, 1);
    lcd.print("Finger-PRESS D");
    //      lcd.clear();
    //      lcd.print("PRESS D");
    char confirm = m_keypad.waitForKey();
    delay(10);
    if (confirm == 'D')
    {
      break;
    }

  }
  //    lcd.clear();
  //    lcd.print("WELCOME");
  //    delay(500);
  while (1)
  {
    lcd.clear();
    lcd.print("Ready to");
    lcd.setCursor(0, 1);
    lcd.print("Register..");
    delay(1000);
    //      lcd.clear();
    //      lcd.print("Type ID ");
    //      lcd.setCursor(0, 1);
    //      lcd.print("(from 1 to 127)");

    admin_id = '0';
    _admin_id = admin_id - '0';
    id = _admin_id;
    Serial.println(id);



    lcd.clear();
    lcd.print("ID: ");
    lcd.setCursor(3, 0);
    lcd.print(_admin_id);
    delay(500);
    lcd.setCursor(5, 0);
    lcd.print("PUT FINGURE");
    lcd.setCursor(0, 1);
    lcd.print("ON SENSOR");
    delay(500);

    while (finger.templateCount == 0)
    {
      Serial.println(finger.templateCount);
      getFingerprintEnroll();
      finger.getTemplateCount();
      if (finger.templateCount > 0)
      {
        break;
      }

    }
    if (finger.templateCount > 0)
    {
      break;
    }

  }
  lcd.clear();
  lcd.print("SET PASSWORD:");
  lcd.setCursor(0, 1);
  while (i < 6)
  {
    char key = m_keypad.getKey();
    if (key)
    {
      //password[i] = key;
      lcd.print(key);
      EEPROM.write(i, key);
      EEPROM.commit();
      i++;
      //Serial.print(key);
    }
  }
  i = 0;
  lcd.clear();
  lcd.print("ADMIN REGISTERED");
  lcd.setCursor(0, 1);
  lcd.print("SUCESSFULLY");
  delay(2000);
  lcd.clear();
  lcd.print("PRESS * TO EXIT");
  while (1)
  {
    if (m_keypad.waitForKey() == '*')
    {
      lcd.clear();
      lcd.print("RESTARTING.......");
      delay(2000);
      ESP.restart();
    }
  }

}

/************** FUNCTION TO STORE PASSWPRD ************************/
//void store_pass(char pass[6])
//{
//
//
//
//}

/************** FUNCTION FOR USER REGISTER*************************************/
void user_register()
{
  int p;
  lcd.clear();
  lcd.print("VERIFY ADMIN ");
  lcd.setCursor(0, 1);
  lcd.print("FINGER");
  delay(1000);
  while (1)
  {

    detect_id = getFingerprintIDez();
    if (detect_id == 0)
    {
      lcd.clear();
      lcd.print("ADMIN VERIFIED");
      delay(500);
      lcd.clear();
      lcd.print("READY TO ");
      lcd.setCursor(0, 1);
      lcd.print("REGISTER USER");
      delay(1000);
      lcd.clear();
      lcd.print("TYPE USER ID");
      lcd.setCursor(0, 1);
      lcd.print("(FROM 1 TO 9):");
      lcd.setCursor(15, 1);
      id = readnumber();
      lcd.print(id);
      delay(1000);
      //Serial.println(id);
      while ( 1 )
      {
        p = getFingerprintEnroll();
        if (p == 0)
        {
          break;
        }
        //Serial.println("p = ");
        //Serial.print(p);
      }
      lcd.clear();
      lcd.print("USER REGISTERED");
      lcd.setCursor(0, 1);
      lcd.print("SUCESSFULLY");
      delay(2000);
      lcd.clear();
      lcd.print("PRESS * TO EXIT");
      if (m_keypad.waitForKey() == '*')
      {
        ESP.restart();
      }

    }
    else
    {

      //      lcd.clear();
      //      lcd.print("ADMIN NOT ");
      //      lcd.setCursor(0, 1);
      //      lcd.print("VERIFIED");
      //      delay(1000);
      lcd.clear();
      lcd.print("VERIFY ADMIN OR");

      lcd.setCursor(0, 1);
      lcd.print("PRESS * TO EXIT");

      delay(20);
      if (m_keypad.getKey() == '*')
      {
        break;
      }

    }

  }
}
/************** FUNCTION FOR READING PERSON ID ********************/
uint8_t readnumber(void) {
  //Serial.println("hello");
  int _user_id = 0;
  char user_id = '0' ;
  //user_id = m_keypad.waitForKey();


  while (user_id == '0')
  {
    user_id = m_keypad.waitForKey();
    if (isdigit(user_id) != 1)
    {
      while (isdigit(user_id) != 1)
      {
        lcd.clear();
        lcd.print("Enter number");
        user_id = m_keypad.waitForKey();

        if (isdigit(user_id) == 1)
        {
          break;
        }
        delay(40);

      }

    }
    if (user_id == '0')
    {
      lcd.clear();
      lcd.print("ID should not ");
      lcd.setCursor(0, 1);
      lcd.print("be 0");
      delay(1000);
    }

  }
  _user_id = user_id - 48;
  //lcd.clear();
  return _user_id;

}

/********************FUNCTION FOR USER FINGURE DETECTION*************/

void detect_fingure()
{

  detect_id = getFingerprintIDez();
  //Serial.println(detect_id);
  lcd.clear();
  lcd.print("PUT FINGER ON");
  lcd.setCursor(0, 1);
  lcd.print("SENSOR");
  if (detect_id == -1)
  {
    detect_id = getFingerprintIDez();

  }
  for (int id = 0; id < 10; id++)
  {
    if (id == detect_id)
    {
      lcd.clear();
      lcd.print("ENTER PASSWPRD:");
      lcd.setCursor(0, 1);
      while (i < 6)
      {
        
        char key = m_keypad.getKey();
        if (key)
        {
          user_pass[i] = key;

          //password[i] = EEPROM.read(i);

          lcd.print(key) ;
          Serial.print(key);
          i++;
        }
      
      }
      i = 0;
      //delay(20);
      Serial.println();
      //Serial.println(user_pass[]);
      // Retriving password from memory
      for (int j = 0 ; j < 6 ; j++)
      {
        password[j] = EEPROM.read(j);
      }
      Serial.println(password);
      Serial.println();
      for (int i = 0; i < 6; i++)
      {
        Serial.print(user_pass[i]);
      }
      Serial.println();
      for (int i = 0; i < 6; i++)
      {
        Serial.print(password[i]);
      }
      //Serial.println(strcmp(user_pass, password));
      //Serial.println(password);
      //delay(50);
      if (!(strncmp(user_pass, password, 6)))
      {
        lcd.clear();
        lcd.print("PASSWORD VERIFIED");
        lcd.setCursor(0, 1);
        lcd.print("WELCOME");
        delay(2000);
      }
      else
      {
        lcd.clear();
        lcd.print("WRONG PASSWORD");
        //          lcd.setCursor(0, 1);
        //          lcd.print((3 - k));
        //          lcd.print("ATTEMPTS LEFT");
        delay(1000);

      }
    }
  }
}



/*********************FUNCTION FOR USER ENROLLEMENT*****************/

uint8_t getFingerprintEnroll() {

  int p = -1;
  lcd.clear();
  lcd.print("WAITING FOR ID:");
  lcd.setCursor(15, 0);
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("FINGERE.......");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.print("Image taken");
        delay(DELAY);
        break;
      case FINGERPRINT_NOFINGER:
        lcd.clear();
        lcd.print("PUT FINGURE");
        lcd.setCursor(0, 1);
        lcd.print("ON SENSOR");
        //delay(DELAY);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.print("Communication error");
        delay(DELAY);
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.print("Imaging error");
        delay(DELAY);
        break;
      default:
        lcd.clear();
        lcd.print("Unknown error");
        delay(DELAY);
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(DELAY);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image too messy");
      delay(DELAY);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication error");
      delay(DELAY);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Could not find fingerprint features");
      delay(DELAY);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Could not find fingerprint features");
      delay(DELAY);
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown error");
      delay(DELAY);
      return p;
  }
  lcd.clear();
  lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.print("Place same ");
  lcd.setCursor(0, 1);
  lcd.print("finger again");
  //delay(DELAY);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.print("Image taken");
        delay(DELAY);
        break;
      case FINGERPRINT_NOFINGER:
        lcd.clear();
        lcd.print("PUT FINGURE");
        lcd.setCursor(0, 1);
        lcd.print("ON SENSOR");
        //delay(DELAY);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.print("Communication error");
        delay(DELAY);
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.print("Imaging error");
        delay(DELAY);
        break;
      default:
        lcd.clear();
        lcd.print("Unknown error");
        delay(DELAY);
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(DELAY);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image too messy");
      delay(DELAY);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication error");
      delay(DELAY);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Could not find fingerprint features");
      delay(DELAY);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Could not find fingerprint features");
      delay(DELAY);
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown error");
      delay(DELAY);
      return p;
  }

  // OK converted!
  lcd.clear();
  lcd.print("Creating model ");
  lcd.setCursor(0, 1);
  lcd.print("for:");
  lcd.setCursor(5, 1);
  lcd.print(id);
  delay(DELAY);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Prints matched!");
    delay(DELAY);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Communication error");
    delay(DELAY);
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("Fingerprints did not match");
    delay(DELAY);
    return p;
  }
  else {
    lcd.clear();
    lcd.print("Unknown error");
    delay(DELAY);
    return p;
  }

  // Serial.print("ID "); Serial.println(_admin_id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("DATA STORED!");
    Serial.println(p);
    delay(1000);
    return p;

  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Communication error");
    delay(DELAY);
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.print("Could not store ");
    lcd.setCursor(0, 1);
    lcd.print("in that location");
    delay(DELAY);
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.print("Error writing to flash");
    delay(DELAY);
    return p;
  }
  else {
    lcd.clear();
    lcd.print("Unknown error");
    delay(DELAY);
    return p;
  }
}
/**************************FUNCTION FOR FINGER DETECTION*************/
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

/****************** EMPTY DATABASE ********************************/
void empty_database()
{
  lcd.clear();
  lcd.print("PRESS D TO CONFIRM");
  lcd.setCursor(0, 1);
  lcd.print("PRESS * TO CANCEL");
  while (1)
  {
    get_key = m_keypad.getKey();
    if (get_key == 'D')
    {
      finger.emptyDatabase();
      for (int i = 0; i < EEPROM.length(); i++)
      {
        EEPROM.write(i, 0);
      }
      lcd.clear();
      lcd.print("DATABASE CLEARED");
      lcd.setCursor(0, 1);
      lcd.print("RESTARTING.......");
      delay(2000);
      ESP.restart();
    }
    if (get_key == '*')
    {
      break;
    }
  }

}

/******************LOOP FUNCTION************************************/

void loop() {
  get_key = m_keypad.getKey();                // get key value for menu
  detect_fingure();
  switch (get_key)
  {
    case 'A': Serial.println("User Register");
      user_register();
      break;
    case 'B': Serial.println("Delete User");
      break;
    case 'C': Serial.println("Empty database");
      empty_database();
      break;
    default : break;
  }
}
