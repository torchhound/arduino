#include <dht.h>

void setup(){
	Serial.begin(115200);
	Serial.println("DHT11 TEST PROGRAM");
}

void loop(){
	int check = DHT.read11(DHT11_PIN);
	if(check == DHTLIB_ERROR_CHECKSUM || DHTLIB_ERROR_TIMEOUT || DHTLIB_ERROR_CONNECT || DHTLIB_ERROR_ACK_L || DHTLIB_ERROR_ACK_H){
		Serial.println(check);
	}
	Serial.print(DHT.humidity, 1);
	Serial.println(DHT.temperature, 1);
	delay(2000); //2 seconds between readings
}