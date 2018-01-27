#include <stdio.h>
#include "f2_poly.h"

int main(int argc, char *argv[]){
	if(argc!=2){
		fprintf(stderr, "Veuillez entree le degre du polynome irreductible  a trouver\n");
		return 0;
	}
	f2_deg_t deg=atoi(argv[1]);
	f2_poly_t res=f2_poly_irred_random(deg);
	f2_poly_print(res, 'X', stdout);
	return 0;
}
	
