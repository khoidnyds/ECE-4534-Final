
#include "debug_if.h"

void intToString(int inInt, char* outStr){
    if(inInt==0){
        strcpy(outStr, "0");
        return;
    }
    int sign = 1;
    if(inInt<0){
        sign=-1;
        inInt*=-1;
    }
    char tempRev[BIG_STR];

    int i=0;
    while (inInt>0){
        tempRev[i]=(char)(inInt%10)+'0';
        inInt/=10;
        i++;
    }
    if(sign<0){
        tempRev[i]='-';
        i++;
    }
    int j=-1;
    outStr[i]='\0';
    while(i>0){
        i--;
        j++;
        outStr[j]=tempRev[i];
    }

}

void init_gpio(){
    GPIO_init();

//    GPIO_setConfig(CONFIG_GPIO_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//
//    GPIO_write(CONFIG_GPIO_0, 0);
//    GPIO_write(CONFIG_GPIO_1, 0);
//    GPIO_write(CONFIG_GPIO_2, 0);
//    GPIO_write(CONFIG_GPIO_3, 0);
//    GPIO_write(CONFIG_GPIO_4, 0);
//    GPIO_write(CONFIG_GPIO_5, 0);
//    GPIO_write(CONFIG_GPIO_6, 0);
//    GPIO_write(CONFIG_GPIO_7, 0);

}

void dbgOutputLoc(unsigned int outLoc){
    if(outLoc > 127)
        errorHalt("\n\rbad debug location");
    else
    {
//        GPIO_write(CONFIG_GPIO_7, 1);
//        GPIO_write(CONFIG_GPIO_0, (outLoc & 0x1));
//        GPIO_write(CONFIG_GPIO_1, ((outLoc>>1) & 0x1));
//        GPIO_write(CONFIG_GPIO_2, ((outLoc>>2) & 0x1));
//        GPIO_write(CONFIG_GPIO_3, ((outLoc>>3) & 0x1));
//        GPIO_write(CONFIG_GPIO_4, ((outLoc>>4) & 0x1));
//        GPIO_write(CONFIG_GPIO_5, ((outLoc>>5) & 0x1));
//        GPIO_write(CONFIG_GPIO_6, ((outLoc>>6) & 0x1));
//        GPIO_write(CONFIG_GPIO_7, 0);
    }
}

void doubleToStr(double inDouble, char* outStr, int numDec){
    intToString((int)inDouble, outStr);
    int wholeLen = strlen(outStr);
    if(numDec>0){
        outStr[wholeLen]='.';
        intToString((int)((inDouble-(int)inDouble)*pow(10, numDec)), outStr+(wholeLen+1));
    }
}

double strToDouble(const char* inStr, int start, int len){
    double sign = 1.0;
    if(inStr[start]=='-'){
        start+=1;
        len-=1;
        sign = -1.0;
    }
    int decPos = len;
    int i;
    for(i=0; i<len; i++){
        if(inStr[start+i]=='.'){
            decPos=i;
            break;
        }
    }
    double mult=pow(10, decPos-1);
    double outD = 0;
    for(i=0; i<len; i++){
        if(i==decPos)
            continue;
        outD+=(inStr[start+i]-'0')*mult;
        mult/=10;
    }
    return outD*sign;

}
void errorHalt(char* location){
    vTaskSuspendAll();
    taskENTER_CRITICAL();
    LOG_ERROR("errorHalt at location: %s", location);
    dbgOutputLoc(DEBUG_ERROR); //output "123" to show errorHalt was entered
    while(1);
}
