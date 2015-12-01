//
//  ResourceManager.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "ResourceManager.hpp"
#include "sci/Sci32ResourceFileScanner.hpp"
#include "tools/DecompressorSource.hpp"
#include "tools/LzsDecompressor.hpp"

#include <list>
#include <unordered_map>

#include <cassert>

#include <fcntl.h>

template <>
class std::hash<std::pair<size_t, size_t>>
{
public:
    std::size_t operator()(std::pair<size_t, size_t> const& s) const
    {
        return std::hash<size_t>()(s.first) ^ std::hash<size_t>()(s.second);
    }
};

namespace scissum {
    
    
    class ResourceManagerImpl
    : public ResourceManager
    {
        
        std::list<int> d_fileDescritors;
        
    public:
        
        ResourceManagerImpl()
        {
            
        }
        
        ~ResourceManagerImpl()
        {
            for (auto i : d_fileDescritors) {
                close(i);
            }
            d_fileDescritors.clear();
        }
        
        virtual void loadResourceFile(std::string const &path)
        {
            int fd = open(path.c_str(), O_RDONLY);
            
            if (fd == -1) {
                throw std::runtime_error(path+": error opening resource file");
            }
            
            d_fileDescritors.push_back(fd);
            
            loadResources(fd);
        }
        
        virtual std::shared_ptr<Reader> openResource(size_t type, size_t id)
        {
            auto i = d_table.find(std::make_pair(type, id));
            if (i == d_table.end()) {
                return std::shared_ptr<Reader>();
            }
            
            if (i->second->compressedSize == i->second->uncompressedSize) {
                // TODO: implement general file segment Reader.
                assert(0);
            } else {
                std::unique_ptr<DecompressorSource> source(new DecompressorFileSource(i->second->fd,
                                                                                  i->second->offset,
                                                                                  i->second->compressedSize));
                return std::make_shared<LzsDecompressor>(std::move(source));
            }
        }
        
        virtual size_t getResourceSize(size_t type, size_t id)
        {
            auto i = d_table.find(std::make_pair(type, id));
            return (i == d_table.end()?0:i->second->uncompressedSize);
        }
        
    private:
        
        struct ResourceEntry {
            size_t type;
            size_t id;
            int fd;
            size_t offset;
            size_t compressedSize;
            size_t uncompressedSize;
        };
        
        std::unordered_map<std::pair<size_t, size_t>, std::shared_ptr<ResourceEntry>> d_table;
        
        void loadResources(int fd)
        {
            scissum::Sci32ResourceFileScanner scanner(fd);
            
            while (scanner.next()) {
                
                auto entry = std::make_shared<ResourceEntry>();
                
                entry->type = scanner.resourceType();
                entry->id = scanner.resourceId();
                entry->fd = fd;
                entry->offset = scanner.resourceOffset();
                entry->compressedSize = scanner.resourceCompressedSize();
                entry->uncompressedSize = scanner.resourceUncompressedSize();

                d_table.insert(std::make_pair(std::make_pair(entry->type, entry->id), entry));
                
            }

        }
        
    };

    ResourceManager::ResourceManager()
    {
        
    }
    
    ResourceManager &ResourceManager::instance()
    {
        static ResourceManagerImpl s_instance;
        return s_instance;
    }
    
    ResourceManager::~ResourceManager()
    {
    }
    

}



