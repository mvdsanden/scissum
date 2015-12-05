//
//  MemoryManager.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/30/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "MemoryManager.hpp"

#include <stdexcept>
#include <algorithm>
#include <new>
#include <cstring>

#include <iostream>
#include <fstream>

#include <sys/mman.h>
//#include <sys/memfd.h>

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
        
        int d_heapDescriptor;
        
        enum Flags {
            FLAG_FREE = 1,
        };
        
        struct HeapHeader {
            uint32_t size;
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

        ChunkHeader const *prevChunk(ChunkHeader const *chunk) const
        {
            return reinterpret_cast<ChunkHeader const*>(d_base + chunk->prev * 8);
        }
        
        ChunkHeader const *nextChunk(ChunkHeader const *chunk) const
        {
            return reinterpret_cast<ChunkHeader const*>(reinterpret_cast<uint8_t const*>(chunk) + chunk->size);
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
        
        size_t ptrToIndex(void const *ptr) const
        {
            assert((reinterpret_cast<uint8_t const*>(ptr) - d_base) % 8 == 0);
            return (reinterpret_cast<uint8_t const*>(ptr) - d_base) / 8;
        }
        
        size_t ptrToOffset(void const *ptr) const
        {
            return (reinterpret_cast<uint8_t const*>(ptr) - d_base);
        }
        
        size_t indexToOffset(size_t index) const
        {
            return index * 8;
        }
        
        size_t offsetToIndex(size_t index) const
        {
            assert((index % 8) == 0);
            return index / 8;
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

        ChunkHeader *beginChunk()
        {
            return reinterpret_cast<ChunkHeader *>(d_base + sizeof(HeapHeader));
        }
        
        ChunkHeader *endChunk()
        {
            return reinterpret_cast<ChunkHeader *>(d_base + d_heapSize);
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
        : d_heapSize(512 * 1024), d_base(nullptr)
        {
            //reset();
        }
        
        ~MemoryManagerImpl()
        {
            freeHeap();
        }
        
        virtual void initialize(size_t size)
        {
            d_heapSize = size;
            reset();
        }
        
        virtual void reset()
        {
            if (d_base != nullptr) {
                freeHeap();
                d_base = nullptr;
            }
            
            allocateHeap();
        }
        
        virtual void load(std::string const &filename)
        {
            // TODO: use mmap!!!
            
            std::ifstream s(filename);
            s.seekg(0, s.end);
            d_heapSize = s.tellg();
            s.seekg(0, s.beg);

            reset();
            
            s.read(reinterpret_cast<char *>(d_base), d_heapSize);
        }
        
        virtual void save(std::string const &filename)
        {
            zeroFreeChunks();
            
            std::ofstream s(filename);
            
            if (!s) {
                throw std::runtime_error("error saving heap");
            }
            
            s.write(reinterpret_cast<char const*>(d_base), d_heapSize);
        }
        
        virtual HeapIndex allocate(size_t bytes)
        {
//            printHeapChunks();
            
//            std::cout << "MM: Allocate " << bytes << ".\n";
            
            
            
            FreeChunk *fc = findFreeChunk(bytes);
            
            if (fc == null()) {
                throw std::bad_alloc();
            }
            
            ChunkHeader *chunk = splitFreeChunkAndAllocate(fc, bytes);
            
//            std::cout << "- size: " << chunk->size << ".\n";
            
            HeapIndex index = (reinterpret_cast<uint8_t*>(chunk) + 8 - d_base);

//            std::cout << "MemoryManager: allocated " << index << "\n";
            
//            printHeapChunks();
            
            return index;
        }
        
        virtual void free(HeapIndex p)
        {
            if (p == 0) {
                throw std::runtime_error("trying to free NULL pointer");
            }
            
            if (!validate(p, 8)) {
                throw std::runtime_error("heap fault");
            }
            
//            printHeapChunks();
            
//            std::cout << "MM: free " << p << "\n";
            
            ChunkHeader *chunk = chunkFromHandle(p);
            
//            std::cout << "- size: " << chunk->size << ".\n";
            
            // TODO: sanity check!
            
            FreeChunk *fc = insertFreeChunk(chunk);
            
            chunk->flags |= FLAG_FREE;
            
            ChunkHeader *c = prevChunk(&fc->ch);
            if ((c != null()) && (c->flags & FLAG_FREE)) {
                fc = mergeFreeChunks(reinterpret_cast<FreeChunk*>(c), fc);
            }
            
            c = nextChunk(&fc->ch);
            if ((c != null()) && (c->flags & FLAG_FREE)) {
                mergeFreeChunks(fc, reinterpret_cast<FreeChunk*>(c));
            }
            
//            printHeapChunks();
        }
        
        virtual bool validate(HeapIndex p, size_t bytes) const
        {
            /*
            if (p % 8 != 0) {
                std::cerr << "MemoryManager::validate(): handle not multiple of 8.\n";
                assert(0);
                return false;
            }
             */
            
            if (p > d_heapSize || p < sizeof(HeapHeader)) {
                std::cerr << "MemoryManager::validate(): handle points outside of usable heap space.\n";
                assert(0);
                return false;
            }
            
            if ((d_heapSize - p) < bytes) {
                std::cerr << "MemoryManager::validate(): memory area overlapts with heap boundary.\n";
                assert(0);
                return false;
            }
            
            ChunkHeader const *chunk = chunkFromHandle(p);
            
            if (chunk->size < bytes) {
                std::cerr << "MemoryManager::validate(): memory chunk smaller than specified bytes.\n";
                assert(0);
                return false;
            }
            
            ChunkHeader const *next = nextChunk(chunk);

            if (next != endChunk() && ((next->prev * 8) != (ptrToOffset(chunk)))) {
                std::cerr << "MemoryManager::validate(): this does not look like a valid chunk (next->prev does not point to chunk).\n";
                assert(0);
                return false;
            }
                
            if (chunk != beginChunk()) {
                ChunkHeader const *prev = prevChunk(chunk);
                
                if (ptrToOffset(prev) + prev->size != ptrToOffset(chunk)) {
                    std::cerr << "MemoryManager::validate(): this does not look like a valid chunk (prev->next does not point to chunk)\n";
                    assert(0);
                    return false;
                }
            }
            
            return true;
        }
        
        virtual size_t sizeOf(HeapIndex handle) const
        {
            ChunkHeader const *chunk = chunkFromHandle(handle);
            return chunk->size - 8;
        }
        
        virtual uint8_t const *nativePtr(HeapIndex handle) const
        {
            if (handle < sizeof(HeapHeader) || handle >= d_heapSize) {
                throw std::runtime_error("heap fault");
            }
            
            return d_base + handle;
            
//            ChunkHeader const *chunk = chunkFromHandle(handle);
//            return reinterpret_cast<uint8_t const*>(chunk) + 8;
        }
        
        virtual uint8_t *nativePtr(HeapIndex handle)
        {
            if (handle < sizeof(HeapHeader) || handle >= d_heapSize) {
                throw std::runtime_error("heap fault");
            }
  
            return d_base + handle;
            
//            ChunkHeader *chunk = chunkFromHandle(handle);
//            return reinterpret_cast<uint8_t*>(chunk) + 8;
        }
        
        virtual HeapIndex pointerTo(void *p)
        {
            assert(p > (d_base + sizeof(HeapHeader)) && p < (d_base + d_heapSize) && "p points outside of the heap");
            return reinterpret_cast<uint8_t*>(p) - d_base;
        }
        
        virtual void printHeapChunks()
        {
            std::cout << "Heap chunks:\n";
            for (auto i = beginChunk(); i != endChunk(); i = nextChunk(i)) {
                std::cout << i << " size=" << i->size << ", flags=" << static_cast<size_t>(i->flags) << ".\n";
            }
            
            std::cout << "Free list:\n";
            for (size_t bin = 0; bin < 10; ++bin) {
                if (heapHeader()->freeBins[bin] == 0) {
                    continue;
                }
                
                std::cout << "bin[" << bin << "]:\n";
                
                auto fc = offsetToFreeChunk(heapHeader()->freeBins[bin]);
                auto i = fc;

                do {
                    std::cout << i << " size=" << i->ch.size << ", flags=" << static_cast<size_t>(i->ch.flags) << ".\n";
                    i = nextFreeChunk(i);
                } while (i != fc);
                
            }
            
        }
        
        virtual bool checkHeap()
        {
            size_t total = 0;
            size_t totalAlloc = 0;
            size_t totalFree = 0;
            
            size_t cprev = 0;
            
            for (auto i = beginChunk(); i != endChunk(); i = nextChunk(i)) {
                
                if (reinterpret_cast<uint8_t*>(i) < d_base || reinterpret_cast<uint8_t*>(i) > (d_base + d_heapSize - 16)) {
                    std::cerr << "checkHeap(): heap chunk points outside of heap.\n";
                    return false;
                }
                
                if (indexToOffset(i->prev) != cprev) {
                    std::cerr << "checkHeap(): chunk->prev does not point to the previous chunk.\n";
                    return false;
                }
                
                if (i->size > d_heapSize) {
                    std::cerr << "checkHeap(): chunk->size is higher than then heap size.\n";
                    return false;
                }
                
                if (i->flags != 0 && i->flags != FLAG_FREE) {
                    std::cerr << "checkHeap(): chunk has unsupported flags.\n";
                    return false;
                }
                
                total += i->size;
                
                ((i->flags & FLAG_FREE)?totalFree:totalAlloc) += i->size;
                
                cprev = ptrToOffset(i);
            }
            
            if (total != d_heapSize - sizeof(HeapHeader)) {
                std::cerr << "checkHeap(): total chunk sizes do not sum up to the heap size.\n";
                return false;
            }
            
            std::cerr << "checkHeap(): total: " << total << ".\n";
            std::cerr << "checkHeap(): total allocated: " << totalAlloc << ".\n";
            std::cerr << "checkHeap(): total free: " << totalFree << ".\n";
            
            return true;
        }
        
    private:
        
        /*
        void openHeapDescriptor()
        {
            if (d_heapDescriptor != -1) {
                return;
            }
            
            d_heapDescriptor = memfd_create("vmheap", 0);
            
            if (d_heapDescriptor == -1) {
                throw std::runtime_error("error creating heap descriptor");
            }
        }
        
        void closeHeapDescriptor()
        {
            if (d_heapDescriptor != -1) {
                close(d_heapDescriptor);
                d_heapDescriptor = -1;
            }
        }
         */
        
        void allocateHeap()
        {
            //openHeapDescriptor();
            /*
            if (ftruncate(d_heapDescriptor, d_heapSize) == -1) {
                throw std::runtime_error("error sizing heap memory file");
            }
             */
            
            // TODO: use to open saved game?
            //d_base = mmap(nullptr, d_heapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, d_heapDescriptor, 0);
            
            void *base = mmap(nullptr, d_heapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

            assert(base != MAP_FAILED);
            
            if (base == reinterpret_cast<void*>(-1)) {
                throw std::runtime_error("error memory mapping heap");
            }
            
            d_base = reinterpret_cast<uint8_t*>(base);
            
            // Initialize the heap.
            //d_base = new uint8_t [ d_heapSize ];
            HeapHeader *header = heapHeader();
            memset(header, 0, sizeof(HeapHeader));
            header->size = d_heapSize;
            header->freeBins[9] = sizeof(HeapHeader);
            
            // Initialize the first and only free chunk.
            FreeChunk *fc = offsetToFreeChunk(sizeof(HeapHeader));
            validatePtr((void*)fc);
            fc->ch.flags = FLAG_FREE;
            fc->ch.prev = 0;
            fc->ch.size = header->size - sizeof(HeapHeader);
            fc->fh.prev = fc->fh.next = sizeof(HeapHeader);
            validatePtr(fc);
        }
        
        void freeHeap()
        {
            if (munmap(d_base, d_heapSize) == -1) {
                throw std::runtime_error("error unmapping heap");
            }
            
            d_base = nullptr;
            //d_heapSize = 0;
            
            //closeHeapDescriptor();
        }
        
        void zeroFreeChunks()
        {
            for (size_t i = 0; i < 10; ++i) {
                
                if (heapHeader()->freeBins[i] == 0) {
                    continue;
                }
                
                FreeChunk *head = offsetToFreeChunk(heapHeader()->freeBins[i]);
                FreeChunk *fc = head;
                
                do {
                    memset(reinterpret_cast<uint8_t*>(fc) + 16, 0, fc->ch.size - 16);
                    fc = nextFreeChunk(fc);
                } while (fc != head);
                
            }
        }
        
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
            
//            std::cout << "freeChunkBinIndex for " << bytes << " is " << bin << " (" << (1 << bin) << ").\n";
            
            bin += ((bytes & ~(1 << bin))?1:0);

//            std::cout << "freeChunkBinIndex for " << bytes << " is " << bin << " corrected.\n";
            
            return bin;
        }
        
        FreeChunk *findFreeChunkBinHead(size_t bytes)
        {
            size_t bin = calculateFreeChunkBin(bytes);
            
            while (bin < 9 && heapHeader()->freeBins[bin] == 0) {
                ++bin;
            }
            
//            std::cout << "first free bin with chunks: " << bin << ".\n";
            
            return offsetToFreeChunk(heapHeader()->freeBins[std::min<size_t>(9, bin)]);
        }
        
        FreeChunk *findFreeChunk(size_t bytes)
        {
            FreeChunk *head = findFreeChunkBinHead(bytes);
            
            if (head == null()) {
//                std::cout << "findFreeChunk: all free bins empty.\n";
                return reinterpret_cast<FreeChunk*>(null());
            }
         
            validatePtr(head);
            
            // Add chunk header overhead.
            bytes += 8;
            
            FreeChunk *fc = head;
            while (fc->ch.size < bytes) {
                if ((fc = nextFreeChunk(fc)) == head) {
//                    std::cout << "findFreeChunk: no large enough free chunks found.\n";
                    return reinterpret_cast<FreeChunk*>(null());
                }
            }

            validatePtr(fc);
            
            return fc;
        }
        
        ChunkHeader *splitFreeChunkAndAllocate(FreeChunk *fc, size_t bytes)
        {
            assert(fc->ch.size >= bytes + 8);
            
            validatePtr(fc);
            
            ChunkHeader *chunk = removeFreeChunk(fc);
            validatePtr(chunk);
            
            // Get the chunk after it.
            ChunkHeader *next = nextChunk(chunk);
            
            size_t blocks = (bytes / 8) + (bytes % 8?1:0);
            
            assert(chunk->size >= (8 + blocks * 8));
            
            size_t restBytes = chunk->size - 8 - blocks * 8;
            
            if (restBytes > 16) {
                chunk->size = 8 + blocks * 8;
                ChunkHeader *nfc = nextChunk(chunk);
                validatePtr((void*)nfc);
                nfc->flags = FLAG_FREE;
                nfc->prev = ptrToIndex(chunk);
                nfc->size = restBytes;
                validatePtr(nfc);
                
                if (reinterpret_cast<uint8_t*>(next) - d_base + 16 < d_heapSize) {
                    validatePtr(next);
                    next->prev = ptrToIndex(nfc);
                }
                
                insertFreeChunk(nfc);
            }
            
            return chunk;
        }
        
        FreeChunk *mergeFreeChunks(FreeChunk *left, FreeChunk *right)
        {
            validatePtr(left);
            validatePtr(right);
            
            ChunkHeader *l = removeFreeChunk(left);
            ChunkHeader *r = removeFreeChunk(right);
            
            validatePtr(l);
            validatePtr(r);
            
            ChunkHeader *next = nextChunk(r);
            
            if (next != endChunk()) {
                validatePtr(next);
                next->prev = r->prev;
            }

            l->size += r->size;
            
            return insertFreeChunk(l);
        }
        
        FreeChunk *insertFreeChunk(ChunkHeader *chunk)
        {
            assert(chunk->size >= 16);
            
            validatePtr(chunk);
            
            chunk->flags |= FLAG_FREE;
            
            FreeChunk *fc = reinterpret_cast<FreeChunk*>(chunk);
            
            size_t bin = std::min<size_t>(9, calculateFreeChunkBin(fc->ch.size - 8));
            
            FreeChunk *head = freeChunkBin(bin);
            
            if (head == null()) {
                fc->fh.prev = fc->fh.next = ptrToOffset(fc);
                setFreeChunkBin(bin, fc);
            } else {
                validatePtr(head);
                fc->fh.prev = head->fh.prev;
                fc->fh.next = ptrToOffset(head);
                validatePtr(offsetToFreeChunk(head->fh.prev));
                offsetToFreeChunk(head->fh.prev)->fh.next = ptrToOffset(fc);
                head->fh.prev = ptrToOffset(fc);
                validatePtr(fc);
                validatePtr(head);
            }
            
            return fc;
        }
        
        ChunkHeader *removeFreeChunk(FreeChunk *fc)
        {
            validatePtr(fc);
            
            fc->ch.flags &= ~FLAG_FREE;
            
            if (fc->fh.next == ptrToOffset(fc)) {
                // List only contains this node, clear the list.
                size_t bin = std::min<size_t>(9, calculateFreeChunkBin(fc->ch.size - 8));
                setFreeChunkBin(bin, reinterpret_cast<FreeChunk*>(null()));
            } else {
                // List contains more nodes, remove node from list.
                validatePtr(offsetToFreeChunk(fc->fh.next));
                validatePtr(offsetToFreeChunk(fc->fh.prev));
                offsetToFreeChunk(fc->fh.next)->fh.prev = fc->fh.prev;
                offsetToFreeChunk(fc->fh.prev)->fh.next = fc->fh.next;
            }
            
            return reinterpret_cast<ChunkHeader*>(fc);
        }
        
        void validatePtr(void const *ptr) const
        {
            assert(reinterpret_cast<uint8_t const *>(ptr) > d_base);
            assert(reinterpret_cast<uint8_t const *>(ptr) + 16 < (d_base + d_heapSize));
        }

        void validatePtr(ChunkHeader const *ptr) const
        {
            assert(reinterpret_cast<uint8_t const *>(ptr) > d_base);
            assert(reinterpret_cast<uint8_t const *>(ptr) + 16 < (d_base + d_heapSize));
            assert(reinterpret_cast<uint8_t const *>(ptr) + ptr->size <= (d_base + d_heapSize));
            
            if (ptr->prev != 0) {
                assert(indexToOffset(ptr->prev) > 0 && indexToOffset(ptr->prev) < d_heapSize);
            }
        }
        
        void validatePtr(FreeChunk const *fc) const
        {
            validatePtr(reinterpret_cast<ChunkHeader const *>(fc));
            
            assert(fc->fh.prev > 0 && fc->fh.prev < d_heapSize);
            assert(fc->fh.next > 0 && fc->fh.next < d_heapSize);
        }
        
    };
    
    MemoryManager &MemoryManager::instance()
    {
        static MemoryManagerImpl s_instance;
        return s_instance;
    }
    
}
