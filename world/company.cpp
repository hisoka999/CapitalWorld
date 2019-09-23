#include "company.h"

namespace world {

Company::Company(std::string name,float cash,bool player)
    :name(name),cash(cash),profit(0),player(player)
{

}
Company::~Company()
{

}

std::string Company::getName()
{
    return name;
}
float Company::getCash()
{
    return cash;
}
float Company::getProfit()
{
    return profit;
}
void Company::incCash(float value)
{
    cash+=value;
}
bool Company::isPLayer()
{
    return player;
}

}
