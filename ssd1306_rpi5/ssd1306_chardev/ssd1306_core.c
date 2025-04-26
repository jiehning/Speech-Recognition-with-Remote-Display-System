#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "ssd1306.h"
#include "font.h"



//command function
void ssd1306_send_cmd(ssd1306_t *p, uint8_t cmd) {
    uint8_t cmd_buffer[2] = {SSD1306_CMD, cmd};
    int ret = i2c_smbus_write_i2c_block_data(p->client, cmd_buffer[0], 1,&cmd_buffer[1] );
	if (ret < 0){
		printk("Failed to send command to SSD1306\n");
	}
}


//initial ssd1306
int ssd1306_init(ssd1306_t *p, uint16_t width, uint16_t height, uint8_t address, struct i2c_client *client) {
	p->width = width;
	p->height = height;
	p->address = address;
	p->client = client;
	p->pages = height / 8;
	p->bufsize = p->pages * p->width;
	p->buffer = kmalloc(p->bufsize+1, GFP_KERNEL); //keep one byte for control command
	
	if (p->buffer == NULL){
		return -ENOMEM;
	}
	
	++(p->buffer); //keep the first byte for the control command

	
    ssd1306_send_cmd(p, SET_DISP); // Display OFF
    
	ssd1306_send_cmd(p, SET_DISP_CLK_DIV);
    ssd1306_send_cmd(p, 0x80);
    
    ssd1306_send_cmd(p, SET_MUX_RATIO);
    ssd1306_send_cmd(p, p->height-1);
    
    ssd1306_send_cmd(p, SET_DISP_OFFSET);
	ssd1306_send_cmd(p, 0x00);
	
	ssd1306_send_cmd(p, SET_DISP_START_LINE);
	ssd1306_send_cmd(p, 0x00);
	
	ssd1306_send_cmd(p, SET_CHARGE_PUMP);
	ssd1306_send_cmd(p, 0x14); //set 0x14 for internal power


	ssd1306_send_cmd(p, SET_SEG_REMAP|0x01);
	ssd1306_send_cmd(p, SET_COM_OUT_DIR | 0x08);
	
	ssd1306_send_cmd(p, SET_COM_PIN_CFG);
	ssd1306_send_cmd(p, p->width >2* p->height?0x02:0x12);
	
	ssd1306_send_cmd(p, SET_CONTRAST);
	ssd1306_send_cmd(p, 0xff);
	
	ssd1306_send_cmd(p, SET_PRECHARGE);
	ssd1306_send_cmd(p, 0xF1); //set 0xF1 for internal power
	
	ssd1306_send_cmd(p, SET_VCOM_DESEL);
	ssd1306_send_cmd(p, 0x30);
	
	ssd1306_send_cmd(p, SET_ENTIRE_ON);
	
    ssd1306_send_cmd(p, SET_NORM_DISP); // Normal display
    
    ssd1306_send_cmd(p, SET_DISP|0x01); //display on
    
    ssd1306_send_cmd(p, SET_MEM_ADDR);
    ssd1306_send_cmd(p, 0x00);
	
	ssd1306_clear(p);

    return 0; 
}

void ssd1306_deinit(ssd1306_t *p){
	kfree(p->buffer-1);
}

void ssd1306_poweroff(ssd1306_t *p){
	ssd1306_send_cmd(p, SET_DISP);
}

void ssd1306_poweron(ssd1306_t *p){
	ssd1306_send_cmd(p, SET_DISP|0x01);
}

void ssd1306_contrast(ssd1306_t *p, uint8_t val){
	ssd1306_send_cmd(p, SET_CONTRAST);
	ssd1306_send_cmd(p, val);
}
void ssd1306_invert(ssd1306_t *p, uint8_t inv){
	ssd1306_send_cmd(p, SET_NORM_DISP|(inv & 1));
}

void ssd1306_clear(ssd1306_t *p){
	memset(p->buffer, 0, p->bufsize);
}

void ssd1306_show(ssd1306_t *p) {

    /*
    0 表示從第 0 欄（或頁）開始
    p->width - 1 表示最後一個col的index(寫到最後一列)
    p->pages - 1 表示最後一個 page（寫到最後一個page）
    */

	ssd1306_send_cmd(p, SET_COL_ADDR);
	ssd1306_send_cmd(p, 0);
	ssd1306_send_cmd(p, p->width-1);
	
	ssd1306_send_cmd(p, SET_PAGE_ADDR);
	ssd1306_send_cmd(p, 0);
	ssd1306_send_cmd(p, p->pages-1);

    *(p->buffer-1)=SSD1306_DATA;

    struct i2c_msg msg = {
	.addr = p->client->addr,
	.flags = 0,
	.len = p->bufsize +1,
	.buf = p->buffer -1,
    };
    int ret = i2c_transfer(p->client->adapter, &msg, 1);
    if (ret < 0) {
        pr_err("Failed to write to SSD1306 display\n");
    }
}

void ssd1306_clear_pixel(ssd1306_t *p, uint32_t x, uint32_t y){
	if(x>=p->width || y>=p->height) return;
    p->buffer[(y>>3) * p->width + x] &= ~(1<<(y & 7));
}

/*
x + p->width * (y >> 3)
這是計算出目標pixel(x, y) 在 buffer 陣列中的index。
y >> 3 等於 y / 8，算出 y 所在的 page 編號
p->width * (y >> 3) 表示目前是第幾個 page，每頁有 width 個 byte
加上 x，就定位到這個 page 的第 x 個 column，即 buffer中的第幾個byt    e位址

1 << (y & 7) 這是要設亮的 bit。
y & 7 就是 y % 8，因為每 byte 控制 8 個 pixel，我們要知道是哪一個     bit
從第0個bit開始算
*/


void ssd1306_draw_pixel(ssd1306_t *p, uint32_t x, uint32_t y){
	if(x>=p->width || y>=p->height) return;

    p->buffer[(y>>3) * p->width + x]|= 1<<(y & 7); // y>>3==y/8 && y&0x7==y%8
}

/*
font[0] = 字型高度（pixel） 8
font[1] = 字型寬度（pixel） 5
font[2] = 字跟字之間的space
font[3] = 字元範圍起始ASCII編碼（32）
font[4] = 字元範圍結束編碼（126）
font[5...] = 每個字元的點陣圖資料
*/



//draw one char
void ssd1306_draw_char_with_font(ssd1306_t *p, uint32_t x, uint32_t y, const uint8_t *font, char c) {
    if(c<font[3]||c>font[4])
        return; //return if exceed the range of ASCII

    //uint32_t parts_per_line=(font[0]>>3)+((font[0]&7)>0);
    /*
    計算「這個字型一個字元的點陣，在垂直方向要用幾個 byte 來表示」
    parts_per_line = height / 8 + (height % 8 != 0) 若引用的字型為8x5，8剛好整除，所以1個字型在垂直方向用了一個byte
    */
	//每次處理字元的一行pixel（垂直方向），總共會做 font[1] 次（即字寬的pixel數）
    for(uint8_t col=0; col<font[1]; col++) { // width
        uint32_t font_idx= 5 + (c-font[3]) * font[1] + col;
		
		//第0列(前五個資訊)+從32數來第幾個字元*字型寬度*高度幾個byte + 第幾個col
        uint8_t line=font[font_idx];
        for(int8_t j=0; j<8; j++, line>>=1) {
                if(line & 1)
                    ssd1306_draw_pixel(p, x + col , y +j);
        }
    }
}

//draw one char with a given font
void ssd1306_draw_char(ssd1306_t *p, uint32_t x, uint32_t y, char c){
	ssd1306_draw_char_with_font(p, x, y, font_8x5, c);
} 


//draw string 
void ssd1306_draw_string_with_font(ssd1306_t *p, uint32_t x, uint32_t y, const uint8_t *font, const char *s){
    uint32_t cursor_x = x;
    uint32_t cursor_y = y;
    uint32_t char_width = font[1]+font[2];
    uint32_t char_height = font[0];
    
    while (*s != '\0'){
	
	
	if (cursor_x + char_width > p->width){
		cursor_x = x;
		cursor_y += char_height;
	}
	if (cursor_y + char_height > p->height){
		break;
	}
	
	ssd1306_draw_char_with_font(p, cursor_x, cursor_y, font, *(s++)); 
	cursor_x += char_width;
	
	 	
	if (*s == ' '){
		cursor_x += char_width;
		++s;
	}
	else if (*s == '\n'){
		cursor_x = x;
		cursor_y += char_height;
		++s;
	}
    }
} 


//draw string with a given font
void ssd1306_draw_string(ssd1306_t *p,uint32_t x, uint32_t y, const char *s){
	ssd1306_draw_string_with_font(p, x, y, font_8x5, s);
}

