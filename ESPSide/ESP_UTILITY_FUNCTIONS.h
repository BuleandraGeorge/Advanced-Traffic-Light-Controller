bool granted()
{
	return (!digitalRead(GRANTED));
}
void leaveHigh(unsigned char pin) {
	pinMode(pin, INPUT_PULLUP);
}
void pullLow(unsigned char pin) {
	digitalWrite(pin, LOW);
	pinMode(pin, OUTPUT);
}