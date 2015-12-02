//
//  MemoryManager.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "MemoryManager.hpp"

#include <algorithm>
#include <new>
#include <cstring>

namespace scissum {
    
    /**
     *  TODO: Optimise by using a best fit approach instead of a first fit.
     *        - Use bitwise trie on top of the existing chunk structure.
     *        - Scan the heap on import and create the trie.
     */
    class MemoryManagerImpl
    : public MemoryManager
    {
        
        size_t d_heapSize;
        
        uint8_t *d_base;
        
        enum Flags {
            FLAG_FREE = 1,
        };
        
        struct HeapHeader {
            uint32_t size;
//            uint32_t freeHead;
            uint32_t freeBins[11];
            // Must be multiple of 8 bytes!!
        } __attribute__ ((packed));
        
        struct ChunkHeader {
            uint8_t flags : 2;
            uint32_t prev : 30; // in multiples of 8 bytes.
            uint32_t size; // in bytes.
        } __attribute__ ((packed));
        
        struct FreeHeader {
            uint32_t prev; // in bytes.
            uint32_t next; // in bytes.
        } __attribute__ ((packed));
        
        struct FreeChunk {
            ChunkHeader ch;
            FreeHeader fh;
        } __attribute__ ((packed));
        
        ChunkHeader *chunkFromHandle(size_t handle)
        {
            return reinterpret_cast<ChunkHeader*>(d_base + handle - 8);
        }

        ChunkHeader const *chunkFromHandle(size_t handle) const
        {
            return reinterpret_cast<ChunkHeader const*>(d_base + handle - 8);
        }
        
        ChunkHeader *prevChunk(ChunkHeader *chunk)
        {
            return reinterpret_cast<ChunkHeader*>(d_base + chunk->prev * 8);
        }

        ChunkHeader *nextChunk(ChunkHeader *chunk)
        {
            return reinterpret_cast<ChunkHeader*>(reinterpret_cast<uint8_t*>(chunk) + chunk->size);
        }

        FreeChunk *prevFreeChunk(FreeChunk *fc)
        {
            assert(fc->ch.flags & FLAG_FREE && fc->ch.size >= 8);
            return offsetToFreeChunk(fc->fh.prev);
        }
        
        FreeChunk *nextFreeChunk(FreeChunk *fc)
        {
            assert(fc->ch.flags & FLAG_FREE && fc->ch.size >= 8);
            return offsetToFreeChunk(fc->fh.next);
        }
        
        size_t ptrToIndex(void const *ptr)
        {
            assert((reinterpret_cast<uint8_t const*>(ptr) - d_base) % 8 == 0);
            return (reinterpret_cast<uint8_t const*>(ptr) - d_base) / 8;
        }
        
        size_t ptrToOffset(void const *ptr)
        {
            return (reinterpret_cast<uint8_t const*>(ptr) - d_base);
        }
        
        FreeChunk *offsetToFreeChunk(size_t offset)
        {
            assert(offset % 8 == 0);
            return reinterpret_cast<FreeChunk*>(d_base + offset);
        }

        ChunkHeader const *beginChunk() const
        {
            return reinterpret_cast<ChunkHeader const*>(d_base + sizeof(HeapHeader));
        }
        
        ChunkHeader const *endChunk() const
        {
            return reinterpret_cast<ChunkHeader const*>(d_base + d_heapSize);
        }
        
        HeapHeader *heapHeader()
        {
            return reinterpret_cast<HeapHeader*>(d_base);
        }

        HeapHeader const *heapHeader() const
        {
            return reinterpret_cast<HeapHeader const*>(d_base);
        }
        
    public:
        
        MemoryManagerImpl()
        : d_heapSize(512 * 1025), d_base(nullptr)
        {
            reset();
        }
        
        virtual void initialize(size_t size)
        {
            d_heapSize = size;
            reset();
        }
        
        virtual void reset()
        {
            if (d_base != nullptr) {
                delete [] d_base;
                d_base = nullptr;
            }
            
            // Initialize the heap.
            d_base = new uint8_t [ d_heapSize ];
            memset(d_base, 0, sizeof(HeapHeader));
            HeapHeader *header = reinterpret_cast<HeapHeader*>(d_base);
            header->size = d_heapSize;
            header->freeBins[9] = sizeof(HeapHeader);
            
            // Initialize the first and only free chunk.
            FreeChunk *fc = offsetToFreeChunk(sizeof(HeapHeader));
            fc->ch.flags = FLAG_FREE;
            fc->ch.prev = 0;
            fc->ch.size = header->size - sizeof(HeapHeader);
            fc->fh.prev = fc->fh.next = sizeof(HeapHeader);
        }
        
        virtual void import(uint8_t *buffer, size_t size)
        {
            
        }
        
        virtual Ptr allocate(size_t bytes)
        {
            FreeChunk *fc = findFreeChunk(bytes);
            
            if (fc == null()) {
                throw std::bad_alloc();
            }
            
            ChunkHeader *chunk = splitFreeChunkAndAllocate(fc, bytes);
            
            return Ptr(reinterpret_cast<uint8_t*>(chunk) + 8 - d_base);
        }
        
        virtual void free(Ptr const &p)
        {
            ChunkHeader *chunk = chunkFromHandle(p.handle());
            
            // TODO: sanity check!
            
            FreeChunk *fc = insertFreeChunk(chunk);
            
            chunk->flags |= FLAG_FREE;
            
            ChunkHeader *c = prevChunk(&fc->ch);
            if (c && c->flags & FLAG_FREE) {
                fc = mergeFreeChunks(reinterpret_cast<FreeChunk*>(c), fc);
            }
            
            c = nextChunk(&fc->ch);
            if (c && c->flags & FLAG_FREE) {
                mergeFreeChunks(fc, reinterpret_cast<FreeChunk*>(c));
            }
        }
        
        virtual size_t sizeOf(size_t handle) const
        {
            ChunkHeader const *chunk = chunkFromHandle(handle);
            return chunk->size - 8;
        }
        
        virtual uint8_t const *nativePtr(size_t handle) const
        {
            ChunkHeader const *chunk = chunkFromHandle(handle);
            return reinterpret_cast<uint8_t const*>(chunk) + 8;
        }
        
        virtual uint8_t *nativePtr(size_t handle)
        {
            ChunkHeader *chunk = chunkFromHandle(handle);
            return reinterpret_cast<uint8_t*>(chunk) + 8;
        }
        
    private:
        
        void const *null() const
        {
            return d_base;
        }

        void *null()
        {
            return d_base;
        }
        
        FreeChunk *freeChunkBin(size_t bin)
        {
            assert(bin < 10);
            return offsetToFreeChunk(heapHeader()->freeBins[bin]);
        }

        void setFreeChunkBin(size_t bin, FreeChunk *fc)
        {
            assert(bin < 10);
            heapHeader()->freeBins[bin] = ptrToOffset(fc);
        }
        
        size_t calculateFreeChunkBin(size_t bytes)
        {
            size_t bin = 31 - __builtin_clz(static_cast<uint32_t>(bytes));
            
            bin += ((bin ^ (1 << bin))?1:0);

            return bin;
        }
        
        FreeChunk *findFreeChunkBinHead(size_t bytes)
        {
            size_t bin = calculateFreeChunkBin(bytes);
            
            while (bin < 9 && heapHeader()->freeBins[bin] == 0) {
                ++bin;
            }
            
            return offsetToFreeChunk(heapHeader()->freeBins[std::min<size_t>(9, bin)]);
        }
        
        FreeChunk *findFreeChunk(size_t bytes)
        {
            FreeChunk *head = findFreeChunkBinHead(bytes);
            
            if (head == null()) {
                return reinterpret_cast<FreeChunk*>(null());
            }
            
            bytes += 8;
            
            FreeChunk *fc = head;
            while (fc->ch.size < bytes) {
                if ((fc = nextFreeChunk(fc)) == head) {
                    return reinterpret_cast<FreeChunk*>(null());
                }
            }
            
            return fc;
        }
        
        ChunkHeader *splitFreeChunkAndAllocate(FreeChunk *fc, size_t bytes)
        {
            assert(fc->ch.size > bytes + 8);
            
            ChunkHeader *chunk = removeFreeChunk(fc);
            ChunkHeader *next = nextChunk(chunk);

            size_t blocks = (bytes / 8) + (bytes % 8?1:0);
            
            assert(chunk->size >= (8 + blocks * 8));
            
            size_t restBytes = chunk->size - 8 - blocks * 8;
            
            if (restBytes > 16) {
                chunk->size = 8 + blocks * 8;
                ChunkHeader *nfc = reinterpret_cast<ChunkHeader*>(reinterpret_cast<uint8_t*>(chunk) + chunk->size);
                nfc->flags = FLAG_FREE;
                nfc->prev = ptrToIndex(chunk);
                nfc->size = restBytes;
                
                if (reinterpret_cast<uint8_t*>(next) - d_base < d_heapSize) {
                    next->prev = ptrToIndex(nfc);
                }
                
                insertFreeChunk(nfc);
            }
            
            return chunk;
        }
        
        FreeChunk *mergeFreeChunks(FreeChunk *left, FreeChunk *right)
        {
            ChunkHeader *l = removeFreeChunk(left);
            ChunkHeader *r = removeFreeChunk(right);
            
            ChunkHeader *next = nextChunk(r);
            
            if (next != endChunk()) {
                next->prev = r->prev;
            }

            l->size += r->size;
            
            return insertFreeChunk(l);
        }
        
        FreeChunk *insertFreeChunk(ChunkHeader *chunk)
        {
            assert(chunk->size >= 16);
            
            chunk->flags |= FLAG_FREE;
            
            FreeChunk *fc = reinterpret_cast<FreeChunk*>(chunk);
            
            size_t bin = std::min<size_t>(9, calculateFreeChunkBin(fc->ch.size));
            
            FreeChunk *head = freeChunkBin(bin);
            
            if (head == null()) {
                fc->fh.prev = fc->fh.next = ptrToOffset(fc);
                setFreeChunkBin(bin, fc);
            } else {
                fc->fh.prev = head->fh.prev;
                fc->fh.next = ptrToOffset(head);
                offsetToFreeChunk(head->fh.prev)->fh.next = ptrToOffset(fc);
                head->fh.prev = ptrToOffset(fc);
            }
            
            return fc;
        }
        
        ChunkHeader *removeFreeChunk(FreeChunk *fc)
        {
            fc->ch.flags &= ~FLAG_FREE;
            
            if (fc->fh.next == ptrToOffset(fc)) {
                size_t bin = std::min<size_t>(9, calculateFreeChunkBin(fc->ch.size));
                setFreeChunkBin(bin, reinterpret_cast<FreeChunk*>(null()));
            } else {
                offsetToFreeChunk(fc->fh.next)->fh.prev = fc->fh.prev;
                offsetToFreeChunk(fc->fh.prev)->fh.next = fc->fh.next;
            }
            
            return reinterpret_cast<ChunkHeader*>(fc);
        }
        
    };
    
    MemoryManager &MemoryManager::instance()
    {
        static MemoryManagerImpl s_instance;
        return s_instance;
    }
    
}
