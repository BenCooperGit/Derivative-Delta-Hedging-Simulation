#include "underlying.h"

#include <random>
#include <cmath>
#include <chrono>


// Constructors, destructor, =
Underlying::Underlying(const World* world, double price, double annualisedVolatility)
	: m_world(world), m_price(price), m_annualisedVolatility(annualisedVolatility)
{

}

Underlying::Underlying(const Underlying& underlying)
	: m_world(m_world), m_price(underlying.m_price), m_annualisedVolatility(underlying.m_annualisedVolatility)
{

}

Underlying::~Underlying()
{

}

Underlying& Underlying::operator=(const Underlying& underlying)
{
	if (this != &underlying)
	{
		m_world = m_world;
		m_price = underlying.m_price;
		m_annualisedVolatility = underlying.m_annualisedVolatility;
	}

	return *this;
}



// Member functions
double Underlying::random_BM(double stepSize)
{
	static std::default_random_engine engine(std::time(nullptr));

	std::normal_distribution<double> brownianMotion(0, stepSize);
	return brownianMotion(engine);
}

void Underlying::takeRandomStep(double stepSize)
{
	double mu = m_world->getInterestRate();
	double S_t = m_price;
	double sigma = m_annualisedVolatility;
	double t = stepSize;
	double B_t = random_BM(stepSize);

	m_price = S_t * exp((mu - 0.5 * pow(sigma, 2)) * t + sigma * B_t);
}



// Observer function
void Underlying::notify(double stepSize)
{
	takeRandomStep(stepSize);
}

void Underlying::notify()
{

}
