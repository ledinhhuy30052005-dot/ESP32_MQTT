# D23_Lê Đình Huy

## A.Công việc đã làm 

### 1. Tìm hiểu về giao thức MQTT
### 2. Thiết lập kết nối MQTT cho ESp32
### 3. Làm 1 project nhỏ về ESP32 và MQTT kiểm tra dữ liệu trên MQTT Explorer

## B.Công việc chi tiết

### 1. Tìm hiểu về giao thức MQTT

  Định nghĩa : MQTT là giao thức truyền tin nhắn theo mô hình Publish/Subscribe thông qua Broker.


<img src="image-2.png" width="400">

* **Phân tích hình ảnh:**
   * Publisher (Người gửi): ESP32 đọc dữ liệu cảm biến và đẩy lên một địa chỉ gọi là Topic.
   * Subscriber (Người nhận): App điện thoại hoặc máy tính đăng ký vào Topic đó để nhận dữ liệu về.
   * Broker (Người điều phối): Đứng ở giữa, nhận tin nhắn từ người gửi và phân phối chính xác đến những người đăng ký.
   * Các thiết bị từ bên ngoài có thể gửi lệnh điều khiển ngược về ESP32 

### 2. Thiết lập và Viết chương trình cho ESP32

  #### 2.1. Khởi tạo đối tượng kết nối
Việc thiết lập đòi hỏi sự phối hợp giữa hai thực thể:
*   **`WiFiClient espClient;`**: Lệnh này tạo ra một "ống dẫn" dữ liệu ở tầng TCP/IP. Nó chịu trách nhiệm thiết lập và duy trì kết nối vật lý qua sóng Wi-Fi giữa ESP32 và máy chủ (Broker). Nếu không có đối tượng này, dữ liệu không thể truyền đi trên môi trường không dây.
*   **`PubSubClient mqtt(espClient);`**: Lệnh này khởi tạo "người đưa thư" MQTT. Bằng cách truyền `espClient` vào trong, ta cho phép giao thức MQTT chạy đè lên con đường TCP đã tạo trước đó, giúp đóng gói dữ liệu theo đúng tiêu chuẩn MQTT.

#### 2.2. Các hàm chức năng chính

#### a. Hàm `setup_wifi()`
Đảm bảo thiết bị gia nhập mạng thành công trước khi thực hiện các tác vụ khác.

#### b. Hàm `reconnect()` 
Hàm này xử lý logic kết nối lại tự động nếu gặp sự cố rớt mạng hoặc mất tín hiệu từ Broker.

#### c. Hàm `mqttcallback()` - Xử lý dữ liệu nhận về
Hoạt động như một cơ chế "ngắt phần mềm". Mỗi khi Broker nhận được tin nhắn trong Topic đã đăng ký, nó sẽ đẩy dữ liệu về và hàm này sẽ tự động thực thi.
*   **`payload` & `length`**: Dữ liệu nhận về mặc định ở dạng mảng Byte thô.
*   **Xử lý chuỗi**: Sử dụng vòng lặp `for` để nối các Byte thành một chuỗi `String` hoàn chỉnh, từ đó thực hiện các phép so sánh logic để điều khiển thiết bị (ví dụ: nhận lệnh "ON" để bật LED).

#### d. Hàm `mqtt.loop()` trong `loop()` chính
Đây là lệnh quan trọng nhất để duy trì sự sống cho kết nối.
*   Nó đảm nhận việc gửi gói tin duy trì.
*   Kiểm tra và giải phóng bộ đệm khi có tin nhắn mới đến. Nếu thiếu lệnh này, ESP32 sẽ bị Broker coi là đã "chết" và ngắt kết nối chỉ sau vài giây.

#### 2.3. Các lệnh gửi và nhận dữ liệu (Publish/Subscribe)
*   **`mqtt.publish(topic, payload)`**: 
    *   Lệnh gửi dữ liệu từ ESP32 lên Broker (ví dụ: báo cáo trạng thái cảm biến).
*   **`mqtt.subscribe(topic)`**: 
    *   Khi có bất kỳ tin nhắn nào được gửi vào Topic này, Broker có trách nhiệm đẩy tin nhắn đó xuống ESP32.

















