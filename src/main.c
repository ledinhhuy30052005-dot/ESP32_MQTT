#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define LED_YELLOW 2
#define LED_RED 13    
#define LED_GREEN 12  

#define BUTTON4 4
#define BUTTON5 5
#define BUTTON18 18

// WiFi
const char* ssid = "TANG03";
const char* password = "0976152886";

// Khởi tạo các đối tượng kết nối
WiFiClient client;
PubSubClient mqtt(client);

// --- Cấu hình MQTT Broker (HiveMQ Cloud) ---
const char* mqtt_sever = "broker.hivemq.com"; 
const int   mqtt_port = 1883;//sử dụng cổng mặc định
//const char* mqtt_user = "ptit";
//const char* mqtt_password = "huytz@12345";


// Topic gửi trạng thái (Publish) từ ESP32 lên MQTT
const char* pubTopicLedYellow = "esp32/LED_YELLOW/status";
const char* pubTopicLedRed = "esp32/LED_RED/status";
const char* pubTopicLedGreen = "esp32/LED_GREEN/status";

// Topic nhận lệnh (Subscribe) từ MQTT về ESP32
const char* SubLedyellow_control = "esp32/LED_YELLOW/cmd";
const char* SubLedRed_control = "esp32/LED_RED/cmd";
const char* SubLedGreen_control = "esp32/LED_GREEN/cmd";

void setup_wifi(){
  Serial.println("Connecting to WiFi....");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
     delay(500);
     Serial.print('.');
  }
  Serial.println("\nWiFi_connected...");
}

void mqttcallback(char* topic, byte* payload, unsigned int length){
   String mess = ""; 
   //// Chuyển đổi mảng Byte nhận được sang dạng chuỗi (String)
   for(int i=0; i<length; i++){
     mess += (char)payload[i]; 
   }

   Serial.println("da nhan lenh [" + String(topic) + "]: " + mess);
 // --- Logic xử lý lệnh điều khiển LED Vàng ---
   if(String(topic) == SubLedyellow_control){
      if(mess == "LED_YELLOW_OFF"){
        digitalWrite(LED_YELLOW, LOW);
        mqtt.publish(pubTopicLedYellow, "LED_YELLOW_OFF");
      }
      else if(mess == "LED_YELLOW_ON"){
        digitalWrite(LED_YELLOW, HIGH);
        mqtt.publish(pubTopicLedYellow, "LED_YELLOW_ON");
      }
   }
   // --- Logic xử lý lệnh điều khiển LED Đỏ ---
   else if(String(topic) == SubLedRed_control){
       if(mess == "LED_RED_OFF"){
        digitalWrite(LED_RED, LOW);
        mqtt.publish(pubTopicLedRed, "LED_RED_OFF"); 
      }
      else if(mess == "LED_RED_ON"){ 
        digitalWrite(LED_RED, HIGH);
        mqtt.publish(pubTopicLedRed, "LED_RED_ON");
      }
   }
   // --- Logic xử lý lệnh điều khiển LED Xanh ---
   else if(String(topic) == SubLedGreen_control){
       if(mess == "LED_GREEN_OFF"){
        digitalWrite(LED_GREEN, LOW);
        mqtt.publish(pubTopicLedGreen, "LED_GREEN_OFF");
      }
      else if(mess == "LED_GREEN_ON"){
        digitalWrite(LED_GREEN, HIGH);
        mqtt.publish(pubTopicLedGreen, "LED_GREEN_ON");
      }
   }
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP32-" + String(WiFi.macAddress());
    // Gửi yêu cầu kết nối tới Broker
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
     // Sau khi kết nối thành công, đăng ký nhận lệnh từ các topic control
      mqtt.subscribe(SubLedyellow_control);
      mqtt.subscribe(SubLedRed_control);
      mqtt.subscribe(SubLedGreen_control);
    } else {
    // Nếu thất bại, in mã lỗi và thử lại sau 5 giây
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      delay(5000);
    }
  }
}

void setup(){
  Serial.begin(9600); 
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);

  // vì mình nối button dùng nội trở bên trong esp32 =>cấu hình là INPUT_PULLUP
  //mạch nối INPUT_PULLUP khi dùng nội trở ESp: GPIO--> BUTTON->GND
  //mạch nối INPUT_PULLDOWN khi dùng nội trở ESp: 3v3 BUTTON GPIO
  //còn nối không dùng nội trở ESP32 => cấu hình INPUT bên ngoài mạch nối thêm trở 
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);
  pinMode(BUTTON18, INPUT_PULLUP);

  setup_wifi();
  mqtt.setServer(mqtt_sever, mqtt_port); // Cấu hình địa chỉ và cổng Broker
  mqtt.setCallback(mqttcallback);// Cấu hình hàm xử lý tin nhắn đến
}

// Khởi tạo trạng thái ban đầu là 1 (vì dùng PULLUP nhấn là 0)
int prev_state1 = 1;
int prev_state2 = 1;
int prev_state3 = 1;

// Biến lưu trạng thái bật/tắt hiện tại của LED
int led_yellow_state = 0; 
int led_red_state = 0; 
int led_green_state = 0; 

void loop(){
  // Kiểm tra nếu mất kết nối MQTT thì gọi hàm kết nối lại
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();// Duy trì kết nối MQTT và xử lý các gói tin đến/đi
  
  int button1_state = digitalRead(BUTTON4);
  int button2_state = digitalRead(BUTTON5);
  int button3_state = digitalRead(BUTTON18);

  // Xử lý nút 1 - LED Vàng
  if(prev_state1 == 1 && button1_state == LOW){ 
      led_yellow_state = !led_yellow_state;
      if(led_yellow_state != 0){
          digitalWrite(LED_YELLOW, HIGH);
          mqtt.publish(pubTopicLedYellow, "LED_YELLOW_ON");
      }
      else{
          digitalWrite(LED_YELLOW, LOW);
          mqtt.publish(pubTopicLedYellow, "LED_YELLOW_OFF");
      }
      delay(200); // Chống rung nút nhấn
  }
  prev_state1 = button1_state; 

  // Xử lý nút 2 - LED Đỏ
  if(prev_state2 == 1 && button2_state == LOW){
      led_red_state = !led_red_state;
      if(led_red_state != 0){
          digitalWrite(LED_RED, HIGH);
          mqtt.publish(pubTopicLedRed, "LED_RED_ON");
      }
      else{
          digitalWrite(LED_RED, LOW);
          mqtt.publish(pubTopicLedRed, "LED_RED_OFF");
      }
      delay(200);
  }
  prev_state2 = button2_state;

  // Xử lý nút 3 - LED Xanh
  if(prev_state3 == 1 && button3_state == LOW){
      led_green_state = !led_green_state;
      if(led_green_state != 0){
          digitalWrite(LED_GREEN, HIGH);
          mqtt.publish(pubTopicLedGreen, "LED_GREEN_ON");
      }
      else{
          digitalWrite(LED_GREEN, LOW);
          mqtt.publish(pubTopicLedGreen, "LED_GREEN_OFF");
      }
      delay(200);
  }
  prev_state3 = button3_state;
}