#include "gelasia_compact.h"

#define MOD(n,M) (((n % M) + M) % M)

typedef long long vlong;
typedef unsigned long long uvlong;
typedef unsigned char byte;

struct _gelasia_compacter{
	int w_sign;
	byte *seq;
	int seq_size;
	int seq_pos;
	int numbers_count;
};

int get_size(vlong number,int added){
	//get the size of a number (it must be on absolute value) plus added.
	uvlong unumber= (uvlong)(((uvlong)(number))+added);
	int size=0;
	while(unumber>0){
		unumber= unumber>>1;
		size++;
	}
	return size;
}

void gelasia_compacter_write_on_seq(gelasia_compacter *comp, uvlong number,int size){
	#ifdef GELASIA_DEBUG
	printf("\tWriting: %lli\tsiz:%lli\n",number,size);
	#endif
	//getting the part to be added.
	uvlong extra= number;
	extra= extra << (sizeof(uvlong)*CHAR_BIT-size);
	extra= extra >> (sizeof(uvlong)*CHAR_BIT-size);
	//appends a number to the sequence with the given bits, 
	int new_seq_pos= comp->seq_pos+size;
	int new_seq_size= new_seq_pos/CHAR_BIT;
	if((new_seq_pos-new_seq_size*CHAR_BIT)>0) new_seq_size++;
	comp->seq= realloc(comp->seq,new_seq_size); //asking for more space if necessary.
	int p; for(p=comp->seq_size;p<new_seq_size;p++){
		comp->seq[p]=0;
	}
	comp->seq_size= new_seq_size;
	int next_bits= MOD(-comp->seq_pos,CHAR_BIT);
	if(next_bits==0) next_bits=CHAR_BIT;
	while(comp->seq_pos<new_seq_pos){
		uvlong segment= extra;
		if(size>=next_bits){
			segment= segment >> (size-next_bits);
		}else{
			segment= segment << (next_bits-size);
		}
		comp->seq[comp->seq_pos/CHAR_BIT]+= (byte) segment;
		comp->seq_pos+=next_bits;
		size-=next_bits;
		next_bits=CHAR_BIT;
	}
	comp->seq_pos= new_seq_pos;
}

gelasia_compacter *new_gelasia_compacter(int with_sign){
	gelasia_compacter *gcompacter = malloc(sizeof(gelasia_compacter));
	gcompacter->w_sign= (with_sign)? 1:0;
	gcompacter->seq= NULL;
	gcompacter->seq_size=0;
	gcompacter->seq_pos=0;
	return gcompacter;
}

void delete_gelasia_compacter(gelasia_compacter *comp){
	free(comp->seq);
	free(comp);
}

void gelasia_compacter_write_number(gelasia_compacter *comp, uvlong number, int time_done){
	int size;
	if(time_done==0) size= get_size(number,2);
	else size= get_size(number,0);
	if(size>1){
		gelasia_compacter_write_number(comp,size-1,time_done+1);
		// writing on the sequence
		uvlong substract= 1;
		substract= substract << (size-1);
		if(time_done==0) substract-=2;
		gelasia_compacter_write_on_seq(comp,number-substract,size-1);
	}else{
		int tindic=0;
		int p; for(p=0;p<time_done-1;p++){
			tindic++;
			tindic= tindic << 1;
		}
		gelasia_compacter_write_on_seq(comp,tindic,time_done);
	}
}


int gelasia_compacter_add_number(gelasia_compacter *comp, vlong number){
	#ifdef GELASIA_DEBUG
	printf("Number: %lli\n",number);
	#endif
	int sign=0;
	//get the absolute value of a number, if it's negative, the absolute value minus 1.
	if(number<0){
		if(! comp->w_sign){
			printf("Error on gelasia_compacter: Cannot add a negative number to an unsigned compacter.\n");
			return 0;
		}
		sign=1;
		number+=1;
		number*=-1;
	}
	//write the sign bit if the compacter has size.
	if(comp->w_sign){
		gelasia_compacter_write_on_seq(comp,sign,1);
	}
	gelasia_compacter_write_number(comp,number,0);
	comp->numbers_count++;
	return 1;
}

byte *gelasia_compacter_get_sequence(gelasia_compacter *comp, int *size_pointer){
	byte* retorn= malloc(comp->seq_size);
	memcpy(retorn,comp->seq,comp->seq_size);
	*size_pointer= comp->seq_size;
	return retorn;
}

int gelasia_compacter_get_number_count(gelasia_compacter *comp){
	return comp->numbers_count;
}

typedef struct _gelasia_reader{
	int w_sign;
	vlong *numbers;
	int numbers_left;
	int numbers_readed;
	byte *res;
	int res_size;
	int res_pos;

	int next_number_size;
	int recursivity;
	int calculating_recursivity;
	int number_sign;
} gelasia_reader;


gelasia_reader *new_gelasia_reader(int with_sign, int numbers){
	gelasia_reader *greader= malloc(sizeof(gelasia_reader));
	greader->numbers= malloc(sizeof(vlong)*numbers);
	greader->numbers_left= numbers;
	greader->numbers_readed= 0;
	greader->res= NULL;
	greader->res_pos=0;
	greader->res_size=0;
	greader->w_sign= (with_sign)? 1:0;

	greader->calculating_recursivity=2;

	return greader;
}

int gelasia_reader_read_on_seq(gelasia_reader *readr, int size, uvlong *number){
	#ifdef GELASIA_DEBUG
	printf("\tReading siz:%lli\n",size);
	#endif
	int new_res_pos= readr->res_pos+size;
	if(new_res_pos>(readr->res_size*CHAR_BIT)) return 0;
	uvlong target= 0;
	while(readr->res_pos<new_res_pos){
		uvlong part=0;
		byte current_byte= readr->res[readr->res_pos/CHAR_BIT];
		int next_bits= MOD(-readr->res_pos,CHAR_BIT);
		if(next_bits==0) next_bits=CHAR_BIT;
		#ifdef GELASIA_DEBUG
		printf("\t\tReading bit: %lli %lli \n",current_byte,next_bits);
		#endif
		if(next_bits<CHAR_BIT){
			current_byte= (current_byte << (CHAR_BIT-next_bits));
			current_byte= (current_byte >> (CHAR_BIT-next_bits));
		}
		part= current_byte;
		if((size-next_bits)>0) part= (part << (size-next_bits));
		else part= (part >> (next_bits-size));
		
		target+= part;
		size-= next_bits;
		readr->res_pos+= next_bits;
	}
	readr->res_pos= new_res_pos;
	#ifdef GELASIA_DEBUG
	printf("\t\tResult: %i\n",target);
	#endif
	*number= target;
	return 1;
}

int gelasia_reader_feed(gelasia_reader *readr, byte *input, int size_receibed, int *readed){
	#ifdef GELASIA_DEBUG
	printf("Reading feed.\n");
	#endif
	if(readr->numbers_left>0){
		//adding input to the res buffer.
		readr->res= realloc(readr->res,readr->res_size+size_receibed);
		int old_res_size= readr->res_size;
		memcpy(readr->res+readr->res_size,input,size_receibed);
		readr->res_size+= size_receibed;
		*readed= size_receibed;
		uvlong result;
		while(readr->numbers_left>0){
			if(readr->calculating_recursivity==2){
				#ifdef GELASIA_DEBUG
				printf("Reading number.\n");
				#endif
				if(readr->w_sign){
					#ifdef GELASIA_DEBUG
					printf("Reading sign.\n");
					#endif
					if(!gelasia_reader_read_on_seq(readr,1,&result)) return 0;
					readr->number_sign= result; 
				}else{
					readr->number_sign= 0;
				}
				readr->recursivity=0;
				readr->next_number_size=1;
				readr->calculating_recursivity=1;
			}else if(readr->calculating_recursivity==1){
				if(!gelasia_reader_read_on_seq(readr,1,&result)) return 0;
				if(result==1){
					readr->recursivity+=1;
				}else{
					readr->calculating_recursivity=0;
					readr->next_number_size=1;
				}
			}else if(readr->calculating_recursivity==0){
				#ifdef GELASIA_DEBUG
				printf("\tRecursivity: %lli\n",readr->recursivity);
				#endif
				if(!gelasia_reader_read_on_seq(readr,readr->next_number_size,&result)) return 0;
				uvlong value= 1;
				value= (value << readr->next_number_size);
				value-=1;
				value+= result;
				if(readr->recursivity==0){
					//Reading done, save the number.
					vlong save_val;
					if(readr->number_sign){
						save_val= value*-1;
					}else{
						save_val= value-1;
					}
					readr->numbers[readr->numbers_readed]= save_val;
					//Preparation to read the next number.
					readr->numbers_left--;
					readr->numbers_readed++;
					readr->calculating_recursivity=2;
				}else{
					//Advance to the next recursion.
					readr->next_number_size= value+1;
					readr->recursivity-=1;
				}
			}
		}
		*readed= (readr->res_pos/CHAR_BIT)-old_res_size;
		if((readr->res_pos%CHAR_BIT)>0) *readed+=1;
		return 1;
	}
	*readed= 0;
	return 1;
}

vlong *delete_gelasia_reader_and_get_numbers(gelasia_reader *readr){
	if(readr->numbers_left>0){
		printf("Error on gelasia_reader: Attempt to get numbers with an incomplete feed.\n");
		return NULL;
	}else{
		free(readr);
		return readr->numbers;
	}
}

void gelasia_compacter_show(gelasia_compacter *comp){
	int k; for(k=0;k< comp->seq_size ;k++){
		int j; for(j=0;j<CHAR_BIT;j++){
			byte num= (byte) comp->seq[k];
			num= (num << j);
			num= (num >> (CHAR_BIT-1));
			printf("%i", num);
		}
		printf(" ");
	}
	printf("\n");
}
