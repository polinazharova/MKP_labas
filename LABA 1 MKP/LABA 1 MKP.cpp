﻿#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

#define EPS 0.00001
#define PI 3.14159265359 

double iterations(double e, double M);
double popolam(double e, double M);
double golden(double e, double M);
double newton(double e, double M);

int main()
{	
	//parametri orbiti
	double Ra, Rp, a, e, GM, n, T, M, E, true_anomaly, p, Vp, Vr, V, r;
	Ra = 6793;
	Rp = 6786;
	a = (Ra + Rp) / 2;
	e = (Ra - Rp) / (Ra + Rp);
	GM = 398600.4415;
	n = sqrt(GM / pow(a, 3));
	T = 2 * PI / n;
	std::cout << T << std::endl;
	std::cout << n << std::endl;
	p = a * (1 - pow(e, 2));


	//file creation
	std::ofstream fout;

	try {
		fout.open("Data_newton.txt");
	}
	catch (std::exception& ex) {
		std::cout << "File opening error.";	
		return 0;
	}

	//vivod v file poluchennih znacheniy anomaliy
	fout << "t, c\t";
	fout << "M(t), рад\t";
	fout << "E(t), рад\t";
	fout << "Theta(t), рад\t";
	fout << "Vp, (км/с)\t";
	fout << "Vr, (км/c)\t";
	fout << "V, (км\c)\t";
	fout << "r (км)" << std::endl;

	for (int t{ 0 }; t <= T; t++) {
		fout << t << "\t";
		M = n * t;
		E = newton(e, M);
		true_anomaly = atan(sqrt((1 + e) / (1 - e)) * tan(E / 2)) * 2;

		if (true_anomaly < 0)
			true_anomaly += 2 * PI;

		/*double cosinus = cos(true_anomaly);
		if (cosinus < 0) 
			cosinus += 2 * PI;
		
		double sinus = sin(true_anomaly);
		if (sinus < 0)
			sinus += 2 * PI;*/

		Vp = sqrt(GM/p) * (1 + (e * cos(true_anomaly)));
		Vr = (sqrt(GM / p)) * e * sin(true_anomaly);
		V = sqrt(pow(Vr, 2) + pow(Vp, 2));
		r = p / (1 + e * cos(true_anomaly));

		fout << M << "\t";
		fout << E << "\t";
		fout << true_anomaly << "\t";
		fout << Vp << "\t";
		fout << Vr << "\t";
		fout << V << "\t";
		fout << r << std::endl;

	}

	fout.close();
	std::cout << "The data was successfully written down in the 'Data.txt' file.";

	return 0;
}


//E metodom iteraciy
double iterations(double e, double M) {

	double Ek1, Ek = M;

	for (int i{ 0 }; i < 50; i++) {
		
		Ek1 = e * sin(Ek) + M;

		if (abs(Ek1 - Ek) < EPS) 
			return Ek1; 
		
		Ek = Ek1;
	}

	return 0;
}


//E metodom polovinnogo deleniya
double popolam(double e, double M) {

	double a = M - 2, b = M + 2, c;
	
	if (((a - e * sin(a) - M) * (b - e * sin(b) - M)) < 0) {
		
		for (int i{ 0 }; i < 50; i++) {

			c = (a + b) / 2;

			if (abs(c - e * sin(c) - M) < EPS) 
				return c;
			
			if ((a - e * sin(a) - M) * (c - e * sin(c) - M) < 0)
				b = c;
			else
				a = c;
		}
	}
	return 0;
}


//E metodom deleniya v sootnoshenii zolotogo secheniya
double golden(double e, double M) {
	
	double a = M - 3, b = M + 3, c;

	if (((a - e * sin(a) - M) * (b - e * sin(b) - M)) < 0) {

		for (int i{ 0 }; i < 100; i++) {

			c = a + ((b - a) / ((sqrt(5) + 1) / 2));

			if (abs(c - e * sin(c) - M) < EPS) 
				return c;
			
			if ((a - e * sin(a) - M) * (c - e * sin(c) - M) < 0)
				b = c;
			else
				a = c;
		}
	}
	return 0;
}

//E metodom Newtona
//proizvodnuyu vzyla ne sovsem korrektno poluchaetsya (ne Ek, a E(k-1), no znacheniya OK. it works)
double newton(double e, double M) {

	double Ek1, Ek = M, f, f1, temp_Ek1, temp_Ek;

	for (int i{ 0 }; i < 50; i++) {
		if (i == 0)	
			Ek1 = Ek - ((Ek - e * sin(Ek) - M) / (1 - e * cos(Ek)));
		else
			Ek1 = Ek - ((Ek - e * sin(Ek) - M) / ((f1 - f) / (temp_Ek1 - temp_Ek)));
	
		if (abs(Ek1 - Ek) < EPS) 
			return Ek1;

		f = Ek - e * sin(Ek) - M;
		f1 = Ek1 - e * sin(Ek1) - M;
		temp_Ek = Ek;
		temp_Ek1 = Ek1;

		Ek = Ek1;
	}
	return 0;
}
