#include <common.h>

void single_block_decipher(int* key, int* text, int* expected , bool is_128key);
void main() {
    mgmt_gpio_wr(0);
    mgmt_gpio_o_enable();
    enable_user_interface();
    for (int i =0; i < 3;i++ ) {
        configure_gpio(i,GPIO_MODE_MGMT_STD_OUTPUT);
    }
    gpio_config_load();
    int key[] = {0x2b7e1516,0x28aed2a6,0xabf71588,0x09cf4f3c,0x0,0x0,0x0,0x0};
    int text[] = {0xf5d3d585,0x03b9699d,0xe785895a,0x96fdbaaf};
    int expected[] = {0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51};
    single_block_decipher(key,text,expected,true);
    mgmt_gpio_wr(1);
    int key2[] = {0x00010203,0x04050607,0x08090a0b,0x0c0d0e0f,0x10111213,0x14151617,0x18191a1b,0x1c1d1e1f};
    int text2[] = {0x8ea2b7ca, 0x516745bf, 0xeafc4990, 0x4b496089};
    int expected2[] = {0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff};
    single_block_decipher(key2,text2,expected2,false);
    mgmt_gpio_wr(0);
}

void single_block_decipher(int* key, int* text, int* expected , bool is_128key) {
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
        write_user_double_word(0x0,0x0a);// configuration
    }else{
        write_user_double_word(0x2,0x0a);// configuration
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