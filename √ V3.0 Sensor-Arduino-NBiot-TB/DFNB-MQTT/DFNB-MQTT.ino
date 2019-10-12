#include <Wire.h>
#include <DFRobot_SIM7000.h>
#include <DFRobot_BME280.h>

#define serverIP        "139.224.9.212"
#define IOT_CLIENT      "L9ZPRGlt6Bl7P8FXYRRd"
#define IOT_USERNAME    "L9ZPRGlt6Bl7P8FXYRRd"
#define IOT_KEY         ""
#define IOT_TOPIC       "v1/devices/me/telemetry"

#define PIN_TX          7
#define PIN_RX          8
SoftwareSerial          mySerial(PIN_RX,PIN_TX);
DFRobot_SIM7000         sim7000;

#define SEA_LEVEL_PRESSURE  1013.25f
#define BME_CS 10
DFRobot_BME280 bme; 
float temp, pa, hum, alt;

void setup(){
    Serial.begin(115200);
    bme.begin(0x76);
    while(!Serial);
    sim7000.begin(mySerial);
    step1();//Turn ON SIM7000
    step2();//Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
    step3();//Check SIM card
    step4();//Set net mod：NB-IOT卡为NB，SIM为GPRS
    step5();//Check signal quality from (0-30)
    step6();//Open the connection
    }



void loop(){
    step7(); //Connect to server
    step8(); //MQTT connect request
    
    temp = bme.temperatureValue();
    pa = bme.pressureValue();
    hum = bme.humidityValue();
    alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);

    String payload = "{";                     
    payload += "\"TEMP\":"; 
    payload += temp;
    payload += ",";
    payload += "\"HUMI\":";
    payload += hum;
    payload += ",";
    payload += "\"Pa\":";
    payload += pa;                            
    payload += "}";

    if(sim7000.mqttPublish(IOT_TOPIC,payload)){                 //Send data to topic
        Serial.print(F("⑨.数据发送成功："));
        Serial.println(payload);
    }else{
        Serial.println(F("⑨.发送失败"));
        return;
        }
    delay(200);
    step10();//Close connection
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
     if(sim7000.mqttConnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){    
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


/*****************************************************************************************
 ***********************************定位函数******************************************************
**********************************************************************************************/
void step11(){
     while(1){
       if(sim7000.init()){                                        //Init SIM7000
         Serial.println("AT command READY");
         break;
       }else{
         Serial.println("AT command ERROR");
         delay(500);
      }}}
void step12(){
     while(1){
       if(sim7000.initPos()){
         Serial.println(F("   坐标函数初始化"));
         break;
       }else{
         Serial.println(F("   坐标函数初始化失败"));
         delay(500);
       }}}
void step13(){
  if(sim7000.getPosition()){                                     //Get the current position
        Serial.print(F("    经度:"));
        Serial.print(sim7000.getLongitude());                    //Get longitude
        Serial.print(F(",    纬度:"));
        Serial.println(sim7000.getLatitude());                     //Get latitude
    }else{
        Serial.println(F("数据错误请重试"));
    }
  }
/*发送输入的数据并检测数据有效性
 *Serial.println(F("⑨.输入数据并以CRLF结束 : "));
 *sendData = readSerial(sendData);
String readSerial(String result){
    int i = 0;
    while(1){
        while(Serial.available() > 0){
            char inChar = Serial.read();
            if(inChar == '\n'){
                result += '\0';
                while(Serial.read() >= 0);
                return result;
            }
            if(i == 50){
                Serial.println(F("   此数据过长"));
                result += '\0';
                while(Serial.read() >= 0);
                return result;
            }
            if(inChar != '\r'){
                result += inChar;
                i++;
            }
        }
    }
}*/
