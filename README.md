# Open Watch Winder

This allows you to control the cycles of a stepper motor with a momentary switch button to wind your automatic watch, using the 3D printed [Gyrocope Watch Winder](https://www.thingiverse.com/thing:3520031).

## Using the Winder
On powerup, the LED will fade on and the winder will be in standby, waiting for a button press.

The button knows three types of inputs:
* **click**, which is input shorter than 500ms
* **long-press**, which is input longer than 500ms
* **double-click**

The winder works in cycles, which means it will do the configured number of cycles (`CYCLES`), with each cycle consisting of a configured number of rotations in each direction (`ROT_R`, `ROT_L`), then wait for the
configured amount of minutes (`PAUSE_MIN`), and eventually restart the cycles. This will be refered to as "_winding_".

### The very short version of a manual:
* click the button to start winding
* long-press the button to stop winding
* double-click to toggle the LED (on/off)

### Switching States (a.k.a the slightly longer version of a manual)
* When clicking the button while the winder is in standby, it will start winding.
* When long-pressing the button while it is winding, it will return to the home position and stop winding.
* When long-pressing the button while it is waiting for the next cycle, it will stop winding.
* When clicking the button while it is waiting for the next cycle, it will start winding again immediately.
* When double-clicking the button in any state, the LED will turn off if it was on, or on (in the appropriate status mode) if it was off.

## Configuration
I have no special knowledge about automatic watches, but i read that these watches require between 650 and 950 turns per day (TPD) to keep going.
To find the right number of rotations, cycles and minutes to pause, we need to do a bit of calculation:

* 1 cycle = 20 turns (5 * 2 turns in each direction)
* 800 TPD / 20 turns per cycle = 40 cycles per day

Alright, so 40 cycles will be required. But how long would it take to get 40 cycles?

* 1 turn = 4.5 seconds
* 20 turns = 90 seconds
* 40 cycles = 3600 seconds (1 hour)

That means 800 TPD would be achieved in 1 hour of non-stop rotation, but that could possibly harm your watch if you leave it on there the whole day. Or so i heard...

So we need to find the appropriate time to pause between cycles, to spread those 800 TPD over the whole 24 hours:

* 1 day = 1440 minutes
* 1440 minutes / 40 cycles = 36 minutes in total, for a cycle including the pause between cycles
* 36 minutes total - 90 seconds cycle = 34.5 minutes pause

Now let's round this up to **35 minutes of pausing** between cycles, as i think a watch that's only "nearly completely wound all the time" is better than a watch that's "nearly broken soon".

To confirm our calculation, let's see how close we got to 800 TPD:

* (35 + 1.5) * 40 cycles = 1460 minutes 
* 1440 / 1460 minutes * 800 TPD = **789 TPD** (or: 0.9863%)

If your watch requires more or fewer TPD, i would suggest adjusting the pause between cycles first:

* for 650 TPD: 
  * 650 / 20 turns = 32.5 cycles
  * 1440 minutes / 32.5 cycles = 44 minutes total
  * 44 - 1.5 = 42.5 -> **43 minutes of pause**
  * 1440 / (43 + 1.5) * 32.5 * 650 = **~647 TPD**
* for 950 TPD:
  * 950 / 20 turns = 47.5 cycles
  * 1440 minutes / 47.5 cycles = ~30 minutes total
  * 30 - 1.5 = 28.5 -> **29 minutes of pause**
  * 1440 / (29 + 1.5) * 47.5 * 950 = **~944 TPD**

Please bear in mind that those are theoretical numbers, and that the actual values will depend on
how long a rotation takes on your build. I take no responsibility for any damage whatsoever! ;-)

## LED Status
* **Solid**: Standby / Manually Stopped
* **Long Blinking** (1s on, 0.2s off): Winding
* **Short Blinking** (0.1 on, 0.5s off): Stopping, returning to home position (when manually stopped)
* **Slow Breathing**: Paused, waiting for next cycles
* **Fast Breathing**: Paused, the next cycle will start in <1 minute

## Wiring Diagram

<a href="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png"><img src="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png" alt="Wiring Diagram"  width="500"/></a>

## Bill of Materials
* [Arduino Nano v3.0](https://amzn.to/2WuV7lG)
* [28BYJ-48 Stepper Motor + ULN2003A Driver](https://amzn.to/2YHja3K)
* [16mm Momentary Switch Button with LED](https://s.click.aliexpress.com/e/_dSWuGXf)
* [Terminal Block to DC Jack Adapter (5.5x2.5)](https://amzn.to/2LdH2Us)
* [5V 2A Power Supply with 5.5x2.5 plug](https://amzn.to/3dCZyCf)
* Bolts and Nuts (**Socket Head Cap**, unless noted otherwise)
  * 3x M3x6
  * 1x M3x8
  * 2x M3x10
  * 3x M3x12
  * 2x M3x8 (**Countersunk Head**)
  * 12x M3 Nuts
* 3 pieces of 4-stranded Wire (to keep things neat)
  * Arduino -> Driver board 
  * Arduino -> Button + LED
  * Terminal Block -> Arduino & Driver board (split strands into 2x 5V/GND)

## ToDo
* Allow stopping immediately, not only after current rotations are done
* Convert to an ESP8266 to make it easily controlable and configurable via a web-interface

## Credits
* Philipp Klimek for his [Gyrocope Watch Winder](https://www.thingiverse.com/thing:3520031)
* [Peter D.](https://www.thingiverse.com/Dilbert0815/about) for the [initial version](https://www.thingiverse.com/thing:2763503/comments/#comment-2067423) of the code
