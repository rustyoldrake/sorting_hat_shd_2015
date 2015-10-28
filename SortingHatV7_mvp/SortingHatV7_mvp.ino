#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

void setup() {
  sort_setup();
  blinky(5);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  digitalWrite(13, LOW);
  server.listenOnLocalhost();
  server.begin();
}


void loop() {
  YunClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }
  sort_loop(); //SortingHat Loop!
  delay(10); 
}


void process(YunClient client) {
  String command = client.readStringUntil('/');
  client.print("Command Sent: ");
  client.print(command);
  client.print("\r");
if (command == "digital") {
    digitalCommand(client);
  }
  if (command == "analog") {
    analogCommand(client);
  }
  if (command == "mode") {
    modeCommand(client);
  }
  
  if (command == "gryffindor") {
     client.print("gryffindor");
     client.print("\r");
     digitalWrite(13, LOW);
     gryffindor();
     blinky(1);
  }
  if (command == "hufflepuff") {
     client.print("hufflepuff");
     client.print("\r");
     digitalWrite(13, LOW);
     hufflepuff();
     blinky(2);
  }
  if (command == "ravenclaw") {
     client.print("ravenclaw");
     client.print("\r");
     digitalWrite(13, LOW);
     ravenclaw();
     blinky(3);
     
  }
  if (command == "slytherin") {
     client.print("slytherin");
     client.print("\r");
     digitalWrite(13, LOW);
     slytherin();
     blinky(4);
  }
  if (command == "standbye") {
     client.print("standbye");
     client.print("\r");
     digitalWrite(13, LOW);
     blinky(10);
  }
  if (command == "thinking") {
     client.print("thinking");
     client.print("\r");
     digitalWrite(13, LOW);
     thinking();
     blinky(2);
  }
   
}

void blinky(int times){
  for(int i = 0; i < times; i++){
 digitalWrite(13, HIGH);
 delay(250);
 digitalWrite(13, LOW);
 delay(250);
  }

}

void digitalCommand(YunClient client) {
  int pin, value;
  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } 
  else {
    value = digitalRead(pin);
  }
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);

  String key = "D";
  key += pin;
  Bridge.put(key, String(value));
}

void analogCommand(YunClient client) {
  int pin, value;

  pin = client.parseInt();

  if (client.read() == '/') {
    value = client.parseInt();
    analogWrite(pin, value);

    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" set to analog "));
    client.println(value);

    String key = "D";
    key += pin;
    Bridge.put(key, String(value));
  }
  else {
    value = analogRead(pin);

    client.print(F("Pin A"));
    client.print(pin);
    client.print(F(" reads analog "));
    client.println(value);

    String key = "A";
    key += pin;
    Bridge.put(key, String(value));
  }
}

void modeCommand(YunClient client) {
  int pin;
  pin = client.parseInt();
  if (client.read() != '/') {
    client.println(F("error"));
    return;
  }

  String mode = client.readStringUntil('\r');

  if (mode == "input") {
    pinMode(pin, INPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as INPUT!"));
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    // Send feedback to client
    client.print(F("Pin D"));
    client.print(pin);
    client.print(F(" configured as OUTPUT!"));
    return;
  }

  client.print(F("error: invalid mode "));
  client.print(mode);
}
