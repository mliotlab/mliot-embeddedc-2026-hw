// HW02: KI?N TRÚC PH?N M?M VÀ QU?N L? B? NH? SMART BIKE

#include <stdio.h>
#include <stdint.h>

// TASK 1: STRUCT, UNION & BIT-FIELDS

typedef union {
    uint16_t raw_value;
    struct {
        // H?C VIÊN B?T Ð?U VI?T CODE T? ÐÂY
        uint16_t PWR_ON       : 1;  // Bit [0]
        uint16_t ASSIST_LEVEL : 2;  // Bit [2:1]
        uint16_t LIGHT_BRIGHT : 4;  // Bit [6:3]
        uint16_t RESERVED     : 9;  // Bit [15:7]
        // H?C VIÊN K?T THÚC VI?T CODE
    } fields;
} Bike_Status_t;


// TASK 2: FUNCTION POINTERS & TABLE-DRIVEN ARCHITECTURE

void drive_eco(void) {
    printf("Bike Mode: ECO.\n");
}
void drive_normal(void) {
    printf("Bike Mode: NORMAL.\n");
}
void drive_sport(void) {
    printf("Bike Mode: SPORT.\n");
}

// H?C VIÊN B?T Ð?U VI?T CODE T? ÐÂY
typedef void (*Drive_Func_t)(void);
Drive_Func_t drive_modes[3] = {drive_eco, drive_normal, drive_sport};
// H?C VIÊN K?T THÚC VI?T CODE


// TASK 3: ARCHITECTURE CALLBACKS

void Battery_Monitor(void (*overheat_cb)(void)) {
    int battery_temp = 45; 
    
    // H?C VIÊN B?T Ð?U VI?T CODE T? ÐÂY
    if (battery_temp > 40) {
        if (overheat_cb != NULL) {
            overheat_cb();
        }
    }
    // H?C VIÊN K?T THÚC VI?T CODE
}

void Critical_Battery_Handler(void) {
    printf("[WARNING] Battery is overheated!\n");
}


// TASK 4: MEMORY MAP & CRASH LAB

const char BIKE_MODEL[] = "E-Bike X2026"; 
uint32_t total_odometer = 0;             

void crash_simulation(void) {
    // H?C VIÊN B?T Ð?U VI?T CODE T? ÐÂY
    volatile uint8_t stack_burner[1024]; // T?o m?ng c?c b? ð? tiêu t?n Stack nhanh hõn
    stack_burner[0] = 0;
    crash_simulation(); // G?i ð? quy vô h?n t?o Stack Overflow
    // H?C VIÊN K?T THÚC VI?T CODE
}


// HÀM MAIN KI?M TRA (H?c viên gi? nguyên ð? ch?y th? nghi?m)

int main() {
    Bike_Status_t my_bike;
    
    // 1. Test Task 1
    my_bike.raw_value = 0x0025; 
    printf("REGISTER STRUCTURE: \n");
    printf("PWR_ON: %d | ASSIST_LEVEL: %d | LIGHT_BRIGHT: %d\n\n", 
           my_bike.fields.PWR_ON, my_bike.fields.ASSIST_LEVEL, my_bike.fields.LIGHT_BRIGHT);

    // 2. Test Task 2
    printf("ENGINE CONTROLLING: \n");
    // H?C VIÊN B?T Ð?U VI?T CODE T? ÐÂY
    if (my_bike.fields.ASSIST_LEVEL < 3) {
        drive_modes[my_bike.fields.ASSIST_LEVEL]();
    }
    printf("\n");
    // H?C VIÊN K?T THÚC VI?T CODE

    // 3. Test Task 3
    Battery_Monitor(Critical_Battery_Handler);
    printf("\n");

    // 4. Test Task 4
    int current_speed = 25; 
    printf("MEMORY ADDRESSES OF VARIABLES \n");
    printf("BIKE_MODEL (ROM/.rodata):   %p\n", (void*)BIKE_MODEL);
    printf("total_odometer (RAM/.bss):  %p\n", (void*)&total_odometer);
    printf("current_speed (RAM/Stack):  %p\n", (void*)&current_speed);

    // B? comment d?ng dý?i ð? ch?y th? bài Crash Lab
    // crash_simulation();

    return 0;
}
