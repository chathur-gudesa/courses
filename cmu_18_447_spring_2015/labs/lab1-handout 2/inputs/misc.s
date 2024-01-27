 # Tests instructions that are not present in the other files
	.text
main:   
        addi    $2, $zero, 0xfffffffb
        addi    $3, $zero, 0x3
        div     $2, $3
        addiu   $4, $zero, 0x5
        addiu   $5, $zero, 0x3
        divu    $4, $5
        mfhi    $6 
        mflo    $7 
        addiu   $8, $zero, 0xd00d
        mthi    $8 
        mtlo    $8 
        addi    $9, $zero, 0xfffffffb
        addi    $10, $zero, 0x3
        mult    $9, $10
        addiu   $11, $zero, 0x0f000000
        addiu   $12, $zero, 0x3
        multu   $11, $12
        nor     $13, $11, $12
        addiu   $14, $zero, 0x5
        addiu   $15, $zero, 0xf05
        sllv    $16, $14, $15
        slt     $17, $14, $15
        slt     $18, $15, $14
        slti    $19, $14, 0x4
        slti    $20, $14, 0xfffffff4
        sltiu   $21, $14, 0x4
        sltiu   $22, $14, 0xfffffff4
        addiu   $25, $zero, 0xffffffff
        sltu    $2, $14, $25
        addiu   $25, $zero, 0x4
        sltu    $3, $14, $25
        addiu   $4, $zero, 0x5
        addiu   $5, $zero, 0x2
        srlv    $6, $4, $5
        addiu   $4, $zero, 0x5
        addiu   $5, $zero, 0x2
        srav    $7, $4, $5
        addiu $v0, $zero, 0xa
        syscall