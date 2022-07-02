void toggleLatch()
{
    LATCH_HIGH;
    LATCH_LOW;
}

void toggleCLK()
{
    CLOCK_HIGH;
    CLOCK_LOW;
}

void pushtBit(bool data)
{
    (data) ? W_DATA_HIGH : W_DATA_LOW;
    toggleCLK();
}
void WRITE_TO_DISPLAY(byte  l)
{
    byte mask = B10000000;
    for (int i = 0; i < 8; i++)
    {
        pushtBit(l & mask);
        mask = mask >> 1;
    }
    toggleLatch();
}
bool demandRequest()
{
    return (!(analogRead(A6) >> 8));
}
void processCommand(int x)
{
    COMMAND_REQUEST = Wire.read();
}
void returnSTATUS()
{
    for (int i = 0; i < sizeof(STATUS_DATA_RESPONSE); i++)
    {
        byte ch = ptr[i];
        Wire.write(ch);
    }
}
void init_MPU()
{
    init_MPU6050 = false;
    Wire.beginTransmission(MPU_6050);
    Wire.write(PWR_MGMT_1); // Power Management 1 Register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}
