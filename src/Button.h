#ifndef BUTTON_H
#define BUTTON_H
#endif

#define DEFAULT_PIN 2
#define DEBOUNCE 150

class Button
{
	public:
		Button();
		void assignPin(int p);
		bool uniquePress();
	
	private:
		int pin;
		int oldValue;
		int value;
};