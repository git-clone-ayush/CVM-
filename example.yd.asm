section .data
SRef0:
db "Hello"
SRef0_L :equ $-SRef0

SRef1:
db "Hello again"
SRef1_L :equ $-SRef1

Var0: dq 0
Var1: dq 0
section .text
global _start
_start:
mov rax, 1
mov rdi, 1
mov rsi, SRef0
mov rdx, SRef0_L
syscall
mov rax, 1
mov rdi, 1
mov rsi, SRef1
mov rdx, SRef1_L
syscall
mov rax, 10
mov [Var0], rax
mov rax, 2
mov [Var1], rax
mov rax, [Var0]
push rax
mov rax, [Var1]
mov rbx, rax
pop rax
imul rax, rbx
push rax
mov rax, 2
mov rbx, rax
pop rax
add rax, rbx
mov rdi, rax
mov rax, 60
syscall
