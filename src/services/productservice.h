#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include "../world/product.h"
#include <memory>
#include <mutex>

namespace services
{

    class ProductService
    {
    public:
        static ProductService &Instance()
        {
            std::call_once(onceFlag, [] {
                initSingleton();
            });

            return *(instance);
        }

        std::vector<std::shared_ptr<Product>> getProductsByBuildingType(BuildingType type);
        std::vector<std::shared_ptr<Product>> getBaseProductsByBuildingType(BuildingType type);
        std::vector<std::shared_ptr<Resource>> getResourcesByBuildingType(BuildingType type);
        std::vector<std::shared_ptr<Product>> getProductsByTypeAndResource(BuildingType type, std::shared_ptr<Resource> resource);
        std::shared_ptr<Resource> getResourceByName(std::string name);
        std::shared_ptr<Product> getProductByName(std::string name);
        void loadProducts(std::string path);
        void loadResources(std::string path);

    private:
        ProductService() = default;
        ~ProductService() = default;
        ProductService(const ProductService &) = delete;
        ProductService &operator=(const ProductService &) = delete;

        static ProductService *instance;
        static std::once_flag onceFlag;

        std::vector<std::shared_ptr<Product>> products;
        std::vector<std::shared_ptr<Resource>> resources;
        static void initSingleton()
        {
            instance = new ProductService();
        }
    };

}

#endif // PRODUCTSERVICE_H
