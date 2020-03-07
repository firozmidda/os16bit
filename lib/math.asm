			
.model small
.code

public _mod32
public _div32
public _mul32
public _make_fp
public _break_fp
public _mk_fp
public _fp_seg

_div32 proc near

    push bp
    mov bp,sp
    push si
    mov bx,2[bp+2];
    mov ax,2[bp+4];
    mov cx,2[bp+6];
    xor dx,dx
    div cx
    mov si,ax
    mov ax,bx
    div cx
    mov cx,dx
    mov dx,si
   
    pop si
    pop bp
ret
_div32 endp


_mod32 proc near

    push bp
    mov bp,sp
    push si
    mov bx,2[bp+2];
    mov ax,2[bp+4];
    mov cx,2[bp+6];
    xor dx,dx
    div cx
    mov si,ax
    mov ax,bx
    div cx
    mov ax,dx
     
    pop si
    pop bp
ret
_mod32 endp

_mul32 proc near
     
     push bp
     mov bp,sp
     push si
     push di
     mov ax,2[bp+2];
     mov bx,2[bp+4];
     mov cx,2[bp+6];
     xor dx,dx
     mul cx
     mov si,ax
     mov di,dx
     mov ax,bx
     mul cx
     add ax,di
     mov dx,ax
     mov ax,si

     pop di      
     pop si
     pop bp

ret
_mul32 endp
   

_make_fp proc near
 
       push bp
       mov bp,sp
       
       mov bx,2[bp+2]
       mov ax,2[bp+4]
       cmp ax,00h
       je finish
       mov cx,1000h
       mul cx
       
      
finish:
       mov dx,ax
       mov ax,bx
       
      pop bp
      ret 

_make_fp endp


_break_fp proc near
    push bp
    mov bp,sp
    mov bx,2[bp+2]
    mov ax,2[bp+4]
    cmp ax,00h
    je  finish
    mov dx,00
    mov cx,1000h
    div cx
     
finish:
     mov dx,ax
     mov ax,bx
      
    pop bp
    ret
    
_break_fp endp


_mk_fp proc near

       push bp
       mov bp,sp
       mov dx,2[bp+2]
       mov ax,2[bp+4]
       pop bp
       ret

_mk_fp endp

_fp_seg proc near
  
        push bp
        mov bp,sp
        mov ax,2[bp+4]
        pop bp
        ret
_fp_seg endp  



end
           
  
