#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(6, 7); // RX, TX

const long SENSE_DELAY = 20;

void checkCapacitance();

CapacitiveSensor sensor = CapacitiveSensor(8, 4);

long triggerOnTimer  = 0;
long onTimer         = 0;
long triggerOffTimer = 0;
long offTimer        = 0;
double brightness    = 1.0;

void setup()
{
  Serial.begin(115200);
  BTSerial.begin(115200);

  // Disable the automatic re-calibration feature of the
  // capacitive sensor library
//  sensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
}


void loop()
{
  checkCapacitance();
}

void checkCapacitance()
{
  long capacitance = sensor.capacitiveSensorRaw(30);
//  Serial.println(capacitance);

  if (capacitance > 29000)
  {
    // Not on yet
    if (onTimer == 0)
    {
      // Not triggered yet
      if (triggerOnTimer == 0)
      {
        triggerOnTimer = millis();
      }
      // Triggered over 10ms
      else if (millis() - triggerOnTimer > SENSE_DELAY)
      {
        onTimer         = millis();
        offTimer        = 0;
        triggerOffTimer = 0;
        BTSerial.print("<1>");
//        Serial.println("On");
      }
    }
  }
  else
  {
    // Not off yet
    if (offTimer == 0)
    {
      // Not triggered yet
      if (triggerOffTimer == 0)
      {
        triggerOffTimer = millis();
      }
      // Triggered over 10ms
      else if (millis() - triggerOffTimer > SENSE_DELAY)
      {
        offTimer       = millis();
        onTimer        = 0;
        triggerOnTimer = 0;
        BTSerial.print("<0>");
      }
    }
  }

  if (capacitance >= 28000)
  {
    if (brightness != 1.0)
    {
      brightness = 1.0;
      BTSerial.print("<100>");
    }
  }
  else if (capacitance >= 26000)
  {
    if (brightness != 0.75)
    {
      brightness = 0.75;
      BTSerial.print("<75>");
    }
  }
  else if (capacitance >= 24500)
  {
    if (brightness != 0.5)
    {
      brightness = 0.5;
      BTSerial.print("<50>");
    }
  }
  else
  {
    if (brightness != 0.25)
    {
      brightness = 0.25;
      BTSerial.print("<25>");
    }
  }
}
