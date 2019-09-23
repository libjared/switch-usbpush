#include "main.h"

int main() {
    gfxInitDefault();
    consoleInit(NULL);

    printf("USB transfer program\n");
    printf("Creating USB thread...\n");
    Result res;
    Thread t;
    bool threadCreated = false;
    res = threadCreate(&t, &fun, NULL, 0x2000, 0x20, -2);
    if (R_FAILED(res)) {
        printf("  Fail %d (module=%d)\n", res, R_MODULE(res));
    } else {
        printf("  Success\n");
        threadCreated = true;
        printf("Starting USB thread...\n");
        res = threadStart(&t);
        if (R_FAILED(res)) {
            printf("  Fail %d (module=%d)\n", res, R_MODULE(res));
        } else {
            printf("  Success\n");
        }
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

    if (threadCreated)
        threadClose(&t);
    gfxExit();
    return 0;
}

void fun(void* _unused) {
    Result res;
    printf("Opening USB...\n");
    res = usbCommsInitialize();
    if (R_FAILED(res)) {
        printf("  Fail %d (module=%d)\n", res, R_MODULE(res));
        return;
    }
    printf("  Success\n");
    printf("Setting error handling...\n");
    usbCommsSetErrorHandling(true);
    printf("  Success\n");
    printf("Getting into a read loop.\n");
    char buf[64];
    size_t cnt = 0;
    while ((cnt = usbCommsRead(buf, sizeof(buf)))) {
        printf("RECV %02zx: %s\n", cnt, buf);
        //while (unsigned int i = 0; i < cnt; i++) {
        //    char here = buf[i];
        //    printf("%02x");
        //}
        //printf("\n");
    }
    printf("End of stream.\n");
}
