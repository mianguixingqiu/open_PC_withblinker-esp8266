#define BLINKER_WIFI
#define BLINKER_DUEROS_OUTLET

#include <Blinker.h>

char auth[] = "123456123456"; //你的blinker密钥
char ssid[] = "123456123456";  //你的wifi名称
char pswd[] = "12345678";  //你的wifi密码

bool oState = true;
const int OUT_PIN = 3;//RXD控制继电器，可以避免上电乱动的问题

#define BUTTON_1 "OUTButton"
BlinkerButton Button1(BUTTON_1);

void duerPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    digitalWrite(OUT_PIN, LOW);

    BlinkerDuerOS.powerState("on");
    BlinkerDuerOS.print();

    oState = true;
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(OUT_PIN, HIGH);

    BlinkerDuerOS.powerState("off");
    BlinkerDuerOS.print();

    oState = false;
  }
}

void duerQuery(int32_t queryCode)
{
  BLINKER_LOG("DuerOS Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_TIME_NUMBER :
      BLINKER_LOG("DuerOS Query time");
      BlinkerDuerOS.time(millis());
      BlinkerDuerOS.print();
      break;
    default :
      BlinkerDuerOS.time(millis());
      BlinkerDuerOS.print();
      break;
  }
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();
  Blinker.print(BlinkerTime);
  Blinker.print("millis", BlinkerTime);
}

void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);
  BlinkerDuerOS.attachPowerState(duerPowerState);
  BlinkerDuerOS.attachQuery(duerQuery);
}

void loop()
{
  Blinker.run();
}
void button1_callback(const String & state)
{
  if (state == BLINKER_CMD_BUTTON_TAP)
  {
    if (oState) {
      digitalWrite(OUT_PIN, HIGH);//拉高电平，通电
      oState = false;
      delay(100);//连接0.1秒
      digitalWrite(OUT_PIN, LOW);//拉低电平，断开
      oState = true;
    }
    else {

    }
    Button1.print();
  }
}
