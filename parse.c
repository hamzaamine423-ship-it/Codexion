#include <stdio.h>
#include <unistd.h>
#include "my_header.h"

long _atoi(const char* str, int nb){
	int i;
	long res;

	i = 0;
	if (str[0] == '-'){
		printf("Error:\n");
		printf("   Invalid argument at position %d: expected an positive integer.\n", nb);
		exit(0);
	}
	res = 0;
	while(str[i]){
		res = res * 10 + (str[i] - 48);
		i++;
		if ((res > 2147483647)){
			printf("Error:\n");
			printf("   Invalid argument at position %d: expected an positive integer.\n", nb);
			exit(0);
		}
	}

	return ((int)res);
}

int is_valid_integer(char* str){

	int i = 0;
	while(str[i]){
		i++;
	}
	if (i == 0){
		return 0;
	}

	if (str[0] == '-' && str[1]){
		i = 1;
	}
	else{
		i = 0;
	}
	while(str[i]){
		if (!((str[i] < 58) && (str[i] > 47))){
			return 0;
		}
		i++;
	}


	return 1;
}

void parse_scheduler(char* str){
	str = str_tolower(str);
	if (!ft_cmp(str, "edf") && !ft_cmp(str, "fifo")){
		printf("Error:\n");
		printf("    Type of the scheduler is incorrect !!\n");
		printf("        the available options are: 'edf' or 'fifo'");
		exit(0);
	}
}

int* parse(int ac, char* av[]){
	int *list;
	int i;

	list = malloc(7 * sizeof(int));
	if (list == NULL)
		return NULL;

	if (ac != 9){
		printf("The number of the arguments should be 8.\n");
		exit(0);
	}
	i = 1;
	while(i < ac - 1){
		if (!is_valid_integer(av[i])){
			printf("Invalid argument number %d.\n", i);
			return 0;
		}
		list[i - 1] = _atoi(av[i], i);
		i++;
	}
	parse_scheduler(av[8]);
	return list;
}


int main(int ac, char* av[]){
	int *list;

	list = parse(ac, av);

	for(int i= 0; i < 7; i++){
		printf("list[%d]: %d\n", i, list[i]);
	}
	printf("Succes!");


	// free(list);
	return 0;
}
