//
//  Sci32View.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32View_hpp
#define Sci32View_hpp

#include <memory>
#include <vector>

namespace scissum {
    
    class Sci32ViewCel;
    
    class Sci32View {
    public:
    
        typedef std::vector<std::unique_ptr<Sci32ViewCel>> Group;
        
        Sci32View(size_t width, size_t height,
                  std::vector<Group> &&groups);
        
        ~Sci32View();
        
        size_t width() const;
        size_t height() const;
        
        std::vector<Group> const &groups() const;
        std::vector<Group> &groups();
        
    private:
        
        size_t d_width;
        size_t d_height;
        
        std::vector<Group> d_groups;
        
    };
    
}

#endif /* Sci32View_hpp */
