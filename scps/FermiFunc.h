#ifndef FERMI_H
#define FERMI_H

//#define NO_FERMI

void ImpG(bool fermi, double x, double& G, double& dG, int g = 2);

void FermiIntegral(bool fermi, double X, double& F, double& dF);

void FD1_2(double X, double& F, double& dF);

void FermiFunc(double x, double g, double& f, double& df);

float SiliconCP(float Nd, float Na, float beta, bool fermi = true);

float FindSemiconductorCP(float Ec, float Ev, float Nc, float Nv,
						  float Nd, float Na, float beta, bool fermi = true);


#endif // FERMI_H
