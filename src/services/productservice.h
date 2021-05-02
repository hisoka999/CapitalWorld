#ifndef PRODUCTSERVICE_H
#define PRODUCTSERVICE_H

#include "../world/product.h"
#include <memory>
#include <mutex>
#include <engine/utils/json/jsonservice.h>

namespace services
{

    class ProductService : public utils::json::JSONService<Product>
    {
    public:
        static ProductService &Instance()
        {
            static ProductService instance;
            return instance;
        }

        std::vector<std::shared_ptr<Product>> getProductsByBuildingType(world::BuildingType type);
        std::vector<std::shared_ptr<Product>> getBaseProductsByBuildingType(world::BuildingType type);
        std::vector<std::shared_ptr<Resource>> getResourcesByBuildingType(world::BuildingType type);
        std::vector<std::shared_ptr<Product>> getProductsByTypeAndResource(world::BuildingType type, std::shared_ptr<Resource> resource);
        std::shared_ptr<Resource> getResourceByName(std::string name);
        std::shared_ptr<Product> getProductByName(std::string name);
        void loadProducts(std::string path);
        void loadResources(std::string path);

    protected:
        virtual std::shared_ptr<Product> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

    private:
        ProductService() = default;
        ~ProductService() = default;
        ProductService(const ProductService &) = delete;
        ProductService &operator=(const ProductService &) = delete;

        std::vector<std::shared_ptr<Product>> products;
        std::vector<std::shared_ptr<Resource>> resources;
    };

}

#endif // PRODUCTSERVICE_H
