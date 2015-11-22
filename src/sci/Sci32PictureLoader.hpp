//
//  Sci32PictureLoader.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32PictureLoader_hpp
#define Sci32PictureLoader_hpp

#include <memory>

namespace scissum {
    
    class Sci32Picture;
    class Reader;
    
    class Sci32PictureLoader
    // : public SciPictureLoader
    {
    public:
        
        virtual std::shared_ptr<Sci32Picture> loadPicture(std::shared_ptr<Reader> const &reader);
        
    };
    
}

#endif /* Sci32PictureLoader_hpp */
