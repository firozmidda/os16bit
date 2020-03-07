#include <keyboard.h>
#include <stdio.h>


char getch()
{
	char ascii,scan;
	 __asm
	 {
		 mov ah,00h
		 int 16h
         mov ascii,al
		 mov scan,ah
	 }
	   
	 return ascii;
	 
}
char *gets(char *str)
{
	char ch;
	int pos=0;
        str[pos]=0; 
	while(1)
	{
		ch=getch();
		if(ch==0);
		
		if(ch==13)
		{
			str[pos]='\0';
			str[pos+1]=0;
			break;
		}
        else if(ch=='\b')
        {
            if(pos!=0)
            {
                pos--;
                putc('\b');                             
            }
        }
    	else if(ch!=0)
		{
			putc(ch);
            str[pos]=ch;
			pos++;
                        
		}
	}
	return str;
}
