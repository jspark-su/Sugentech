#ifndef __CONFIG_FILE_CTRL_H
 #define __CONFIG_FILE_CTRL_H

#include <stdio.h>

int Save_Config_Data(int panel, char *filename);
int Load_Config_Data(int panel, char *filename);
char *GetStringItem(FILE *fp, char *ItemName, char *val);


#endif /* __CONFIG_FILE_CTRL_H*/
