    li sp, 1024
    slli sp, sp, 3
    
    li a7, 5
    ecall

    jal ra, fib
    
    mv a0, a7
    li a7, 1
    ecall
    
    li a7, 10 
    ecall

fib:
    li t0, 2
    bgeu a0, t0, recurse
    mv a7, a0
    jalr zero, ra, 0
    
recurse:
    addi sp, sp, -12
    sw a0, 0(sp)
    sw s0, 4(sp)
    sw ra, 8(sp)
    
    addi a0, a0, -1
    jal ra, fib
    
    mv s0, a7
    
    lw a0, 0(sp)
    addi a0, a0, -2
    jal ra, fib
    
    add a7, s0, a7
    
    lw ra, 8(sp)
    lw s0, 4(sp)
    lw a0, 0(sp)
    addi sp, sp, 12
    
    jalr zero, ra, 0  
