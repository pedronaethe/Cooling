
#include <stdio.h>
#include <math.h>


int main()
{
    double v1 = 0;
	double v2 = 0;
	double v3 = 0;
	double v4 = 0;
	double i = 0, j = 0, k = 0, l = 0;
    FILE *Fheight;
    Fheight = fopen("scale_height_33.txt", "w");
	FILE *Ftemperature;
	Ftemperature = fopen("te_33.txt", "w");
	FILE *Feletronicdensity;
	Feletronicdensity = fopen("ne_33.txt", "w");
	FILE *Fmag;
	Fmag = fopen("mag_33.txt", "w");

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
    while (i <= 32)
    {
        v1 = 3 + 5 * i/32;
        fprintf(Fheight, "%.7f\n", v1);
        i = i + 1;
    }
	while (k <= 32)
	{
		v2 = 2 + 23 * k/32;
		fprintf(Feletronicdensity, "%.7f\n", v2);
		k = k + 1;
	}
	while (j <= 32)
	{
		v3 = 2 + 13 * j/32;
		fprintf(Ftemperature, "%.7f\n", v3);
		j = j + 1;
	}
	while (l <= 32)
	{
		v4 = 10 * l/32;
		fprintf(Fmag, "%.7f\n", v4);
		l = l + 1;
	}

    fclose(Fheight);
	fclose(Ftemperature);
	fclose(Feletronicdensity);
	fclose(Fmag);
	return 0;
}
