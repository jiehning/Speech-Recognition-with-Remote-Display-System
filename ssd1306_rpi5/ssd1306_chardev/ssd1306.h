#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <linux/types.h>	   // uint8_t
#include <linux/i2c.h>         // struct i2c_client
#include <linux/kernel.h>      // printk 
#include <linux/slab.h>        // kmalloc/kfree
#include <linux/uaccess.h>     // user-space copy_from_user

#define SSD1306_CMD 0x00    //表示要送command
#define SSD1306_DATA 0x40   //表示要送data
#define SET_CONTRAST 0x81   //設定對比度，最亮為0xff
#define SET_ENTIRE_ON 0xA4  //顯示全部的RAM內容
#define SET_NORM_DISP 0xA6  //正常顯示，not inverted
#define SET_DISP 0xAE       //設定顯示開啟或關閉，預設0xAE為關閉， (0xAE | 0x01)=0xAF 為開啟
#define SET_MEM_ADDR 0x20   //設定記憶體位置模式，接著輸入0x00表示水平mode
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_SATRT_LINE 0x40 //設定顯示器的起始行，128x32就有32行，通常設0表示從第一行開始顯示
#define SET_SEG_REMAP 0xA0  //設定水平翻轉，要翻轉: SET_SEG_REMPA | 0x01，不翻轉: SET_SEG_REMPA | 0x00，設定翻轉才是字體的正常顯示
#define SET_COM_OUT_DIR 0xC0     //設定垂直翻轉
#define SET_MUX_RATIO 0xA8      //設定顯示行數，對於128x32，會設(32-1)，因為index從0開始
#define SET_DISP_OFFSET 0xD3    //設定顯示內容的位置，通常設0
#define SET_COM_PIN_CFG 0xDA    //設定掃描COM的方向，決定垂直或水平
#define SET_DISP_CLK_DIV 0xD5   //設定時鐘分頻，將原本的clock訊號降低頻率的處理，以穩定訊號，0x80為預設
#define SET_PRECHARGE 0xD9      //設定預充電時間，外部電源設0x22，內部設0xF1
#define SET_VCOM_DESEL 0xDB     //設定VCOM的餘量，影響顯示器的黑白對比，常設0x30或0x40來調整VCOM的電壓閾值
#define SET_CHARGE_PUMP 0x8D    //啟動charge pump設定，0x10表示外部，0x14表示內部，通常設定0x14
 
#define OLED_FB_SIZE 4096 //framebuffer size = one page size

#define STRING 0x01
#define FIG 0x02

typedef struct {
    uint8_t width; 		/**< width of display */
    uint8_t height; 	/**< height of display */
    uint8_t pages;		/**< stores pages of display (calculated on initialization*/
    uint8_t address; 	/**< i2c address of display*/
    struct i2c_client *client; 	/**< i2c client device */
    uint8_t *buffer;	/**< display buffer */
    size_t bufsize;		/**< buffer size */
} ssd1306_t;

void ssd1306_send_cmd(ssd1306_t *p, uint8_t cmd);
int ssd1306_init(ssd1306_t *p, uint16_t width, uint16_t height, uint8_t address, struct i2c_client *client);
void ssd1306_deinit(ssd1306_t *p);
void ssd1306_poweroff(ssd1306_t *p);
void ssd1306_poweron(ssd1306_t *p);
void ssd1306_contrast(ssd1306_t *p, uint8_t val);
void ssd1306_invert(ssd1306_t *p, uint8_t inv);
void ssd1306_show(ssd1306_t *p, int mode);

void ssd1306_clear(ssd1306_t *p);
void ssd1306_clear_frambuf(u8 *framebuffer);
void ssd1306_clear_pixel(ssd1306_t *p, uint32_t x, uint32_t y);
void ssd1306_draw_pixel(ssd1306_t *p, uint32_t x, uint32_t y);

void ssd1306_draw_char_with_font(ssd1306_t *p, uint32_t x, uint32_t y, const uint8_t *font, char c);
void ssd1306_draw_char(ssd1306_t *p, uint32_t x, uint32_t y, char c);
void ssd1306_draw_string_with_font(ssd1306_t *p, uint32_t x, uint32_t y, const uint8_t *font, const char *s );
void ssd1306_draw_string(ssd1306_t *p,uint32_t x, uint32_t y, const char *s);

#endif


