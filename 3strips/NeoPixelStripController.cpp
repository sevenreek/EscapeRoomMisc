#include <Adafruit_NeoPixel.h> 

#define DEBOUNCE_TIME 2000
#define STATE_COUNT 3
enum StripState {
	RUN_RIGHT,
	RUN_LEFT,
	STOPPED
};

class NeoPixelStripController {
private:
	StripState stripState;
	Adafruit_NeoPixel * LEDStrip;
	unsigned long lastInterruptTime;
	uint32_t stripColor;
	int lastLitRelative;
	int rightStopPixel;
	int leftStopPixel;
	int leftRightGap;
	int pixelCount;
public:
	NeoPixelStripController(Adafruit_NeoPixel * strip, uint32_t stripColor, unsigned int pixelCount)
	{
		this->stripColor = stripColor;
		this->LEDStrip = strip;
		this->pixelCount = pixelCount;
		lastInterruptTime = 0;
		lastLitRelative = -1;
		stripState = RUN_RIGHT;
		leftStopPixel = 0;
		rightStopPixel = pixelCount -1;
		leftRightGap = rightStopPixel - leftStopPixel;
		
	}
	void init()
	{
		LEDStrip->begin();
		LEDStrip->show();
	}
	void updateStrip()
	{
		switch(stripState)
		{
			case RUN_RIGHT:
			{
				int relativeToLight = (lastLitRelative + 1);
				if(relativeToLight >= (leftRightGap+1))
				{
						//Serial.println("CLEARIN!");
						LEDStrip->clear();
						//Serial.print(relativeToLight); Serial.print("%"); Serial.print(leftRightGap); Serial.print("\n");
						relativeToLight %= (leftRightGap+1);
				}
				uint32_t clr = LEDStrip->Color(255,0,0);
				//Serial.print("Lighting "); Serial.print(leftStopPixel+relativeToLight); Serial.print(" with "); Serial.print(stripColor); Serial.print("\n"); 
				LEDStrip->setPixelColor(leftStopPixel+relativeToLight,clr);
				LEDStrip->show();
				lastLitRelative = relativeToLight;
				break;
			}
			case RUN_LEFT:
			{
				int relativeToLight = (lastLitRelative + 1);
				if(relativeToLight >= (leftRightGap+1))
				{
					LEDStrip->clear();
					relativeToLight %= (leftRightGap+1);
				}
				LEDStrip->setPixelColor(rightStopPixel-relativeToLight,stripColor);
				LEDStrip->show();
				lastLitRelative = relativeToLight;
				break;
			}
			case STOPPED:
			{
				return;
			}
			
		}
	}
	void handleIRQ()
	{
		if ((millis() - lastInterruptTime) < DEBOUNCE_TIME)
			return;
		switch(stripState)
		{
			case RUN_RIGHT:
			{
				rightStopPixel = lastLitRelative;
				leftRightGap = rightStopPixel-leftStopPixel;
				lastLitRelative = rightStopPixel ;
				LEDStrip->clear();
				Serial.print("IRQ IN RUN_RIGHT! RIGHT: "); Serial.print(rightStopPixel); Serial.print(" LEFT: "); Serial.print(leftStopPixel); Serial.print(" LAST LIT:"); Serial.print(lastLitRelative); Serial.print("\n"); 
				break;
			}
			case RUN_LEFT:
			{
				leftStopPixel = rightStopPixel - lastLitRelative;
				leftRightGap = rightStopPixel-leftStopPixel;
				lastLitRelative = leftStopPixel - 1;
				Serial.print("IRQ IN RUN_LEFT! RIGHT: "); Serial.print(rightStopPixel); Serial.print(" LEFT: "); Serial.print(leftStopPixel); Serial.print(" LAST LIT:"); Serial.print(lastLitRelative); Serial.print("\n"); 
				break;
			}
			case STOPPED:
			{
				leftStopPixel = 0;
				rightStopPixel = pixelCount-1;
				lastLitRelative = -1;
				leftRightGap = rightStopPixel - leftStopPixel;
				LEDStrip->clear();
				Serial.print("IRQ IN STOPPED! RIGHT: "); Serial.print(rightStopPixel); Serial.print(" LEFT: "); Serial.print(leftStopPixel); Serial.print(" LAST LIT:"); Serial.print(lastLitRelative); Serial.print("\n"); 
				break;
			}
		}
		this->stripState = static_cast<StripState>((this->stripState+1)%STATE_COUNT);
	}
	StripState getState()
	{
		return stripState;
	}
	bool isMatchingSolution(int left, int right)
	{
		return ( (left == leftStopPixel) && (right == rightStopPixel) );
	}
};
