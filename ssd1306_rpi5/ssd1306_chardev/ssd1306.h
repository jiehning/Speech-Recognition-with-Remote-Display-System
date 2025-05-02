#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <linux/types.h>	   // uint8_t
#include <linux/i2c.h>         // struct i2c_client
#include <linux/kernel.h>      // printk 
#include <linux/slab.h>        // kmalloc/kfree
#include <linux/uaccess.h>     // user-space copy_from_user

#define SSD1306_CMD 0x00    //��ܭn�ecommand
#define SSD1306_DATA 0x40   //��ܭn�edata
#define SET_CONTRAST 0x81   //�]�w���סA�̫G��0xff
#define SET_ENTIRE_ON 0xA4  //��ܥ�����RAM���e
#define SET_NORM_DISP 0xA6  //���`��ܡAnot inverted
#define SET_DISP 0xAE       //�]�w��ܶ}�ҩ������A�w�]0xAE�������A (0xAE | 0x01)=0xAF ���}��
#define SET_MEM_ADDR 0x20   //�]�w�O�����m�Ҧ��A���ۿ�J0x00��ܤ���mode
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_SATRT_LINE 0x40 //�]�w��ܾ����_�l��A128x32�N��32��A�q�`�]0��ܱq�Ĥ@��}�l���
#define SET_SEG_REMAP 0xA0  //�]�w����½��A�n½��: SET_SEG_REMPA | 0x01�A��½��: SET_SEG_REMPA | 0x00�A�]�w½��~�O�r�骺���`���
#define SET_COM_OUT_DIR 0xC0     //�]�w����½��
#define SET_MUX_RATIO 0xA8      //�]�w��ܦ�ơA���128x32�A�|�](32-1)�A�]��index�q0�}�l
#define SET_DISP_OFFSET 0xD3    //�]�w��ܤ��e����m�A�q�`�]0
#define SET_COM_PIN_CFG 0xDA    //�]�w���yCOM����V�A�M�w�����Τ���
#define SET_DISP_CLK_DIV 0xD5   //�]�w�������W�A�N�쥻��clock�T�����C�W�v���B�z�A�Hí�w�T���A0x80���w�]
#define SET_PRECHARGE 0xD9      //�]�w�w�R�q�ɶ��A�~���q���]0x22�A�����]0xF1
#define SET_VCOM_DESEL 0xDB     //�]�wVCOM���l�q�A�v�T��ܾ����¥չ��A�`�]0x30��0x40�ӽվ�VCOM���q���H��
#define SET_CHARGE_PUMP 0x8D    //�Ұ�charge pump�]�w�A0x10��ܥ~���A0x14��ܤ����A�q�`�]�w0x14
 
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


