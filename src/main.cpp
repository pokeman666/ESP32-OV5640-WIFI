#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "camera.h"
#include "esp_camera.h"

const char* WIFI_SSID = "ROBOT";
const char* WIFI_PASS = "robot2021";
const char* URL = "/Stream";
camera_fb_t * fb = NULL;
// const int FRAME_RATE = 10;

/* 创建一个HTTP服务对象 在这里提供视频流 默认使用80*/
WebServer server(80);

void handleStream() {
  static char head[128];
  /* 获取连接到服务器的客户端对象 */
  WiFiClient client = server.client();

  /* HTTP头部 告知客户端数据类型和分隔符 */
  server.sendContent("HTTP/1.1 200 OK\r\n"
                     "Content-Type: multipart/x-mixed-replace; "
                     "boundary=frame\r\n\r\n");

  while (client.connected()) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }
    
    /* 将格式化字符串写入字符数组 */
    sprintf(head,
            "--frame\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %ul\r\n\r\n",
            fb->len);
    client.write(head, strlen(head));
    client.write(fb->buf, fb->len);
    client.write("\r\n");

    esp_camera_fb_return(fb);
    fb = NULL;
    // delay(30);
    }
}

/* 初始化WIFI 打印视频流所在路径 */
void initWifi() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Serial.printf("Stream at: http://%s%s\n",
                WiFi.localIP().toString().c_str(), URL);
}

/* 初始化服务 调用handleStream */
void initServer() {
  server.on(URL, handleStream);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  initWifi();
  while(camera_init()){
    Serial.println("Camera Init Failed! Retry...");
    delay(1000);
  }
  initServer();
}

void loop() {
  /* 检查是否有客户端发来HTTP请求 如果有 调用sever on注册的回调函数来处理请求 */
  server.handleClient();
}