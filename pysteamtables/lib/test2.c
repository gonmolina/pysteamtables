#include "steam.h"
#include "steam_pT.h"
#include "steam_ph.h"

/**
	This test shows a problem resulting from repeated calls of forwards and
	backwards functions in freesteam. Results can easily diverge. The solution
	is to ensure that either forwards or backwards functions are use 
	consistently, and preferably never mixed. However, if this kind of use is
	essential, then it can be easily fixed by de-commenting the 'solver2' lines
	in the steam_ph.c file (eg line ~133)
	-- thanks to Ramiro Vignolo for spotting this and letting us know.
*/
int main(void){
	// Minimal working example by Ramiro Vignolo, May 2017.
	int i;
	double p, T;
	SteamState S;

	p = 101325;
	T = 25 + 273.15;

	S = freesteam_set_pT(p, T);

	for(i = 0; i < 30; i++){
		S = freesteam_set_ph(freesteam_p(S), freesteam_h(S));
		printf("%d\t%e\t%e\t%e\n", i, freesteam_p(S), freesteam_T(S), freesteam_h(S));
	}

	return 0;
}

