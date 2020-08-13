/*
 * rgb.c
 *
 *  Created on: Jul 20, 2020
 *      Author: Kaito
 */

#include <task_rgb.h>

void* rgbTask(void *arg0){
    integrationTime = TCS34725_INTEGRATIONTIME_700MS;
    gain = TCS34725_GAIN_60X;
    stateRGB = STATE_CLEAR;

    I2C_init();
    I2C_Handle i2c;
    I2C_Params i2cParams;
    I2C_Transaction i2cTransaction;

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2cParams.transferMode = I2C_MODE_CALLBACK;
    i2cParams.transferCallbackFxn = i2cCallback;

    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (!i2c) {
        Message("\r\nI2C open failed");
        while(1);
    }

    uint8_t transmit_size = 1;
    uint8_t txBuffer[transmit_size];
    int i;
    for(i=0;i<transmit_size;i++){
        txBuffer[i] = 0;
    }
    uint8_t receive_size = 1;
    uint8_t rxBuffer[receive_size];
    for(i=0;i<receive_size;i++){
        rxBuffer[i] = 0;
    }

    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = transmit_size;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = receive_size;
    i2cTransaction.slaveAddress = TCS34725_ADDRESS;


    //check I2C connection
    txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_ID;
    I2C_transfer(i2c, &i2cTransaction);
    usleep(3000);

    transmit_size = 2;
    for(i=0;i<transmit_size;i++){
        txBuffer[i] = 0;
    }
    transmit_size = 2;
    receive_size = 0;
    i2cTransaction.writeCount = transmit_size;
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.readCount  = receive_size;

    //set integration time
    txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_ATIME;
    txBuffer[1] = integrationTime & 0xFF;
    I2C_transfer(i2c, &i2cTransaction);
    usleep(3000);

    //set gain
    txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_CONTROL;
    txBuffer[1] = gain & 0xFF;
    I2C_transfer(i2c, &i2cTransaction);
    usleep(3000);

    //enable RGB reading
    txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_ENABLE;
    txBuffer[1] = TCS34725_ENABLE_PON & 0xFF;
    I2C_transfer(i2c, &i2cTransaction);
    usleep(3000);

    txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_ENABLE;
    txBuffer[1] = (TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN) & 0xFF;
    I2C_transfer(i2c, &i2cTransaction);
    usleep(3000);

    /* Set a delay for the integration time.
      This is only necessary in the case where enabling and then
      immediately trying to read values back. This is because setting
      AEN triggers an automatic integration, so if a read RGBC is
      performed too quickly, the data is not yet valid and all 0's are
      returned */
    switch (integrationTime) {
    case TCS34725_INTEGRATIONTIME_2_4MS:
        usleep(3000);
        break;
    case TCS34725_INTEGRATIONTIME_24MS:
        usleep(24000);
        break;
    case TCS34725_INTEGRATIONTIME_50MS:
        usleep(50000);
        break;
    case TCS34725_INTEGRATIONTIME_101MS:
        usleep(101000);
        break;
    case TCS34725_INTEGRATIONTIME_154MS:
        usleep(154000);
        break;
    case TCS34725_INTEGRATIONTIME_700MS:
        usleep(700000);
        break;
    }


    while(1){
        msgTriggerRGBSwitch newTriggerRGBSwitch;
        if(!receiveMsgFromQueueTriggerRGBSwitch(&newTriggerRGBSwitch)){

            transmit_size = 1;
            for(i=0;i<transmit_size;i++){
                txBuffer[i] = 0;
            }
            receive_size = 2;
            for(i=0;i<receive_size;i++){
                rxBuffer[i] = 0;
            }
            i2cTransaction.writeBuf   = txBuffer;
            i2cTransaction.writeCount = transmit_size;
            i2cTransaction.readBuf    = rxBuffer;
            i2cTransaction.readCount  = receive_size;
            txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_CDATAL;
            I2C_transfer(i2c, &i2cTransaction);
            usleep(3000);

            txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_RDATAL;
            I2C_transfer(i2c, &i2cTransaction);
            usleep(3000);

            txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_GDATAL;
            I2C_transfer(i2c, &i2cTransaction);
            usleep(3000);

            txBuffer[0] = TCS34725_COMMAND_BIT | TCS34725_BDATAL;
            I2C_transfer(i2c, &i2cTransaction);
            usleep(3000);
            }
     }
}

void i2cCallback(I2C_Handle i2c, I2C_Transaction* i2cTransaction, bool success){
    size_t* rxBuffer = i2cTransaction->readBuf;
    if (i2cTransaction->readCount==1 && i2cTransaction->writeCount==1 && rxBuffer[0] != 0x44) {
        Message("\r\nI2C connect failed");
        while(1);
    }
    if(i2cTransaction->readCount==2){
        switch(stateRGB){
        case STATE_CLEAR:{
            stateRGB = STATE_RED;
            clearRaw = rxBuffer[1] << 8 | rxBuffer[0];
            break;
        }
        case STATE_RED:{
            stateRGB = STATE_GREEN;
            redRaw = rxBuffer[1] << 8 | rxBuffer[0];
            break;
        }
        case STATE_GREEN:{
            stateRGB = STATE_BLUE;
            greenRaw = rxBuffer[1] << 8 | rxBuffer[0];
            break;
        }
        case STATE_BLUE:{
            stateRGB = STATE_CLEAR;
            blueRaw = rxBuffer[1] << 8 | rxBuffer[0];

            float redRGB, greenRGB, blueRGB;
            uint32_t sum = clearRaw;
            if (clearRaw == 0) {
                redRGB = greenRGB = blueRGB = 0;
            }
            else{
                redRGB = (float)redRaw / sum * 255.0;
                greenRGB = (float)greenRaw / sum * 255.0;
                blueRGB = (float)blueRaw / sum * 255.0;
            }
            unpackedMsg outMsg;

            if(redRGB>100 && greenRGB<100 && blueRGB<100){
                strcpy(outMsg.payload, "red");
            }
            else if(redRGB<100 && greenRGB<100 && blueRGB>100){
                strcpy(outMsg.payload, "blue");
            }
            else if(redRGB<100 && greenRGB>100 && blueRGB<100){
                strcpy(outMsg.payload, "green");
            }
            else if(redRGB<100 && greenRGB<100 && blueRGB<100){
                strcpy(outMsg.payload, "black");
            }
            else if(redRGB>150 && greenRGB>150 && blueRGB>150){
                strcpy(outMsg.payload, "white");
            }
            else{
                strcpy(outMsg.payload, "unknown");
            }
            strcpy(outMsg.topic, PUB_TOPIC_RGB);

//            int success = sendToGenQueueIsr(&outMsg);
//            if(success){
//                Message("\r\nSend RGB message failed");
//                while(1);
//            }

            mqttMsg sendMsg;
            sendMsg.event = APP_MQTT_PUBLISH;
            strcpy(sendMsg.topic, outMsg.topic);
            outMsg.timestamp=1;
            outMsg.sequenceNum=1;
            strcpy(sendMsg.payload, "[");
            json_pack(&outMsg, sendMsg.payload);
            strcat(sendMsg.payload, "]");

            sendToMqttQueueIsr(&sendMsg);

            break;
        }
        default:
            break;
        }
    }
}
