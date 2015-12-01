//
//  BitWiseTrie.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef BitWiseTrie_hpp
#define BitWiseTrie_hpp

#include <vector>
//#include <iostream>
#include <cstring>

#define __SCISSUM_BITWISE_TRIE_USE_ZERO_TABLE 1

namespace scissum {
    
    /// TODO: Add cache locality and allocation efficiency by pre allocating an array of nodes!!!!!
    template <class _Key, class _Value>
    class BitWiseTrie
    {
    public:
        
        struct Node {
            size_t childs[2];
            _Value value;
            Node()
            {
                childs[0] = 0;
                childs[1] = 0;
            }
        };
        
        BitWiseTrie()
        {
            d_nodes.resize(sizeof(_Key)*8+1);
            for (size_t i = 0; i < sizeof(_Key)*8+1; ++i) {
                d_zeroTable[i] = i;
            }
        }
        
        Node const *node(size_t index) const
        {
            return &d_nodes[index];
        }
        
        Node const *roots(size_t lz) const
        {
            return &d_nodes[d_zeroTable[lz]];
        }
        
        Node *insert(_Key key)
        {
            int z = (key != 0?__builtin_clz(key):(sizeof(_Key) * 8));
            size_t root = d_zeroTable[z];
            int c = (sizeof(_Key) * 8) - z;
            
            while (c-- > 0) {
                //std::cout << "A " << c << " = " << !!(key & (1 << c)) << ".\n";
                
                size_t n = d_nodes[root].childs[!!(key & (1 << c))];
                
                if (n == 0) {
                    break;
                }
                
                root = n;
            }
            
            while (c > 0) {
                //std::cout << "B " << c << " = " << !!(key & (1 << c)) << ".\n";

                size_t n = d_nodes.size();
                d_nodes.push_back(Node());

                d_nodes[root].childs[!!(key & (1 << c--))] = n;
                root = n;
            }
            
            if (c == 0) {
                //std::cout << "C = " << !!(key & 1) << ".\n";
                
                size_t n = d_nodes.size();
                d_nodes.push_back(Node());
                
                d_nodes[root].childs[!!(key & 1)] = n;
                return &d_nodes[n];
            }
            
            return &d_nodes[root];
        }
        
    private:
        
        std::vector<Node> d_nodes;
        
        size_t d_zeroTable[sizeof(_Key)*8+1];
        
    };
    
}

#endif /* BitWiseTrie_hpp */
