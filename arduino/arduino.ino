#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include  <string.h>
// #include "my_uart.h"
#define STASSID "TIEN DUC HOANG"
#define STAPSK "12345678"

#define BUFFER_MAX_SIZE 20
#define TEMP_CMD        "!TEMP:"
uint8_t buf_uart[BUFFER_MAX_SIZE];
uint8_t idx = 0;
int done_receive = 0;
void get_data(uint8_t msg) {
  buf_uart[idx++] = msg;
  if(buf_uart[idx - 1] == '#'){
    done_receive = 1;
    idx = 0;
  }
  if (idx > BUFFER_MAX_SIZE - 1) {
    idx = 0;
  }
}



uint8_t compile_cmd(double *cmd_data) {
  if (done_receive) {
    done_receive = 0;
    if(strstr((char*)buf_uart, TEMP_CMD)){
      *cmd_data = atof((char *)(buf_uart + strlen(TEMP_CMD)));
      return 1;
    }
  }
  return 0;
}

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "germanyy0410"
#define AIO_KEY         "aio_ioGC53iJwbKJ9lXqMrcPer1wydyK"
// setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client , AIO_SERVER , AIO_SERVERPORT , AIO_USERNAME , AIO_KEY);
// set publish
Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish (& mqtt , AIO_USERNAME "/feeds/temp");
// set subcribe
Adafruit_MQTT_Subscribe temp_sub = Adafruit_MQTT_Subscribe (&mqtt , AIO_USERNAME "/feeds/temp", MQTT_QOS_1);
int led_counter = 0;
int led_st = 1;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  // test connection to wifi
  digitalWrite(5, 1);
  Serial.begin (115200);
  // connect Wifi
  WiFi.begin(STASSID , STAPSK);
  while (WiFi.status () != WL_CONNECTED) {
    delay (500);
  }
  // subscribe temperature feed
  mqtt.subscribe (&temp_sub);
  //connect to server
  while (mqtt.connect () != 0) {
    mqtt.disconnect ();
    delay (500);
  }
  digitalWrite(5, 0);
}

int state = 0;


void loop() {
  // receive packet
  mqtt.processPackets (1);
  double temp_data = 0;  
  uint8_t success = 0;
  if (Serial.available()) {
    int msg = Serial.read();
    get_data(msg);
  }
  success = compile_cmd(&temp_data);
  if(success == 1) {
    temp_pub.publish(temp_data);
  }
  led_counter++;
  if (led_counter == 100) {
    if (led_st == 1) led_st = 0;
    else led_st = 1;
    digitalWrite(2, led_st);
    led_counter = 0;
  }
  delay(10);
}