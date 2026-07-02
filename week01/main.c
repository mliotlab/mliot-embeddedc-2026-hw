// HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP

#include <stdio.h>
#include <stdint.h>

// TASK 1: POINTERS & MEMORY

void parse_config(const uint8_t *config_packet, int16_t *high_threshold) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    
    // Kiểm tra an toàn con trỏ đầu vào phòng ngừa crash hệ thống
    if (config_packet == NULL || high_threshold == NULL) {
        return;
    }

    // Ghép 2 byte theo định dạng Little Endian:
    // Index 0 là byte thấp (LSB), Index 1 là byte cao (MSB) dịch sang trái 8 bit.
    // Ép kiểu sang (int16_t) để giữ nguyên tính chất số nguyên có dấu.
    *high_threshold = (int16_t)(config_packet[0] | (config_packet[1] << 8));

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // Kiểm tra an toàn con trỏ
    if (hw_sensor_reg == NULL) {
        return 0;
    }

    // Ép kiểu con trỏ void sang con trỏ kiểu int16_t có thuộc tính volatile
    // volatile ngăn compiler tối ưu hóa (bỏ qua lệnh đọc thực tế từ RAM/thanh ghi phần cứng)
    volatile int16_t *reg_ptr = (volatile int16_t *)hw_sensor_reg;

    // Trả về giá trị đọc trực tiếp từ ô nhớ ngoại vi thông qua giải con trỏ
    return *reg_ptr;

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // Kiểm tra an toàn con trỏ
    if (control_reg == NULL) {
        return;
    }

    // Áp dụng kỹ thuật Clear-then-Set:
    // Bước 1: Clear (Xóa) Bit 0 và Bit 1 về 0, bảo toàn tuyệt đối trạng thái của Bit 2 đến Bit 7.
    // Dùng mặt nạ (Mask): 0xFC (1111 1100b) kết hợp phép toán toán tử bitwise AND (&)
    uint8_t temp_reg = *control_reg & 0xFC;

    // Bước 2: Set (Bật) các bit tương ứng dựa theo trạng thái của tham số truyền vào
    if (fan_enable == 1) {
        temp_reg |= (1 << 0); // BẬT Bit 0 (Cooling Fan) lên 1
    }
    // (Nếu fan_enable == 0, Bit 0 đã được xóa về 0 ở Bước 1 nên không cần xử lý thêm)

    if (alarm_enable == 1) {
        temp_reg |= (1 << 1); // BẬT Bit 1 (Alarm LED) lên 1
    }
    // (Nếu alarm_enable == 0, Bit 1 đã được xóa về 0 ở Bước 1 nên không cần xử lý thêm)

    // Cập nhật giá trị cuối cùng sau khi tính toán vào vùng nhớ của thanh ghi điều khiển
    *control_reg = temp_reg;

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm, chỉ thay đổi input nếu cần)

int main() {

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); // Có thể thay mock_packet thành NULL để test chống crash khi gặp NULL
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105; 
    int16_t current_temp = read_temperature_reg((void*)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold) {
        // Yêu cầu: fan = 1, alarm = 0
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}
