enum COMMAND_REQUESTES
{
    CR_EQUAL_PRIORITY = 0x61,
    CR_SET_1 = 0x62,
    CR_SET_2 = 0x63,
    CR_MAINTENANCE = 0x64,
    CR_BARRIER = 0x65,
    CR_MOTOR_RACE = 0x66,
    CR_3COLOR_OFF = 0x67,
    CR_3COLOR_HAZARD = 0x68,
    CR_3COLOR_EMERGENCY = 0x69,
    CR_3COLOR_DOCTOR = 0x6A,
    CR_3COLOR_POLICE = 0x6B,
    CR_GET_STATUS = 0x6C
};
enum NANO_STATES
{
    IDDLE,
    SLAVE,
    MASTER
};
/// <summary>
/////////////////////////////////////////////// GET STATUS RESPONSE CODE DATA STRUCTURE////////////////////////////////////////////////////////
/// </summary>
enum REQUEST_STATUSES
{
    ACK = 0xFF,
    NAK = 0x00
};
enum ALM_CURRENT_CYCLES
{
    ALMCS_EQUAL = 0x81,
    ALMCS_SET_1 = 0x82,
    ALMCS_SET_2 = 0x83,
    ALMCS_MAINTENANCE = 0x84,
    ALMCS_BARRIER = 0x85,
    ALMCS_MOTOR_RACE = 0x86
};
enum ALM_NEXT_CYCLES
{
    ALMNC_EQUAL = 0x91, 
    ALMNC_SET_1 = 0x92, 
    ALMNC_SET_2 = 0x93, 
    ALMNC_MAINTENANCE = 0x94,
    ALMNC_BARRIER = 0x95, 
    ALMNC_MOTOR_RACE = 0x96
};
enum CURRENT_LIGHT_STATUSES
{ 
    CLS_SET_1 = 0x31, 
    CLS_SET_2 = 0x32, 
    CLS_MAINTENANCE = 0x4D,
    CLS_IDLE_MODE = 0x49,
    CLS_ACTIVE_MODE = 0x41
};
enum _3COLORED_LED_STATUSES 
{
    _3CLS_OFF = 0x4F, 
    _3CLS_AMBER = 0x41, 
    _3CLS_BLUE = 0x42, 
    _3CLS_GREEN = 0x47, 
    _3CLS_RED = 0x52 
};
enum TILT_STATUSES
{
    TS_GOOD = 0x47,
    TS_DANGER = 0x44
};
unsigned char * ptr;
unsigned char STATUS_DATA_RESPONSE[11];
REQUEST_STATUSES REQUEST_STATUS;
ALM_CURRENT_CYCLES ALM_CURRENT_CYCLE;
ALM_NEXT_CYCLES ALM_NEXT_CYCLE;
CURRENT_LIGHT_STATUSES CURRENT_LIGHT_STATUS;
_3COLORED_LED_STATUSES _3COLORED_LED_STATUS;
TILT_STATUSES TILT_STATUS;
unsigned long int nano_active_time;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES
COMMAND_REQUESTES COMMAND_REQUEST;
NANO_STATES NANO_STATE;
#define GRANTED A2
#define GRANTED_HI pinMode(GRANTED,INPUT);
#define GRANTED_LO digitalWrite(GRANTED, LOW); \
 pinMode(GRANTED, OUTPUT);
bool trigger;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////TRAFIC LIGTHS DATA/////////////////////////////////////////////////
#define R1 B00000100
#define Y1 B00001000
#define G1 B00010000
#define R2 B00100000
#define Y2 B01000000
#define G2 B10000000
enum TRAFIC_LIGHTS_MODES { TRAFFIC_LIGHTS, BARRIER, MOTOR_RACE, MAINTENANCE };
enum PRIORITY { P_EQUAL, P_SET_1, P_SET_2 };
unsigned int SET_1_TIME;
unsigned int SET_2_TIME;
TRAFIC_LIGHTS_MODES CURRENT_MODE;
TRAFIC_LIGHTS_MODES NEXT_MODE;
PRIORITY CURRENT_PRIORITY;
PRIORITY NEXT_PRIORITY;
bool INIT_TRAFFIC_LIGHTS_CONTROLLER_CLOCK = true;
bool TLC_doStep = false;
unsigned long TLC_CLOCK;
unsigned int TLC_delay;
unsigned int TLC_STEP = 0;
unsigned long TLC_ADDITIONAL_CLOCK;
///Priority

///Crossing Barrier
unsigned int TRAIN_COUNTER = 0;
////////////////////////////////
////MOTOR RACE DATA////////////
unsigned int randomTime;
///////////////////////////////
//////TRI COLOR LED/////////////
enum TRI_COLOR_STATES { OFF = 0, AMBER = 1, BLUE = 9, GREEN = 10, RED = 11 };
enum INTENSITY_STATES { RISIING, DECREASING };
TRI_COLOR_STATES TRI_COLOR_STATE;
INTENSITY_STATES INTENSITY_STATE;
bool init_INTENSITY_CLOCK;
bool INTENSITY_doStep;
unsigned long int INTENSITY_CLOCK;
unsigned long int INTENSITY_TIME;
unsigned int INTENSITY_timePast;
float INTESITY_SCALE;
int INTESITY_VALUE;

///SWITCHES DATA////////////////
//sw1
#define SW1 B000001 //A0
#define SW1_PRESSED !(PINC & SW1)
enum SWITCH_STATES { B_RELEASED, PARTIALLY, B_PRESSED, HELD, MALFUNCTION };

SWITCH_STATES SW1_STATE = B_RELEASED;
bool SW1_doStep;
bool SW1_CLOCK_INIT = true;
long SW1_CLOCK;
long SW1_PRESSED_TIME;
unsigned int SW1_delay = 1;
unsigned int SW1_IMPULSE_STATE = 0;
bool SW1_IMPULSE = false;

///SW2
#define SW2 B000010 //A0
#define SW2_PRESSED !(PINC & SW2)
SWITCH_STATES SW2_STATE = B_RELEASED;
bool SW2_doStep;
bool SW2_CLOCK_INIT = false;
long SW2_CLOCK;
long SW2_PRESSED_TIME;
unsigned int SW2_delay = 1;
unsigned int SW2_IMPULSE_STATE = 0;
bool SW2_IMPULSE = false;
///HEART_BEAT//////
enum BEAT_STATES { ON, B_OFF };
BEAT_STATES BEAT_STATE = ON;
unsigned long int BEAT_CLOCK = 0;
///// 7SEGMENT///////////////////////
#define CLOCK  B010000 // D12
#define LATCH B100000 // D13
#define DATA B000001 //D08
#define CLOCK_LOW PORTB&=~CLOCK
#define CLOCK_HIGH PORTB|=CLOCK
#define LATCH_LOW PORTB&=~LATCH
#define LATCH_HIGH PORTB|=LATCH
#define W_DATA_HIGH PORTB|=DATA // WRITE DATA HIGH
#define W_DATA_LOW PORTB&=~DATA // WRITE DATA LOW

#define CHARt B01111000
#define CHARE B01111001
#define CHARF B01110001 
#define CHARb B01111100
byte _7SEGMENT_VALUE;
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////TILT&TEMP////////////////
#define MPU_6050 0x68 // I2C address of MPU6050
#define PWR_MGMT_1 0x6B // address of PWR_MGMT_1 register
#define ACCEL_Z 0x3F
#define TEMP_ADDR 0x41
bool INIT_TILT_TEMP_CLOCK;
unsigned long TILT_TEMP_CLOCK;
bool TILT_TEMP_doStep;
unsigned char TILT_TEMP_STATE;
unsigned char TILT_TEMP_DELAY;
float fTEMP;
unsigned char TEMP;
int ACC_Z;
bool init_MPU6050 = true;
