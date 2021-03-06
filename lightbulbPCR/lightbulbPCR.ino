//code originally from: https://tequals0.wordpress.com/2014/04/05/lightbulb-pcr-build-documentation/

#define CYCLE_REPEATS 30
#define tol .5
#define HOT 5
#define FAN 4

#include <dht.h>

void setup() {
	Serial.begin(9600);  //start serial port to computer
	pinMode(HOT,OUTPUT); //set hot and fan pins as outputs.  Default is low.
	pinMode(FAN,OUTPUT);
}

//get raw value from sensor, convert it to temperature and return it as a float
float getTemp(){
	float temp;
	int check = 0;
	bool continue = true;
	do{ 
		check = DHT.read11(DHT11_PIN);
		if(check == DHTLIB_ERROR_CHECKSUM || DHTLIB_ERROR_TIMEOUT || DHTLIB_ERROR_CONNECT || DHTLIB_ERROR_ACK_L || DHTLIB_ERROR_ACK_H){
			continue = false;
		}
		Serial.println(DHT.temperature, 1);
		temp = DHT.temperature;
		delay(2000);
		return temp;
	}while(continue == true);
}

//this is the function that decides what the machine will do- heat, cool, or idle.  the #define tol .5 at the top can be changed to an arbitrary tolerance.
//making the tolerance too small will result in your machine flipping between heating and cooling really fast, making it too big will result in more ringing
int tempTask(float target,float temp){
	if(temp<(target-tol)){
		digitalWrite(FAN,LOW);
		digitalWrite(HOT,HIGH);
	}else if(temp>(target+tol)){
		digitalWrite(HOT,LOW);
		digitalWrite(FAN,HIGH);
	}else{
	digitalWrite(HOT,LOW);
	digitalWrite(FAN,LOW);
	}

	return temp>(target-tol-tol) && temp<(target+tol+tol); //return 1 if temp is within 2 tol- approaching switch over pt. from ramping to waiting for the timer
}

//this function ramps (heats or cools) to the desired temperature, then waits a certain amount of time while holding that temperature
void singleCycle(int seconds, float target){
	float temp;
	temp = getTemp();
	Serial.print(“BEGIN\n”); //prints this at the beginning of every cycle, useful for debugging
	Serial.println(temp,DEC);
	while (!tempTask(target,temp)){ //while temperature is not near the target, keep ramping and checking the temperature.  delay makes each cycle take about 1/8 of a second
		temp = getTemp();
		Serial.println(temp,DEC);
		tempTask(target,temp);
		delay(125);
	}
	for(int i=0; i<seconds*8; i++){ //seconds*8 since this loop takes about 1/8 second.  holds at the desired temp for the desired number of seconds
		temp = getTemp();
		Serial.println(temp,DEC);
		tempTask(target,temp);
		delay(125);
	}
}

void loop(){
	float temp, target;
	target=25; //arb target temp, just so the thing doesn't go crazy it is set to about room temperature

	delay(1000); //necessary?

	int time=0;

	delay(1000);//necessary?

	//prints start at the beginning of the cycle
	Serial.println(“START”);

	//add stuff here that you want to do before the cycle, eg hot start, initial denaturation

	singleCycle(30,98);

	//this for loop is what gets repeated over and over again, change #define CYCLE_REPEATS 30 to change it
	for (int i=0; i<CYCLE_REPEATS;i++){
		//change what is in here to change what the denature-anneal-extend cycle is.
		singleCycle(10,98);//denature
		singleCycle(30,71);//anneal
		singleCycle(30,72);//extend
	}
	//Final extension etc goes here.  Note repeat to get correct time.
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);
	singleCycle(30,72);

	//this block turns off the fan and bulb and waits for the device to be reset
	digitalWrite(HOT,LOW);
	digitalWrite(FAN,LOW);
	while(1){
		Serial.println(“IDLE”);
	}
}
