#include <iostream>
#include "portfolio.h"
#include "option.h"
#include "underlying.h"
#include "world.h"

int main()
{
    // Stock Specific
    double currentUnderlyingPrice = 146.92;
    double annualisedVolatility = 0.216;
    
    // Option Specific
    double contractLength = 1.0;
    double strikePrice = 135;
    OptionType optionType = OptionType_Call;
    double optionCost = 16.15;
    double impliedVolatility = 0.216;

    // Global
    double annualInterestRate = 0.0444;

    // Decision
    int numberOfHedges = 3;

    

    double stepSize = contractLength / numberOfHedges;
    
    World world(contractLength, stepSize, annualInterestRate);
    Underlying underlying1(&world, currentUnderlyingPrice, annualisedVolatility);
    Option option1(contractLength, strikePrice, optionType, optionCost, impliedVolatility, &underlying1);
    Portfolio portfolio1(&world, &option1, 1.0);

    world.addUnderlyingObserver(&underlying1);
    world.addOptionObserver(&option1);
    world.setPortfolioObserver(&portfolio1);
    
    while (world.getTime() > 0.0)
    {
        world.advanceTime();
    }
    



    
    return 0;
}
