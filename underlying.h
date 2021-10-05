#ifndef CPP_OPTIONS_UNDERLYING_H
#define CPP_OPTIONS_UNDERLYING_H

#include "Observer_abstract.h"
#include "World.h"

class Underlying : public Observer
{
public:
	Underlying(const World* world, double price, double annualisedVolatility);
	Underlying(const Underlying& underlying);
	~Underlying();
	Underlying& operator=(const Underlying& underlying);

	inline double getPrice() const { return m_price; }
	inline double delta() const { return 1.0; }

	void takeRandomStep(double stepSize);

	void notify(double stepSize);
	void notify();

private:
	const World* m_world;

	double m_price;
	double m_annualisedVolatility;

	double random_BM(double stepSize);
};

#endif