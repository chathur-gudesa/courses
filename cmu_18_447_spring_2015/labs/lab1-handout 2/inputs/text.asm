
	USER TEXT SEGMENT
[0x00400000]	0x2402000a  addiu $2, $0, 10                ; 2: addiu $v0, $zero, 10
[0x00400004]	0x24080005  addiu $8, $0, 5                 ; 3: addiu $t0, $zero, 5
[0x00400008]	0x2509012c  addiu $9, $8, 300               ; 4: addiu $t1, $t0, 300
[0x0040000c]	0x240a01f4  addiu $10, $0, 500              ; 5: addiu $t2, $zero, 500
[0x00400010]	0x254b0022  addiu $11, $10, 34              ; 6: addiu $t3, $t2, 34
[0x00400014]	0x256b002d  addiu $11, $11, 45              ; 7: addiu $t3, $t3, 45
[0x00400018]	0x0000000c  syscall                         ; 8: syscall
