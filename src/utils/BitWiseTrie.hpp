//
//  BitWiseTrie.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef BitWiseTrie_hpp
#define BitWiseTrie_hpp

#include "tools/Binary.hpp"

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
            bool leaf;
            Node *childs[2];
            Node *parent;
            
            Node(bool _leaf = false)
            : leaf(_leaf)
            {
                childs[0] = nullptr;
                childs[1] = nullptr;
            }
        };
        
        struct Leaf : public Node {
            _Value value;
            
            Leaf()
            : Node(true)
            {
//                std::cout << "Creating leaf node.\n";
            }
        };
        
        BitWiseTrie()
        {
            memset(d_zeroTable, 0, sizeof(d_zeroTable));
            
            Node *p = nullptr;
            
            for (size_t i = 0; i < (sizeof(_Key)*8); ++i) {
                d_zeroTable[i] = new Node();
                d_zeroTable[i]->parent = p;
                
                //if (p != nullptr) {
                //   p->childs[0] = d_zeroTable[i];
                //}
                
                p = d_zeroTable[i];
            }

            
            d_zeroTable[sizeof(_Key)*8] = new Leaf();
            d_zeroTable[sizeof(_Key)*8]->parent = p;
            //p->childs[0] = d_zeroTable[sizeof(_Key)*8];
            
        }
        
        Node *root()
        {
            return d_zeroTable[0];
        }

        Node const *root() const
        {
            return d_zeroTable[0];
        }
        
        Node *roots(size_t lz)
        {
            return d_zeroTable + lz;
        }
        
        Node *find(_Key key)
        {
            int z = (key != 0?countLeadingZeros(key):(sizeof(_Key) * 8));
            Node *root = d_zeroTable[z];
            int c = (sizeof(_Key) * 8) - z;
            
            while (c-- > 0) {
                Node *n = root->childs[!!(key & (1 << c))];
                
                if (n == nullptr) {
                    break;
                }
                
                root = n;
            }

            return root;
        }
        
        Leaf *insert(_Key key)
        {
            int z = (key != 0?countLeadingZeros(key):(sizeof(_Key) * 8));
            Node *root = d_zeroTable[z];
            int c = (sizeof(_Key) * 8) - z;
            
            // Make sure the zero line is connected to the root.
            for (Node *n = root; (n->parent && n->parent->childs[0] != n); n = n->parent) {
                n->parent->childs[0] = n;
            }
            
            while (c-- > 0) {
//                std::cout << "A " << c << " = " << !!(key & (1 << c)) << ".\n";
                
                Node *n = root->childs[!!(key & (1 << c))];
                
                if (n == nullptr) {
                    break;
                }
                
                root = n;
            }
            
            while (c > 0) {
//                std::cout << "B " << c << " = " << !!(key & (1 << c)) << ".\n";
                
                Node *n = new Node();
                root->childs[!!(key & (1 << c--))] = n;
                n->parent = root;
                root = n;
            }
            
            if (c == 0) {
//                std::cout << "C = " << !!(key & 1) << ".\n";
                
                Leaf *n = new Leaf();
                root->childs[!!(key & 1)] = n;
                n->parent = root;
                
//                std::cout << "Leaf? -> " << n->leaf << ".\n";
                
                return n;
            }
            
            return static_cast<Leaf*>(root);
        }
        
        static Leaf *minimum(Node *root)
        {
            if (root == nullptr) {
                return nullptr;
            }
            
            while (true) {
                if (root->childs[0] != nullptr) {
                    root = root->childs[0];
                } else if (root->childs[1] != nullptr) {
                    root = root->childs[1];
                } else {
                    return reinterpret_cast<Leaf*>(root);
                }
            }
            
            return nullptr;
        }

        static Leaf *maximum(Node *root)
        {
            if (root == nullptr) {
                return nullptr;
            }
            
            while (true) {
                if (root->childs[1] != nullptr) {
                    root = root->childs[1];
                } else if (root->childs[0] != nullptr) {
                    root = root->childs[0];
                } else {
                    return reinterpret_cast<Leaf*>(root);
                }
            }
            
            return nullptr;
        }
        
        static Leaf *successor(Node *root)
        {
            while (true) {
                if (root->childs[1] != nullptr) {
                    return reinterpret_cast<Leaf*>(minimum(root->childs[1]));
                } else if (root->parent == nullptr) {
                    return nullptr;
                } else {

                    // Go up as long as we are the right child, then loop again.
                    
                    while (root->parent != nullptr) {
                        Node *n = root;
                        root = root->parent;

                        if (n == root->childs[0]) {
                            break;
                        }
                    }
                    
                }
            }
        }
        
    private:
        
        Node *d_zeroTable[sizeof(_Key)*8+1];
        
        
        
    };
    
}

#endif /* BitWiseTrie_hpp */
