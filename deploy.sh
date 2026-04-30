#!/bin/bash

# ==========================================
# Configuration
# ==========================================
RASPI_USER=""            # Username ของ Raspberry Pi
RASPI_IP=""         # IP Address ของ Raspberry Pi ที่ต่อสาย LAN อยู่
PORT="/dev/ttyACM0"              # Port ของ Arduino Uno ที่ต่อกับ Raspi
FQBN="arduino:avr:uno"           # Board รุ่น Arduino Uno
PROJECT_DIR="Robot_main"         # ชื่อโฟลเดอร์โปรเจกต์ Arduino ของคุณ

# ==========================================
# Script Execution
# ==========================================
echo "[Start] Initiating deployment for $PROJECT_DIR..."

# 1. ส่งไฟล์ไปที่ Raspi ด้วย scp
echo "[1/3] Copying files to Raspberry Pi..."
scp -r ./$PROJECT_DIR $RASPI_USER@$RASPI_IP:~/
if [ $? -ne 0 ]; then
    echo "Error: Failed to copy files. Check your connection to $RASPI_IP."
    exit 1
fi

# 2. สั่ง Compile ผ่าน SSH
echo "[2/3] Compiling code on Raspberry Pi..."
ssh $RASPI_USER@$RASPI_IP "~/bin/arduino-cli compile --fqbn $FQBN ~/$PROJECT_DIR"
COMPILE_STATUS=$?

if [ $COMPILE_STATUS -eq 0 ]; then
    # 3. สั่ง Upload ลงบอร์ด
    echo "[3/3] Compilation successful! Flashing to Arduino Uno..."
    ssh $RASPI_USER@$RASPI_IP "~/bin/arduino-cli upload -p $PORT --fqbn $FQBN ~/$PROJECT_DIR"
    
    if [ $? -eq 0 ]; then
        echo "[Success] Code successfully uploaded to Arduino!"
    else
        echo "[Error] Flashing failed. Check your USB connection and PORT ($PORT) on Raspi."
    fi
else
    echo "[Error] Compilation failed! Please check your code syntax."
    exit 1
fi
