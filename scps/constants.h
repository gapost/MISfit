#ifndef CONSTANTS_H
#define CONSTANTS_H

#define L0 1.e-7 // 1nm in cm
#define N0 5.5263e19 // e0*1eV/|e|^2/(1nm)^2	in cm^-3
#define KT300 0.025851 // kT at 300K in eV
#define ELECTRON_CHARGE 1.60218e-19 // in C
#define VACUUM_KAPPA 8.85418e-14 // F/cm

/* energy scaling factor
	E0 = h^2/2/me/1nm^2 = 1Ry*(a0/1nm)^2 
*/
#define SCHROD_E0 0.03810009007799 // eV
#define HBAR 6.582118893e-16 // eV sec

#define SI_EG 1.12  // eV
#define SI_NC 2.8e19 // cm^-3
#define SI_NV 1.04e19 // cm^-3
#define SI_VTH_E 2.3e7 // Electron thermal velocity (cm/s)
#define SI_VTH_H 1.65e7 // Hole thermal velocity (cm/s)

#define GE_EG 0.66
#define GE_NC 1.0e19
#define GE_NV 5.0e18
#define GE_VTH_E 3.1e7
#define GE_VTH_H 1.9e7

#define GAAS_EG 1.424
#define GAAS_NC 4.7e17 
#define GAAS_NV 7.0e18
#define GAAS_VTH_E 4.4e7
#define GAAS_VTH_H 1.8e7


#endif 
