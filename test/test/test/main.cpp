#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void f(int field[]){
	if (field == NULL){
		return;
	}
	int velikost_pole = (sizeof(field) / sizeof(field[0]));
	printf("%d\n", velikost_pole);
	for (int i = 0; i < velikost_pole; i++){
		printf("%d\n", field[i]);
	}
}

void f2(char *retezec){
	int cislo = 0;
	int offset;
	while (sscanf(retezec, "%d%n", &cislo, &offset) == 1){
		printf("%d\n", cislo);
		retezec += offset;
	}
}

int main(){
	int field[] = {1, 2};
	int velikost_pole = (sizeof(field) / sizeof(field[0]));
	printf("%d\n", velikost_pole);
	f(field);

	//char *field2 = NULL;
	//field2 = "5 10 185 20 1000 100";
	//f2(field2);
}