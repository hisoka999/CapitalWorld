#include "gtest/gtest.h"
#include <world/building.h>
#include <world/buildings/street.h>
#include <world/gamemap.h>
#include <memory>
#include <iostream>
/*
 Z
----
|  X
|__|___
      |
      Y
      
      
Legende:
Y = Transport Büro
X = Farm
Z = Fabrik


1. Ermittle die Strasse neben Y
2. gehe alle Strassen basierend von Y aus durch und Baue ein Netz
3. Suche im Netz die Gebäude mit Produkten
4. erzeuge einen Weg von X nach Z 
5. Berechne die Weglänge 
6. Wird der Weg umgesetzt, dann 
==> Berechne Transportkosten
==> Transportzeit: Strecke /3 = Drei Tiles pro Tag
==> Transportiere Waren von X nach Z, so dass das Lage sich bei X leert und bei Y füllt
==> Stelle in der Fabrik die Produktion so um, dass der Ressourcenverbrauch auch richtig beachtet wird


uint16 << 16 + uint16 = index

*/

std::shared_ptr<world::Building> addStreet(int x, int y)
{
    auto s1 = std::make_shared<world::buildings::Street>();
    s1->setPosition(x, y);
    return s1;
}

TEST(TransportTest, FindStorageBuildings)
{
    std::shared_ptr<world::Company> company = std::make_shared<world::Company>("test", 0, true);
    GameMap map(10, 10);
    world::Building::initComponentMap();

    map.addBuilding(addStreet(1, 1));
    map.addBuilding(addStreet(1, 2));
    map.addBuilding(addStreet(1, 3));
    map.addBuilding(addStreet(1, 4));
    //add start building
    auto office = std::make_shared<world::Building>("Transport Office", "Transport Office", "A factory", 50000, world::BuildingType::Transport, 2, 2);
    office->setPosition(1, 5);
    auto component1 = world::Building::createComponentByName("StorageComponent");
    office->addComponent(component1);
    map.addBuilding(office);
    auto farm = std::make_shared<world::Building>("Farm", "Farm", "A farm", 10000, world::BuildingType::Farm);
    farm->setPosition(2, 1);
    farm->addProduct(std::make_shared<Product>("test", "dummy.png", world::BuildingType::Farm, ProductionCycle(0, 12, 5, 5)));
    map.addBuilding(farm);
    auto component2 = world::Building::createComponentByName("StorageComponent");
    farm->addComponent(component2);
    auto factory = std::make_shared<world::Building>("Factory", "Factory", "A Factory", 10000, world::BuildingType::Factory);
    factory->setPosition(2, 2);
    factory->addProduct(std::make_shared<Product>("test", "dummy.png", world::BuildingType::Farm, ProductionCycle(0, 12, 5, 5)));
    auto component3 = world::Building::createComponentByName("StorageComponent");
    factory->addComponent(component3);
    map.addBuilding(factory);
    company->addBuilding(factory);
    company->addBuilding(farm);
    company->addBuilding(office);

    //TransportOffice office;

    auto result = map.findStorageBuildings(office, company);
    int test = 1;

    //EXPECT_EQ(map.getBuildings().size(), 7);
    EXPECT_EQ(result.size(), 2);
}

TEST(TransportTest, GenerateRoute)
{
    std::shared_ptr<world::Company> company = std::make_shared<world::Company>("test", 0, true);

    GameMap map(10, 10);

    map.addBuilding(addStreet(1, 1));
    map.addBuilding(addStreet(1, 2));
    map.addBuilding(addStreet(1, 3));
    map.addBuilding(addStreet(1, 4));
    //add start building
    auto office = std::make_shared<world::Building>("Transport Office", "Transport Office", "A factory", 50000, world::BuildingType::Transport, 2, 2);
    office->setPosition(1, 5);
    map.addBuilding(office);
    auto farm = std::make_shared<world::Building>("Farm", "Farm", "A farm", 10000, world::BuildingType::Farm);
    farm->setPosition(2, 1);
    map.addBuilding(farm);

    auto factory = std::make_shared<world::Building>("Factory", "Factory", "A Factory", 10000, world::BuildingType::Factory);
    factory->setPosition(2, 2);
    map.addBuilding(factory);
    //TransportOffice office;
    company->addBuilding(factory);
    company->addBuilding(farm);
    company->addBuilding(office);
    //TransportOffice office;

    auto result = map.findStorageBuildings(office, company);
}