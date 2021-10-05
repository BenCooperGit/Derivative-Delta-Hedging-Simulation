#ifndef CPP_OPTIONS_PORTFOLIO_H
#define CPP_OPTIONS_PORTFOLIO_H

#include <unordered_map>
#include "option.h"
#include "underlying.h"
#include "world.h"

class Portfolio : public Observer
{
public:
	Portfolio() = default;
	Portfolio(const World* world, const Option* option, double option_quantity);															// Options only portfolio
	Portfolio(const World* world, const Underlying* underlying, double underlying_quantity);												// Underlyings only portfolio
	Portfolio(const World* world, const Option* option, double option_quantity, const Underlying* underlying, double underlying_quantity);  // Options and Underlyings portfolio
	Portfolio(const Portfolio& portfolio);																				// Copy constructor

	~Portfolio();
	Portfolio& operator=(const Portfolio& portfolio);

	Portfolio& operator+=(std::pair<const Option*, double> options);		  // Add any number of an option to the portfolio
	Portfolio& operator+=(std::pair<const Underlying*, double> underlyings);  // Add any number of an underlying to the portfolio
	Portfolio& operator-=(std::pair<const Option*, double> options);		  // Add any number of an option to the portfolio
	Portfolio& operator-=(std::pair<const Underlying*, double> underlyings);  // Add any number of an underlying to the portfolio

	void sellAll();															  // Close out portfolio selling all
	void tradingStrategy();
	void implementTradingStrategy();
	void display() const;

	void notify();

private:
	const World* m_world;

	std::unordered_map<const Option*, double> m_options;		  // Stores portfolios options and quantities
	std::pair<const Underlying*, double> m_underlying;			  // Stores portfolios underlying and quantity

	double m_accountBalance = 0.0;
	double m_delta = 0.0;

	void recalculateDelta();
};

#endif