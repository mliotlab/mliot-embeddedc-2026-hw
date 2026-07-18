// HW01: HE THONG GIAM SAT LO NHIET CONG NGHIEP

#include <stdio.h>
#include <stdint.h>

// TASK 1: POINTERS & MEMORY

void parse_config(const uint8_t *config_packet, int16_t *high_threshold) {
    // HOC VIEN BAT DAU VIET CODE TU DAY
    
    // Kiem tra an toan con tro dau vao phong ngua crash he thong
    if (config_packet == NULL || high_threshold == NULL) {
        return;
    }

    // Ghep 2 byte theo dinh dang Little Endian:
    // Index 0 la byte thap (LSB), Index 1 la byte cao (MSB) dich sang trai 8 bit.
    // Ep kieu sang (int16_t) de giu nguyen tinh chat so nguyen co dau.
    *high_threshold = (int16_t)(config_packet[0] | (config_packet[1] << 8));

    // HOC VIEN KET THUC VIET CODE
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg) {
    // HOC VIEN BAT DAU VIET CODE TU DAY

    // Kiem tra an toan con tro
    if (hw_sensor_reg == NULL) {
        return 0;
    }

    // Ep kieu con tro void sang con tro kieu int16_t co thuoc tinh volatile
    // volatile ngan jup compiler toi uu hoa (bo qua lenh doc thuc te tu thanh ghi)
    volatile int16_t *reg_ptr = (volatile int16_t *)hw_sensor_reg;

    // Tra ve gia tri doc truc tiep tu o nho ngoai vi thong qua giai con tro
    return *reg_ptr;

    // HOC VIEN KET THUC VIET CODE
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable) {
    // HOC VIEN BAT DAU VIET CODE TU DAY

    // Kiem tra an toan con tro
    if (control_reg == NULL) {
        return;
    }

    // Ap dung ky thuat Clear-then-Set:
    // Buoc 1: Clear (Xoa) Bit 0 va Bit 1 ve 0, bao toan trang thai tu Bit 2 den Bit 7.
    // Dung mat na (Mask): 0xFC (1111 1100b) ket hop phep toan bitwise AND (&)
    uint8_t temp_reg = *control_reg & 0xFC;

    // Buoc 2: Set (Bat) cac bit tuong ung dua theo trang thai cua tham so truyen vao
    if (fan_enable == 1) {
        temp_reg |= (1 << 0); // BAT Bit 0 (Cooling Fan) len 1
    }

    if (alarm_enable == 1) {
        temp_reg |= (1 << 1); // BAT Bit 1 (Alarm LED) len 1
    }

    // Cap nhat gia tri cuoi cung vao vung nho cua thanh ghi dieu khien
    *control_reg = temp_reg;

    // HOC VIEN KET THUC VIET CODE
}

// HAM MAIN KIEM TRA (Hoc vien giu nguyen de chay thu nghiem, chi thay doi input neu can)

int main() {

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); // Co the thay mock_packet thanh NULL de test chong crash
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105; 
    int16_t current_temp = read_temperature_reg((void*)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold) {
        // Yeu cau: fan = 1, alarm = 0
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}
