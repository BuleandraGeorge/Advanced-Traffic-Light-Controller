#include "INDEX_HTML.h"
#include "Styles.h"
void handleRoot()
{
  get_status = true;
  for(int i=1; i<4;i++)
  {
    switch (STATUS_DATA_RESPONSE[i])
            {
            case 0x81:
                strcpy(TEXT_FOR_TEMPLATE[i], " EQUAL_PRIORITY");
                break;
            case 0x82:
                strcpy(TEXT_FOR_TEMPLATE[i], " PRIORITY SET 1");
                break;
            case 0x83:
                strcpy(TEXT_FOR_TEMPLATE[i], " PRIORITY SET 2");
                break;
            case 0x84:
                strcpy(TEXT_FOR_TEMPLATE[i], " MAINTENANCE");
                break;
            case 0x85:
                strcpy(TEXT_FOR_TEMPLATE[i], " BARRIER CROSSING");
                break;
            case 0x86:
                strcpy(TEXT_FOR_TEMPLATE[i], " MOTOR RACING");
                break;
            case 0x91:
                strcpy(TEXT_FOR_TEMPLATE[i], " EQUAL PRIORITY");
                break;
            case 0x92:
                strcpy(TEXT_FOR_TEMPLATE[i], " PRIORITY SET 1");
                break;
            case 0x93:
                strcpy(TEXT_FOR_TEMPLATE[i], " PRIORITY SET 2");
                break;
            case 0x94:
                strcpy(TEXT_FOR_TEMPLATE[i], " MAINTENANCE");
                break;
            case 0x95:
                strcpy(TEXT_FOR_TEMPLATE[i], " BARRIER CROSSING");
                break;
            case 0x96:
                strcpy(TEXT_FOR_TEMPLATE[i], " MOTOR RACING");
                break;
            case 0x31:
                strcpy(TEXT_FOR_TEMPLATE[i], " SET 1 CURRENTLY ACTIVE");
                Serial.println("set 1");
                break;
            case 0x32:
                strcpy(TEXT_FOR_TEMPLATE[i], " SET 2 CURRENTLY ACTIVE");
                Serial.println("set 2");
                break;
            case 0x4D:
                strcpy(TEXT_FOR_TEMPLATE[i], " MAINTENANCE");
                break;
            case 0x49:
                strcpy(TEXT_FOR_TEMPLATE[i], " IDLE");
                break;
            case 0x41:
                strcpy(TEXT_FOR_TEMPLATE[i], " ACTIVE");
                break;
            default:
                break;
            }
  }
  
        switch (STATUS_DATA_RESPONSE[4])
        {
        case 0x4F:
            strcpy(TEXT_FOR_TEMPLATE[4], " OFF ");
            break;
        case 0x41:
            strcpy(TEXT_FOR_TEMPLATE[4], " HAZARD ");
            break;
        case 0x42:
            strcpy(TEXT_FOR_TEMPLATE[4], " EMERGENCY ");
            break;
        case 0x47:
            strcpy(TEXT_FOR_TEMPLATE[4], " DOCTOR ON CALL ");
            break;
        case 0x52:
            strcpy(TEXT_FOR_TEMPLATE[4], " POLICE CAR STOPPED ");
            break;
        default:
            break;
        }
        sprintf(TEXT_FOR_TEMPLATE[5],"%d", STATUS_DATA_RESPONSE[6]);
        switch (STATUS_DATA_RESPONSE[5])
        {
        case 0x47:
            strcpy(TEXT_FOR_TEMPLATE[6], " GOOD ");
            strcpy(TEXT_FOR_TEMPLATE[0], " good-tilt");
            break;
        case 0x44:
            strcpy(TEXT_FOR_TEMPLATE[6], " DANGER ");
            strcpy(TEXT_FOR_TEMPLATE[0], " danger-tilt");
            break;
        default:
            break;
        }
        sprintf(TEXT_FOR_TEMPLATE[7],"%d",timePast);
  sprintf(
            INDEX_HTML,
            TEMPLATE,
            TEXT_FOR_TEMPLATE[0],
            TEXT_FOR_TEMPLATE[1],
            TEXT_FOR_TEMPLATE[2],
            TEXT_FOR_TEMPLATE[3],
            TEXT_FOR_TEMPLATE[4],
            TEXT_FOR_TEMPLATE[5],
            TEXT_FOR_TEMPLATE[6],
            TEXT_FOR_TEMPLATE[7]
        );
    server.send(200, "text/html", INDEX_HTML);
}
void handleStyle()
{
    server.send(200, "text/css", styles);
}
void handleEqualP()
{
    COMMAND = EQUAL_PRIORITY;
    send_command = true;
     handleRoot();
}
void handleSET1()
{
    COMMAND = SET_1;
    send_command = true;
     handleRoot();
}
void handleSET2()
{
    COMMAND = SET_2;
    send_command = true;
     handleRoot();
}
void handleMAINTENANCE()
{
    COMMAND = MAINTENANCE;
    send_command = true;
     handleRoot();
}
void handleBARRIER_CROSSING()
{
    COMMAND = BARRIER;
    send_command = true;
     handleRoot();
}
void handleMOTOR_RACING()
{
    COMMAND = MOTOR_RACE;
    send_command = true;
     handleRoot();
}
void handleLED_OFF()
{
    COMMAND = _3COLOR_OFF;
    send_command = true;
     handleRoot();
}
void handleLED_AMBER()
{
    COMMAND = _3COLOR_HAZARD;
    send_command = true;
    handleRoot();
}
void handleLED_GREEN()
{
    COMMAND = _3COLOR_DOCTOR;
    send_command = true;
     handleRoot();
}
void handleLED_BLUE()
{
    COMMAND = _3COLOR_EMERGENCY;
    send_command = true;
     handleRoot();
}
void handleLED_RED()
{
    COMMAND = _3COLOR_POLICE;
    send_command = true;
    handleRoot();
}
void handleGET_STATUS()
{
    get_status=true;
    handleRoot();
}
