


#define MULTIPLIER 0x015a4e35L
#define INCREMENT 1
static long Seed = 1;
int rand(void)
{
    Seed = MULTIPLIER*Seed + INCREMENT;
    return ((int)(Seed>>16) & 0x7fff);
}
void srand(unsigned seed)
{
    Seed = seed;
}
char* itoa(int num,char* str,int radix)
{/*������*/
    char index[]="0123456789ABCDEF";
    unsigned unum;/*�м����*/
    int i=0,j,k;
    /*ȷ��unum��ֵ*/
    if(radix==10&&num<0)/*ʮ���Ƹ���*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num;/*�������*/
    /*ת��*/
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
       }while(unum);
    str[i]='\0';
    /*����*/
    if(str[0]=='-')
        k=1;/*ʮ���Ƹ���*/
    else
        k=0;
     
    for(j=k;j<=(i-1)/2;j++)
    {       char temp;
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
    return str;
}