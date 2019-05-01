// Configuration
#define CYCLES 1
#define ROT_R 2
#define ROT_L 2
#define PAUSE_MIN 2

// Pins
#define LED_PIN 5
#define SW_PIN 4

// Motor
#define ROT_SPEED 1000
#define ROT_ACCEL 800
#define ROT_PAUSE 4000
#define ROT_STEPS 4096

#include <jled.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <AceButton.h>

using namespace ace_button;

enum StateType
{
  W_INIT,  // 0
  W_SETUP, // 1
  W_IDLE,  // 2
  W_CYCLE, // 3
  W_LEFT,  // 4
  W_RIGHT, // 5
  W_STOP,  // 6
  W_PAUSE  // 7
};

AccelStepper winder(8, 8, 10, 9, 11, false);
JLed pwr_led = JLed(LED_PIN).FadeOn(1000);
AceButton pwr_sw(SW_PIN);

int TargetPos = 0;
long StartTime = 0;

int Rotations = CYCLES;
StateType WState = W_INIT;
int Restart = false;
int Continue = false;
int StopWind = false;
int WaitMin = PAUSE_MIN;

void handleSwEvent(AceButton *, uint8_t, uint8_t);

void setup()
{
  ButtonConfig *buttonConfig = pwr_sw.getButtonConfig();

  WState = W_SETUP;
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT_PULLUP);

  buttonConfig->setEventHandler(handleSwEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  buttonConfig->setClickDelay(500);

  winder.setMaxSpeed(ROT_SPEED);
  winder.setAcceleration(ROT_ACCEL);
  WState = W_IDLE;

  Serial.println(">>Winder Ready<<");
}

void loop()
{
  StateType old_state = WState;

  pwr_sw.check();
  pwr_led.Update();

  switch (WState)
  {
  case W_IDLE:
    if (Restart || Continue)
    {
      WState = W_CYCLE;
      Restart = false;
    }
    StartTime = millis();
    winder.enableOutputs();
    Rotations = CYCLES;
    break;

  case W_CYCLE:
    pwr_led.Blink(1000, 500).Forever();

    if (StopWind)
    {
      WState = W_STOP;
    }
    else if ((Rotations--) > 0)
    {
      Serial.print("Counter : ");
      Serial.println(Rotations);

#if (ROT_R > 0)
      WState = W_RIGHT;
      TargetPos -= (ROT_R * ROT_STEPS); //-4096;  // Une rotation complète avec 2048 pas (1 tour environ 4.5sec)
      winder.moveTo(TargetPos);
#elif (ROT_L > 0)
      WState = W_LEFT;
      TargetPos += (ROT_R * ROT_STEPS); //-4096;  // Une rotation complète avec 2048 pas (1 tour environ 4.5sec)
      winder.moveTo(TargetPos);
#endif
    }
    else
    {
      WState = W_STOP;
    }
    break; // case W_CYCLE

  case W_RIGHT:
    if (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    else if (StopWind)
    {
      WState = W_STOP;
    }
#if (ROT_L > 0)
    else
    {
      WState = W_LEFT;
      TargetPos += (ROT_R * ROT_STEPS); //-4096;  // Une rotation complète avec 2048 pas (1 tour environ 4.5sec)
      winder.moveTo(TargetPos);
    }
#else
    else
    {
      WState = W_CYCLE;
    }
#endif
    break; // case W_RIGHT

  case W_LEFT:
    if (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    else if (StopWind)
    {
      WState = W_STOP;
    }
    else
    {
      WState = W_CYCLE;
    }
    break; // case W_LEFT

  case W_STOP:
    StopWind = false;
    winder.disableOutputs();
    StartTime = millis();
    WState = W_PAUSE;
    if (Continue)
    {
      pwr_led.Breathe(5000).Forever();
      Serial.println(">>Winder Stopped, will continue<<");
    }
    else
    {
      Serial.println(">>Winder Stopped, will not continue<<");
    }
    break; // case W_STOP

  case W_PAUSE:
  {
    long temp = millis() - StartTime;
    long delta = 60L * 1000L;
    static int last_min = false;

    if (Restart)
    {
      WState = W_IDLE;
    }
    else if (Continue && temp > (delta * long(PAUSE_MIN))) // ms per minute
    {
      WState = W_IDLE;
      last_min = false;
      Serial.print("d_time : ");
      Serial.print(temp);
      Serial.print(" > ");
      Serial.println(delta);
    }
    else if ((!last_min) && (temp > (delta * long(PAUSE_MIN - 1)))) // ms per minute
    {
      Serial.println("Last Minute ...");
      if (Continue)
      {
        pwr_led.Breathe(1000).Forever();
      }
      last_min = true;
    }
  }
  break; // W_PAUSE

  case W_INIT:
  case W_SETUP:
  default:
    winder.disableOutputs();
    break;

  } // switch

  if (WState != old_state)
  {
    Serial.print("STATE : ");
    Serial.print(old_state);
    Serial.print(" >> ");
    Serial.println(WState);
  }

} // loop

void handleSwEvent(AceButton *button, uint8_t eventType,
                   uint8_t buttonState)
{
  switch (eventType)
  {
  case AceButton::kEventLongPressed:
    Serial.println("SW LONG");
    pwr_led.Reset();
    switch (WState)
    {
    case W_LEFT:
    case W_RIGHT:
    case W_PAUSE:
      Serial.println(">>Stop Winding<<");
      StopWind = true;
      Continue = false;
      pwr_led.On();
      break;
    default:
      break;
    }
    break;

  case AceButton::kEventClicked:
    Serial.println("SW CLICK");
    pwr_led.Reset();
    Continue = true;
    StopWind = false;
    switch (WState)
    {
    case W_IDLE:
      Serial.println(">>Start Winding<<");
      WState = W_CYCLE;
      break;
    case W_PAUSE:
      Serial.println(">>Restart Winding<<");
      Restart = true;
      break;
    default:
      break;
    }
    break;

  default:
    //Serial.print("SW: ");
    //Serial.print(eventType);
    //Serial.print(", ");
    //Serial.println(buttonState);
    break;
  }
}
