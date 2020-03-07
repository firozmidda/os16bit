.186
.model TINY
.code



public       _genint      
public       _exec

_SS dw 00
_SP dw 00 
_cur_SS dw 00

_genint proc near

   push bp
    
   push ax
    
    mov bp,sp
    
    mov al,2[bp+4]
    mov  byte ptr [gen+1],al
    pop ax
   jmp gen
gen:
    int 0h


pop bp
ret
_genint endp


_exec  proc near
  
       push bp
       mov bp,sp
       mov ax,2[bp+2]
       mov bx,2[bp+4] 
       push 7b00h
       push 0000
       
       retf
       pop bp
       ret
_exec endp
       


end
