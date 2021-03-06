# gelasia-compacter
The gelasia-compacter is a utility (wrote on C) to pack and unpack series of self-delimited integer numbers in a sequence of bytes, usually to be sent or stored in a compressed format. This is done using the, [here](https://github.com/Autopawn/gelasia-compacter/blob/master/representation/gelasia_representation.pdf) first time described, "gelasia number representation" that ensures the smaller numbers to use less bits.

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

# Expected sizes on bits.
To calculate the size that a number N will use, in bits, get the position (from right to left, starting from 0) of the most significant 1 of the value N+2 (in binary form), that will be called S.

    If...
      ...     S == 1   , the size will be 2.
      ...  2  <= S <  4, the size will be S+3.
      ...  4  <= S <  8, the size will be S+6.
      ...  8  <= S < 16, the size will be S+7.
      ...  16 <= S < 32, the size will be S+11.
      ...  32 <= S < 64, the size will be S+12.
            ...
  plus a sign bit if the representation is signed.
  
  (For negative numbers, to calculate S check the most significant 1 of -N+1).

* Numbers smaller than 1048575 will use less than 32 bits on an unsinged compacter.
* Numbers in [-524287,524286] will use less than 32 bits on a signed compacter.
* Numbers smaller than 2251799813685247 will use less than 64 bits on an signed compacter.

The size of a pack of numbers will only be the sum of the bits required for each number (plus up to 7 bits required to complete the last byte).

* [src/test.c](https://github.com/Autopawn/gelasia-compacter/blob/master/src/test.c) has example code of how the utility should be used.

# Packing

#### gelasia_compacter *new_gelasia_compacter(int with_sign);
Creates a new instance of gelasia_compacter, used to pack numbers.
  * with_sign indicates if the numbers will be signed or unsigned, 0 if unsigned, 1 otherwise.
Returns the pointer to the new compacter.

#### int gelasia_compacter_add_number(gelasia_compacter *comp, long long number);
Adds a number to the compacter's bit sequence.
  * comp is the pointer to the compacter.
  * number is the number to be added.
Returns 1 if the operation was successful, 0 if not (this only will happen when an attempt to add a negative number is done on an unsigned compacter).

#### unsigned char *gelasia_compacter_get_sequence(gelasia_compacter *comp, int *size_pointer);
Gets a copy of the bytes that represent the numbers that were added on the compacter.
  * comp is the pointer to the compacter.
  * size_pointer is the pointer to the int that will be used to output the size of the sequence, in bytes.
Returns a pointer to the first byte of the sequence.

#### int gelasia_compacter_get_number_count(gelasia_compacter *comp);
Gets the count of numbers that have been added to the compacter.
  * comp is the pointer to the compacter.
Returns the count of numbers that have been added to comp.

#### void delete_gelasia_compacter(gelasia_compacter *comp);
Frees the memory used by a compacter (And it's internal structs).
  * comp is the pointer to the compacter.

#### void gelasia_compacter_show(gelasia_compacter *comp);
Prints the sequence of bits in binary that stores the added numbers. 
  * comp is the pointer to the compacter.

# Unpacking

#### gelasia_reader *new_gelasia_reader(int with_sign, int numbers);
Creates a new instance of gelasia_reader, used to unpack numbers.
  * with_sign indicates if the numbers will be signed or unsigned, 0 if unsigned, 1 otherwise.
  * numbers is the amount of numbers expected to be rode.
Returns the pointer to the new reader.

#### int gelasia_reader_feed(gelasia_reader *readr, unsigned char *buffer, int size_receibed, int *readed);
Presents a part of a sequence of bytes (generated by a gelasia_compacter) to the reader for it to be added to it's internal known sequence. If the amount of bytes isn't enough to retrieve the numbers, it should be called again with the next part of the sequence.
  * readr is the pointer to the reader.
  * buffer is the pointer to the first byte of this part of the sequence.
  * size_receibed is the size (in bytes) of this part of the sequence.
  * readed is a pointer to a int used to output how many of the bytes were added to the reader. If the message wansn't enough to retrieve the numbers, it will be equal to size_receibed, otherwise it will be equal or less that it.
Returns 1 if the reader has receibed enough bytes, 0 otherwise.

#### long long *delete_gelasia_reader_and_get_numbers(gelasia_reader *readr);
Frees the memory used by a reader (And it's internal structs) and returns a pointer to an array of long long's countaining the unpacked numbers.
  * readr is the pointer to the reader.
Returns a pointer to the first possition of the array of long long's countaining the unpacked numbers.
