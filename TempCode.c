#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Function to get the current date and time as a string
void getCurrentDateTime(char *buffer, size_t size) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, size, "%Y-%m-%d_%H-%M-%S", timeinfo);
}

// Function to write data to a file with the current time
void writeDataToFile(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Get the current time for each line entry
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

    // Get the current date and time for the file name
    getCurrentDateTime(fileName, sizeof(fileName));
    strcat(fileName, ".txt");

    // Open the serial port
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

    // Set serial port parameters
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

    // Set timeouts
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
        // Read data from the serial port
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                printf("Received %s\n", buffer);

                // Write the data to the file
                writeDataToFile(fileName, buffer);
            }
        } else {
            perror("Error reading from serial port");
            break;
        }
    }

    // Close the serial port
    CloseHandle(hSerial);
    return 0;
}
