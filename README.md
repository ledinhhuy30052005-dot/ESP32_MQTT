# BÁO CÁO NGHIÊN CỨU ESP32 & GIAO THỨC MQTT
**Người thực hiện:** D23 Lê Đình Huy 


---

##  A. Công việc đã làm
1. **Nghiên cứu lý thuyết:** Tìm hiểu bản chất giao thức MQTT và mô hình Publish/Subscribe.
2. **Cấu hình hệ thống:** Thiết lập môi trường kết nối MQTT cho ESP32 thông qua thư viện `PubSubClient`.
3. Cách cấu hình ESP32 kết nối với Broker
4. **Triển khai :** Hoàn thành Project điều khiển 3 LED, đồng bộ trạng thái giữa nút nhấn vật lý và phần mềm MQTT Explorer.

---

##  B. Công việc chi tiết

### 1. Tìm hiểu về giao thức MQTT
*   **Định nghĩa:** MQTT là giao thức truyền tin nhắn theo mô hình Publish/Subscribe thông qua một máy chủ trung gian gọi là Broker.

<img src="image/image-2.png" width="500">

*   **Phân tích mô hình hoạt động:**
    *   **Publisher (Người gửi):** ESP32 đọc trạng thái và đẩy dữ liệu lên các địa chỉ cụ thể gọi là **Topic**.
    *   **Subscriber (Người nhận):** Máy tính hoặc App đăng ký vào Topic đó để nhận dữ liệu về theo thời gian thực.
    *   **Broker (Người điều phối):** Đứng ở giữa, tiếp nhận tin nhắn từ người gửi và phân phối chính xác đến những người đăng ký.
    *   **Điều khiển hai chiều:** Các thiết bị bên ngoài có thể gửi lệnh điều khiển ngược về ESP32 thông qua Broker.

---

### 2. Thiết lập chương trình cho ESP32

#### 2.1. Khởi tạo đối tượng kết nối
*   **`WiFiClient espClient;`**: Thiết lập kết nối cơ bản giữa ESP32 với Broker thông qua giao thức TCP/IP.
*   **`PubSubClient mqtt(espClient);`**: Khởi tạo thực thể MQTT chạy trên nền tảng TCP/IP, chịu trách nhiệm đóng gói dữ liệu đúng tiêu chuẩn.

#### 2.2. Các hàm chức năng chính
*   **Hàm `setup_wifi()`**: Đảm bảo ESP32 kết nối mạng thành công trước khi thực hiện các tác vụ khác.
*   **Hàm `reconnect()`**: Xử lý logic tự động kết nối lại nếu gặp sự cố rớt mạng hoặc mất tín hiệu từ Broker.
*   **Hàm `mqttcallback(char* topic, byte* payload, unsigned int length)`**: Xử lý dữ liệu nhận về. Khi có lệnh từ ngoại vi, hàm này tự động sẽ được thực hiện.
   + **`char* topic`**:  là topic mà ESP32 đăng kí để nhận lệnh từ ngoại vi về

  + **`byte* payload`**
    + Là **nội dung dữ liệu nhận từ MQTT Broker**.
    + Dữ liệu được lưu dưới dạng **mảng byte**.
    + Thường cần chuyển sang `char` hoặc `String` để xử lý.
 
  + **`unsigned int length`**
    + Là **độ dài của dữ liệu payload** (số byte).
    + Dùng để biết có bao nhiêu byte dữ liệu trong `payload`.


*   **Hàm `mqtt.loop()`**: Lệnh duy trì sự sống cho kết nối, gửi gói tin giữ nhịp (ping) và giải phóng bộ đệm khi có tin nhắn mới.

#### 2.3. Các lệnh gửi và nhận dữ liệu
*   **`mqtt.publish(topic, payload)`**: Lệnh gửi dữ liệu trạng thái từ ESP32 lên Broker.
*   **`mqtt.subscribe(topic)`**: Lệnh đăng ký nhận dữ liệu từ một chủ đề nhất định trên Broker.

---

### 3. Project: Điều khiển 3 LED qua MQTT & Nút nhấn
*   **Đề tài:** Đọc trạng thái 3 LED hiển thị lên MQTT Explorer, dùng 3 nút nhấn để điều khiển tại chỗ và cho phép điều khiển từ xa qua MQTT Explorer.

####  Mã nguồn chương trình

[ESP32_MQTT_BUTTON](src/main.c)

#### Kết quả thực nghiệm 
* Đây là trạng thái 3 LED ON và hiển thị lên MQTT Explorer

<img src="image/image-3.png" width="300">
<img src="image/image-5.png" width="300">

* Đây là trạng thái 2 led on và 1 led được nhận lệnh tắt đèn từ MQTT Explorer

<img src="image/image-6.png" width="300">
<img src="image/image-7.png" width="300">



## C. Các linh kiện dụng trong project
|Tên|Số lượng|
|:---|:---:|
|ESP32 | 1|
|LED đơn | 3|
|BUTTON| 3|


## D. Khó khăn trong công việc
* Chưa tối ưu hóa hoàn toàn việc quản lý ClientID khi có nhiều thiết bị tham gia.
* Cần nghiên cứu thêm về cách cấu hình Broker khi có nhiều Client hoạt động cùng lúc để tránh xung đột dữ liệu.

## E. Công việc sắp tới
* Tìm hiểu cách làm việc và quản lý nhiều Client MQTT.
* Nghiên cứu ứng dụng trao đổi dữ liệu giữa các board ESP32.
* Tiếp tục tìm hiểu sâu về lập trình ESP32 và STM32.

















