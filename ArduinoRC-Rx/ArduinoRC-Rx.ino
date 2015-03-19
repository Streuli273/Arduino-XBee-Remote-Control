// ArduinoRC-Rx.ino

// (c) Will Streuli 2015
// github.com/Streuli273

#include "Servo.h"
#include "Wire.h"
#include "MPU6050.h"
#include "I2Cdev.h"

// interval for sneding telemetry data
// this will be GPS co-ords, status etc.
// Currently used for RSSI features of XBee
long telemetryInterval = 400;

long previousMillis = 0;

String readString;
Servo throttle, elev, rudder, laileron, raileron;

MPU6050 mpu;

const float alpha = 0.5;
float smooth_ax, smooth_ay, smooth_az;
int rollVal, pitchVal;


void setup() {
	Serial1.begin(38400);

	throttle.attach(9);
	elev.attach(8);
	rudder.attach(10);
	laileron.attach(16);
	raileron.attach(14);

	throttle.write(20);
	elev.write(20);
	rudder.write(20);
	laileron.write(20);
	raileron.write(20);
}

void loop() {

	unsigned long currentMillis = millis();

	relayInfo(currentMillis);

	doControl();

}

// gets data from XBee if available, writes values to pins, via Servo lib
void doControl() {

	if (Serial1.available()) {
		char c = Serial1.read();  //gets one byte from serial buffer
		if (c == ',') { // detects 'next value' char from data recieved.
			if (readString.length() > 1) {
				Serial.println(readString); //prints string to serial port out

				int n = readString.toInt();  //convert readString into a number

				// auto select appropriate value, copied from someone elses code
				Serial.print("writing Angle: ");
				Serial.println(n);
				if(readString.indexOf('a') >0) throttle.write(n);
				if(readString.indexOf('b') >0) elev.write(n);
				if(readString.indexOf('c') >0) rudder.write(n);
				if(readString.indexOf('d') >0) {
					laileron.write(n);
					raileron.write(n);
				}
				readString=""; //clears variable for new input
			}
		}  else {     
			readString += c; //makes the string readString
		}
	}

}

void relayInfo(unsigned long currentTime) {

	if(currentTime - previousMillis > telemetryInterval) {
		previousMillis = currentTime;
		Serial1.print('telemetry');
	}
}

// wil eventually be for self levelling
// TODO complete self levelling
void getAngles(int roll, int pitch) {

	int ax, ay, az, gx, gy, gz;

	mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	/* Apply low pass filter to smooth
	accelerometer values. */
	smooth_ax = ax * alpha + (smooth_ax * (1.0 - alpha));
	smooth_ay = ay * alpha + (smooth_ay * (1.0 - alpha));
	smooth_az = az * alpha + (smooth_az * (1.0 - alpha));

	roll = ((atan2(-smooth_ay, smooth_az)*180)/M_PI)/5;
	pitch = ((atan2(smooth_ax, sqrt(smooth_ay*smooth_ay + smooth_az*smooth_az))*180)/M_PI)/5;

}