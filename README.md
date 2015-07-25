# gelasia-compacter
The gelasia-compacter is a utility (wrote on C) to pack and unpack series of self-delimited integer numbers in a sequence of bytes, usually to be sent or stored in a compressed format. This is done using the, here first time described, "gelasia number representation" that ensures the smaller numbers to use less bits.

Like this:

               0 = 0 0 
               1 = 0 1
               2 = 10 0 00
               3 = 10 0 01 
               4 = 10 0 10 
               5 = 10 0 11 
               6 = 10 1 000 
               7 = 10 1 001 
               8 = 10 1 010 
               9 = 10 1 011 
              10 = 10 1 100 
              11 = 10 1 101 
              12 = 10 1 110 
              13 = 10 1 111 
              14 = 110 0 00 0000
              25 = 110 0 00 1011 
             125 = 110 0 10 111111 
             625 = 110 1 001 001110011 
            3125 = 110 1 011 10000110111 
           15625 = 110 1 101 1110100001011 
           78125 = 1110 0 00 0000 0011000100101111 
          390625 = 1110 0 00 0010 011111010111100011 
         1953125 = 1110 0 00 0100 11011100110101100111 
         9765625 = 1110 0 00 0111 00101010000001011111011 
        48828125 = 1110 0 00 1001 0111010010000111011011111 
       244140625 = 1110 0 00 1011 110100011010100101001010011 
      1220703125 = 1110 0 00 1110 001000110000100111001110010111 
      6103515625 = 1110 0 01 00000 01101011110011000100000111101011 
     30517578125 = 1110 0 01 00010 1100011010111111010100100110001111 
    152587890625 = 1110 0 01 00101 0001110000110111100100110111111000011

---
* src/test.c has example code of how the utility should be used.
* doc/ has a .pdf with the description of the gelasia number representation and a .tex version as well.
---
