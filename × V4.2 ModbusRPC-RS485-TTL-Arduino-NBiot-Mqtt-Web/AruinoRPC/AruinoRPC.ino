/* Mqtt ！- Nbiot
 * 该模块下的MQTT通信，只能进行发布，
 * “DF自定义”函数库中的mqttRecv函数无法正常调用
 * ！！！！导致 Sub订阅主题消息  无法进行解析！！！！！
 * 进而无法根据订阅信息进行反控
 */



#include <ModbusMaster.h>

#include <Wire.h>
#include <DFRobot_SIM7000.h>



SoftwareSerial WSerial(7,8);
ModbusMaster node;//实例化ModbusMaster对象
bool state = true;

#define serverIP        "106.14.221.57"
#define IOT_CLIENT      ""
#define IOT_USERNAME    ""
#define IOT_KEY         ""
#define IOT_TOPIC       "presence"

#define PIN_TX          7
#define PIN_RX          8
SoftwareSerial          mySerial(PIN_RX,PIN_TX);
DFRobot_SIM7000         sim7000;
char buff[100];


void setup(){
  Serial.begin(9600);                                                      /*~~~~~~~~~~~~~~~~~~~~?????????*/
  node.begin(1, Serial);//通过串口与设备ID为2的Modebus从机通信
  
  while(!Serial);
  sim7000.begin(mySerial);
  step1();
  step2();
  step3();
  step4();
  step5();
  step6();
   //Connect to server
   //MQTT connect request
}




void loop(){
  step7();
  step8();
  if(sim7000.mqttSubscribe(IOT_TOPIC)){  
     delay(500);
     sim7000.mqttRecv(IOT_TOPIC,buff,350);
     Serial.print(F("订阅成功"));
     for (int i = 0; i < 100; i++){
        Serial.print((char)buff[i]);
        }
     //Serial.println(buff);
   }else{
     Serial.println(F("订阅失败"));
   }
     
  /*
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[6];
  i++;
  result = node.writeSingleCoil(0x0000, state);
  result = node.writeSingleCoil(0x0001, state);
  //result = node.readHoldingRegisters(2, 6);
  state = !state;
  if (result == node.ku8MBSuccess){
    for (j = 0; j < 6; j++){
      data[j] = node.getResponseBuffer(j);
      }
    }
*/
 step10();  
  }




















/**************************************************************************************************
 *****************************************函数******************************************************
 ***************************************************************************************************/
void step1(){
     if(sim7000.turnON()){                                        
       Serial.println(F("①.SIM7000已打开!"));
       }}
    
void step2(){
     while(1){
       if(sim7000.setBaudRate(19200)){                          
          Serial.println(F("②.波特率已设置:19200"));
          break;
       }else{
          Serial.println(F("②.波特率设置失败"));
          delay(1000);
       }}}
       
void step3(){
     if(sim7000.checkSIMStatus()){                               
       Serial.println(F("③.SIM卡已准备"));
    }else{
       Serial.println(F("③.SIM卡错误, 如果已插入SIM卡，请重启并检查SIM7000"));
       while(1);
    }}
    
void step4(){
     while(1){
       if(sim7000.setNetMode(GPRS)){                              
         Serial.println(F("④.模式已设置"));
         break;
      }else{
         Serial.println(F("④.模式设置失败"));
         delay(1000);
         }}}
         
void step5(){
     int signalStrength;
     signalStrength=sim7000.checkSignalQuality();                
     Serial.print(F("⑤.当前信号质量为（0-30） ="));
     Serial.println(signalStrength);
     } 
     
void step6(){
     while(1){
       if(sim7000.attacthService()){                            
         Serial.println(F("⑥.服务已连接"));
         break;
      }else{
         Serial.println(F("⑥.服务连接失败"));
         delay(1000);
         }}}
         
void step7(){
     if(sim7000.openNetwork(TCP,serverIP,1883)){                 
       Serial.print(F("⑦.MQTT已连接!"));
       Serial.println(serverIP);
     }else{
       Serial.println(F("⑦.MQTT连接失败"));
       return;
     }
     }
     
void step8(){
     if(sim7000.mqttConnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){    /*~~~~~~~~~~~~~~???????????*/
       Serial.print(F("⑧.已连接至用户:"));
       Serial.println(IOT_USERNAME);
    }else{
       Serial.println(F("⑧.连接失败"));
       return;
       }} 
       
void step9(){
  }   
  
void step10(){
    if(sim7000.closeNetwork()){                                  
      Serial.println(F("⑩.连接已关闭 !"));
    }else{
      Serial.println(F("⑩.连接关闭失败!"));
      return;
    }
    Serial.println("-----------------------------------------------------------");
  }
