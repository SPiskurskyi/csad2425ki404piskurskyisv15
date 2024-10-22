#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libserialport.h>
#include "constants.h"

struct sp_port *port;

void parse_serial(char *byte_buff, int byte_num) {
    for (int i = 0; i < byte_num; i++) {
        printf("%c", byte_buff[i]);
    }
    printf("\n");
}

void write_to_serial(const char *message) {
    int result = sp_blocking_write(port, message, strlen(message), 1000);

    if (result < 0) {
        char *error_message = sp_last_error_message();
        printf("Error writing to serial port: %s\n", error_message);
        sp_free_error_message(error_message);
        return;
    }
    printf("Message sent: %s\n", message);
}

void wait_for_arduino_ready() {
    char byte_buff[BUFF_SIZE];
    printf("Waiting for Arduino to be ready...\n");

    while (1) {
        int bytes_waiting = sp_input_waiting(port);
        if (bytes_waiting > 0) {
            int byte_num = sp_blocking_read(port, byte_buff, sizeof(byte_buff), 1000);
            byte_buff[byte_num] = '\0';  // Null-terminate the buffer

            if (strstr(byte_buff, READY_MESSAGE) != NULL) {
                printf("Received from Arduino: %s\n", byte_buff);
                break;
            } else {
                parse_serial(byte_buff, byte_num);
            }
        }
    }
}

void wait_for_arduino_response() {
    char byte_buff[BUFF_SIZE];
    int byte_num;
    
    // Wait until there is something to read
    while (1) {
        int bytes_waiting = sp_input_waiting(port);
        if (bytes_waiting > 0) {
            byte_num = sp_blocking_read(port, byte_buff, sizeof(byte_buff), 1000);
            byte_buff[byte_num] = '\0';  // Null-terminate the buffer
            printf("Response from Arduino: ");
            parse_serial(byte_buff, byte_num);
            break;
        }
    }
}

int main() {
    printf("Opening port '%s'\n", SERIAL_PORT_PATH);

    sp_return error = sp_get_port_by_name(SERIAL_PORT_PATH, &port);
    if (error != SP_OK) {
        printf("Error finding serial device\n");
        return -1;
    }

    error = sp_open(port, SP_MODE_READ_WRITE);
    if (error != SP_OK) {
        printf("Error opening serial device\n");
        return -2;
    }

    error = sp_set_baudrate(port, BAUDRATE);
      if (error != SP_OK) {
        printf("Error setting baudrate!\n");
        return -3;
    }

    // Wait for "Arduino ready!" message
    wait_for_arduino_ready();

    // Interaction loop with Arduino
    while (1) {
        char user_input[BUFF_SIZE];
        printf("Enter message to send to Arduino: ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[strcspn(user_input, "\n")] = '\0';  // Remove newline character

        // Send user input to Arduino
        write_to_serial(user_input);

        // Wait for response from Arduino
        wait_for_arduino_response();

        fflush(stdout);
    }

    sp_close(port);
    return 0;
}
