#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <switch.h>
#include <switch/services/pctl.h>
#include <assert.h>

bool debug;

typedef enum
{
    PctlSafteyLevel_None = 0x0,
    PctlSafteyLevel_Custom = 0x1,
    PctlSafteyLevel_YoungChild = 0x2,
    PctlSafteyLevel_Child = 0x3,
    PctlSafteyLevel_Teen = 0x4,
} PctlSafteyLevel;


typedef struct {
    u16 config[26];
} PlayTimerSettings;


Result pctlGetPlayTimerSettings(PlayTimerSettings *out)
{
    return serviceDispatchOut(pctlGetServiceSession_Service(), 1456, *out);
}

Result pctlSetPlayTimerSettings(PlayTimerSettings data)
{
    return serviceDispatchIn(pctlGetServiceSession_Service(), 1951, data);
}

void init_app(void)
{
    consoleInit(NULL);
    pctlInitialize();
}

void exit_app(void)
{
    consoleExit(NULL);
    pctlExit();
}

void print_display(const char *message)
{
    printf(message);
    consoleUpdate(NULL);
}

void show_menu(void) {
    print_display("\e[38;5;214mSet parental controls for Play timer\e[0m\n\n\n");
    
    if (debug) print_display("\e[1;31mDEBUG MODE: ON\e[0m\n\n\n\n");

    if (debug) {
        print_display("Press (+) to disable debug mode\n\n");
        print_display("Press (-) to check the play timer settings\n\n");
    } else {
        print_display("Press (L) to set the play timer settings\n\n");
        print_display("Press (R) to delete the play timer settings\n\n");
        print_display("Press (Y) to get the play timer settings\n\n");
    }
    print_display("Press (B) to exit\n\n");
}

void print_lock(const char *message)
{
    print_display(message);
    PadState pad;
    padInitializeDefault(&pad);
    while (appletMainLoop())
    {
        padUpdate(&pad);
        uint64_t k = padGetButtonsDown(&pad);
        if (k & HidNpadButton_B) break;
    }
}

void print_hex(const void *data, size_t size) {
    const uint8_t *p = (const uint8_t *)data;
    for(size_t i = 0; i < size; ++i) {
        printf("%02X ", p[i]);
    }
    printf("\n");
}
void checkPlayTimerSettings(void) {
    printf("Testing...\n");
    consoleUpdate(NULL);
    PlayTimerSettings time = {};
    Result r = pctlGetPlayTimerSettings(&time);
    if(R_FAILED(r)) {
        printf("Failed (%x)\n", r);
        printf("Size (%lu)\n", sizeof(time));
        consoleUpdate(NULL);
    } else {
        printf("Success (%x)\n", r);
        printf("Size (%lu)\n", sizeof(time));
        printf("Data:\n");
        for (int i = 0; i < 26; i++) {
            printf("%u\n", time.config[i]);
        }
        printf("Data2:\n");
        print_hex(&time, sizeof(time));
        consoleUpdate(NULL);
    }
}

void getPlayTimerSettings(PadState pad) {
    consoleClear();
    PlayTimerSettings data = {};
    Result r = pctlGetPlayTimerSettings(&data);
    if(R_FAILED(r)) {
        printf("Failed (%x)\n", r);
        printf("Size (%lu)\n", sizeof(data));
        consoleUpdate(NULL);
    } else {
        printf("Your play time on Monday is %u (minutes)\n\n",data.config[10]);

        printf("Your play time on Tuesday is %u (minutes)\n\n",data.config[13]);

        printf("Your play time on Wednesday is %u (minutes)\n\n",data.config[16]);

        printf("Your play time on Thursday is %u (minutes)\n\n",data.config[19]);

        printf("Your play time on Friday is %u (minutes)\n\n",data.config[22]);

        printf("Your play time on Saturday is %u (minutes)\n\n",data.config[25]);

        printf("Your play time on Sunday is %u (minutes)\n\n",data.config[7]);
        consoleUpdate(NULL);
    }
       
}

void setPlayTimerSettings(PadState pad) {
    consoleClear();
    printf("Setting the play timer...\n\n");
    consoleUpdate(NULL);
    PlayTimerSettings data = {};
    data.config[0] = 257;
    data.config[1] = 1;
    data.config[2] = 0;
    data.config[3] = 256;
    data.config[4] = 30;
    data.config[5] = 0;
    data.config[6] = 256;
    data.config[7] = 47;
    data.config[8] = 0;
    data.config[9] = 256;
    data.config[10] = 31;
    data.config[11] = 0;
    data.config[12] = 256;
    data.config[13] = 32;
    data.config[14] = 0;
    data.config[15] = 256;
    data.config[16] = 33;
    data.config[17] = 0;
    data.config[18] = 256;
    data.config[19] = 34;
    data.config[20] = 0;
    data.config[21] = 256;
    data.config[22] = 35;
    data.config[23] = 0;
    data.config[24] = 256;
    data.config[25] = 46;
    
    printf("Set Play time for Monday (minutes)\n");
    pause(pad);
    data.config[10] = setnum();
    printf("Your play time on Monday is %u (minutes)\n\n",data.config[10]);
    
    printf("Set Play time for Tuesday (minutes)\n");
    pause(pad);
    data.config[13] = setnum();
    printf("Your play time on Tuesday is %u (minutes)\n\n",data.config[13]);
    
    printf("Set Play time for Wednesday (minutes)\n");
    pause(pad);
    data.config[16] = setnum();
    printf("Your play time on Wednesday is %u (minutes)\n\n",data.config[16]);
    
    printf("Set Play time for Thursday (minutes)\n");
    pause(pad);
    data.config[19] = setnum();
    printf("Your play time on Thursday is %u (minutes)\n\n",data.config[19]);
    
    printf("Set Play time for Friday (minutes)\n");
    pause(pad);
    data.config[22] = setnum();
    printf("Your play time on Friday is %u (minutes)\n\n",data.config[22]);
    
    printf("Set Play time for Saturday (minutes)\n");
    pause(pad);
    data.config[25] = setnum();
    printf("Your play time on Saturday is %u (minutes)\n\n",data.config[25]);
    
    printf("Set Play time for Sunday (minutes)\n");
    pause(pad);
    data.config[7] = setnum();
    printf("Your play time on Sunday is %u (minutes)\n\n",data.config[7]);
    
    pctlSetPlayTimerSettings(data);
    
    printf("Set the play timer success!\n\n");
    
    consoleUpdate(NULL);
    
}
void delPlayTimerSettings(void) {
    consoleClear();
    printf("Delete the play timer...\n\n");
    consoleUpdate(NULL);
    PlayTimerSettings data = {};
    data.config[0] = 0;
    data.config[1] = 0;
    data.config[2] = 0;
    data.config[3] = 0;
    data.config[4] = 0;
    data.config[5] = 0;
    data.config[6] = 0;
    data.config[7] = 0;
    data.config[8] = 0;
    data.config[9] = 0;
    data.config[10] = 0;
    data.config[11] = 0;
    data.config[12] = 0;
    data.config[13] = 0;
    data.config[14] = 0;
    data.config[15] = 0;
    data.config[16] = 0;
    data.config[17] = 0;
    data.config[18] = 0;
    data.config[19] = 0;
    data.config[20] = 0;
    data.config[21] = 0;
    data.config[22] = 0;
    data.config[23] = 0;
    data.config[24] = 0;
    data.config[25] = 0;
    
    pctlSetPlayTimerSettings(data);
    
    printf("Delete the play timer success!\n\n");
    
    consoleUpdate(NULL);
}


int setnum(void)
{
    Result rc=0;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
    SwkbdConfig kbd;
    char tmpoutstr[16] = {0};
    rc = swkbdCreate(&kbd, 0);
    int num = 0;
    if (R_SUCCEEDED(rc)) {
        swkbdConfigMakePresetDownloadCode(&kbd);
        rc = swkbdShow(&kbd, tmpoutstr, sizeof(tmpoutstr));
        if (R_SUCCEEDED(rc)) {
            num = atoi(tmpoutstr);
        }
        swkbdClose(&kbd);
    }
    return num;
}

void pause(PadState pad)
{
    print_display("Press (A) to continue\n");
    bool pause = false;
    while (!pause) {
        padUpdate(&pad);
        uint64_t key = padGetButtonsDown(&pad);
        if (key & HidNpadButton_A) {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    init_app();
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    debug = false;
    show_menu();

    PadState pad;
    padInitializeDefault(&pad);

    while (appletMainLoop())
    {
        padUpdate(&pad);
        uint64_t k = padGetButtonsDown(&pad);

        if (k & HidNpadButton_L) {
            if (debug) {
                
            } else {
                setPlayTimerSettings(pad);
            }
            
        }
        
        if (k & HidNpadButton_Y) {
            if (debug) {
                
            } else {
                getPlayTimerSettings(pad);
            }
            
        }
        
        if (k & HidNpadButton_R) {
            if (debug) {
                
            } else {
                delPlayTimerSettings();
            }
            
        }
        
        if (k & HidNpadButton_Minus)
        {
            if (debug) {
                checkPlayTimerSettings();
            } else {
                
            }
        }
        

        if (k & HidNpadButton_Plus) {
            debug = !debug;
            consoleClear();
            printf("Toggling debug mode\n\n");
            show_menu();
        }

        if (k & HidNpadButton_B)
        {
            break;
        }
    }

    exit_app();
    return 0;
}
