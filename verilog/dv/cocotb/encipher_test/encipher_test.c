#include <common.h>

void single_block_encipher(int* key, int* text, int* expected , bool is_128key);
void main() {
    mgmt_gpio_wr(0);
    mgmt_gpio_o_enable();
    enable_user_interface();
    for (int i =0; i < 3;i++ ) {
        configure_gpio(i,GPIO_MODE_MGMT_STD_OUTPUT);
    }
    gpio_config_load();
    int key[] = {0x2b7e1516,0x28aed2a6,0xabf71588,0x09cf4f3c,0x0,0x0,0x0,0x0};
    int text[] = {0x6bc1bee2,0x2e409f96,0xe93d7e11,0x7393172a};
    int expected[] = {0x3ad77bb4,0x0d7a3660,0xa89ecaf3,0x2466ef97};
    single_block_encipher(key,text,expected,true);
    mgmt_gpio_wr(1);
    int key2[] = {0x603deb10,0x15ca71be,0x2b73aef0,0x857d7781,0x1f352c07,0x3b6108d7,0x2d9810a3,0x0914dff4};
    int text2[] = {0xf69f2445,0xdf4f9b17,0xad2b417b,0xe66c3710};
    int expected2[] = {0x23304b7a,0x39f9f3ff,0x067d8d8f,0x9e24ecc7};
    single_block_encipher(key2,text2,expected2,false);
    mgmt_gpio_wr(0);
}

void single_block_encipher(int* key, int* text, int* expected , bool is_128key) {
    // keys 
    for (int i = 0; i < 8; i++) {
        write_user_double_word(key[i],0x10 + i);
    }
    if (is_128key) {
        write_user_double_word(0x0,0x0a);// configuration 
        
    }else{
        write_user_double_word(0x2,0x0a);// configuration 
    }
    write_user_double_word(0x1,0x08); // control
    // plain text 
    for (int i = 0; i < 4; i++) {
        write_user_double_word(text[i],0x20 + i);
    }
    if (is_128key) {
        write_user_double_word(0x1,0x0a);// configuration
    }else{
        write_user_double_word(0x3,0x0a);// configuration
    }
    write_user_double_word(0x2,0x08);// control

    // reading 
    int rec;
    set_gpio_l(0x7);
    for (int i = 0; i < 4; i++) {
        rec = read_user_double_word(0x30 + i);
        if (rec != expected[i]) {
            set_gpio_l(i+1);
            break;
        }
    }   
}