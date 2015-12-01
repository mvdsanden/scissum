//
//  ResourceManager.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <string>
#include <memory>

namespace scissum {
    
    class Reader;
    
    class ResourceManager
    {
    protected:
        
        ResourceManager();

    public:
        
        static ResourceManager &instance();
        
        ~ResourceManager();
        
        virtual void loadResourceFile(std::string const &path) = 0;
        
        virtual std::shared_ptr<Reader> openResource(size_t type, size_t id) = 0;
        
        virtual size_t getResourceSize(size_t type, size_t id) = 0;
        
    };
    
}

#endif /* ResourceManager_hpp */
