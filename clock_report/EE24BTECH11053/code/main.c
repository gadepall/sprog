#include <avr/io.h>
#include <util/delay.h>

#define byte uint8_t


byte incUn(byte led);
byte incTenMin(byte led);
byte incTenHr(byte led);

int main(){
    DDRB |= 0x3F;
    DDRD |= 0x3C;
    DDRC |= 0x07;

    byte L1 = 0x00;
    byte L2 = 0x00;
    byte L3 = 0x00;
    byte L4 = 0x14;
    byte L5 = 0x0C;
    byte L6 = 0x08;
    byte flip = 0x00;
    int count = 84;

    byte prevState = 0x00;
    byte buttonSelect = 0x00;

    while(1){

        if(!(L1 ^ 0x24)){ 

            if(!(L2 ^ 0x14)){

                if(!(L3 ^ 0x24)){

                    if(!(L4 ^ 0x14)){

                        if(!(L5 ^ 0x24)){

                            L6 = incTenHr(L6);
                        }

                        else if(!(L6 ^ 0x08)){
                            if(!(L5 ^ 0x0C)) {
                                L5 = 0x24;
                                L6 = 0x00;
                            }
                        }
                        L5 = incUn(L5);
                    }
                    L4 = incTenMin(L4);
                }
                L3 = incUn(L3);
            }
            L2 = incTenMin(L2);
        }
        L1 = incUn(L1);


        // PORTB |= 0xFF;
        for(int i=1;i<count;i++){
            byte buttons = PINC;

            if((PINC & (1)) != (prevState & (1))){
                if(PINC & (1)){
                    if(!(buttonSelect)){
                        buttonSelect = 0x01;
                    }
                    else{
                        buttonSelect = 0x00;
                    }
                }
            }
            // prevState ^= (1);

            if((PINC & (1<<1)) != (prevState & (1<<1))){
                if((PINC & (1<<1)) && (buttonSelect)){
                    buttonSelect = buttonSelect<<1;
                    if(buttonSelect == 0x08) buttonSelect = 0x01;
                }
            }

            if((PINC & (1<<2)) != (prevState & (1<<2)) && (buttonSelect)){
                if((PINC & (1<<2))){
                    switch (buttonSelect)
                    {
                    case 0x01:
                        L1 = 0x00;
                        L2 = 0x00;
                        break;
                    
                    case 0x02:
                        if(L3 == 0x24){
                            L4 = incTenMin(L4);
                        }
                        L3 = incUn(L3);
                        break;

                    case 0x04:
                        if(!(L5 ^ 0x24)){

                            L6 = incTenHr(L6);
                        }

                        else if(!(L6 ^ 0x08)){
                            if(!(L5 ^ 0x0C)) {
                                L5 = 0x24;
                                L6 = 0x00;
                            }
                        }
                        L5 = incUn(L5);
                    default:
                        break;
                    }
                }
            }
            // if(buttons & (1<<7))
            prevState = buttons;
            if(!(buttonSelect)){
                PORTD = L1;
                PORTB = 1;
                _delay_ms(2);

                PORTD = L2;
                PORTB = (1<<1);
                _delay_ms(2);

                PORTD = L3;
                PORTB = (1<<2);
                _delay_ms(2);

                PORTD = L4;
                PORTB = (1<<3);
                _delay_ms(2);

                PORTD = L5;
                PORTB = (1<<4);
                _delay_ms(2);

                PORTD = L6;
                PORTB = (1<<5);
                _delay_ms(2);
            }
            else{
                if(!((buttonSelect & 0x01) & (i<count/2))){
                    PORTD = L1;
                    PORTB = 1;
                }
                _delay_ms(2);
                if(!((buttonSelect & 0x01) & (i<count/2))){
                    PORTD = L2;
                    PORTB = (1<<1);
                }            
                _delay_ms(2);
                if(!((buttonSelect & 0x02)>>1 & (i<count/2))){
                    PORTD = L3;
                    PORTB = (1<<2);
                }                  
                _delay_ms(2);  
                if(!((buttonSelect & 0x02)>>1 & (i<count/2))){
                    PORTD = L4;
                    PORTB = (1<<3);
                }                   
                _delay_ms(2);
                if(!((buttonSelect & 0x04)>>2 & (i<count/2))){
                    PORTD = L5;
                    PORTB = (1<<4);
                }
                _delay_ms(2);
                if(!((buttonSelect & 0x04)>>2 & (i<count/2))){
                    PORTD = L6;
                    PORTB = (1<<5);
                }
                _delay_ms(2);                             
            }
        }
    }
}

byte incUn(byte led1){
    byte led = led1 >> 2;
    byte D = led & 1;
    byte C = (led & (1<<1))>>1;
    byte B = (led & (1<<2))>>2;
    byte A = (led & (1<<3))>>3;

    byte W = (A&(C^1)&(D^1)) | (B&C&D);
    byte X = ((B^1)&C&D) | (B&(C^1)) | (B&(D^1));
    byte Y = ((A^1)&(C^1)&D) | (C&(D^1));
    byte Z = D^1;

    byte ledOut = Z | (Y<<1) | (X<<2) | (W<<3);
    ledOut = ledOut << 2;
    return ledOut;

}

byte incTenMin(byte led1){
    byte led = led1>>2;

    byte Z = led & 1;
    byte Y = (led & (1<<1))>>1;
    byte X = (led & (1<<2))>>2;

    byte A = (Y&Z) | (X&(Y^1)&(Z^1));
    byte B = ((X^1)&(Y^1)&Z) | ((Y)&(Z^1));
    byte C = ((Y^1)&(Z^1)) | ((Y)&(Z^1));

    byte ledOut = C | (B<<1) | (A<<2);
    ledOut = ledOut << 2;
    return ledOut;
}

byte incTenHr(byte led1){
    byte led = led1 >> 2;

    byte Y = led & 1;
    byte X = (led & (1<<1))>>1;

    byte A = Y;
    byte B = (X^1)&(Y^1);

    byte ledOut = B | (A<<1);
    ledOut = ledOut << 2;
    return ledOut;

}