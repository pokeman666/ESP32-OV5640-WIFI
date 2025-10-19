#include "camera.h"
#include <Arduino.h>
#include "esp_camera.h"

uint8_t camera_init(void)
{
    Serial.println("Camera Init Start");
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.xclk_freq_hz = 20000000;

    config.pin_d0 = OV_D0_PIN;
    config.pin_d1 = OV_D1_PIN;
    config.pin_d2 = OV_D2_PIN;
    config.pin_d3 = OV_D3_PIN;
    config.pin_d4 = OV_D4_PIN;
    config.pin_d5 = OV_D5_PIN;
    config.pin_d6 = OV_D6_PIN;
    config.pin_d7 = OV_D7_PIN;

    config.pin_xclk = OV_XCLK_PIN;
    config.pin_pclk = OV_PCLK_PIN;
    config.pin_vsync = OV_VSYNC_PIN;
    config.pin_href = OV_HREF_PIN;

    config.pin_sscb_sda = OV_SDA_PIN;
    config.pin_sscb_scl = OV_SCL_PIN;

    config.pin_pwdn = OV_PWDN_PIN;
    config.pin_reset = OV_RESET_PIN;

    config.frame_size = FRAMESIZE_QQVGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_DRAM;

    config.jpeg_quality = 24;  //0-63 lower number means higher quality
    config.fb_count = 1;

    // // if (spilcd_dir == 0){
    //     config.frame_size = FRAMESIZE_240X240;
    // // }

    // config.frame_size = FRAMESIZE_240X240;

    // if (OV_PWDN_PIN == -1){
    //     xl9555_io_config(OV_PWDN, IO_SET_OUTPUT);
    //     xl9555_pin_set(OV_PWDN, IO_SET_LOW); /* 使能摄像头 */
    // }
    Serial.println("Camera Power Up");
    pinMode(OV_PWDN_PIN, OUTPUT);
    digitalWrite(OV_PWDN_PIN, LOW); /* 使能摄像头
    // if (OV_RESET_PIN == -1){
    //     xl9555_io_config(OV_RESET, IO_SET_OUTPUT);
    //     xl9555_pin_set(OV_RESET, IO_SET_LOW); /* 复位摄像头 */
    //     delay(20);
    //     xl9555_pin_set(OV_RESET, IO_SET_HIGH); /* 结束复位 */
    //     delay(20);
    // }
    Serial.println("Camera Reset");
    pinMode(OV_RESET_PIN, OUTPUT);
    digitalWrite(OV_RESET_PIN, LOW); /* 复位摄像头 */
    delay(20);
    digitalWrite(OV_RESET_PIN, HIGH); /* 结束复位 */
    delay(20);
    Serial.println("Camera Configuring");
    esp_err_t err = esp_camera_init(&config);
    Serial.println("Camera Init Done");
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return 1;
    }
    sensor_t * s = esp_camera_sensor_get();
    Serial.printf("Camera ID: 0x%02X \n", s->id.PID);

    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_hmirror(s, 1);       // 0,1
    s->set_vflip(s, 1);         // 0,1

    if (s->id.PID == OV2640_PID) {
        s->set_vflip(s, 0);         // 0,1
    }
    return 0;
}

// uint8_t camera_capture_show(void)
// {
//     fb = esp_camera_fb_get();  
//     if (!fb) {
//         Serial.println("Camera capture failed");
//         return 1;
//     }
//     if (spilcd_dir == 1){
//         lcd_show_pic(80, 60, 160, 120, fb->buf); /* 显示图像 */
//     }
//     else{
//         lcd_show_pic(0, 39, 240, 240, fb->buf); /* 显示图像 */
//     }
//     esp_camera_fb_return(fb);
//     fb = NULL;
//     return 0;
// }


