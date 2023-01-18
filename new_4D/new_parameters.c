
#include <stdio.h>
#include <math.h>


int main()
{
    double v1 = 0;
	double v2 = 0;
	double v3 = 0;
	double v4 = 0;
	double i = 0, j = 0, k = 0, l = 0;
    FILE *Fradius;
    Fradius = fopen("radius.txt", "w");
	FILE *Ftemperature;
	Ftemperature = fopen("te.txt", "w");
	FILE *Feletronicdensity;
	Feletronicdensity = fopen("ne.txt", "w");
	FILE *Fmag;
	Fmag = fopen("mag.txt", "w");

	// If you want to generate the table for R, ne and Te out of the log space use the code below:
	//
	// for (R = 3e6; R < 1e12; R = pow(M_E, log(R) + logspacestep(3e6, 1e12, steps)))
	// {
	// 	fprintf(Fradius, "%le\n", R);
	// }
	// for (Te = 1e6; Te < 1e10; Te = pow(M_E, log(Te) + logspacestep(1e6, 1e10, steps)))
	// {
	// 	fprintf(Ftemperature, "%le\n", Te);
	// }
	// for (ne = 1e12; ne < 2e20; ne = pow(M_E, log(ne) + logspacestep(1e12, 2e20, steps)))
	// {
	// 	fprintf(Feletronicdensity, "%le\n", ne);
	// }
    while (i <= 31)
    {
        v1 = 6 + 3 * i/31;
        fprintf(Fradius, "%.2f\n", v1);
        i = i + 1;
    }
	while (k <= 31)
	{
		v2 = 12 + 10 * k/31;
		fprintf(Feletronicdensity, "%.2f\n", v2);
		k = k + 1;
	}
	while (j <= 31)
	{
		v3 = 4 + 11 * j/31;
		fprintf(Ftemperature, "%.2f\n", v3);
		j = j + 1;
	}
	while (l <= 31)
	{
		v4 = 10 * l/31;
		fprintf(Fmag, "%.2f\n", v4);
		l = l + 1;
	}

    fclose(Fradius);
	fclose(Ftemperature);
	fclose(Feletronicdensity);
	fclose(Fmag);
	return 0;
}
