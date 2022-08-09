// Microcontrollers Laboratory
// Practice 9
// Esteban Lavín Treviño

// Libraries and Headers
#include "Config_header.h" // Include the config header file

// Directives
#define _XTAL_FREQ 10000000 // frequency


// Constants

        
        
// Function Prototypes
void Ports_configuration(void);                     // Ports configuration.
void release(void);                                 // Release function
void debounceSR(void);
void delay_1s(void);                                // delay 1 second
void __interrupt (high_priority) high_isr(void);    // High priority Interrupt Service Request.
void __interrupt (low_priority) low_isr(void);      // Low priority Interrupt Service Request.

int i = 1;
int button_pressed = 0;

// Main Function
void main(void){
    Ports_configuration(); // ports configuration function
    while (1) {                         // infinite loop   

        switch (i) {
            case 1:
                LATD = 0b00010100; // TL1 RED - TL2 GREEN
                i = 2; // state 2
                delay_1s(); //delay 1s
                delay_1s(); //delay 1s
                delay_1s(); //delay 1s
                break;
            case 2:
                LATD = 0b00010010; // TL1 RED - TL2 YELLOW
                if (button_pressed > 0) // if there's a pedestrian request
                    i = 3; // state 3
                else
                    i = 4; // state 5
                delay_1s(); //delay 1s
                break;
            case 3:
                LATD = 0b10011001; // TL2 & TL1 RED - TL3 & TL4 GREEN
                switch(button_pressed) {
                    case 1:
                        delay_1s(); //delay 1s
                        break;
                    case 2:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 3:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 4:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 5:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 6:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 7:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                    case 8:
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        delay_1s(); //delay 1s
                        break;
                }
                i = 4; // state 4
                button_pressed = 0; // clear pedestrian requests
                break;
                
            case 4:
                LATD = 0b01000001; // TL1 GREEN - TL2 RED
                i = 5; // state 5
                delay_1s(); //delay 1s
                delay_1s(); //delay 1s
                delay_1s(); //delay 1s
                break;
                
            case 5:
                LATD = 0b00100001; // TL1 YELLOW - TL2 RED
                i = 1; // state 1
                delay_1s(); //delay 1s
                break;
                    
        }
    }              
}     
 

// Function Definition
void Ports_configuration(void){//            Ports configuration.
    // PORTB
    ANSELB = 0x00;    // Digital
    TRISB = 0xFF;    // Input

    // PORTD
    LATD = 0x00; // starting value 0x00
    ANSELD = 0x00; // Digital
    TRISD = 0x00; // Output
    
    // Oscillator Configuration
    OSCCON = 0b01110010; // 16 MHz
    
    
    // interrupt configuration
    RCONbits.IPEN = 0; // enable interrupt priority 
    INTCONbits.GIE = 1; // global interrupt enabled
    INTCONbits.PEIE = 0; // enable peripheral interrupts
    INTCONbits.TMR0IE = 0; // timer 0 interrupt enabled
    INTCONbits.INT0IE = 1; // external interrupt enabled
    INTCONbits.TMR0IF = 0; // timer 0 flag cleared
    INTCONbits.INT0IF = 0; //interrupt flag cleared
    INTCON2bits.INTEDG0 = 0; //interrupt on falling edge
}

void release(void){                      // Release function
    __delay_ms(50);                      // delay
    while(1){                            // infinite loop until returned
        if((PORTB & 0x0F) == 0x0F) {    // if button is released
            LATC = 0xFF;                // LATC = 0xFF
            __delay_ms(100);            // delay
            return;                     // break loop
        }
    }
    __delay_ms(50);                     // delay
}

void delay_1s(void){
    TMR0H = 0x0B;                     // high byte 0x0BDC
    TMR0L = 0xDC;                     // low byte 0x0BDC
    INTCONbits.TMR0IF = 0;          //clear the timer overflow flag
    T0CON = 0b10000101;             // 64 pre-scaler
    while(INTCONbits.TMR0IF == 0);  // wait for overflow
    T0CON = 0x00;                   // stop the timer
}

void __interrupt (high_priority) high_isr(void){//  High priority Interrupt Service Request.
    if(INTCONbits.INT0IF){                      // if interrupt flag 0 is enabled
        button_pressed = button_pressed+1;      // button delay counter + 1
        INTCONbits.INT0IF = 0;                  // clear flag
    }
}
void __interrupt (low_priority)  low_isr(void){//   Low priority Interrupt Service Request.
    Nop();
}
