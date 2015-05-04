/*
 * Device::SerialPort::Arduino Interface
 *
 * Joaquín Ferrero
 * v20150504 - First version
 *
 * Commands  available:
 *
 * 	on|off <pin>		set one pin
 * 	on|off all		set all pins
 * 	toggle <pin>		toggle one pin
 * 	toggle all		toggle all
 * 	echo on|off		set echo mode
 *
 * Commands separated by new line character.
 */

// ============================================================================
// User configuration ---------------------------------------------------------
const byte Pins[] =					// pins handled
{// 0   1   2   3   4   5   6   7   8   9  10  11  12  13
    4,  5,  6,  7, 13
};
// End configuration ----------------------------------------------------------

// ============================================================================
// Definitions ----------------------------------------------------------------
#define TRUE			1
#define FALSE			0
#define ForeachAllPins		for (byte i = 0; i < sizeof(Pins); i++ )

// Constants ------------------------------------------------------------------


// Variables ------------------------------------------------------------------
String Command;					// store command received
byte Is_Echo = FALSE;				// flag if echo command

// ============================================================================
void Run_Command(void);
void Toggle(byte Pin);
// ============================================================================
// Setup ----------------------------------------------------------------------
void setup() {

    // I/O configuration
    ForeachAllPins {
	pinMode(Pins[i], OUTPUT);
    }

    // Serial communication
    Serial.begin(115200);
}

// Loop -----------------------------------------------------------------------
void loop() {

    while (Serial.available()) {
	char character = Serial.read();
	Command.concat(character);
	delay(10);

	if (character == '\n') {
	    Command.trim();
	    Command.toLowerCase();
	    if (Is_Echo)
		Serial.println(Command);
	    Run_Command();
	    Command = "";
	}
    }
}

void Run_Command() {

    byte i = Command.indexOf(' ');		// search parameter
    if (i == -1) return;			// return if not parameter

    String Cmd    = Command.substring(0,i);
    String Target = Command.substring(i+1);
    Target.trim();

    if ((Cmd == "on" )				// change pin state
    ||  (Cmd == "off")) {
	byte pin    = Target.toInt();
	byte action = (Cmd    == "on" ) ? HIGH : LOW;
	byte all    = (Target == "all") ? TRUE : FALSE;
	ForeachAllPins
	    if (all || pin == Pins[i]) {
		digitalWrite(Pins[i], action);
		if (!all)
		    break;
	    }
    }
    else if (Cmd == "toggle") {			// change state
	byte pin    = Target.toInt();
	byte all    = (Target == "all") ? TRUE : FALSE;
	ForeachAllPins
	    if (all || pin == Pins[i]) {
		Toggle(Pins[i]);
		if (!all)
		    break;
	    }
    }
    else if (Cmd == "echo") {
	Is_Echo = (Target == "on") ? TRUE : FALSE;
    }
}

void Toggle(byte Pin) {
    if (Pin >= 8) {
	PINB = 1 << (Pin-8);
    }
    else {
	PIND = 1 << Pin;
    }
}

// End ------------------------------------------------------------------------
