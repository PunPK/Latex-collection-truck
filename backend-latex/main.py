import serial
import time

# 1. ตั้งค่าพอร์ตให้ตรงกับ Step 1 และ Baud Rate ให้ตรงกับโค้ด Arduino (เช่น 9600)
arduino_port = '/dev/ttyACM0' 
baud_rate = 115200

try:
    # เปิดการเชื่อมต่อ
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    ser.reset_input_buffer() # เคลียร์ข้อมูลเก่าที่ค้างในสายทิ้งก่อน
    print(f"เชื่อมต่อกับ {arduino_port} สำเร็จ! กำลังรอข้อมูล...\n")

    # 2. วนลูปอ่านข้อมูลตลอดเวลา
    while True:
        if ser.in_waiting > 0:
            # อ่านข้อมูลมา 1 บรรทัด ถอดรหัสเป็นข้อความ และตัดช่องว่าง/ขึ้นบรรทัดใหม่ส่วนเกินออก
            raw_data = ser.readline().decode('utf-8').rstrip()
            
            # ปริ้นออกมาเป็น Log ใน Terminal
            print(f"[LOG] ได้รับข้อมูล: {raw_data}")
            
        # หน่วงเวลาเล็กน้อยไม่ให้ CPU ของ Pi ทำงานหนักเกินไป
        time.sleep(0.05)

except serial.SerialException as e:
    print(f"เกิดข้อผิดพลาด: ไม่สามารถเชื่อมต่อกับพอร์ตได้ ({e})")
except KeyboardInterrupt:
    print("\nปิดการทำงานของ Log")
    ser.close()
