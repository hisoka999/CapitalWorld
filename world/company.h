#ifndef COMPANY_H
#define COMPANY_H

#include <string>

namespace world {

class Company
{
public:
    Company(std::string name,float cash,bool player);
    ~Company();

    std::string getName();
    float getCash();
    float getProfit();
    void incCash(float value);
    bool isPLayer();


private:
    std::string name;
    float cash;
    float profit;
    bool player;
};
}
#endif // COMPANY_H
