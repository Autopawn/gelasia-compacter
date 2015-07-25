#include <stdlib.h>
#include "gelasia_compact.h"

int main(int argc, char const *argv[]){

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>
	//@@@> SENDER CODE:
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>
	// Create a signed gelasia compacter.
	gelasia_compacter *compacter = new_gelasia_compacter(1);
	// Add some numbers.
	gelasia_compacter_add_number(compacter,-4087543);
	gelasia_compacter_add_number(compacter,1);
	gelasia_compacter_add_number(compacter,1213);
	gelasia_compacter_add_number(compacter,152587890625);
	gelasia_compacter_add_number(compacter,-13);
	// You can see the sequence of bits:
	gelasia_compacter_show(compacter);
	// Get the sequence of bytes and it's size (in bytes too).
	int size;
	unsigned char *sequence;
	sequence= gelasia_compacter_get_sequence(compacter,&size);
	printf("Bytes sended: %d\n",size);
	// Delete the compacter.
	delete_gelasia_compacter(compacter);

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>
	//@@@> RECIPENT CODE:
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>
	// Pointer to the received sequence of bytes:
	unsigned char *receibed= sequence;
	// Create a signed reader that expects 5 numbers.
	gelasia_reader *reader= new_gelasia_reader(1,5);

	// Let's simulate that we receibe data in parts of 4 bytes.

	// This are used to know how much of the stream has been used by the reader.
	int total_bytes_readed=0; int bytes_readed;
	// To simulate diferent parts of the sequence the pointer is moved 4 bytes on each read.
	unsigned char *data_part= receibed; 
	/* We tell the reader to get up to 4 bytes of the message, it will do so unless it
	   doesn't need more bytes, bytes_readed will indicate how many bytes the reader has
	   used. */
	while(!gelasia_reader_feed(reader,data_part,4,&bytes_readed)){
		// ^ Feed the reader until it has receibed all the needed bytes. 
		total_bytes_readed+= bytes_readed;
		data_part= data_part+4;
	}
	total_bytes_readed+= bytes_readed;
	// The bytes readed by the reader should be exactly the same that the sendend.
	printf("Bytes readed: %d\n",total_bytes_readed);
	// Retrieve the numbers and destroy the reader.
	long long *numbers= delete_gelasia_reader_and_get_numbers(reader);
	int j; for(j=0;j<5;j++){
		printf("Number %i:\t%lli\n",j,numbers[j]);
	}

	// Cleaning.
	free(sequence);
	free(numbers);
	return 0;
}