#include <iostream>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

class LcdApi{
public:    
    const uint8_t LCD_CLR             = 0x01;  
    const uint8_t LCD_HOME            = 0x02;  

    const uint8_t LCD_ENTRY_MODE      = 0x04;  
    const uint8_t LCD_ENTRY_INC       = 0x02;  
    const uint8_t LCD_ENTRY_SHIFT     = 0x01;  

    const uint8_t LCD_ON_CTRL         = 0x08;  
    const uint8_t LCD_ON_DISPLAY      = 0x04;  
    const uint8_t LCD_ON_CURSOR       = 0x02;  
    const uint8_t LCD_ON_BLINK        = 0x01;  

    const uint8_t LCD_MOVE            = 0x10;  
    const uint8_t LCD_MOVE_DISP       = 0x08;  
    const uint8_t LCD_MOVE_RIGHT      = 0x04;  

    const uint8_t LCD_FUNCTION        = 0x20;  
    const uint8_t LCD_FUNCTION_8BIT   = 0x10;  
    const uint8_t LCD_FUNCTION_2LINES = 0x08;  
    const uint8_t LCD_FUNCTION_10DOTS = 0x04;  
    const uint8_t LCD_FUNCTION_RESET  = 0x30;  

    const uint8_t LCD_CGRAM           = 0x40;  
    const uint8_t LCD_DDRAM           = 0x80; 

    const uint8_t LCD_RS_CMD          = 0;
    const uint8_t LCD_RS_DATA         = 1;

    const uint8_t LCD_RW_WRITE        = 0;
    const uint8_t LCD_RW_READ         = 1;
    

    int num_lines;
    int num_columns;
    int cursor_x;
    int cursor_y;
    bool implied_newline;
    bool backlight;
    LcdApi(){
        if (num_lines > 4){
            num_lines = 4;
        }
        if (num_columns > 40){
            num_columns = 40;
        }
        cursor_x = 0;
        cursor_y = 0;
        implied_newline = false;
        backlight = true;
        display_off();
        backlight_on();
        clear();
        hal_write_command(LCD_ENTRY_MODE|LCD_ENTRY_INC);
        hide_cursor();
        display_on();

    };
    void clear(){
        hal_write_command(LCD_CLR);
        hal_write_command(LCD_HOME);
        cursor_x = 0;
        cursor_y = 0;
    }

    void show_cursor(){
        hal_write_command(LCD_ON_CTRL|LCD_ON_DISPLAY|LCD_ON_CURSOR);
    }

    void hide_cursor(){
        hal_write_command(LCD_ON_CTRL|LCD_ON_DISPLAY);
    }

    void blink_cursor_on(){
        hal_write_command(LCD_ON_CTRL|LCD_ON_DISPLAY|LCD_ON_CURSOR|LCD_ON_BLINK);
    }

    void blink_cursor_off(){
        hal_write_command(LCD_ON_CTRL|LCD_ON_DISPLAY|LCD_ON_CURSOR);
    }

    void display_on(){
        hal_write_command(LCD_ON_CTRL|LCD_ON_DISPLAY|LCD_ON_CURSOR);
    }

    void display_off(){
        hal_write_command(LCD_ON_CTRL);
    }

    void backlight_on(){
        backlight = true;
        hal_backlight_on();
    }

    void backlight_off(){
        backlight = false;
        hal_backlight_off();
    }

    void move_to(int cursor_xs,int cursor_ys){
        cursor_x = cursor_xs;
        cursor_y = cursor_ys;
        int addr = cursor_x & 0x3f;
        if (cursor_y & 1){
            addr += 0x40;
        }
        if (cursor_y & 2){
            addr += num_columns;
        }
        hal_write_command(LCD_DDRAM|addr);
    }
    void putchar(char achar){
        if (achar == '\n'){
            if (implied_newline){
                ;
            }
            else{
                cursor_x = num_columns;
            }
        }
        else{
            int bchar = (int)achar;
            hal_write_data(bchar);
            cursor_x += 1;
        }
        if (cursor_x >+ num_columns){
            cursor_x = 0;
            cursor_y += 1;
        }
        if (cursor_y >= num_lines){
            cursor_y = 0;
        }
        move_to(cursor_x,cursor_y);
    }
    void custom_char(int location, int charmap[]){
        location &= 0x7;
        hal_write_command(LCD_CGRAM|location<<3);
        hal_sleep_us(30);
        for (int i =0;i <9;i++){
            hal_write_command(charmap[i]);
            hal_sleep_us(40);
        }
        move_to(cursor_x,cursor_y);
    }
    void hal_backlight_on(){
        ;
    }
    void hal_backlight_off(){
        ;
    }
    void hal_write_command(int cmd){
        throw "NotImplementedError";
    }
    void hal_write_data(int data){
        throw "NotImplementedError";
    }
    void hal_sleep_us(int usecs){
        sleep_ms(1000);
    }
};

writeto
class I2cLcd{
public:
    int MASK_RS = 0x01;
    int MASK_RW = 0x02;
    int MASK_E  = 0x04;

    int SHIFT_BACKLIGHT = 3;
    int SHIFT_DATA      = 4;
    I2cLcd(){
        int i2c;
        int i2c_addr;


    };
};