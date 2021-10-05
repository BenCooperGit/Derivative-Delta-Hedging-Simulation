#include "world.h"
#include "Observer_abstract.h"
#include "Portfolio.h"
#include <iostream>


// Constructor, destructor
World::World(double time, double stepSize, double interestRate)
	: m_time(time), m_stepSize(stepSize), m_interestRate(interestRate)
{

}

World::~World()
{

}



// Add and remove observers
void World::addUnderlyingObserver(Underlying* underlying)
{
	m_underlying_observers.insert(underlying);
}

void World::removeUnderlyingObserver(Underlying* underlying)
{
	if (m_underlying_observers.find(underlying) != m_underlying_observers.end())
	{
		m_underlying_observers.erase(underlying);
	}
}

void World::addOptionObserver(Option* option)
{
	m_option_observers.insert(option);
}

void World::removeOptionObserver(Option* option)
{
	if (m_option_observers.find(option) != m_option_observers.end())
	{
		m_option_observers.erase(option);
	}
}

void World::setPortfolioObserver(Portfolio* portfolio)
{
	if (m_portfolio_observer != portfolio)
	{
		m_portfolio_observer = portfolio;
	}
}



// Intereact with observers
void World::triggerNotifications()
{
	for (auto i : m_underlying_observers)
	{
		i->notify(m_stepSize);
	}
	for (auto i : m_option_observers)
	{
		i->notify();
	}
	m_portfolio_observer->notify();
}

void World::advanceTime()
{
	m_portfolio_observer->implementTradingStrategy();
	m_time -= m_stepSize;
	triggerNotifications();
}
