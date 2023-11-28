#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdbool.h>

const char *DEVICE_PATH = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
const char *OUTPUT_FILE = "ex1.txt";

int openDeviceFile() {
    // Function to open the input device file and handle errors
    int fd = open(DEVICE_PATH, O_RDONLY);

    if (fd == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    return fd;
}

int fd;
bool flag_E = false;
bool flag_X = false;

bool flag_C = false;
bool flag_G = false;
bool flag_A = false;
bool flag_P = false;

void printEvent(struct input_event *event) {
    const char *eventType;

    switch (event->type) {
        case EV_KEY:
            if (event->value == 0) {
                eventType = "RELEASED";

                // Reset flags when corresponding keys are released
                if (event->code == KEY_E) {
                    flag_E = false;
                }
                if (event->code == KEY_C) {
                    flag_C = false;
                }
                if (event->code == KEY_A) {
                    flag_A = false;
                }
                if (event->code == KEY_P) {
                    flag_P = false;
                }
                if (event->code == KEY_X){
                    flag_X = false;
                }
                if (event->code == KEY_G){
                    flag_G = false;
                }
            
            } else if (event->value == 1) {
                eventType = "PRESSED";

                // Set flags when corresponding keys are pressed
                if (event->code == KEY_E) {
                    flag_E = true;
                }
                if (event->code == KEY_C) {
                    flag_C = true;
                }
                if (event->code == KEY_A) {
                    flag_A = true;
                }
                if (event->code == KEY_P) {
                    flag_P = true;
                }
                if (event->code == KEY_X){
                    flag_X = true;
                }
                if (event->code == KEY_G){
                    flag_G = true;
                }
            } else if (event->value == 2) {
                eventType = "REPEATED";

                // Set flags when corresponding keys are repeated
                if (event->code == KEY_E) {
                    flag_E = true;
                }
                if (event->code == KEY_P) {
                    flag_P = true;
                }
                if (event->code == KEY_C) {
                    flag_C = true;
                }
                if (event->code == KEY_A) {
                    flag_A = true;
                }
                if (event->code == KEY_X){
                    flag_X = true;
                }
                if (event->code == KEY_G){
                    flag_G = true;
                }

            } else {
                return;
            }

            // Open the output file in append mode
            FILE *outputFile = fopen(OUTPUT_FILE, "a");

            if (outputFile == NULL) {
                perror("Error opening output file");
                close(fd);
                exit(EXIT_FAILURE);
            }

            // Write the event to the output file
            fprintf(outputFile, "%s 0x%04X (%d)\n", eventType, event->code, event->code);

            // Check for the termination condition (E+X)
            if (flag_E == true && flag_X == true) {
                fclose(outputFile);
                exit(EXIT_FAILURE);
            }


            // Additional event handling
            if (flag_P && flag_E) {
                fprintf(outputFile, "I passed the Exam!\n");
                flag_P = false;
                flag_E = false;
            }

            if (flag_A && flag_C && flag_P){
                fprintf(outputFile, "“Get some cappuccino!”\n");
                flag_A = false;
                flag_C = false;
                flag_P = false;
            }

            if (flag_A && flag_G) {
                fprintf(outputFile, "Amir will get C for Operating systems!\n");
                flag_A = false;
                flag_G = false;
            }

            // Close the output file
            fclose(outputFile);
            break;
        default:
            break;
    }
}

int main() {
    // Open the input device file
    fd = openDeviceFile();

    struct input_event event;

    // Print available shortcuts
    printf("Available shortcuts:\nP+E → “I passed the Exam!”\nC+A+P → “Get some cappuccino!”\nA+G → ”something secret”\n\nE+X to terminate the program\n");

    // Main loop to read and process events
    while (true) {
        ssize_t bytesRead = read(fd, &event, sizeof(event));

        if (bytesRead == -1) {
            perror("Error reading from device");
            close(fd);
            exit(EXIT_FAILURE);
        } else if (bytesRead == sizeof(event)) {
            if (event.type == EV_KEY) {
                // Process and print the event
                printEvent(&event);
            }
        }
    }

    // Close the input device file
    close(fd);
    return 0;
}
