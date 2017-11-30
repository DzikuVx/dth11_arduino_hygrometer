#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Adafruit_SSD1306.h>
#include <DHT_U.h>
#include <DHT.h>
#include <LowPower.h>

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

#define POWER_PIN 5
#define DHT_POWER_PIN 6

DHT dht(9, DHT11);
Adafruit_SSD1306 display(-1);

void setup()
{
    pinMode(POWER_PIN, OUTPUT);
    pinMode(DHT_POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);
    digitalWrite(DHT_POWER_PIN, HIGH);
    
    LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(25, 25);
    display.println("WAIT...");
    display.display();
    delay(2500);
}

void loop()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(4);
    display.print((int8_t)dht.readHumidity());
    display.print("%");

    display.setCursor(70, 36);
    display.setTextSize(3);
    display.print((int8_t)dht.readTemperature());
    display.print("C");

    digitalWrite(DHT_POWER_PIN, LOW);

    display.display();

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    display.clearDisplay();
    display.display();
    digitalWrite(POWER_PIN, LOW);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    // LowPower.powerExtStandby(SLEEP_FOREVER, ADC_OFF, BOD_OFF, TIMER2_OFF);
}
