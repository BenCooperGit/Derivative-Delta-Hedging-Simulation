#ifndef CPP_OPTIONS_OBSERVER_ABSTRACT_H
#define CPP_OPTIONS_OBSERVER_ABSTRACT_H

class Observer
{
public:
	Observer();
	Observer(const Observer& observer);  // Not implemented
	~Observer();
	Observer& operator= (const Observer& observer);  // Not implemented

	virtual void notify() = 0;
};

#endif