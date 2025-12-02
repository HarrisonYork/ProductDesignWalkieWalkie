# ProductDesign

Walkie Walkie is a smart device designed to connect hikers in the wilderness. It combines the sturdiness of a hiking stick with the features of a walkie talkie and flashlight, ensuring you can keep in contact with your hiking party despite physical separation. This project was a part of the Fall 2025 Product Design showcase at Duke.

Full project documentation: https://www.harrisonyork.com/productdesign

This project uses the following external libraries:
* NewSoftSerial: https://github.com/HarrisonYork/NewSoftSerial (provides serial over digital pins)
* NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel (RGB LED control)
* Adafruit SSD1306: https://github.com/adafruit/Adafruit_SSD1306 (OLED Display control)
* Adafruit GFX: https://github.com/adafruit/Adafruit-GFX-Library (OLED Display control)

## FRS Frequencies
SA818s module transmits at 1 W at high power. Some FRS frequencies allow only 0.5 W, others up to 2 W. To use the 25 KHz bandwidth offered by the module, transmitting at 1 W, can use the following channels:

|Ch#|   MHz    |
|---|----------|
| 1 | 462.5625 |
| 2 | 462.5875 |
| 3 | 462.6125 |
| 4 | 462.6375 |
| 5 | 462.6625 |
| 6 | 462.6875 |
| 7 | 462.7125 |
| 8 | 467.5625 |

## Programming the SA818s
On serial monitor, make sure “Both NL and CR” selected for line ending

Setting transmit, receive frequencies
```python
AT+DMOSETGROUP=0,462.5625,462.5625,0000,4,0000
```

AT+DMOSETGROUP=GBW, TFV, RFV, Tx_CXCSS, SQ, Rx_CXCSS

GBW = bandwidth (0 → 12.5K, 1 → 25K)

TFV: transmit frequency value (400.0000 - 470.0000)

RFV: receiving frequency value (same range)

- must be multiple of 12.5K or 25K

Tx_CXCSS: Transmit CXCSS value

SQ: squelch (0 - 8)

Rx_CXCSS: Receive CXCSS value

expected response: `+DMOSETGROUP:0`

- `+DMOSETGROUP:1` means error

Set volume:

```python
AT+DMOSETVOLUME=X
```

volume can be 1-8

expected response: `+DMOSETVOLUME:0`

- `+DMOSETVOLUME:1` means error, volume level does not matter for response

Read version:

```python
AT+VERSION
```

expected response: `+VERSION:SA818S_V1.2`
