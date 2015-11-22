//
//  ImageSaver.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/21/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef ImageSaver_hpp
#define ImageSaver_hpp

#include <string>

namespace scissum {
    
    class Image;
    
    class ImageSaver
    {
    public:
        
        static ImageSaver &getPnmSaver();
        
        virtual void saveImage(Image const &image, std::string const &path) = 0;
        
    };
    
}

#endif /* ImageSaver_hpp */
