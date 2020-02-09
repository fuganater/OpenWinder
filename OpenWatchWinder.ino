// Configuration
#define CYCLES 5
#define ROT_R 2
#define ROT_L 2
#define PAUSE_MIN 35

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
  W_IDLE,  // 0
  W_CYCLE, // 1
  W_RIGHT, // 2
  W_LEFT,  // 3
  W_STOP,  // 4
  W_PAUSE  // 5
};

AccelStepper winder(8, 8, 10, 9, 11, false);
JLed pwr_led = JLed(LED_PIN).FadeOn(1000);
AceButton pwr_sw(SW_PIN);

StateType WState = W_IDLE;
int Rotations = CYCLES;
int Start = false;
int Continue = false;
int Stop = false;
int LedOn = true;
int LastMinute = false;
int TargetPos = 0;
long StartTime = 0;

void handleSwEvent(AceButton *, uint8_t, uint8_t);

void setup()
{
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT_PULLUP);

  ButtonConfig *buttonConfig = pwr_sw.getButtonConfig();
  buttonConfig->setEventHandler(handleSwEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
  buttonConfig->setClickDelay(500);

  winder.setMaxSpeed(ROT_SPEED);
  winder.setAcceleration(ROT_ACCEL);
  WState = W_IDLE;

  Serial.println("<< Winder: Ready");
}

void loop()
{
  StateType old_state = WState;

  pwr_sw.check();

  switch (WState)
  {
  case W_IDLE:
    if (Start || Continue)
    {
      Start = false;
      StartTime = millis();
      winder.enableOutputs();
      Rotations = CYCLES;
      WState = W_CYCLE;
    }
    break;

  case W_CYCLE:
    if (LedOn)
    {
      pwr_led.Reset();
      pwr_led.Blink(1000, 200).Forever();
    }

    if (Stop)
    {
      WState = W_STOP;
    }
    else if ((Rotations--) > 0)
    {
      Serial.print("## Cycles until Pause: ");
      Serial.println(Rotations);
      WState = W_RIGHT;
      TargetPos -= (ROT_R * ROT_STEPS);
      winder.moveTo(TargetPos);
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
    else if (Stop)
    {
      WState = W_STOP;
    }
    else
    {
      WState = W_LEFT;
      TargetPos += (ROT_L * ROT_STEPS);
      winder.moveTo(TargetPos);
    }
    break; // case W_RIGHT

  case W_LEFT:
    if (winder.distanceToGo() != 0)
    {
      winder.run();
    }
    else if (Stop)
    {
      WState = W_STOP;
    }
    else
    {
      WState = W_CYCLE;
    }
    break; // case W_LEFT

  case W_STOP:
    Stop = false;
    winder.disableOutputs();
    StartTime = millis();
    if (Continue)
    {
      Serial.println("<< Winder: Stopped, waiting for next cycle");
      WState = W_PAUSE;
      if (LedOn)
      {
        pwr_led.Reset();
        pwr_led.Breathe(5000).Forever();
      }
    }
    else
    {
      Serial.println("<< Winder: Stopped, will not restart");
      if (LedOn)
      {
        pwr_led.Reset();
        pwr_led.On();
      }
      WState = W_IDLE;
    }
    break; // case W_STOP

  case W_PAUSE:
    long temp = millis() - StartTime;
    long delta = 60L * 1000L;

    if (Start)
    {
      WState = W_IDLE;
    }
    // run once, 1 minute before PAUSE_MIN elapses
    else if ((!LastMinute) && (temp > (delta * long(PAUSE_MIN - 1))))
    {
      Serial.println("<< Winder: Restarting in 1 minute");
      if (Continue && LedOn)
      {
        pwr_led.Reset();
        pwr_led.Breathe(1000).Forever();
      }
      LastMinute = true;
    }
    // PAUSE_MIN has elapsed && Continue
    else if (Continue && temp > (delta * long(PAUSE_MIN)))
    {
      LastMinute = false;
      WState = W_IDLE;
    }
    break; // W_PAUSE

  default:
    winder.disableOutputs();
    break;

  } // switch

  if (WState != old_state)
  {
    Serial.print("-- Changing State: ");
    Serial.print(old_state);
    Serial.print(" -> ");
    Serial.println(WState);
  }

  pwr_led.Update();

} // loop

void handleSwEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  switch (eventType)
  {

  case AceButton::kEventClicked:
    switch (WState)
    {
    case W_IDLE:
    case W_PAUSE:
      Serial.println(">> Click: Start Winding");
      Stop = false;
      Start = true;
      Continue = true;
      WState = W_IDLE;
      break;
    default:
      break;
    }
    break;

  case AceButton::kEventLongPressed:
    switch (WState)
    {
    case W_LEFT:
    case W_RIGHT:
    case W_PAUSE:
      Serial.println(">> LongPress: Stop Winding");
      Stop = true;
      Continue = false;
      pwr_led.Reset();
      if (WState == W_PAUSE)
      {
        pwr_led.On();
      }else{
        pwr_led.Blink(100, 500).Forever();
      }
      break;
    default:
      break;
    }
    break;

  case AceButton::kEventDoubleClicked:
    pwr_led.Reset();
    if (LedOn)
    {
      Serial.println(">> DoubleClick: LED Disabled");
      pwr_led.Stop();
      LedOn = false;
    }
    else
    {
      Serial.println(">> DoubleClick: LED Enabled");
      LedOn = true;
      switch (WState)
      {
      case W_IDLE:
        Serial.println("<< LED: On");
        pwr_led.On();
        break;

      case W_PAUSE:
        if (!LastMinute)
        {
          Serial.println("<< LED: Breathe Slow");
          pwr_led.Breathe(5000).Forever();
        }
        else
        {
          Serial.println("<< LED: Breathe Fast");
          pwr_led.Breathe(1000).Forever();
        }
        break;

      case W_LEFT:
      case W_RIGHT:
        Serial.println("<< LED: Blink");
        pwr_led.Blink(1000, 200).Forever();
        break;

      default:
        Serial.println("<< LED: WTF?");
        pwr_led.Blink(50, 50).Forever();
        break;
      }
    }
    break;

  default:
    break;
  }
}
