main:
    li a3, 1024     
    li t0, 0
    li t1, 4

while_input:
    bge t0, t1, end_input
    li a7, 5
    ecall

    slli t2, t0, 2
    add t2, t2, a3
    sw a0, 0(t2)

    addi t0, t0, 1
    jal zero, while_input


end_input:
    li t0, 0
    li t1, 3


while_output:
    blt t1, t0, done
    slli t2, t1, 2
    add t2, t2, a3
    lw a0, 0(t2)

    li a7, 1
    ecall

    addi t1, t1, -1
    jal zero, while_output

done:
    li a7, 10
    ecall