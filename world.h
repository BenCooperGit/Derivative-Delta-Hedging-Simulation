#ifndef CPP_OPTIONS_WORLD_H
#define CPP_OPTIONS_WORLD_H

#include <set>
#include <memory>

class Observer;
class Underlying;
class Option;
class Portfolio;

class World
{
public:
	World(double time, double stepSize, double interestRate);
	~World();

	inline double getInterestRate() const { return m_interestRate; }
	inline double getTime() const { return m_time; }

	void addUnderlyingObserver(Underlying* underlying);
	void removeUnderlyingObserver(Underlying* underlying);

	void addOptionObserver(Option* option);
	void removeOptionObserver(Option* option);

	void setPortfolioObserver(Portfolio* portfolio);

	void advanceTime();

private:
	double m_time;
	double m_stepSize;
	double m_interestRate;
	std::set<Underlying*> m_underlying_observers;
	std::set<Option*> m_option_observers;
	Portfolio* m_portfolio_observer;

	void triggerNotifications();
};

#endif