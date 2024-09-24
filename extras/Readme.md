

# A nice button for 3D printing

[Also available on Thingiverse](https://www.thingiverse.com/thing:6772678)

A touch button with LED highlight for ESP32, to be used with TouchLed library - the LED positive terminal doubles as touch input; two wires to two GPIO pins (one with Touch).

Minimal soldering required (just two wires to the LED). Also scissors work.

### Artifacts list 

* a CR2032/CR2025/CR2016 "big coin" battery, preferably depleted.
* a rectangular "flat" LED, 5x2x7mm style, bright.
* some (thin) wires to connect it up.
* like 5mm of thermal shrink tube, or electric tape. 
* some translucent plastic (look for exceptionally poor quality packaging, look in trash.)
  * alternatively some transparent plastic, and go to town with somewhat fine but not too fine sandpaper on it
* An ESP32 with touch support, obviously. Maybe a breadboard?
* a piece of paper, or a printer (the normal kind, not 3D), or whiteout, or white filament if your printer can do multi-color.

### Instructions

* Using snips, extract the central 'top' (smaller) part of the battery - bend away the edges, until you can separate the top and bottom.
* Clean the top thoroughly, especially scratch all the lithium from the inside; it oxidizes so fast there will be no electrical contact with it in place.
* Solder one wire to the negative (shorter) lead of the LED, as close to the LED as possible. Snip the lead to make the connection very short. 
* Insulate the connection with the thermal shrink tube, right up to the LED.
* Solder the other wire to the positive lead, again as close as possible to the LED. Don't snip the lead! Instead, bend it almost 180 degrees so it stick a bit upwards.
* Print out the STLs on 3D printer
  * If you have a multi-color printer, paint the inside of the "bottom" white.
  * If you have whiteout or some white dye you can apply to the print, paint the bottom white
  * If you have a normal (paper) printer, print the PDF, cut out the "fan" shape and lay on the bottom. Stay within the lines (rather too small than too big)
* Cut a ring of plastic. Doesn't need to be perfect, there's plenty of "slop". 
   * Glue the the "ring" from PDF printout with some weak glue to plastic, cut out the shape, remove paper, or
  * Use the 'Top" 3D print and a sharpie to draw the shape on plastic, cut the ring within the lines (rather too thin than too broad). 
* Thread the wires from inside the 'bottom' through the small hole in the side wall
* Place the LED in the middle, with its trailing lead pointing upwards, towards the middle.
* Place the battery top on top of that, squishing the lead below so it makes a good springy contact.
* Place the translucent plastic ring over the battery top and inside the groove of the printed part
* Apply the "top" 3D printout, aligning its "cross" with the "cross" of the "bottom"
* The top has small tabs on 4 sides, and the bottom has notches to fit them. Match them.
  Don't squish straight down - bend the top "center-upwards, edges down" fitting the tabs into slots.

FreeCAD project attached but it's not parametric, sorry.
  
When everything is aligned, solid and doesn't wobble, connect "+" wire to any working 'Touch' pin of the ESP32, "-" to any other GPIO pin (Not GND!), and go to the [TouchLed library Github page](https://github.com/bwucke/TouchLed) for instructions on the software side.

### Affiliate links:
* [Bright LEDs, 2x5x7, various colors](https://s.click.aliexpress.com/e/_Dlyuld1)
* [CR2302 battery](https://s.click.aliexpress.com/e/_DD7dCH9), replace in something you have, like your PC, and use the old one. 
* [ESP32 Devkit V1](https://s.click.aliexpress.com/e/_DcQx44L), the most common, generic ESP32 board with touch support.
* [Translucent plastic](https://s.click.aliexpress.com/e/_DBRwEjZ) if you want to go industrial scale. You *do* have some laying around.
* [Double-sided tape](https://s.click.aliexpress.com/e/_DBJIHOj) to affix your button to a surface
* [Connectors with pre-crimped wires](https://s.click.aliexpress.com/e/_DDIrLFl), for extra convenience.