//
//  BitWiseTrie.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef BitWiseTrie_hpp
#define BitWiseTrie_hpp

namespace scissum {
    
    /// TODO: Add cache locality and allocation efficiency by pre allocating an array of nodes!!!!!
    template <class _Key, class _Value>
    class BitWiseTrie
    {
    public:
    
        struct Node {
            Node *childs[2];
            
            Node()
            : childs({nullptr, nullptr})
            {
            }
        };
        
        struct Leaf : public Node {
            _Value value;
            
            Leaf(_Value const &_value)
            : value(_value)
            {
            }
        };
        
        BitWiseTrie()
        {
            d_root.childs[0] = nullptr;
            d_root.childs[1] = nullptr;
        }
        
        Node *insert(_Key key)
        {
            Node *root = &d_root;
            
            size_t c = (sizeof(_Key) * 8);
            
            while (c-- > 0) {
                Node *n = root->childs[!!(key & (1 << c))];
                
                if (n == nullptr) {
                    break;
                }
                
                root = n;
            }
            
            while (c > 0) {
                Node *n = new Node();
                root->childs[!!(key & (1 << c--))] = n;
                root = n;
            }
            
            
        }
        
    private:
        
        Node d_root;
        
    };
    
}

#endif /* BitWiseTrie_hpp */
