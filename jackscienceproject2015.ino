#include "SparkFun_Photon_Weather_Shield_Library/SparkFun_Photon_Weather_Shield_Library.h"

// Code to read power data from wind turbine & solar panel, along with weather data from sparkfun weather
// station and post to Azure for data logging.

/* TODO
1. stub out sensor polling framework in loop() - DONE
2. stub out methods for solar panel power measurement, turbine power measurement and weather monitoring - DONE
3. stub out methods to post data to Azure - DONE
4. finish weather sampling methods
5. finish power sampling methods
6. finish Azure posting method
7. extra credit - write code for wind speed/direction & rain sensors
*/


// Weather weather;

int solar = A1;
int wind = A0;

double vSolar = 0.0;
double iSolar = 0.0;
double pSolar = 0.0;
double vWind = 0.0;
double iWind = 0.0;
double pWind = 0.0;
//float humidity = 0;
//float tempf = 0;
//float pascals = 0;
//float baroTemp = 0;


int count = 0; // counter in case we need to post less frequently than we sample.

char Org[] = "GORDON_BOY_WORKS";
char Disp[] = "JTG_SCIENCE_PROJECT_2015";
char Locn[] = "DENVER";

void setup() {

    Particle.variable("vSolar", &vSolar, DOUBLE);
    Particle.variable("iSolar", &iSolar, DOUBLE);
    Particle.variable("pSolar", &pSolar, DOUBLE);
    Particle.variable("vWind", &vWind, DOUBLE);
    Particle.variable("iWind", &iWind, DOUBLE);
    Particle.variable("pWind", &pWind, DOUBLE);

    pinMode(solar, INPUT);
    pinMode(wind, INPUT);
    Serial.begin(9600);
    Serial.println("Jack's Science Project 2015");

//    weather.begin();

       /*You can only receive acurate barrometric readings or acurate altitiude
    readings at a given time, not both at the same time. The following two lines
    tell the sensor what mode to use. You could easily write a function that
    takes a reading in one made and then switches to the other mode to grab that
    reading, resulting in data that contains both acurate altitude and barrometric
    readings. For this example, we will only be using the barometer mode. Be sure
    to only uncomment one line at a time. */
//    weather.setModeBarometer();//Set to Barometer Mode
    //baro.setModeAltimeter();//Set to altimeter Mode

    //These are additional MPL3115A2 functions the MUST be called for the sensor to work.
//    weather.setOversampleRate(7);

    //Set Oversample rate
    //Call with a rate from 0 to 7. See page 33 for table of ratios.
    //Sets the over sample rate. Datasheet calls for 128 but you can set it
    //from 1 to 128 samples. The higher the oversample rate the greater
    //the time between data samples.


//    weather.enableEventFlags();
    //Necessary register calls to enble temp, baro and alt
}

void loop() {

    // might need to implement a while loop that will allow us to sample power readings frequently
    // weather readings and azure posts less frequently - let's see if it's a problem first

    //getWeather();
    int solar_pin = 0;
    double solar_voltage = 0.0;
    int wind_pin = 0;
    double wind_voltage = 0.0;

    solar_pin = analogRead(solar);
    wind_pin = analogRead(wind);

    solar_voltage = (solar_pin * 3.3) / 4095;
    wind_voltage = (wind_pin * 3.3) / 4095;
    solar_voltage /= 0.32; //this accounts for the resistor values in the voltage divider
    wind_voltage /= 0.32;

    vSolar = solar_voltage;
    vWind = wind_voltage;

    if (vSolar > 0) {
        iSolar = vSolar / 690;
    } else {
        iSolar = 0;
    }

    pSolar = vSolar * iSolar;

    if (vWind > 0) {
        iWind = vWind / 690;
    } else {
        iWind = 0;
    }
    pWind = vWind * iWind;

    postData();

    delay(5000);
}

void postData() {
    // post weather & power readings to Azure event hub
    char payload[255];
    snprintf(payload, sizeof(payload), "{ \"vs\": %f,\"is\": %f,\"ps\": %f,\"vw\": %f,\"iw\": %f,\"pw\": %f,\"org\":\"%s\",\"dn\":\"%s\" }", vSolar, iSolar, pSolar, vWind, iWind, pWind, Org, Disp);
    Serial.println(payload);
    Particle.publish("Jack2015ScienceProject", payload);
}

void getWeather() {
    // store weather data
    // do nothing for now
}
