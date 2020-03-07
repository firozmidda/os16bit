;------------------------------------------------------------

.186						   ; CPU type
;------------------------------------------------------------
.model TINY
							   ; memory of model
;---------------------- EXTERNS -----------------------------
extrn				_KMain:near	   ; prototype of C func
     
;------------------------------------------------------------
;------------------------------------------------------------   
.code   
org				0000h		   ; for BootSector
 
main:
				jmp short start	   ; go to main

				nop
						
;----------------------- CODE SEGMENT -----------------------


start:
    
       
   cli        
        mov ax,cs
        mov ds,ax
        mov es,ax
        mov ss,ax
        mov sp,0ffffh
          
   sti
   
           
       call        _KMain
 
        mov ah,00
        int 16h   
        hlt


      

end   
               
               

        END main                ; End of program
