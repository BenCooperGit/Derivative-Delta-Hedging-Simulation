#include "portfolio.h"

#include <cmath>
#include <iostream>


// Constructors
Portfolio::Portfolio(const World* world, const Option* option, double option_quantity)
	: m_world(world),
	m_options{ {option, option_quantity} }, 
	m_accountBalance(-option->getCost() * option_quantity * std::exp(world->getInterestRate() * option->getTimeToExp())),
	m_underlying(option->getUnderlying(), 0.0),
	m_delta(option->delta())

{

}

Portfolio::Portfolio(const World* world, const Underlying* underlying, double underlying_quantity)
	: m_world(world),
	m_underlying(underlying, underlying_quantity),
	m_accountBalance(
		-underlying->getPrice() * underlying_quantity 
		* std::exp(world->getInterestRate() * world->getTime())
	)
{

}

Portfolio::Portfolio(
	const World* world, const Option* option, double option_quantity, 
	const Underlying* underlying, double underlying_quantity
)
	: m_world(world), 
	m_options{ {option, option_quantity} }, m_underlying(underlying, underlying_quantity),
	m_accountBalance( 
		(-option->getCost() * option_quantity * std::exp(world->getInterestRate() * option->getTimeToExp()))
		+ (-underlying->getPrice()* underlying_quantity * std::exp(world->getInterestRate() * world->getTime())) 
	)
{

}
											
Portfolio::Portfolio(const Portfolio& portfolio)
	: m_world(portfolio.m_world),
	m_underlying(portfolio.m_underlying), m_options(portfolio.m_options),
	m_accountBalance(portfolio.m_accountBalance), m_delta(portfolio.m_delta)
{

}


// Destructor, =
Portfolio::~Portfolio()
{

}

Portfolio& Portfolio::operator=(const Portfolio& portfolio)
{
	if (this != &portfolio)
	{
		m_world = portfolio.m_world;
		m_options = portfolio.m_options;
		m_underlying = portfolio.m_underlying;
		m_accountBalance = portfolio.m_accountBalance;
		m_delta = portfolio.m_delta;
	}

	return *this;
}



// Overloaded addition and subtraction to portfolio operators
Portfolio& Portfolio::operator+=(std::pair<const Option*, double> options)
{
	m_options[options.first] += options.second;
	m_accountBalance -= (
		options.first->getCost() * options.second 
		* std::exp(m_world->getInterestRate() * options.first->getTimeToExp())
		);

	return *this;
}

Portfolio& Portfolio::operator+=(std::pair<const Underlying*, double> underlyings)
{
	if (m_underlying.first == underlyings.first)
	{
		m_underlying.second += underlyings.second;
	}

	m_accountBalance -= (
		underlyings.first->getPrice() * underlyings.second 
		* std::exp(m_world->getInterestRate() * m_world->getTime())
		);

	return *this;
}

Portfolio& Portfolio::operator-=(std::pair<const Option*, double> options)
{
	m_options[options.first] -= options.second;
	if ((0.00001 <= m_options[options.first]) || (m_options[options.first] <= 0.00001))
	{
		m_options.erase(options.first);
	}

	m_accountBalance += (
		options.first->getCost() * options.second 
		* std::exp(m_world->getInterestRate() * options.first->getTimeToExp())
		);

	return *this;
}

Portfolio& Portfolio::operator-=(std::pair<const Underlying*, double> underlyings)
{
	if (m_underlying.first == underlyings.first)
	{
		m_underlying.second -= underlyings.second;
	}

	m_accountBalance += (
		underlyings.first->getPrice() * underlyings.second 
		* std::exp(m_world->getInterestRate() * m_world->getTime())
		);

	return *this;
}



// Member function
void Portfolio::recalculateDelta()
{
	m_delta = 0.0;

	for (auto it = m_options.begin(); it != m_options.end(); ++it)
	{
		m_delta += (((it->first)->delta()) * (it->second));
	}

	m_delta += m_underlying.second;
}



//Trading Strategy
void Portfolio::sellAll()
{
	// Excercise the options
	for (auto it = m_options.begin(); it != m_options.end(); ++it)
	{
		m_accountBalance += (((it->first)->value()) * (it->second));
	}

	// Sell the underlying assets
	m_accountBalance += (((m_underlying.first)->getPrice()) * (m_underlying.second));
}

void Portfolio::display() const
{
	std::cout << "Underlying Price: " << (m_underlying.first)->getPrice() << '\n';
	std::cout << "Underlyings Owned: " << (m_underlying.second) << '\n';
	std::cout << "Balance: " << m_accountBalance << "\n\n";
}

void Portfolio::tradingStrategy()  // Rebalance to a delta neutral position by going long/short on the underlying
{
	*this += std::pair<const Underlying*, double>(m_underlying.first, -m_delta);
}

void Portfolio::implementTradingStrategy()
{
	std::cout << "Time: " << m_world->getTime() << "\n";

	if ((-0.001 <= m_world->getTime()) && (m_world->getTime() <= 0.001))
	{
		std::cout << "Underlying Price: " << (m_underlying.first)->getPrice() << '\n';
		std::cout << "Underlyings Owned: " << (m_underlying.second) << '\n';
		std::cout << "Closing out positions\n";
		sellAll();
		std::cout << "Final Balance: " << m_accountBalance << '\n';
		return;
	}

	std::cout << "Portfolio Delta: " << m_delta << '\n';

	if (m_delta < 0.0)  // Buy delta units of asset
	{
		std::cout << "Delta is negative, purchase underlying asset\n";
	}
	else if (m_delta > 0.0)  // Short delta units of asset
	{
		std::cout << "Delta is positive, short sell underlying asset\n";
	}

	tradingStrategy();

	display();
}



// Observer function
void Portfolio::notify()
{
	recalculateDelta();
}