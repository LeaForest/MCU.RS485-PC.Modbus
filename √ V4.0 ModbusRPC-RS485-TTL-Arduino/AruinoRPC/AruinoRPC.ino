/* 此案例是通过Modbus协议对设备进行写寄存器/度寄存器
 * 通过4路modbus继电器、485转TTL、进行控制响应（TX-TX、RX-RX）
 * 无法使用软串口进行从机的通信
 * 详情：https://github.com/4-20ma/ModbusMaster/tree/master/src
 */

#include <ModbusMaster.h>


ModbusMaster node;//实例化ModbusMaster对象


void setup(){
  Serial.begin(9600);
  node.begin(1, Serial);//通过串口与设备ID为2的Modebus从机通信
}

bool state = true;
void loop(){
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[6];
  i++;

  result = node.writeSingleCoil(0x0000, state);
  result = node.writeSingleCoil(0x0001, state);
  state = !state;

  //result = node.readHoldingRegisters(2, 6); // slave:通过RX缓存，从首地址为2的寄存器中读取数据长度为6的16bit数据
  
  if (result == node.ku8MBSuccess){
    for (j = 0; j < 6; j++){
      data[j] = node.getResponseBuffer(j);
    }
  }
  delay(3000);
}
