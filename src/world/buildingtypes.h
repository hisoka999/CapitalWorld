#ifndef BUILDINGTYPES_H
#define BUILDINGTYPES_H

namespace world
{
    enum class BuildingType
    {
        Resource,
        Factory,
        Shop,
        Street,
        House,
        Transport,
        Other

    };

    enum class ProductType
    {
        Food,
        Entertainment,
        Cloth,
        Technology,
        Transport,
        Resource,

    };
    enum class RawResource : int
    {
        None = 0,
        Oil = 1,
        Iron = 2,
        Coal = 3,
        Aluminum = 4,
        Gold = 5,
        Copper = 6,
        Silicon = 7
    };
};
#endif // BUILDINGTYPES_H
