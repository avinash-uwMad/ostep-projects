#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
Task Type Id 
1 . Put
2 . Get
3 . Delete
4 . Clear All
5 . List All
6 . NotVaid 
*/

/* 
Data Structure to Store the values
DataBase
1. int Key
2. char *arbitrary string
*/
struct DataBase
{
    char* key;
    char* arString;
};
void InitDataBase()
{
    //Clear Everything
    FILE *fpointer;
    fpointer = fopen("DB_File.txt","w");
    fclose(fpointer);
}
void PrintAll()
{
    FILE *fpointer;
    fpointer = fopen("DB_File.txt","r");
    if(fpointer == NULL )
    {
        printf("Could Not Load DataBase \n" ) ;
    } 
    char c = fgetc(fpointer);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fpointer);
    }
    fclose(fpointer);
}
uint8_t CheckComma(char *str)
{
    int len = strlen(str);

    for(int i=0;i<len;i++)
    {
        if(str[i]==',')
            return 1;
    }
    return 0;
}

void stringAppend(char *str1,char *str2)
{
    int len1 = strlen(str1);

    int len2 = strlen(str2);

    str1[len1] = ' ';
    int i;
    for(i=1;i<=len2;i++)
    {
        str1[len1+i] = str2[i-1];
    }
    str1[len1+len2+1] = '\0';

}
uint8_t Comparekey(char *temp,char *key)
{
    char string[256];
    int i=0;
    while(temp[i]!=',')
    {
        string[i]=temp[i];
        i++;
    }
    string[i]='\0';
    if(strcmp(string,key)==0)
        return 1;
    else
        return 0;
}
uint8_t SearchAndUpdateDB(char *key,char* arbiString,int type)
{
    FILE *originalFile,*tempFile;
    originalFile = fopen("DB_File.txt","r");
    //char *string = malloc( sizeof(char) * ( 500 + 1 ) );
    uint8_t flag=0;
    char string[500];
    int len = strlen(key);
    if(len==0)
        return 0;
    if(originalFile == NULL )
    {
        printf( "Could Not Load DataBase \n" ) ;
    } 
    tempFile = fopen("temp.txt","w");
    if(tempFile == NULL )
    {
        printf( "Could Not Load TempDB \n" ) ;
    }
    while(fscanf(originalFile,"%s", string) == 1)
    {
        int tempLen = strlen(string);
        if((tempLen != 0) && (Comparekey(string,key)==1) && (type == 1))
            continue;
        else if((tempLen != 0) && (Comparekey(string,key)==1) && (type==2))
        {
            //Update the existing Arbirary string with new one
            //printf("%s -> %s\n",string,key);
            flag = 1;
            fprintf(tempFile,"%s,%s\n",key,arbiString);
        }
        else
        {
            fprintf(tempFile,"%s\n",string);
        }
    }
    fclose(originalFile);
    fclose(tempFile);
    originalFile = fopen("DB_File.txt","w");
    if(originalFile == NULL )
    {
        printf( "Could Not Load DataBase \n" );
    } 
    tempFile = fopen("temp.txt","r");
    if(tempFile == NULL )
    {
        printf( "Could Not Load TempDB \n" );
    }    
    while(fscanf(tempFile,"%s", string) == 1)
    {
        fprintf(originalFile,"%s\n",string);
    }
    fclose(originalFile);
    fclose(tempFile);
    return flag;
}
void SearchDB(char *key)
{
    //uint8_t isFound = 0;
    FILE *fpointer;
    fpointer = fopen("DB_File.txt","r");
    char string[500];
    int len = strlen(key);
    if(len==0)
        return;
    while(fscanf(fpointer,"%s", string) == 1)
    {
        int tempLen = strlen(string);
        if((tempLen != 0) && (Comparekey(string,key)==1))
        {
            printf("%s \n",string);
            fclose(fpointer);
            return;
            
        }
    }
    printf("K not found\n");
    fclose(fpointer);

}
void InsertDB(struct DataBase element)
{
    uint8_t flag = SearchAndUpdateDB(element.key,element.arString,2);
    if(flag==1)
        return ;

    FILE *fpointer;
    fpointer = fopen("DB_File.txt","a");
    if(fpointer == NULL )
    {
        printf( "Could Not Load DataBase \n" ) ;
    } 
    fprintf(fpointer,"%s,%s\n",element.key,element.arString);
    fclose(fpointer);

}
int main(int argc, char *argv[])
{

    //Traversing the argv

    if(argc < 2)
    {
        printf("bad command\n");
        return -1;
    }
    int Itr = 1;
    uint8_t ArgCount = 1;
    uint8_t TaskType = 6;
    struct DataBase element;
    //FILE *fpointer;
    for(Itr = 1; Itr < argc ; Itr++)
    {
        char **strgp = &argv[Itr];
        //printf("*  %s  *\n",argv[Itr]);
        //Check if the String Contains Comma, 
        //If it does not have a comma and not a First Commad it can be a second name.
        uint8_t FlagComma = CheckComma(argv[Itr]);
        if(FlagComma == 0)
        {
            if(TaskType == 1)
            {
                if((strcmp(argv[Itr],"a")==0) || (strcmp(argv[Itr],"c")==0))
                {
                    //Do Nothing
                }
                else
                    stringAppend(element.arString,argv[Itr]);
            }
            else
            {
                //Invalid Case
            }
        }
        //printf("%d -> ",TaskType);
        if(TaskType == 1)
        {
            if((element.key!= NULL) && (element.arString!=NULL))
                InsertDB(element);
            //printf("%s -> %s \n",element.key, element.arString);
            element.key = NULL;

        }
        ArgCount = 1;
        TaskType = 6;

        while(*strgp!=NULL)
        {
            char *tempString = strsep(strgp,",");
            //printf("%s \n",tempString);
            uint8_t PrintFlag=0;
            if(ArgCount==1)
            {
                if(strcmp(tempString,"p")==0)
                {
                    //its a Put task
                    TaskType = 1;
                    //validate task
                    //printf("Lets Insert in Data Base\n");
                    ArgCount++;
                }
                else if(strcmp(tempString,"g")==0)
                {
                    TaskType = 2;
                    ArgCount++;
                }
                else if(strcmp(tempString,"d")==0)
                {
                    TaskType = 3;
                    ArgCount++;
                }
                else if(strcmp(tempString,"c")==0)
                {
                    TaskType = 4;
                    //InitDataBase();
                    ArgCount++;
                }
                else if(strcmp(tempString,"a")==0)
                {
                    TaskType = 5;
                    //InitDataBase();
                    ArgCount++;
                }
                else
                {
                    if(PrintFlag==0)
                    {
                        PrintFlag=1;
                        printf("bad command\n");
                    }
                    ArgCount++;
                }
            }
            else if(ArgCount == 2)
            {
                if((TaskType==1)||(TaskType==2)||(TaskType==3))
                {
                   //printf("%d ",atoi(tempString));
                   element.key = tempString;
                }
                else if(TaskType==4)
                {
                    if(PrintFlag==0)
                    {
                        PrintFlag=1;
                        printf("bad command\n");
                    }
                }
                else if(TaskType==5)
                {
                    if(PrintFlag==0)
                    {
                        PrintFlag=1;
                        printf("bad command\n");
                    }
                }
                ArgCount++;
            }
            else if(ArgCount==3)
            {
                //printf("%s ",tempString);
                if(TaskType==1)
                {
                    element.arString = tempString;
                    ArgCount++;
                }
                if(TaskType==2)
                {
                    if(PrintFlag==0)
                    {
                        PrintFlag=1;
                        printf("bad command\n");
                    }
                }
                if(TaskType==3)
                {
                    if(PrintFlag==0)
                    {
                        PrintFlag=1;
                        printf("bad command\n");
                    }
                }
                ArgCount++;
            }
            else
            {
                if(PrintFlag==0)
                {
                    PrintFlag=1;
                    printf("bad command\n");
                }
            }

            //printf("%s ",tempString);
        }
        if(TaskType==1)
        {
            if((ArgCount<4))
            {
               printf("bad command\n"); 
            }
        }
        if(TaskType==2)
        {
            SearchDB(element.key);
        }
        if(TaskType==3)
        {
            SearchAndUpdateDB(element.key,NULL,1);
        }     
        if(TaskType==4)
        {
            InitDataBase();
        }
        if(TaskType==5)
        {
            PrintAll();
        }
        //char *tempString = strsep(strgp,",");
        //if(strcmp(tempString,"p")!=0)
        //printf("%s \n",tempString);
        //printf("%s \n",*strgp);
    }
    if((element.key!= NULL) && (element.arString!=NULL))
    {
        if(TaskType==1)
        {
            InsertDB(element);  
            //printf("%s -> %s \n",element.key, element.arString);
            element.key = NULL; 
        } 
    }
    //fclose(fpointer);
    return 0;
}
