#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include <world/product.h>
#include <memory>
#include <mutex>

namespace services {

class ProductService
{
public:
    static ProductService& Instance() {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }


    std::vector<Product> getProductsByBuildingType(BuildingType type);
    std::vector<Resource> getResourcesByBuildingType(BuildingType type);
    Resource* getResourceByName(std::string name);
    Product* getProductByName(std::string name);
    void loadProducts(std::string path);
    void loadResources(std::string path);

private:
    ProductService() = default;
    ~ProductService() = default;
    ProductService(const ProductService&) = delete;
    ProductService & operator=(const ProductService&) = delete;

    static ProductService* instance;
    static std::once_flag onceFlag;


    std::vector <Product> products;
    std::vector <Resource> resources;
    static void initSingleton(){
        instance = new ProductService();
    }

};

}

#endif // PRODUCTSERVICE_H
