

# 📊 MAX6675 Temperature Logger 📊

This project demonstrates how to interface with the MAX6675 thermocouple sensor and log temperature data to a file via serial communication.

## 📋 Table of Contents
- [Hardware Connections](#hardware-connections)
- [Software Overview](#software-overview)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## 🔌 Hardware Connections

| **Component** | **Connection** |
|---------------|----------------|
| MAX6675 GND   | Arduino GND    |
| MAX6675 VCC   | Arduino 5V     |
| MAX6675 SO    | Arduino Pin 7  |
| MAX6675 CS    | Arduino Pin 6  |
| MAX6675 SCK   | Arduino Pin 5  |

## 🖥️ Software Overview

### Arduino Code

```cpp
#include "MAX6675.h"

#define SO   7   // Set So in pin 7
#define CS   6   // Set CS in pin 6
#define SCK  5   // Set SCK in pin 5
#define K   -4.5 // Calibration Constant

MAX6675 thermoCouple(CS, SO, SCK);

void setup() {
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.print("MAX6675_LIB_VERSION: ");
  Serial.println(MAX6675_LIB_VERSION);
  Serial.println();
  delay(250);

  SPI.begin();
  thermoCouple.begin();
  thermoCouple.setSPIspeed(4000000);
}

void loop() {
  delay(100);
  float temp = thermoCouple.getTemperature();
  Serial.print("temp: ");
  Serial.print(temp + K);
  Serial.print("\n");
  delay(1000);
}
```

### C Code for Serial Data Logging

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

void getCurrentDateTime(char *buffer, size_t size);
void writeDataToFile(const char *filename, const char *data);

int main() {
    // Serial port setup code...
}
```

## 📥 Installation

1. **Arduino IDE**: Download and install the Arduino IDE from [the official site](https://www.arduino.cc/en/software).
2. **MAX6675 Library**: Install the MAX6675 library via the Library Manager in the Arduino IDE.
3. **Set Up Serial Communication**:
   - Ensure you have the correct COM port configured in the C code for data logging.

## 🚀 Usage

- **Upload the Arduino code** to your Arduino board.
- **Run the C program** to start listening for data from the Arduino.
- The temperature readings will be printed to the console and logged into a `.txt` file with timestamps.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
