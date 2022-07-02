/////////////////////////////////////////////////////////////ESP DATA STRUCTURES AND VARIABLES////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////I2C BUS//////////////////////////////////////////////////
#define GRANTED D2
#define DEMAND D1
enum ESP_STATES { IDDLE, REQUESTING_ACCESS, MASTER };
ESP_STATES ESP_STATE;
bool trigger;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////COMMUNICATION DATA ///////////////////////////////////////////////
enum COMMANDS {
    EQUAL_PRIORITY = 0x61, SET_1 = 0x62, SET_2 = 0x63, MAINTENANCE = 0x64,
    BARRIER = 0x65, MOTOR_RACE = 0x66, _3COLOR_OFF = 0x67, _3COLOR_HAZARD = 0x68,
    _3COLOR_EMERGENCY = 0x69, _3COLOR_DOCTOR = 0x6A, _3COLOR_POLICE = 0x6B,
    GET_STATUS = 0x6C
};
enum REQUEST_STATUSES
{
    ACK = 0xFF,
    NAK = 0x00
};
enum COMMUNICATION_STATES
{ 
    CHECK_COMMAND_REQUEST, 
    WAIT_FOR_MASTER, 
    SEND_COMMAND
};
COMMANDS COMMAND;
REQUEST_STATUSES COMMAND_STATUS;
COMMUNICATION_STATES COMMUNICATION_STATE;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////RESPONSE DATA//////////////////////////////////////////////////////////////////////
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
    CLS_ACTIVE_MODE = 0x41,
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
    TL_DANGER = 0x44
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char STATUS_DATA_RESPONSE[11];
char TEXT_FOR_TEMPLATE[8][30];
unsigned long GET_STATUS_CLOCK;
byte* ptrCOMMAND_STATUS;
unsigned long int timePast;
bool get_status;
bool send_command;
/// de adaugat response data structure din arduino
