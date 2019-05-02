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

## LED Status
* **Solid**: Standby / Manually Stopped
* **Slow Blinking** (1s on, 0.2s off): Winding
* **Fast Blinking** (0.1 on, 0.5s off): Stopping, returning to home position (when manually stopped)
* **Slow Breathing**: Paused, waiting for next cycles
* **Fast Breathing**: Paused, the next cycle will start in <1 minute

## Wiring Diagram

<a href="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png"><img src="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png" alt="Wiring Diagram"  width="500"/></a>

## Bill of Materials
* [Arduino Nano v3.0](https://www.ebay.de/itm/252742123829)
* [28BYJ-48 Stepper Motor + ULN2003A Driver](https://www.ebay.de/itm/123500100035)
* [16mm Momentary Switch Button with LED](https://www.ebay.de/itm/331807353292)
* [4 Neodymium Magnets (5mm x 1mm)](https://www.amazon.de/dp/B00TACKN4M/)
* [4mm Brass Tube](https://www.ebay.de/itm/312565968067) (cut into 5 pieces of 9mm each)
* [5 Self-Tapping Screws (2.5mm x 9.5mm)](https://www.ebay.de/itm/253942744597)
* [5 Miniature Ball Bearings 684 (9x4x2.5)](https://www.kugellager-express.de/miniature-deep-groove-ball-bearing-684-w2-5-open-4x9x2-5-mm)
* [Terminal Block to DC Jack Adapter (5.5x2.5)](https://www.ebay.de/itm/201994405254)
* [5V 2A Power Supply with 5.5x2.5 plug](https://www.amazon.de/dp/B01I1LRCXW/)
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

## Credits
* Philipp Klimek for his [Gyrocope Watch Winder](https://www.thingiverse.com/thing:3520031)
* [Peter D.](https://www.thingiverse.com/Dilbert0815/about) for the [initial version](https://www.thingiverse.com/thing:2763503/comments/#comment-2067423) of the code