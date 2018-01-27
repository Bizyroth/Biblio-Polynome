#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "arithm.h"
#include <time.h>
typedef uint64_t f2_poly_t;
typedef uint8_t f2_deg_t;
// l'ordre des coefficients polynômiaux est du poids fort au poids faible.


#define F2_VARN 'X'
#define LEN (sizeof(f2_poly_t) << 3)
// Pour pouvoir tirer un polynôme au hasard
#define RANDOMFILE "/dev/urandom"
// Masque pour un polynôme de degré < d, calcule aussi 2^d - 1
#define MASK(d) (  ~((f2_poly_t)0xffffffffffffffff << d) )




/* Les polynômes sur F2 de degré < 64 sont représentés 
 par des entiers de 64 bits, les coefficients sont ordonnés du poids
 fort vers le poids faible. Ex. 0x82 : X^7+X.
*/

float temp_degre;
float temp_rem;
float temp_mul;


uint8_t Mersenne[9]={2,3,5,7,13,17,19,31,61};
int premier[18]= {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61};

 

/* Calcul sur F_2[X], pour des polynômes de degré au plus 63 */
f2_deg_t f2_poly_deg(f2_poly_t bb){


  
 return (f2_deg_t)63-__builtin_clzl(bb); //__builtin_clzl retourne le nombre de 0 consecutif en partant du bit de poids fort
  }

// écriture polynomiale usuelle, avec pour variable le second argument

int f2_poly_print(f2_poly_t p, char a, FILE * desc){
	
	f2_deg_t degre= f2_poly_deg(p);
	f2_poly_t temp=(1ULL<<degre);
	
	if(p==0x0){
		fprintf(desc, "0\n");
		return 1;
	}
	
	fprintf(desc, "%c^%d", a, degre);
	
	degre--;
	temp>>=1ULL;	
	
	while(temp!=0ULL){
		
		if((temp==1ULL)&&((temp&p)==1ULL)){
			fprintf(desc, " + ");
			fprintf(desc, "1\n");
			return 1;
		}
		if((p&temp)!=0ULL){
			fprintf(desc, " + ");
			fprintf(desc, "%c^%d", a, degre);
			degre--;
			temp>>=1ULL;
		}
		else{
			degre--;
			temp>>=1ULL;
		}
	}
	fprintf(desc, "\n");		
	
	return 1;
}	




// (arg1, arg2) = (quotient, reste) de arg3 par arg4

int f2_poly_div(f2_poly_t * quotient,f2_poly_t * reste, f2_poly_t p1, f2_poly_t p2){
	f2_deg_t deg1=0, deg2=0;
	deg1=f2_poly_deg(p1);
	deg2=f2_poly_deg(p2);
	
	f2_poly_t q=0x0;
	
	while((deg1>=deg2) && p1!=0ULL){
		q+=1ULL<<(deg1-deg2);
		p1=p1^(p2<<(deg1-deg2));
		deg1=f2_poly_deg(p1);	
	}
	*quotient=q;
	*reste=p1;
	return 1;

}

// reste de arg1 par arg2
f2_poly_t f2_poly_rem(f2_poly_t p1, f2_poly_t p2){

	f2_deg_t deg1=0, deg2=0;
	deg1=f2_poly_deg(p1);
	deg2=f2_poly_deg(p2);
		
	if(deg1<deg2){
		return p1;
	}
	
	while((deg1>=deg2) && p1!=0ULL){
		p1=p1^(p2<<(deg1-deg2));
		deg1=f2_poly_deg(p1);	
	}

	return p1;
}

// pgcd(arg1, arg2)
f2_poly_t f2_poly_gcd(f2_poly_t p1, f2_poly_t p2){
	f2_poly_t temp=0x0;
	
	while(p2!=0ULL){
		temp=f2_poly_rem(p1, p2);
		p1=p2;
		p2=temp;
	}
	return p1;
}



/* Pour tous les calculs modulo. On peut  quotienter par un
polynôme de degré jusqu'à 63. */

// retourne X*arg1 mod (arg2)
f2_poly_t f2_poly_xtimes(f2_poly_t p1, f2_poly_t p2){
	f2_poly_t temp=0x0;
	p1<<=1ULL;
	temp=f2_poly_rem(p1,p2);
	return temp;
}


// retourne arg1 * arg2 modulo  arg3
f2_poly_t f2_poly_times(f2_poly_t pol1, f2_poly_t pol2, f2_poly_t pol3){

	//pol1=f2_poly_rem(pol1, pol3);
	//pol2=f2_poly_rem(pol2, pol3);
	f2_deg_t deg1= f2_poly_deg(pol1);
	
	f2_poly_t res=0x0;
	
	while(deg1!=0){
		if((pol1&1ULL)==1ULL){
			res^=pol2;
			pol1^=1ULL;
		}
		pol2=f2_poly_xtimes(pol2, pol3);
		pol1>>=1ULL;
		deg1--;	
	}
		res^=pol2;

	return res;
}

// retourne X^{2^n} modulo p
f2_poly_t f2_poly_x2n(f2_deg_t n, f2_poly_t p){
	f2_poly_t res =0x2;
	int i=0;
	for(i=0; i<	n; i++){
		res=f2_poly_times(res, res, p);
	}
	return res;
}

//retourne le reste de la division par X+1 (xor des bits)
f2_poly_t f2_poly_parity(f2_poly_t po){
	//return f2_poly_rem(po, 0x3);
	return (f2_poly_t)__builtin_parityl(po);
}





f2_poly_t f2_poly_recip(f2_poly_t polP){

     polP = ((polP & 0x5555555555555555ULL) << 1) 
	  ^ ((polP & 0xaaaaaaaaaaaaaaaaULL) >> 1);
     polP = ((polP & 0x3333333333333333ULL) << 2) 
	  ^ ((polP & 0xccccccccccccccccULL) >> 2);
     polP = ((polP & 0x0f0f0f0f0f0f0f0fULL) << 4) 
	  ^ ((polP & 0xf0f0f0f0f0f0f0f0ULL) >> 4);
     polP = ((polP & 0x00ff00ff00ff00ffULL) << 8) 
	  ^ ((polP & 0xff00ff00ff00ff00ULL) >> 8);
     polP = ((polP & 0x0000ffff0000ffffULL) << 16) 
	  ^ ((polP & 0xffff0000ffff0000ULL) >> 16);
     polP = ((polP & 0x00000000ffffffffULL) << 32)
	  ^ ((polP & 0xffffffff00000000ULL) >> 32);

	return polP;
	//return (f2_poly_t)(polP>>(63-f2_poly_deg(polP)));
}

// vérifie si le polynôme arg1 est irréductible


int f2_poly_irred(f2_poly_t p){
	
	
	f2_poly_t temp=0x0;
	f2_deg_t n=0;
	n=f2_poly_deg(p);
	int i=0;
	
	
	/*Si le polynome n'a pas de coefficent 1*/
	if((p&1ULL)==0ULL){
		return 0;
	}
	/*si le polynome a un nombre pair de terme ie 1 est racine, n'est pas irreductible*/
	if(f2_poly_parity(p)==0ULL){
		return 0;
	}


	temp=f2_poly_x2n(n, p);
	/*Si P et X^(2^n)-X et p sont premier entre eux P n'est pas premier*/
	temp=f2_poly_gcd(temp^0x2,p);
		
	if(temp==0x1){
		return 0;
	}
	
	/*Montre que P n'est pas un produit de polynome premier*/
	while(premier[i]<n){
		
		if(((int)n%premier[i])==0){
			temp=f2_poly_x2n(n/premier[i], p);
			temp^=0x2;
			temp=f2_poly_gcd(temp, p);
			if(temp!=0x1){
				return 0;
			}	
		}
		i++;
	}
	return 1;
	
}
// retourne X^{arg1} modulo arg 2
f2_poly_t f2_poly_xn(f2_poly_t n, f2_poly_t p){
	uint64_t i=n;
	f2_poly_t res=0x1;
	f2_poly_t temp=0x2;
	while(i!=0UL){
		if((i&1)!=0UL){
			res=f2_poly_times(res, temp, p);
		}
		temp=f2_poly_times(temp, temp, p);
		i>>=1UL;
	}
	return res; 
}

// vérifie si le polynôme arg1 est primitif
int f2_poly_primitive(f2_poly_t p){
	
	/*Si le polynome n'est pas irreductible*/
	if(f2_poly_irred(p)==0){
		return 0;
	}
	
	f2_poly_t temp=0x0;
	f2_poly_t pgcd=0x0;
	uint64_t div=0ULL;
	f2_deg_t degre=f2_poly_deg(p);
	uint64_t deg2n=0ULL;
	uint64_t deg3n=0ULL;
	int i=0;
	
	for(i=0;i<9;i++){
		if(degre==Mersenne[i]){
			return 1;
		}
	}
	
	

	//temp=f2_poly_x2n((1<<degre)-1, p); // calcul de X^(2^n)-1 mod p
	//f2_poly_print(temp, 'X', stdout);
	//temp>>=1;                          // calcul de (X^(2^n)-1) -1
	
	deg2n=(1ULL<<degre)-1ULL;                // (2^n)-1
	deg3n=deg2n;
	while(deg2n!=1ULL){
		div=pp_diviseur_premier(deg2n);        // plus petit diviseur premier de deg2n
		temp=f2_poly_xn(deg3n/div, p);        // calcul de X^((2^q-1)/p)
		temp^=1ULL;                            //calcul de X^((2^q-1)/p)-1
		pgcd=f2_poly_gcd(temp, p);	       // calcul du pgcd
		if(pgcd!=1ULL){                        // si le si p divise temp retourne 0
			return 0;
		}
		while((deg2n%div)==0){                 //On supprime toutes les puissances du facteurs premier de 2^n-1
			deg2n/=div;
		}
	}
		
	
	return 1;
	
}

// renvoie 0 si le polynôme polP n'est pas irréductible, 
// l'ordre multiplicatif d'une racine, soit de  X modulo P sinon
// Si cet ordre égale 1 << f2_poly-deg(P), le polynôme est primitif.

/*uint8_t f2_poly_irred_order(f2_poly_t polP){
	if(f2_poly_irred(polP)==0){
		return 0;
	}
	f2_deg_t deg=f2_poly_deg(polP);
	
	int div=pp_diviseur_premier(deg);

}
*/

// retourne un polynôme tiré au hasard parmi les polynômes de degré < arg

f2_poly_t f2_poly_random_inf(f2_deg_t n){
	FILE * f;
	f=fopen(RANDOMFILE, "r");
	if(f==NULL){
		fprintf(stderr, "Probleme d'ouverture de dev/urandom\n");
		return 1ULL;
	}
	//0xffffffffffffffffULL
	f2_poly_t res=0x0;
	char s=0;
	uint64_t i=0ULL;
	while(i<n){
		
		fread(&s, 1,1,f);
		res^=((s&1ULL)<<i);
		i++;
	}	
	fclose(f);
	return res;
	
}
// retourne un polynôme tiré au hasard parmi les polynômes de degré = arg

f2_poly_t f2_poly_random(f2_deg_t n){
	f2_poly_t res=0x0;
	
	res=f2_poly_random_inf(n);
	
	res^=(1ULL<<n);
	
	return res;
}

// retourne un polynôme tiré au hasard parmi les polynômes irréductibles
// de degré = arg2
f2_poly_t f2_poly_irred_random(f2_deg_t n){
	int vrai=0;
	f2_poly_t res=0x0;
	while(vrai==0){
		res=f2_poly_random(n);
		vrai=f2_poly_irred(res);
	}
	return res;
}

// retourne un polynôme tiré au hasard parmi les polynômes primitifs
// de degré = arg2
f2_poly_t f2_poly_primitive_random(f2_deg_t n){
	int vrai=0;
	f2_poly_t res=0x0;
	while(vrai==0){
		res=f2_poly_random(n);
		vrai=f2_poly_primitive(res);
	}
	return res;
}


int f2_poly_irred_count(f2_deg_t n){
	if(n==1){
		return 2;
	}
	uint64_t i=0;
	int res=0;
	int irre=0;
	uint64_t a=1ULL<<n;
	f2_poly_t temp=0x1;
	for(i=3; i<a; i+=2){
		temp=(f2_poly_t) a^i;
		/*Si le polynome n'a pas 1 et 0comme racine*/
		if((!(temp&1ULL)==0ULL) && !(f2_poly_parity(temp)==0ULL)){
			
			irre=f2_poly_irred(temp);
			if(irre==1){
				res++;
			}
		}
	}
	return res;
}

int f2_poly_primitive_count(f2_deg_t n){
	if(n==1){
		return 2;
	}
	uint64_t i=0;
	int res=0;
	int irre=0;
	uint64_t a=1ULL<<n;
	f2_poly_t temp=0x0;
	int j=0;
	
	for(j=0; j<9;j++){
		if(n==Mersenne[j]){
			return f2_poly_irred_count(n);
		}	
	}
	
	
	for(i=0; i<a; i++){
		temp=(f2_poly_t) a^i;
		irre=f2_poly_primitive(temp);
		if(irre==1){
			res++;
		}
	}
	return res;
}


	



















