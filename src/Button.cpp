#include "Arduino.h"
#include "Button.h"

Button::Button()
{
	pin = DEFAULT_PIN;
	oldValue = 0;
	value = 0;
}

void Button::assignPin(int p)
{
	pin = p;
}

bool Button::uniquePress()
{
	bool ret = false;
	value = digitalRead(pin);
	if ((value == HIGH) && (oldValue == LOW)) {
		ret = true;
		delay(DEBOUNCE);
	}
	oldValue = value;
	return ret;
}