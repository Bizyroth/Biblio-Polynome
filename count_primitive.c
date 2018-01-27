#include <stdio.h>
#include "f2_poly.h"

int main(int argc, char * argv[]){

	if(argc!=2){
		if(argc!=3){
			fprintf(stderr, "Veuillez entrez un degre compris entre 1 et 63\n");
			fprintf(stderr, "Si vous voulez lister tous les primitifs pour un degre <N faire:\n");
			fprintf(stderr,"./count_irred N -f\n"); 
			return 0;
		}
	}
	int res=0;
	f2_deg_t deg=atoi(argv[1]);
	
	if(argc==2){
		res=f2_poly_primitive_count(deg);
		fprintf(stdout, "Il y a %d polynomes primitifs de degre %d\n",res, deg);
	}
	else{
		int i=0;
		for(i=1; i<=deg;i++){
			//f2_deg_t deg=atoi(argv[1]);
			res=f2_poly_primitive_count(i);
			fprintf(stdout, "Il y a %d polynomes primitives de degre %d\n",res, i);
		}
	}
	return 0;
}
