#include "option.h"

#include "Underlying.h"
#include <ql/quantlib.hpp>
#include <ql/pricingengines/blackcalculator.hpp>
#include <cmath>


// Constructors, destructor, =
Option::Option(
	double timeToExpiration, double strike, OptionType type, 
	double cost, double iv, const Underlying* underlying
)
	: m_timeToExpiration(timeToExpiration), m_strike(strike),
	m_type(type), m_cost(cost), m_impliedVolatility(iv),
	m_underlying(underlying)
{
	recalculateDelta();
}

Option::Option(const Option& option)
	: m_timeToExpiration(option.m_timeToExpiration), m_strike(option.m_strike),
	m_type(option.m_type), m_cost(option.m_cost), m_impliedVolatility(option.m_impliedVolatility),
	m_underlying(option.m_underlying), m_delta(option.m_delta)

{

}

Option::~Option()
{

}

Option& Option::operator=(const Option& option)
{
	if (this != &option)
	{
		m_timeToExpiration = option.m_timeToExpiration;
		m_strike = option.m_strike;
		m_type = option.m_type;
		m_cost = option.m_cost;
		m_impliedVolatility = option.m_impliedVolatility;
		m_underlying = option.m_underlying;
		m_delta = option.m_delta;
	}
	
	return *this;
}



// Member functions
void Option::recalculateDelta()
{
	boost::shared_ptr<QuantLib::PlainVanillaPayoff>
		vanillaOption(new QuantLib::PlainVanillaPayoff(((m_type == OptionType_Call) ? QuantLib::Option::Call : QuantLib::Option::Put), m_strike));

	double cur_disc = std::exp(-0.04 * m_timeToExpiration);				 // current discount rate
	double for_disc = 1.0;												 // forward discount rate
	double stdev = m_impliedVolatility * std::sqrt(m_timeToExpiration);  // standard deviation

	QuantLib::BlackScholesCalculator pricer(vanillaOption, m_underlying->getPrice(), for_disc, stdev, cur_disc);

	m_delta = pricer.delta();
}

double Option::value() const
{
	double value = 0.0;
	double underlyingPrice = m_underlying->getPrice();

	if (m_type == OptionType_Call)
	{
		if (underlyingPrice > m_strike)
		{
			value = underlyingPrice - m_strike;
		}
	}

	else  // m_type == OptionType_Put
	{
		if (m_strike > underlyingPrice)
		{
			value = m_strike - underlyingPrice;
		}
	}

	return value;
}



// Observer function
void Option::notify()
{
	recalculateDelta();
}