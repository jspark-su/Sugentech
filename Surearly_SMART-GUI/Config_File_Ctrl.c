#include "main.h"


int Save_Config_Data(int panel, char *filename)
{
	Get_Standard_Stick_Data(tabPanel_QA);
    Get_Standard_Stick_Tolerance_Data(tabPanel_QA);
    
	int ret = 0;
	FILE *fp;
	
	if((fp = fopen(filename, "wt")) != NULL)
	{        
        //QA Standard stick data
		fprintf(fp, "QA_STD_STICK_DATA_CH1=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[1] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH2=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[2] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH3=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[3] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH4=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[4] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH5=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[5] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH6=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[6] * 10));
        fprintf(fp, "QA_STD_STICK_DATA_CH7=%d\n", (int)(DATA_BANK.QA_STD_STICK_RAW_DATA[7] * 10));
        
        //QA Tolerance data
        fprintf(fp, "QA_TOLERANCE_DATA_CH1=%d\n",DATA_BANK.QA_TOLERANCE_DATA[1]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH2=%d\n",DATA_BANK.QA_TOLERANCE_DATA[2]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH3=%d\n",DATA_BANK.QA_TOLERANCE_DATA[3]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH4=%d\n",DATA_BANK.QA_TOLERANCE_DATA[4]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH5=%d\n",DATA_BANK.QA_TOLERANCE_DATA[5]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH6=%d\n",DATA_BANK.QA_TOLERANCE_DATA[6]);
        fprintf(fp, "QA_TOLERANCE_DATA_CH7=%d\n",DATA_BANK.QA_TOLERANCE_DATA[7]);
        
		fclose(fp);
		ret = 1;
	}
	else{
		ret = 0;	
	}
	return ret;
}


int Load_Config_Data(int panel, char *filename)
{
	int ret = 0;
	FILE *fp;
	char Buff_item[256];
	
	if((fp = fopen(filename, "rt")) != NULL)
	{        
        //Standard stick data
        DATA_BANK.QA_STD_STICK_RAW_DATA[1] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH1=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[2] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH2=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[3] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH3=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[4] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH4=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[5] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH5=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[6] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH6=", Buff_item)) / 10.0;
        DATA_BANK.QA_STD_STICK_RAW_DATA[7] = (float)atoi(GetStringItem(fp, "QA_STD_STICK_DATA_CH7=", Buff_item)) / 10.0;
		//
        
        //Standard stick tolerance data
        DATA_BANK.QA_TOLERANCE_DATA[1] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH1=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[2] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH2=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[3] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH3=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[4] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH4=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[5] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH5=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[6] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH6=", Buff_item));
        DATA_BANK.QA_TOLERANCE_DATA[7] = atoi(GetStringItem(fp, "QA_TOLERANCE_DATA_CH7=", Buff_item));
        //
        
		Set_Standard_Stick_Data(tabPanel_QA);
        Set_Standard_Stick_Tolerance_Data(tabPanel_QA);
        
		fclose(fp);
		ret = 1;
		
	}
	else{
		ret = 0;	
	}
	return ret;	
}


char *GetStringItem(FILE *fp, char *ItemName, char *val)
{
    static char buff[256];
    static long curr_position;
    static int Len_ItemName, indx, ret;
    
    curr_position=ftell(fp);
    Len_ItemName=strlen(ItemName);

    val[0]=0;
    ret=0;
    // 파일의 처음으로 이동
    fseek (fp, 0L, SEEK_SET);
    while( fgets (buff, 256, fp) != NULL)
    {
        if( !strncmp (buff, ItemName, Len_ItemName) )
        {
            strcpy(val, buff+Len_ItemName);
            ret=1;
            break;
        }
    }
    
    if(ret)
    {
        // 문자열 끝까지 조사해서 '\n' 문자는 Null로 만들것.
        for(indx=0;indx < 256;indx++)
        {
            if(!val[indx]) break;   // NULL문자를 만나면 탈출
            if(val[indx] == '\n')     // NULL 문자 이전에 '\n'을 만나면 NULL로 대체
            {
                val[indx]=0;
                break;
            }
        }
    }
    
    // 파일의 원래 위치에 포인터를 이동
    fseek(fp, curr_position, SEEK_SET);
    return val;
}
