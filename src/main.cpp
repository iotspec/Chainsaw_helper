

VERSION 1.1

#include <Arduino.h>

#include <Wire.h> // for LiquidCrystal
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define BTN_UP   1
#define BTN_DOWN 2
#define BTN_LEFT 3
#define BTN_RIGHT 4
#define BTN_SELECT 5
#define BTN_NONE 10

int detectButton() {
        int keyAnalog =  analogRead(A0);
        if (keyAnalog < 100) {
                // right
                return BTN_RIGHT;
        } else if (keyAnalog < 200) {
                //  UP
                return BTN_UP;
        } else if (keyAnalog < 400) {
                //  DOWN
                return BTN_DOWN;
        } else if (keyAnalog < 600) {
                //  LEFT
                return BTN_LEFT;
        } else if (keyAnalog < 800) {
                //  SELECT
                return BTN_SELECT;
        } else {
                return BTN_NONE;
        }
}

volatile byte revolutions;
unsigned int rpm;
unsigned int rpmMax;

unsigned long timeold;
unsigned long timedif;

unsigned long printDisplayDelay;
unsigned long buttonDelay;

int varIn = 0;
int hallVal = 0;

void setup()
{
        Serial.begin(115200);
        lcd.begin(16, 2);
        lcd.setCursor(0, 0);
        lcd.print("RPM ");
        lcd.setCursor(0, 1);
        lcd.print("MAX ");
        lcd.setCursor(11, 0);
        lcd.print("M ");

        // attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
        revolutions = 0;
        rpm = 0;
        timeold = 0;
}




void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
        revolutions++;
        // Serial.println("detect");
}

void hallVal_detect()
{
        hallVal = analogRead(A5);
        if (hallVal < 430) varIn = 1;
        if (hallVal > 430 && varIn == 1) {
                revolutions++;
                varIn = 0;
        }
}



void rpmCalculation() {

        // detachInterrupt(0);

        timedif = millis() - timeold;
        rpm = 60000 / timedif * revolutions;
        timeold = millis();
        revolutions = 0;
        if (rpm > rpmMax) rpmMax = rpm;

        // attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
}



void printDisplay() {
        lcd.setCursor(4, 0);
        lcd.print("     ");

        // Serial.println(rpm);

        lcd.setCursor(4, 0);
        lcd.print(rpm, DEC);

        lcd.setCursor(4, 1);
        lcd.print(rpmMax, DEC);

        lcd.setCursor(13, 0);
        lcd.print(hallVal);

        Serial.println(hallVal);

        printDisplayDelay = millis();
}


void buttonJob() {
        int button = detectButton();
        switch (button) {
        case BTN_UP:
                break;
        case BTN_DOWN:
                break;
        case BTN_LEFT:
                break;
        case BTN_RIGHT:
                break;
        case BTN_SELECT:
                rpmMax = 0;
                lcd.setCursor(4, 1);
                lcd.print("     ");
                break;
        case BTN_NONE:
                break;
        }
        buttonDelay = millis();
}

void loop()//Measure RPM
{
        hallVal_detect();
        if (revolutions >= 20) rpmCalculation();
        // if ((millis() - buttonDelay) > 1000) buttonJob();
        if ((millis() - printDisplayDelay) > 1000) printDisplay();
}
