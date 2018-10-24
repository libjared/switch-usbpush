#include "main.h"

int main() {
    Result res;
    bool initOk = false;
    gfxInitDefault();
    consoleInit(NULL);

    printf("USB transfer program\n");

    printf("Opening USB...");
    res = usbCommsInitialize();
    if (R_SUCCEEDED(res)) {
        printf("Success\n");

        printf("Setting error handling...\n");
        usbCommsSetErrorHandling(true);

        printf("Getting into a read loop... TODO\n");
        initOk = true;
    } else {
        printf("Failure\n");
    }

    // Main loop
    while (appletMainLoop()) {
        gfxSwapBuffers();
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;

        gfxFlushBuffers();
    }

    gfxExit();
    return 0;
}
