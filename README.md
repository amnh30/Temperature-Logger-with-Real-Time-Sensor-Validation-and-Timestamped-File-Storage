Here’s the complete README content in a single code block for easy copying:

```markdown
# 🌡️ MAX6675 Temperature Logger 🌡️

This project utilizes the MAX6675 thermocouple temperature sensor to read temperature data and log it into a file. The code is split into two main parts: one for interfacing with the MAX6675 sensor using an Arduino and the other for logging the received temperature data to a file on a Windows system.

---

## 📦 Contents

1. **MAX6675 Arduino Code** 
2. **Windows Serial Data Logger Code**

---

## ⚙️ MAX6675 Arduino Code

This code reads the temperature from the MAX6675 sensor and sends it to the serial port. 

### 🔌 Wiring Diagram
```plaintext
GND --> GND
VCC --> 5V
SO  --> 7
CS  --> 6
SCK --> 5
```

### 📋 Code
```cpp
#include "MAX6675.h"

#define SO   7   // Set SO on pin 7
#define CS   6   // Set CS on pin 6
#define SCK  5   // Set SCK on pin 5
#define K   -4.5 // Calibration Constant

MAX6675 thermoCouple(CS, SO, SCK);

uint32_t start, stop;

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
  int status = thermoCouple.read();
  float temp = thermoCouple.getTemperature();

  Serial.print("temp: ");
  Serial.print(temp + K);
  Serial.print("\n");
  
  delay(1000);
}
```
---

## 🖥️ Windows Serial Data Logger Code

This C program listens for incoming temperature data via a specified COM port and logs it into a timestamped text file.

### 📋 Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

void getCurrentDateTime(char *buffer, size_t size) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, size, "%Y-%m-%d_%H-%M-%S", timeinfo);
}

void writeDataToFile(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char timeBuffer[20];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", timeinfo);

    fprintf(file, "%s - %s\n", timeBuffer, data);
    fclose(file);
}

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    char *comPortName = "\\\\.\\COM3"; // Replace with your COM port number
    char buffer[256];
    DWORD bytesRead;
    char fileName[50];

    getCurrentDateTime(fileName, sizeof(fileName));
    strcat(fileName, ".txt");

    hSerial = CreateFile(
        comPortName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        perror("Error opening serial port");
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        perror("Error getting state");
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        perror("Error setting serial port state");
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        perror("Error setting timeouts");
        return 1;
    }

    printf("Listening for data on %s...\n", comPortName);

    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                printf("Received %s\n", buffer);
                writeDataToFile(fileName, buffer);
            }
        } else {
            perror("Error reading from serial port");
            break;
        }
    }

    CloseHandle(hSerial);
    return 0;
}
```

---

## 📥 Installation & Usage

1. Clone this repository.
   ```bash
   git clone https://github.com/yourusername/MAX6675-Temperature-Logger.git
   ```

2. Connect the MAX6675 to your Arduino following the wiring diagram.

3. Upload the Arduino code to your board.

4. Run the Windows code to start logging the temperature data from the COM port.

5. Check the generated `.txt` file for logged data!

---

## 🎨 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Feel free to submit issues or pull requests if you have suggestions or improvements!

---

## 🌟 Acknowledgments

- Special thanks to the creators of the MAX6675 library.
- Inspiration from various Arduino projects!
```

You can simply copy and paste this code block into your README.md file on GitHub!
