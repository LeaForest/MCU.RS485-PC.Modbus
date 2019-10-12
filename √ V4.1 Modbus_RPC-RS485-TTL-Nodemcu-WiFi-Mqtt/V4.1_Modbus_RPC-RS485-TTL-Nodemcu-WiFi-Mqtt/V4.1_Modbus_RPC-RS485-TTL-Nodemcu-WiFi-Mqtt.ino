/* RS485 !- ModbusRPC
 * 报文接收后的判断响应——写modbus寄存器，存在延迟,且继电器无法响应:
       >>>将 Serial.print 注释即可解决延迟；
 * 单独进行报文订阅（无延迟）；单独进行继电器判断写入响应（能响应）；
 * nodemcu不支持485-TTL模块的5V供电，只能采用3.3V导致modbus传输的不稳定
       >>>采用 Vin引脚进行供电；
 * modbus通讯主要依赖nodemcu的GPIO3、GPIO1口（RX、TX）
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>

ModbusMaster node;//实例化ModbusMaster对象
String databuff="";
bool state = true;

const char* ssid = "YUHAN001";
const char* password = "yuhan123456";
const char* mqtt_server = "106.14.221.57";
WiFiClient espClient;
PubSubClient client(espClient);



void setup() {
  Serial.begin(9600);
  node.begin(1, Serial);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {    
          delay(500);
          }
  randomSeed(micros());        
  //Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.publish("presence3", "ConnectReady");
  }



void loop() {
     client.connect("");
     client.subscribe("presence");
     client.setCallback(callback);
     client.loop();
     
     databuff=String("");
     }



void callback(char* topic, byte* payload, unsigned int length) {
      static uint32_t i;
      uint8_t j, result;
      uint16_t data[6];
      i++;
  
      //Serial.print(topic);
      for (int i = 0; i < length; i++) {
           databuff += (char)payload[i];
           }

      if (databuff == "o1") {
           result = node.writeSingleCoil(0x0000, state); 
      }else if(databuff == "c1"){
           result = node.writeSingleCoil(0x0000,!state);  
      }else if(databuff == "o2"){
           result = node.writeSingleCoil(0x0001,state);
      }else if(databuff == "c2"){
           result = node.writeSingleCoil(0x0001,!state);  
      }else if(databuff == "o3"){
           result = node.writeSingleCoil(0x0002,state);
      }else if(databuff == "c3"){
           result = node.writeSingleCoil(0x0002,!state); 
      }else if(databuff == "o4"){
           result = node.writeSingleCoil(0x0003,state);
      }else{
           result = node.writeSingleCoil(0x0003,!state);
           }   
      
  
      }
