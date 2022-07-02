#include "Wire.h"
#include "DATA_STRUCTURES.h";
#include "ADDITIONAL_FUNCTIONS.h";


void setup() {
  // put your setup code here, to run once:
    Wire.begin(8); // join the bus with address 8
    Wire.onReceive(processCommand);
    Wire.onRequest(returnSTATUS); // on request return orientation
    GRANTED_HI; // pull the granted line high
    trigger = false;
    Serial.begin(9600);
    NANO_STATE = IDDLE;
    //TRAFFIC LIGHTS SETUP/////////
    DDRD |= B11111100;
    PORTD &= B00000000;
    CURRENT_MODE = TRAFFIC_LIGHTS;
    NEXT_MODE = TRAFFIC_LIGHTS;
    ALM_CURRENT_CYCLE = ALMCS_EQUAL;
    ALM_NEXT_CYCLE = ALMNC_EQUAL;
    CURRENT_PRIORITY = P_EQUAL;
    NEXT_PRIORITY = P_EQUAL;
    CURRENT_LIGHT_STATUS = CLS_SET_1;
    _3COLORED_LED_STATUS = _3CLS_OFF;

    ////3COLOR/////////////////
    TRI_COLOR_STATE = OFF;
    INTENSITY_STATE = RISIING;
    init_INTENSITY_CLOCK = true;
    //////////////////////////////
    //SWITCHES SETUP//////////////
    DDRC &= ~(SW1 | SW2); // SET UP FOR READ
    PORTC |= SW1 | SW2;// SET UP PULL UP REZISTOR 
    /////////////////////////////
    ////7SEGMENT///
    DDRB |= (LATCH | CLOCK | DATA); // SET FOR OUTPUT
    WRITE_TO_DISPLAY(0x00);
    ////////////////////////////////
    ////////TILD AND TEMP/////////
    INIT_TILT_TEMP_CLOCK=true;
    init_MPU6050 = true;
}


void loop() {
    nano_active_time = millis();
    // put your main code here, to run repeatedly:
    switch (NANO_STATE)
    {
    case IDDLE:
        GRANTED_HI;
        if (trigger) NANO_STATE = MASTER;
        else if (demandRequest()) NANO_STATE = SLAVE;
        else NANO_STATE = IDDLE;
        break;
    case SLAVE:
        GRANTED_LO;
        if (demandRequest()) NANO_STATE = SLAVE;
        else NANO_STATE = IDDLE;
        break;
    case MASTER:
        GRANTED_HI;
        if (trigger) NANO_STATE = MASTER;
        else NANO_STATE = IDDLE;
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////COMMANDS CONTROLLER//////////////////////////////////////////////////////////////
    switch (COMMAND_REQUEST)
    {
    case CR_EQUAL_PRIORITY:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_EQUAL;
        NEXT_MODE = TRAFFIC_LIGHTS;
        NEXT_PRIORITY = P_EQUAL;
        break;
    case CR_SET_1:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_SET_1;
        NEXT_MODE = TRAFFIC_LIGHTS;
        NEXT_PRIORITY = P_SET_1;
        break;
    case CR_SET_2:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_SET_2;
        NEXT_MODE = TRAFFIC_LIGHTS;
        NEXT_PRIORITY = P_SET_2;
        break;
    case CR_MAINTENANCE:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_MAINTENANCE;
        NEXT_MODE = MAINTENANCE;
        break;
    case CR_BARRIER:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_BARRIER;
        NEXT_MODE = BARRIER;
        break;
    case CR_MOTOR_RACE:
        STATUS_DATA_RESPONSE[0]=ACK;
        ALM_NEXT_CYCLE = ALMNC_MOTOR_RACE;
        NEXT_MODE = MOTOR_RACE;
        break;
    case CR_3COLOR_OFF:
        STATUS_DATA_RESPONSE[0]=ACK;
        _3COLORED_LED_STATUS = _3CLS_OFF;
        TRI_COLOR_STATE = OFF;
        break;
    case CR_3COLOR_HAZARD:
        STATUS_DATA_RESPONSE[0]=ACK;
        _3COLORED_LED_STATUS = _3CLS_AMBER;
        TRI_COLOR_STATE = AMBER;
        break;
    case CR_3COLOR_EMERGENCY:
        STATUS_DATA_RESPONSE[0]=ACK;
        _3COLORED_LED_STATUS = _3CLS_BLUE;
        TRI_COLOR_STATE = BLUE;
        break;
    case CR_3COLOR_DOCTOR:
        STATUS_DATA_RESPONSE[0]=ACK;
        _3COLORED_LED_STATUS = _3CLS_GREEN;
        TRI_COLOR_STATE = GREEN;
        break;
    case CR_3COLOR_POLICE:
        STATUS_DATA_RESPONSE[0]=ACK;
        _3COLORED_LED_STATUS = _3CLS_RED;
        TRI_COLOR_STATE = RED;
        break;
    case CR_GET_STATUS:
        STATUS_DATA_RESPONSE[0]=REQUEST_STATUS;
        STATUS_DATA_RESPONSE[1]=ALM_CURRENT_CYCLE;
        STATUS_DATA_RESPONSE[2]=ALM_NEXT_CYCLE;
        STATUS_DATA_RESPONSE[3]=CURRENT_LIGHT_STATUS;
        STATUS_DATA_RESPONSE[4]=_3COLORED_LED_STATUS;
        STATUS_DATA_RESPONSE[5]=TILT_STATUS;
        STATUS_DATA_RESPONSE[6]=TEMP;
        unsigned char * tPTR = (unsigned char*)&nano_active_time;
        for(int i = 0; i<sizeof(nano_active_time);i++)
        {
          STATUS_DATA_RESPONSE[i+7]=(unsigned char) tPTR[i];
        }
        ptr = (unsigned char*)&STATUS_DATA_RESPONSE;
        break;
    default:
        STATUS_DATA_RESPONSE[0]=NAK;
        break;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    /////////////////////////////////////////////////////////////TRAFIC LIGHTS//////////////////////////////////////////////////////////////
    if (INIT_TRAFFIC_LIGHTS_CONTROLLER_CLOCK)
    {
        TLC_CLOCK = millis();
        TLC_delay = 1;
        INIT_TRAFFIC_LIGHTS_CONTROLLER_CLOCK = false;
    }
    else if (((long)(millis() - TLC_CLOCK)) > TLC_delay)
    {
        TLC_doStep = true;
        TLC_CLOCK = millis();
    }
    else TLC_doStep = false;
    if (TLC_doStep)
    {
        switch (CURRENT_MODE)
        {
            ///// ADD PRIORITY

        case TRAFFIC_LIGHTS:
            _7SEGMENT_VALUE = CHARE;
            switch (TLC_STEP)
            {
            case 0:
                PORTD = (Y1 | Y2);// Y1 ON Y2 ON
                TLC_delay = 2000;
                TLC_STEP = 1;
                break;
            case 1:
            switch (NEXT_PRIORITY)
                {
                case P_EQUAL:
                    SET_1_TIME = 5000;
                    SET_2_TIME = 5000;
                    ALM_CURRENT_CYCLE = ALMCS_EQUAL;
                    CURRENT_PRIORITY = NEXT_PRIORITY;
                    break;
                case P_SET_1:
                    SET_1_TIME = 7000;
                    SET_2_TIME = 3000;
                    ALM_CURRENT_CYCLE = ALMCS_SET_1;
                    CURRENT_PRIORITY = NEXT_PRIORITY;
                    break;
                case P_SET_2:
                    SET_1_TIME = 3000;
                    SET_2_TIME = 7000;
                    ALM_CURRENT_CYCLE = ALMCS_SET_2;
                    CURRENT_PRIORITY = NEXT_PRIORITY;
                    break;
                }
                PORTD = (R1 | R2);
                TLC_delay = 1000;
                TLC_STEP = 2;
                CURRENT_LIGHT_STATUS = CLS_SET_1;
                break;
            case 2:
                PORTD = (R1 | R2 | Y1);
                TLC_delay = 1000;
                TLC_STEP = 3;
                break;
            case 3:
                PORTD = (G1 | R2);
                TLC_delay = SET_1_TIME;
                TLC_STEP = 4;
                break;
            case 4:
                PORTD = (Y1 | R2);
                TLC_delay = 2000;
                TLC_STEP = 5;
                break;
            case 5:
                PORTD = (R1 | R2);
                TLC_delay = 2000;
                TLC_STEP = 6;
                CURRENT_LIGHT_STATUS = CLS_SET_2;
                break;
            case 6:
                PORTD = (R1 | R2 | Y2);
                TLC_delay = 1000;
                TLC_STEP = 7;
                break;
            case 7:
                PORTD = (R1 | G2);
                TLC_delay = SET_2_TIME;
                TLC_STEP = 8;
                break;
            case 8:
                PORTD = (R1 | Y2);
                TLC_delay = 2000;
                TLC_STEP = 1;
                PORTD &= B00000000;
                TLC_delay = 1; TLC_STEP = 1;
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                if(CURRENT_MODE != NEXT_MODE) { 
                  TLC_delay = 1; TLC_STEP = 0; CURRENT_MODE = NEXT_MODE;
                  }
                break;
            default:break;
            }
            break;
        case BARRIER:
            _7SEGMENT_VALUE = CHARb;
            ALM_CURRENT_CYCLE = ALMCS_BARRIER;
            switch (TLC_STEP)
            {
            case 0:// idle state
                CURRENT_LIGHT_STATUS = CLS_IDLE_MODE;
                PORTD &= B00000000;
                if (NEXT_MODE == TRAFFIC_LIGHTS) TLC_STEP = 1;
                else TLC_STEP = 0;
                CURRENT_MODE = NEXT_MODE;
                if (SW1_IMPULSE) { TRAIN_COUNTER = TRAIN_COUNTER + 1;}
                if (TRAIN_COUNTER > 0) { TLC_STEP = 1; TLC_ADDITIONAL_CLOCK = millis(); }
                else TLC_STEP = 0;
                SW1_IMPULSE = false; 
                SW2_IMPULSE = false;
                break;
            case 1: //state 1
                CURRENT_LIGHT_STATUS = CLS_ACTIVE_MODE;
                PORTD = (Y1 | Y2);
                if (SW1_IMPULSE) { TRAIN_COUNTER = TRAIN_COUNTER + 1; }
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 5000) { TLC_STEP = 2; TLC_ADDITIONAL_CLOCK = millis(); }
                SW1_IMPULSE = false; 
                SW2_IMPULSE = false;
                break;
            case 2: //state 2
                PORTD = (R1 | R2);
                if (SW1_IMPULSE) { TRAIN_COUNTER = TRAIN_COUNTER + 1; }
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 500) TLC_STEP = 3;
                SW1_IMPULSE = false; 
                SW2_IMPULSE = false;
                break;
            case 3:
                PORTD = R1;
                if (SW1_IMPULSE) { TRAIN_COUNTER = TRAIN_COUNTER + 1; }
                if (SW2_IMPULSE) {
                    if (TRAIN_COUNTER > 0) TRAIN_COUNTER = TRAIN_COUNTER - 1;
                }
                 if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 500) {
                     TLC_STEP = 4; 
                     TLC_ADDITIONAL_CLOCK = millis(); 
                     }
                 if(TRAIN_COUNTER ==0)   TLC_STEP=0;
                 SW2_IMPULSE = false;
                 SW1_IMPULSE = false;
                break;
            case 4:
                PORTD = R2;
                if (SW1_IMPULSE) { TRAIN_COUNTER = TRAIN_COUNTER + 1; }
                if (SW2_IMPULSE) {
                    if (TRAIN_COUNTER > 0) TRAIN_COUNTER = TRAIN_COUNTER - 1;
                }
                 if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 500) 
                 {
                     TLC_STEP = 3; 
                     TLC_ADDITIONAL_CLOCK = millis();
                 }
                 if(TRAIN_COUNTER ==0)   TLC_STEP=0;
                 SW2_IMPULSE = false;
                 SW1_IMPULSE = false;
                break;
            default:
                break;
            }
            break;
        case MOTOR_RACE:
            _7SEGMENT_VALUE = CHARF;
            ALM_CURRENT_CYCLE = ALMCS_MOTOR_RACE;
            switch (TLC_STEP)
            {
            case 0:
                PORTD = 0x00;
                CURRENT_LIGHT_STATUS = CLS_IDLE_MODE;
                if (NEXT_MODE == TRAFFIC_LIGHTS) TLC_STEP = 1;
                else TLC_STEP = 0;
                CURRENT_MODE = NEXT_MODE;
                if (SW1_IMPULSE) { TLC_STEP = 1; TLC_ADDITIONAL_CLOCK = millis(); }
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 1://start sequence
                CURRENT_LIGHT_STATUS = CLS_ACTIVE_MODE;
                PORTD = R1;
                if (SW1_IMPULSE) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); } //abort
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 1000) { TLC_STEP = 2; TLC_ADDITIONAL_CLOCK = millis(); } // next state
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 2://start sequence
                PORTD = (R1 | Y1);
                if (SW1_IMPULSE) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); } //abort
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 1000) { TLC_STEP = 3; TLC_ADDITIONAL_CLOCK = millis(); }// next state
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 3://start sequence
                PORTD = (R1 | Y1 | G1);
                if (SW1_IMPULSE) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); SW1_IMPULSE = false; SW2_IMPULSE = false; }//abort
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 1000) { TLC_STEP = 4; TLC_ADDITIONAL_CLOCK = millis(); }// next state
                break;
            case 4://start sequence
                PORTD = (R1 | Y1 | G1 | R2);
                if (SW1_IMPULSE) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); }//abort
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 1000) { TLC_STEP = 5; TLC_ADDITIONAL_CLOCK = millis(); }// next state
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 5://start sequence
                PORTD = (R1 | Y1 | G1 | R2 | Y2);
                if (SW1_IMPULSE) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); } //abort
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 1000) {// next state
                    TLC_STEP = 6;
                    TLC_ADDITIONAL_CLOCK = millis();
                    // finding a random time for state 6
                    randomTime = TLC_ADDITIONAL_CLOCK % 10; // getting the last digit of the clock
                    if (randomTime > 5) randomTime = randomTime - 4;// making sure that the digit is between 2 and 5
                    if (randomTime < 2) randomTime = randomTime + 2;// making sure that the digit is between 2 and 5
                }
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 6:// waiting green
                PORTD = 0x00;
                if (SW1_IMPULSE) { TLC_STEP = 8; TLC_ADDITIONAL_CLOCK = millis(); } //red flag
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > (1000 * randomTime)) { TLC_STEP = 7; }
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 7:
                // race underway
                if (SW2_IMPULSE) TLC_STEP = 0; // return to idle
                if (SW1_IMPULSE) { TLC_STEP = 8; TLC_ADDITIONAL_CLOCK = millis(); } //red flag
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                PORTD = (G1 | G2);
                break;
            case 8:
                // red flagged 
                PORTD = (R1 | R2);
                if (SW2_IMPULSE) { TLC_STEP = 0; } // go to idle
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 9:
                /// aborded sequence yellow
                PORTD = 0x00;
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 500) { TLC_STEP = 10; TLC_ADDITIONAL_CLOCK = millis(); }
                if (SW2_IMPULSE) { TLC_STEP = 0; }
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                break;
            case 10:
                // aborded sequence off
                if (((long)(millis() - TLC_ADDITIONAL_CLOCK)) > 500) { TLC_STEP = 9; TLC_ADDITIONAL_CLOCK = millis(); }
                if (SW2_IMPULSE) { TLC_STEP = 0; } // go to idle
                SW1_IMPULSE = false; SW2_IMPULSE = false;
                PORTD = (Y1 | Y2);
                break;
            default:
                break;
            }
            break;
        case MAINTENANCE:
            _7SEGMENT_VALUE = CHARt;
            ALM_CURRENT_CYCLE = ALMCS_MAINTENANCE;
            CURRENT_LIGHT_STATUS = CLS_MAINTENANCE;
            switch (TLC_STEP)
            {
            case 0:
                PORTD = (Y1 | Y2);
                TLC_delay = 500;
                TLC_STEP = 1;

                if (NEXT_MODE != CURRENT_MODE)
                {
                    CURRENT_MODE = NEXT_MODE;
                    if (NEXT_MODE == TRAFFIC_LIGHTS) TLC_STEP = 1;
                    else TLC_STEP = 0;
                }

                break;
            case 1:
                PORTD = 0x00;
                TLC_STEP = 0;

                if (NEXT_MODE != CURRENT_MODE)
                {
                    CURRENT_MODE = NEXT_MODE;
                    if (NEXT_MODE == TRAFFIC_LIGHTS) TLC_STEP = 1;
                    else TLC_STEP = 0;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////SW1 MODULE//////////////////////////////////////////////////////
    if (SW1_CLOCK_INIT)
    {
        SW1_CLOCK_INIT = false;
        SW1_CLOCK = millis();
    }
    else if (((long)(millis() - SW1_CLOCK)) > SW1_delay)
    {
        SW1_doStep = true;
        SW1_CLOCK = millis();
    }
    else SW1_doStep = false;
    if (SW1_doStep)
    {
        switch (SW1_STATE)
        {
        case B_RELEASED:
            if (SW1_PRESSED) { SW1_STATE = PARTIALLY; SW1_PRESSED_TIME = millis(); }
            else SW1_STATE = B_RELEASED;
            break;
        case PARTIALLY:
            if ((SW1_PRESSED) && (((long)(millis() - SW1_PRESSED_TIME)) > 300)) SW1_STATE = B_PRESSED;
            else if (!(SW1_PRESSED)) SW1_STATE = B_RELEASED;
            else SW1_STATE = PARTIALLY;
            break;
        case B_PRESSED:
            if ((SW1_PRESSED) && (((long)(millis() - SW1_PRESSED_TIME)) > 3000)) SW1_STATE = HELD;
            else if (!(SW1_PRESSED)) SW1_STATE = B_RELEASED;
            else SW1_STATE = B_PRESSED;
            break;
        case HELD:
            if ((SW1_PRESSED) && (((long)(millis() - SW1_PRESSED_TIME)) > 5000)) SW1_STATE = MALFUNCTION;
            else if (!(SW1_PRESSED)) SW1_STATE = B_RELEASED;
            else SW1_STATE = HELD;
            break;
        case MALFUNCTION:
            if (!SW1_PRESSED)SW1_STATE = B_RELEASED;
            break;
        default:
            break;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////SW2 MODULE///////////////////////////////////////////////////////////
    if (SW2_CLOCK_INIT)
    {
        SW2_CLOCK_INIT = false;
        SW2_CLOCK = millis();
    }
    else if (((long)(millis() - SW2_CLOCK)) > SW2_delay)
    {
        SW2_doStep = true;
        SW2_CLOCK = millis();
    }
    else SW2_doStep = false;
    if (SW2_doStep)
    {
        switch (SW2_STATE)
        {
        case B_RELEASED:
            if (SW2_PRESSED) { SW2_STATE = PARTIALLY; SW2_PRESSED_TIME = millis(); }
            else SW2_STATE = B_RELEASED;
            break;
        case PARTIALLY:
            if ((SW2_PRESSED) && (((long)(millis() - SW2_PRESSED_TIME)) > 300)) SW2_STATE = B_PRESSED;
            else if (!(SW2_PRESSED)) SW2_STATE = B_RELEASED;
            else SW2_STATE = PARTIALLY;
            break;
        case B_PRESSED:
            if ((SW2_PRESSED) && (((long)(millis() - SW2_PRESSED_TIME)) > 3000)) SW2_STATE = HELD;
            else if (!(SW2_PRESSED)) SW2_STATE = B_RELEASED;
            else SW2_STATE = B_PRESSED;
            break;
        case HELD:
            if ((SW2_PRESSED) && (((long)(millis() - SW2_PRESSED_TIME)) > 5000)) SW2_STATE = MALFUNCTION;
            else if (!(SW2_PRESSED)) SW2_STATE = B_RELEASED;
            else SW2_STATE = HELD;
            break;
        case MALFUNCTION:
            if (!SW2_PRESSED)SW2_STATE = B_RELEASED;
            break;
        default:
            break;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////CONTROL BUTTON IMPLUSE //////////////////////////////////////////////////////
    switch (SW1_IMPULSE_STATE)
    {
    case 0:
        if (SW1_STATE == PARTIALLY) SW1_IMPULSE_STATE = 1;
        break;
    case 1:
        if (SW1_STATE == B_PRESSED) { SW1_IMPULSE_STATE = 0; SW1_IMPULSE = true; }
        break;
    default:
        break;
    }
    switch (SW2_IMPULSE_STATE)
    {
    case 0:
        if (SW2_STATE == PARTIALLY) SW2_IMPULSE_STATE = 1;
        break;
    case 1:
        if (SW2_STATE == B_PRESSED) { SW2_IMPULSE_STATE = 0; SW2_IMPULSE = true; }
        break;
    default:
        break;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////HEART BEAT////////////////////////////////////////////////////////////////
    switch (BEAT_STATE)
    {
    case ON:
        if (((long)(millis() - BEAT_CLOCK)) > 500)
        {
            BEAT_STATE = B_OFF;
            BEAT_CLOCK = millis();
        }
        _7SEGMENT_VALUE |= B10000000;
        break;
    case B_OFF:
        if (((long)(millis() - BEAT_CLOCK)) > 500)
        {
            BEAT_STATE = ON;
            BEAT_CLOCK = millis();
        }
        _7SEGMENT_VALUE &= B01111111;
        break;
    default:
        break;
    }
    WRITE_TO_DISPLAY(_7SEGMENT_VALUE);
    ////////////////////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////TRICOLOR LED////////////////////////////////////////
    if (init_INTENSITY_CLOCK)
    {
        INTENSITY_CLOCK = millis();
        init_INTENSITY_CLOCK = false;
    }
    else if (((long)(millis() - INTENSITY_CLOCK)) > 1)
    {
        INTENSITY_doStep = true;
        INTENSITY_CLOCK = millis();
    }
    else INTENSITY_doStep = false;

    if (INTENSITY_doStep) switch (INTENSITY_STATE)
    {
    case RISIING:
        INTENSITY_timePast = millis() - INTENSITY_TIME;
        INTESITY_SCALE = (float)INTENSITY_timePast / (float)1000;
        if (((long)(millis() - INTENSITY_TIME)) > 1000)
        {
            INTENSITY_STATE = DECREASING;
            INTENSITY_TIME = millis();
        }
        break;
    case DECREASING:
        INTENSITY_timePast = millis() - INTENSITY_TIME;
        INTESITY_SCALE = (float)1 - ((float)INTENSITY_timePast / (float)1000);
        if (((long)(millis() - INTENSITY_TIME)) > 1000)
        {
            INTENSITY_STATE = RISIING;
            INTENSITY_TIME = millis();
        }
        break;
    }
    switch (TRI_COLOR_STATE)
    {
    case OFF:
        analogWrite(GREEN, 0);
        analogWrite(BLUE, 0);
        analogWrite(RED, 0);
        init_INTENSITY_CLOCK = true;
        break;
    case AMBER:
        analogWrite(GREEN, (90 * INTESITY_SCALE));
        analogWrite(RED, (255 * INTESITY_SCALE));
        analogWrite(BLUE, 0);
        break;
    case BLUE:
        analogWrite(BLUE, (255 * INTESITY_SCALE));
        analogWrite(GREEN, 0);
        analogWrite(RED, 0);
        break;
    case GREEN:
        analogWrite(GREEN, (255 * INTESITY_SCALE));
        analogWrite(BLUE, 0);
        analogWrite(RED, 0);
        break;
    case RED:
        analogWrite(RED, (255 * INTESITY_SCALE));
        analogWrite(GREEN, 0);
        analogWrite(BLUE, 0);
        break;
    default:
        break;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////MPU6050////////////////////////////////////////////////////////

    if (INIT_TILT_TEMP_CLOCK)
    {
        TILT_TEMP_CLOCK = millis();
        INIT_TILT_TEMP_CLOCK = false;
        TILT_TEMP_STATE = 0;
        TILT_TEMP_DELAY = 1;

    }
    else if (((long)(millis() - TILT_TEMP_CLOCK) > TILT_TEMP_DELAY))
    {
        TILT_TEMP_doStep = true;
        TILT_TEMP_CLOCK = millis();
    }
    else TILT_TEMP_doStep = false;
    if (TILT_TEMP_doStep) switch (TILT_TEMP_STATE)
    {
        case 0:
            /// set the waiting time;
            TILT_TEMP_DELAY = 100;
            TILT_TEMP_STATE = 1;
            break;
        case 1:
            /// after the waiting time has passed request access to the bus
            trigger = true;
            TILT_TEMP_STATE = 2;
            TILT_TEMP_DELAY = 1;
            break;
        case 2:
            // if arduino is a master get the orientation
            if (NANO_STATE == MASTER) TILT_TEMP_STATE = 3;
            else TILT_TEMP_STATE = 2;
            break;
        case 3:
            /// get the orientation and go back to initial state
            if (init_MPU6050) init_MPU(); // a safe place to initialize the MPU_6050 because here Nano is the master thus is safe to do it.
            Wire.beginTransmission(MPU_6050);
            Wire.write(ACCEL_Z);  // starting Y axis acc
            Wire.endTransmission(false);
            Wire.requestFrom(MPU_6050, 4, true); // requesting 4 registers to read the Y acc + TEMP in the same time
            ACC_Z = Wire.read() << 8 | Wire.read(); // storing Y acc
            fTEMP = Wire.read() << 8 | Wire.read(); // reading TEMP
            TEMP = (unsigned char)(((float)fTEMP / 340.00) + 36.53);
            TILT_STATUS = TS_GOOD;
            if (ACC_Z < 15500) TILT_STATUS = TS_DANGER;
            // pull the trigger low
            trigger = false;
            // return to idle state;
            TILT_TEMP_STATE = 0;
            break;
    }
}
