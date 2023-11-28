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
bool flag = false;
bool flag_exam = false;
bool flag_cap1 = false;
bool flag_cap2 = false;
bool flag_amir = false;

void printEvent(struct input_event *event) {
    const char *eventType;

    switch (event->type) {
        case EV_KEY:
            if (event->value == 0) {
                eventType = "RELEASED";

                // Reset flags when corresponding keys are released
                if (event->code == KEY_E) {
                    flag = false;
                }
                if (event->code == KEY_P) {
                    flag_exam = false;
                }
                if (event->code == KEY_C) {
                    flag_cap1 = false;
                }
                if (event->code == KEY_A && flag_cap1) {
                    flag_cap2 = false;
                }
                if (event->code == KEY_A) {
                    flag_amir = false;
                }
            } else if (event->value == 1) {
                eventType = "PRESSED";

                // Set flags when corresponding keys are pressed
                if (event->code == KEY_E) {
                    flag = true;
                }
                if (event->code == KEY_P) {
                    flag_exam = true;
                }
                if (event->code == KEY_C) {
                    flag_cap1 = true;
                }
                if (event->code == KEY_A && flag_cap1) {
                    flag_cap2 = true;
                }
                if (event->code == KEY_A) {
                    flag_amir = true;
                }
            } else if (event->value == 2) {
                eventType = "REPEATED";

                // Set flags when corresponding keys are repeated
                if (event->code == KEY_E) {
                    flag = true;
                }
                if (event->code == KEY_P) {
                    flag_exam = true;
                }
                if (event->code == KEY_C) {
                    flag_cap1 = true;
                }
                if (event->code == KEY_A && flag_cap1) {
                    flag_cap2 = true;
                }
                if (event->code == KEY_A) {
                    flag_amir = true;
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
            if (flag == true && event->code == KEY_X) {
                fclose(outputFile);
                exit(EXIT_FAILURE);
            }

            // Additional event handling
            if (flag_exam == true && event->code == KEY_E) {
                fprintf(outputFile, "I passed the Exam!\n");
                flag_exam = false;
            }

            if (flag_cap1 && flag_cap2 && event->code == KEY_P) {
                fprintf(outputFile, "Get some cappuccino!\n");
                flag_cap1 = flag_cap2 = false;
            }

            if (flag_amir && event->code == KEY_C) {
                fprintf(outputFile, "Amir will get C for Operating systems!\n");
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
    printf("Available shortcuts:\nP+E → “I passed the Exam!”\nC+A+P → “Get some cappuccino!”\nA+C → ”something secret”\n\nE+X to terminate the program\n");

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
