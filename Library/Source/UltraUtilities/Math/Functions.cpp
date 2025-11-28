#include "UltraUtilities/Math/Functions.h"

namespace UU
{
	static double NaturalLogarithmTaylorSeries(double x)
	{
		double taylorSum = 0.0;
		double y = x - 1.0;
		double z = y;

		for (int i = 1; i <= 20.0; i++)
		{
			if (i % 2 == 1)
				taylorSum += z / double(i);
			else
				taylorSum -= z / double(i);

			z *= y;
		}

		return taylorSum;
	}

	double NaturalLogarithm(double x)
	{
		// We want Ln(x) = Ln(m*2^k) = Ln(m) + k*ln(2) where 1 <= m <= 1.
		// This puts 'm' in the range of convergence for the Taylor series.
		
		// Find the smallest power of 2 that is greater than x.
		double k = 0.0;
		double powerOfTwo = 1.0;
		while (powerOfTwo < x)
		{
			k += 1.0;
			powerOfTwo *= 2.0;
		}

		double m = x / powerOfTwo;
		double naturalLogOfTwo = 0.69314718055994530941723212145818;
		double result = NaturalLogarithmTaylorSeries(m) + k * naturalLogOfTwo;
		return result;
	}

	double Logarithm(double base, double x)
	{
		return NaturalLogarithm(x) / NaturalLogarithm(base);
	}
}