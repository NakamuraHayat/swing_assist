/* TODO:
    割り込みを使う
*/

#include "AD5328_1.h"
#include "MPU9250.h"

unsigned long time_from_start;
unsigned long time_valve_start;

float accx, accy, accz;
float gyrox, gyroy, gyroz;
float eulerx, eulery, eulerz;

int cs1 = 10;
int cs2 = 9;
AD5328 ad5328_1(cs1);
AD5328 ad5328_2(cs2);

MPU9250 mpu9250;

float pres_MPa = 0.0; // 0~0.8MPA
int valve_port_0 = 0;   // 0~15
float valve_operate_time = 2.0; // second
int valve_port_1 = 1;
int valve_port_2 = 2;

char show_start = '0';
int flag = 1;

void valve(float pres_MPa, int valve_port);
void printPMU();

void setup() {
  Serial.begin(9600);
  ad5328_1.begin();
  ad5328_2.begin();
  Wire.begin();
  mpu9250.setup(0x68);
  mpu9250.selectFilter(QuatFilterSel::MAHONY);

}

void loop() {
  time_from_start = micros();
  Serial.print(time_from_start);
  Serial.print(",");
  printPMU();

  if (flag == 1) {
    if (Serial.available()) {
      show_start = Serial.read();
      if (show_start == '1') {
        pres_MPa = 0.6;

        valve(pres_MPa, valve_port_0);
        //Serial.print(valve_port_0);
        //Serial.print(" -> ");
        //Serial.println(pres_MPa);

        valve(pres_MPa, valve_port_1);
        //Serial.print(valve_port_1);
        //Serial.print(" -> ");
        //Serial.println(pres_MPa);

        valve(pres_MPa, valve_port_2);
        //Serial.print(valve_port_2);
        //Serial.print(" -> ");
        //Serial.println(pres_MPa);


        time_valve_start = micros();
        time_from_start = micros();
        flag = 2;
        Serial.println("valve start");
      }
    }

    //delay(2000);

    else if (show_start == '0') {
      pres_MPa = 0;
      valve(pres_MPa, valve_port_0);
      //Serial.print(valve_port_0);
      //Serial.print(" -> ");
      //Serial.println(pres_MPa);

      valve(pres_MPa, valve_port_1);
      //Serial.print(valve_port_1);
      //Serial.print(" -> ");
      //Serial.println(pres_MPa);

      valve(pres_MPa, valve_port_2);
      //Serial.print(valve_port_2);
      //Serial.print(" -> ");
      //Serial.println(pres_MPa);
    }
  }

  else if (flag == 0) {
    pres_MPa = 0;
    valve(pres_MPa, valve_port_0);
    //Serial.print(valve_port_0);
    //Serial.print(" -> ");
    //Serial.println(pres_MPa);

    valve(pres_MPa, valve_port_1);
    //Serial.print(valve_port_1);
    //Serial.print(" -> ");
    //Serial.println(pres_MPa);

    valve(pres_MPa, valve_port_2);
    //Serial.print(valve_port_2);
    //Serial.print(" -> ");
    //Serial.println(pres_MPa);

    flag = 1;
  }

  if (uint32_t(time_from_start - time_valve_start) > (valve_operate_time * 1000000) && flag == 2) {
    flag = 0;
    Serial.println("valve stop");
  }
}

void printPMU() {
  if (mpu9250.update()) {
    accx = mpu9250.getAccX();
    accy = mpu9250.getAccY();
    accz = mpu9250.getAccZ();
    gyrox = mpu9250.getGyroX();
    gyroy = mpu9250.getGyroY();
    gyroz = mpu9250.getGyroZ();
    eulerx = mpu9250.getEulerX();
    eulery = mpu9250.getEulerY();
    eulerz = mpu9250.getEulerZ();
  }

  Serial.print(accx);
  Serial.print(",");
  Serial.print(accy);
  Serial.print(",");
  Serial.print(accz);
  Serial.print(",");
  Serial.print(gyrox);
  Serial.print(",");
  Serial.print(gyroy);
  Serial.print(",");
  Serial.print(gyroz);
  Serial.print(",");
  Serial.print(eulerx);
  Serial.print(",");
  Serial.print(eulery);
  Serial.print(",");
  Serial.print(eulerz);
  Serial.println();
}

void valve(float pres_MPa, int valve_port) {
  float pres_bar    = 0.0;
  float V_amplifier = 0.0;
  float V_DAC       = 0.0;
  float V_DAC_bit   = 0.0;

  pres_bar = pres_MPa * 10.0;
  V_amplifier = pres_bar / 8.0 * 10.0;
  V_DAC = V_amplifier / 2.0;        // Gain = 1 + R1 / R2, R1 = R2 = 10kΩ
  V_DAC_bit = V_DAC * 4096.0 / 5.0; // V_DAC = V_ref * V_DAC_bit / 2^N , V_ref = 5V, N = 12bit(分解能)

  switch (valve_port) {
    case 0:
      ad5328_1.write(DAC_A, V_DAC_bit);
      break;
    case 1:
      ad5328_1.write(DAC_B, V_DAC_bit);
      break;
    case 2:
      ad5328_1.write(DAC_C, V_DAC_bit);
      break;
    case 3:
      ad5328_1.write(DAC_D, V_DAC_bit);
      break;
    case 4:
      ad5328_1.write(DAC_E, V_DAC_bit);
      break;
    case 5:
      ad5328_1.write(DAC_F, V_DAC_bit);
      break;
    case 6:
      ad5328_1.write(DAC_G, V_DAC_bit);
      break;
    case 7:
      ad5328_1.write(DAC_H, V_DAC_bit);
      break;
    case 8:
      ad5328_2.write(DAC_A, V_DAC_bit);
      break;
    case 9:
      ad5328_2.write(DAC_B, V_DAC_bit);
      break;
    case 10:
      ad5328_2.write(DAC_C, V_DAC_bit);
      break;
    case 11:
      ad5328_2.write(DAC_D, V_DAC_bit);
      break;
    case 12:
      ad5328_2.write(DAC_E, V_DAC_bit);
      break;
    case 13:
      ad5328_2.write(DAC_F, V_DAC_bit);
      break;
    case 14:
      ad5328_2.write(DAC_G, V_DAC_bit);
      break;
    case 15:
      ad5328_2.write(DAC_H, V_DAC_bit);
      break;
  }
}
