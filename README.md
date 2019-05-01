# Open Watch Winder

This allows you to control the cycles of a stepper motor with a momentary switch button to wind your automatic watch, using the 3D printed [Gyrocope Watch Winder](https://www.thingiverse.com/thing:3520031).

## Using the Winder
On powerup, the LED will fade on and the winder will be in standby, waiting for a button press.

The button knows two types of inputs:
* **press**, which is input shorter than 500ms
* **long-press**, which is input longer than 500ms

The winder works in cycles, which means it will do the configured number of cycles (`CYCLES`), with each cycle consisting of a configured number of rotations in each direction (`ROT_R`, `ROT_L`), then wait for the
configured amount of minutes (`PAUSE_MIN`), and eventually restart the cycles. This will be refered to as "_winding_".

### The very short version of a manual:
* press the button to start winding
* long-press the button to stop winding

### Switching Modes (a.k.a the slightly longer version of a manual)
* When pressing the button while the winder is in standby, it will start winding.
* When long-pressing the button while it is winding, it will return to the home position and stop winding.
* When long-pressing the button while it is waiting for the next cycle, it will stop winding.
* When pressing the buttong while it is waiting for the next cycle, it will start winding.

## LED Status
* **Solid**: Standby
* **Blinking** (1s on, 0.5s off): Winding
* **Slow Breathing**: Waiting for next cycles
* **Fast Breathing**: Next cycle will start in <1 minute

## Wiring Diagram

<a href="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png"><img src="https://git.faked.org/jan/openwatchwinder/raw/master/wiring.png" alt="Wiring Diagram"  width="500"/></a>

## Credits
* Philipp Klimek for his [Gyrocope Watch Winder](https://www.thingiverse.com/thing:3520031)
* [Peter D.](https://www.thingiverse.com/Dilbert0815/about) for the [initial version](https://www.thingiverse.com/thing:2763503/comments/#comment-2067423) of the code