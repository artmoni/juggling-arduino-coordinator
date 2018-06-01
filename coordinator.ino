#include <SoftwareSerial.h>
#include "XBee.h"

SoftwareSerial mySerial(10, 11); // RX, TX

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	Serial.println("Juggling Controller: Starting");

	// set the data rate for the SoftwareSerial port
	mySerial.begin(9600);
	xbee.setSerial(mySerial);

	Serial.println("Juggling Controller: Started");
}

void loop() { // run over and over
	xbee.readPacket();

	if (xbee.getResponse().isAvailable()) {
		// got something

		if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
			// got a zb rx packet

			// now fill our zb rx class
			xbee.getResponse().getZBRxResponse(rx);

//			Serial.println("Got an rx packet!");

			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
				// the sender got an ACK
//				Serial.println("packet acknowledged");
			} else {
//				Serial.println("packet not acknowledged");
			}

//			Serial.print("checksum is ");
//			Serial.println(rx.getChecksum(), HEX);
//
//			Serial.print("packet length is ");
//			Serial.println(rx.getPacketLength(), DEC);

			Serial.print(rx.getRemoteAddress16());
			Serial.print("$");
			Serial.print((char*)rx.getData());
			Serial.println();

			for (int i = 0; i < rx.getDataLength(); i++) {
//				Serial.print("payload [");
//				Serial.print(i, DEC);
//				Serial.print("] is ");
//				Serial.println(rx.getData()[i], HEX);
			}

			for (int i = 0; i < xbee.getResponse().getFrameDataLength(); i++) {
//				Serial.print("frame data [");
//				Serial.print(i, DEC);
//				Serial.print("] is ");
//				Serial.println(xbee.getResponse().getFrameData()[i], HEX);
			}
		}
	} else if (xbee.getResponse().isError()) {
		Serial.print("error code:");
		Serial.println(xbee.getResponse().getErrorCode());
	}

	if (Serial.available()) {
		mySerial.write(Serial.read());
	}
}
