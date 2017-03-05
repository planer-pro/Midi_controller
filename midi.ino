#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX/Adafruit_GFX.h>
#include <Encoder.h>

#define OLED_RESET 4
//#define button 4

Adafruit_SSD1306 display(OLED_RESET);
Encoder myEnc(2, 3);

int encOld = -999;

void setup()
{
	Serial.begin(57600);

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();

	myEnc.write(240);
}

void loop()
{
	int encNew = myEnc.read();
	if (encNew != encOld)
	{
		encOld = encNew;
		encNew /= 2;
		if (encNew > 127)
		{
			encNew = 127;
			myEnc.write(encNew * 2);
		}
		if (encNew < 0)
		{
			encNew = 0;
			myEnc.write(encNew);
		}
		doSendMidi(0xB0, 11, encNew); // MIDI control change - канал 1, MIDI controller #11, MIDI controller value

		display.clearDisplay();

		display.setTextSize(2);
		display.setTextColor(WHITE);
		display.setCursor(36, 2);
		display.println("Value:");

		int posX;
		if (encNew < 10)
		{
			posX = 60;
		}
		if (encNew >= 10 && encNew < 100)
		{
			posX = 50;
		}
		if (encNew > 99)
		{
			posX = 40;
		}

		display.setTextSize(3);
		display.setCursor(posX, 32);
		display.print(encNew);

		display.display();
	}
}

void doSendMidi(int chanel, int controller, int value)
{
	Serial.write(chanel);
	Serial.write(controller);
	Serial.write(value);
}