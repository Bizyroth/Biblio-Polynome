#include <inttypes.h>
#include <stdio.h>
uint64_t pp_diviseur_premier(uint64_t n){
	
	uint64_t i=0;
	
	if((n&1ULL)==0){
		return 2ULL;
	}
	
	for(i=3; i<n; i+=2){
		if((n%i)==0){
			return i;
		}
		if((i*i)>(n>>1ULL)){
			break;
		}
	}
	return n;
}

