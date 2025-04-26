#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "tcp_server.h"

#include "ssd1306.h"

#define I2C_PORT i2c0	//GPIO4 GPIO5
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_ADDR 0x3C
#define MAX_CHAR 84


int main() {
	
	//all standard I/O initialize
    if(!stdio_init_all()){
    	printf("Failed to initialize I/O");
		return 1;	
	}

	//CYW43 module initialize
    if (cyw43_arch_init()) {
        printf("Failed to initialize cyw43\n");
        return 1;
    }
    
    //enable station mode (讓Pico變成用戶端模式，可以去連接其他Wi-Fi熱點) 
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Failed to connect to Wi-Fi.\n");
        return 1;
    } else {
        printf("Wi-Fi connected.\n");
    }

    //tcp_server initialize
    TCP_SERVER_T *state = tcp_server_init();
    if (!state){
		return 1;
    }
    
    //open tcp_server
    if(!tcp_server_open(state)){
		tcp_server_close(state);
		return 1;
    }
    
    //i2c initialize
    i2c_init(I2C_PORT, 400 * 1000); // 400 kHz (return Actual set baudrate)
    gpio_set_function(4, GPIO_FUNC_I2C); //SDA
    gpio_set_function(5, GPIO_FUNC_I2C); //SCL
    gpio_pull_up(4);
    gpio_pull_up(5);

    //SSD1306 initialize
    ssd1306_t disp;
    if(!ssd1306_init(&disp, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_ADDR, I2C_PORT)){
		printf("SSD1306 OLED init failed");
		return 1;
    }
    
    //clear screen buffer
    ssd1306_clear(&disp);
    
    
    //draw string
    while(1){
#if PICO_CYW43_ARCH_POLL
		  cyw43_arch_poll();
		  cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
#else
		  sleep_ms(1000);
#endif
		
		  ssd1306_draw_string(&disp, 0, 0, "Recognizing speech...");
		  ssd1306_show(&disp);
		  if(state->msg_ready){
			  for (int offset = 0; offset < strlen(state->buffer_recv);offset += MAX_CHAR-1){
				  char msg[MAX_CHAR] = {0};
				  memcpy(msg, state->buffer_recv+offset, MAX_CHAR-1);
				  msg[MAX_CHAR -1] = '\0';
				  printf("Client says: %s\n", msg);
				  ssd1306_clear(&disp);
				  ssd1306_show(&disp);
				  ssd1306_draw_string(&disp, 0, 0, msg);
				  ssd1306_show(&disp);
				  state->msg_ready = false;
				  sleep_ms(3000);
				  ssd1306_clear(&disp);	
			  }		
		  }
    }
    
    free(state);
    cyw43_arch_deinit();
    return 0;
}
