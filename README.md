# ProductDesign
Product Design repository

## FRS Frequencies
SA818s module transmits at 1 W at high power. Some FRS frequencies allow only 0.5 W, others up to 2 W. To use the 25 KHz bandwidth offered by the module, transmitting at 1 W, can use the following 8 channels:
|Ch#|   MHz    |
|---|----------|
| 1 | 462.5500 |
| 2 | 462.5750 |
| 3 | 462.6000 |
| 4 | 462.6250 |
| 5 | 462.6500 |
| 6 | 462.6750 |
| 7 | 462.7000 |
| 8 | 462.7250 |

## SA818s Debug Circuit
| Arduino                     | SA818S  | Notes                                 |
| --------------------------- | ------- | ------------------------------------- |
| **GND**                     | **GND** | Common ground                         |
| **4 V**                     | **VCC** | Must supply 4 V @ ≥1.5 A              |
| **D2**                      | **PTT** | HIGH = RX mode, LOW = TX              |
| **D3**                      | **PD**  | HIGH = on, LOW = power down           |
| **D10 (Ard RX, radio TX)**  | **TXD** | Module → Arduino                      |
| **D11 (TX)**                | **RXD** | Arduino → Module (use V divider)      |

Refer to the SA818s datasheet for the rest of the pinout.

## Programming the SA818s
On serial monitor, make sure “Both NL and CR” selected for line ending

Setting transmit, receive frequencies
```python
AT+DMOSETGROUP=0,462.5500,462.5500,0000,4,0000
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
