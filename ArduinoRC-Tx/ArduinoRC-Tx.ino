// ArduinoRC-Tx.ino

// (c) Will Streuli 2015
// github.com/Streuli273


// read values from analog sticks
int throttleval;
int elevval;
int rudderval;
int aileronval;

// read values from pots for trim
int throttletrimval;
int elevtrimval;
int ruddertrimval;
int ailerontrimval;

// values to send to plane, via XBee
String sendthrottleval;
String sendelevval;
String sendrudderval;
String sendaileronval;

// dual rate values from pots
float elevdualrate;
float ailerondualrate;

boolean simulator = false;

void setup() {

	Serial1.begin(38400);

	pinMode(13, OUTPUT);

	pinMode(12, INPUT_PULLUP);
	//digitalWrite(12, HIGH);

	pinMode(11, INPUT_PULLUP);
	//digitalWrite(11, HIGH);
	pinMode(10, INPUT_PULLUP);
	//digitalWrite(10, HIGH);

	// check plugged into computer for simulator
	if(digitalRead(12) == HIGH) {
		simulator = false;
	} else {
		simulator = true;
		digitalWrite(13, HIGH);
	}
	
}

void loop() {

	aileronDualRead();
	//elevDualRead();

	throttleRead();
	elevRead();
	aileronRead();
	rudderRead();

	sendData();

}

void throttleRead() {
	throttleval = analogRead(0);
	throttleval = map(throttleval, 0, 1023, 0, 179);
	throttletrimval = analogRead(4);
	throttletrimval = map(throttletrimval, 0, 1023, -25, 25);
	throttleval += throttletrimval;
	constrain(throttleval, 0, 179);
	sendthrottleval = String(throttleval);
	sendthrottleval += "a";
}

void elevRead() {
	elevval = analogRead(1);
	elevval = map(elevval, 0, 1023, 0, 179);
	elevtrimval = analogRead(5);
	elevtrimval = map(elevtrimval, 0, 1023, -25, 25);
	elevval += elevtrimval;
	//elevval *= elevdualrate;
	constrain(elevval, 0, 179);
	sendelevval = String(elevval);
	sendelevval += "b";
}

void aileronRead() {
	aileronval = analogRead(3);
	aileronval = map(aileronval, 0, 1023, 0, 179);
	ailerontrimval = analogRead(A7);
	ailerontrimval = map(ailerontrimval, 0, 1023, -25, 25);
	aileronval += ailerontrimval;
	aileronval *= ailerondualrate;
	aileronval /= 100;
	constrain(aileronval, 0, 179);
	sendaileronval = String(aileronval);
	sendaileronval += "d";
}

void rudderRead() {
	rudderval = analogRead(2);
	rudderval = map(rudderval, 0, 1023, 0, 179);
	ruddertrimval = analogRead(A6);
	ruddertrimval = map(ruddertrimval, 0, 1023, -25, 25);
	rudderval += ruddertrimval;
	constrain(rudderval, 0, 179);
	sendrudderval = String(rudderval);
	sendrudderval += "c";
}

// for dual rates (factor of control surface movement)
void elevDualRead() {
	if(digitalRead(12) == HIGH) {
	    elevdualrate = analogRead(A8);
	    elevdualrate = map(elevdualrate, 0, 1023, 1, 100);
	} else {
	    elevdualrate = 1;
	}
}

// for dual rates (factor of control surface movement)
void aileronDualRead() {
	if(digitalRead(11) == HIGH) {
	    ailerondualrate = analogRead(A9);
	    ailerondualrate = map(ailerondualrate, 0, 1023, 1, 100);
	} else {
	    ailerondualrate = 1;
	}
}

// send data via XBee/equivalent
void sendData() {
		if(simulator == false){
		Serial1.print(sendthrottleval);
		Serial1.print(",");
		Serial1.print(sendelevval);
		Serial1.print(",");
		Serial1.print(sendrudderval);
		Serial1.print(",");
		Serial1.print(sendaileronval);
		Serial1.println(",");
		delay(10);
	}
}

void simulatorControl() {
	// TODO Simulator control
}
