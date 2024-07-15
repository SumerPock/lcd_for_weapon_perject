

#ifndef _BSP_LCDUI_
#define _BSP_LCDUI_

void DISPlay_Voltage_2(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Voltage_1(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Current_1(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Current_2(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Poweramp_1(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Poweramp_2(unsigned char* gettemp , unsigned char gettrmplen);
void DISPlay_Open(void);
void DISPlay_FUN4(void);

void DISPlay_FUN3(void);

void DISPlay_FUN2(void);

void DisplayElectricity(unsigned char data);

void DISPlay_Battery_Voltage(unsigned short getvol);

void DISPlay_Title(void);

void DISPlay_Function_1(unsigned char selected);

void DISPlay_Function_2(unsigned char selected);

void DISPlay_Function_3(unsigned char selected);

void DISPlay_Function_4(unsigned char selected);

void DISPlay_Battery_electric_current(unsigned short getcul);

void DISPlay_Temperature(unsigned char* gettemp , unsigned char gettrmplen);

//void DISPlay_image_park();
//void DISPlay_image_park(enum DYTState dyts , enum DYTPlace dytp);
void DISPlay_DrawCircle_Break(void);


void LCD_DrawCircle(int xc,int yc,int r,int color);

void DISPlay_self_test();

//void UI_CilckFun(unsigned char cilck);

void DISPlay_FUN1_Break(void);

void DISPlay_FUN1(void);
unsigned char UI_CilckFun(unsigned char cilck);

void DISPlay_Return(void);
#endif


