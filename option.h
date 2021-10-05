#ifndef CPP_OPTIONS_OPTION_H
#define CPP_OPTIONS_OPTION_H

#include "observer_abstract.h"

class Underlying;

enum OptionType
{
	OptionType_Call,
	OptionType_Put
};

class Option : public Observer
{
public:
	Option(double timeToExpiration, double strike, OptionType type, double cost, double iv, const Underlying* underlying);
	Option(const Option& option);
	~Option();
	Option& operator=(const Option& option);

	inline double getCost() const { return m_cost; }
	inline double getTimeToExp() const { return m_timeToExpiration; }
	inline double delta() const { return m_delta; }
	inline const Underlying* getUnderlying() const { return m_underlying; }

	double value() const;

	void notify();

private:
	double m_timeToExpiration;
	double m_strike;
	OptionType m_type;
	double m_cost;
	double m_impliedVolatility;
	const Underlying* m_underlying;
	double m_delta = 0.0;

	void recalculateDelta();
};

#endif