/*
 * 注意RX-RX、TX-TX
 * 问题：前高八位存在值时，数据不准确
 * 模块与485通信，没有任何问题，唯一的问题是数据的解析
 */

#include <SoftwareSerial.h>
SoftwareSerial WSerial(7,8);
byte commend[3] ={0xA5,0x09,0xAE};
String databuffer="";
unsigned char BUFF1,BUFF2,BUFF3,BUFF4;
 
 void setup() {
   Serial.begin(9600);
   WSerial.begin(9600);
   }
 
 void loop() {
   delay(500);
   WSerial.write(&commend[0],3);////////////////////////////////////////////串口请求指令发送！！！！！！！！！！！！！！！！！！
   while(WSerial.available() > 0){////////////////////////////若串口接收到数据
     databuffer+=char(WSerial.read());/////////////////////////////依次读出存入字串缓存
     delay(2);
     }   
     

  
   if(databuffer.length() > 0) {
       Serial.print(F("返回数据："));
       for(int i=0; i<9; i++){
             int a = databuffer[i]& 0xf0;                          // 将左边高4位进行平移
             a=a>>4;                                                //平移4位后赋给a
             int b = databuffer[i]& 0x0f;                           //右边低4位
             char c = a < 10 ? a + '0' : a + 'A' - 10;
             Serial.print(c);                                       //打印16进制高位数
             char d = b < 10 ? b + '0' : b + 'A' - 10;
             Serial.print(d);                                       //打印16进制低位数
             
             Serial.print(" ");
             } 
                     
     
       
       BUFF1=(databuffer[4]); BUFF2=(databuffer[5]);
       BUFF3=(databuffer[6]); BUFF4=(databuffer[7]);
       //int Lux=((databuffer[4]<<24)+(databuffer[5]<<16)+databuffer[6]*256+databuffer[7]);
       unsigned Lux=int((BUFF1<<24)|(BUFF2<<16)|(BUFF3<<8)|(BUFF4));//后高8位*256等=左移8位
       Serial.print(F("光照："));
       Serial.println(Lux);
       Serial.println("————————————————————————————————————————");             
       }      
   databuffer=""; 
   }
