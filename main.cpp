#include <stdio.h>    // Used for printf() statements
#include <unistd.h>
#include <RadioLib.h>
#include "PiHal.h"

PiHal* hal = new PiHal(0);

// Setup CC1101 radio module.
// HAL, CS, GDO0, RST, GD02
// GPIO Numbering
CC1101 radio = new Module(hal, 8, 24, RADIOLIB_NC, 25);

// Create pager instance using the FSK module.
PagerClient pager(&radio);

float offset = .032;
float freq = 904.069;
float txFreq = freq + offset;

int main(void)
{
    char buff [50];
    int n;
    // Setup stuff:
    //wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
    printf("[CC1101] Initializing ...\n");
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        printf("[CC1101] Initialized.\n");
    } else {
        n = sprintf(buff, "[CC1101] Initialization failed: %d\n", state);
        printf("%s", buff, n);

        return 1;
    }
    radio.setOutputPower(1);
    radio.setBandwidth(12.5)
    printf("[Pager] Initializing ...\n");
    state = pager.begin(txFreq, 512);
    if (state == RADIOLIB_ERR_NONE) {
        printf("[Pager] Initialized.\n");
    } else {
        n = sprintf(buff, "[Pager] Initialization failed: %d\n", state);
        printf("%s", buff, n);

        return 1;
    }
    n = sprintf(buff, "[Pager] Sending message on freq: %d (%d)\n", freq, txFreq);
    printf("%s", buff, n);

    state = pager.transmit("HLLWRLD!", 1234567, RADIOLIB_PAGER_ASCII);
    usleep(50000);
    if (state == RADIOLIB_ERR_NONE) {
        printf("[Pager] Message sent.\n");
    } else {
        n = sprintf(buff, "[Pager] Send failed: %d\n", state);
        printf("%s", buff, n);

        return 1;
    }

    return 0;
}
