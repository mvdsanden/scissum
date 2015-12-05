//
//  HeapPointer.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/3/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "HeapPointer.hpp"
#include "MemoryManager.hpp"

#include <iostream>
#include <iomanip>

using namespace scissum;

void *HeapPointerBase::operator->() const
{
    return d_index?MemoryManager::instance().nativePtr(d_index):nullptr;
}
/*
void const *HeapPointerBase::operator->() const
{
    return d_index?MemoryManager::instance().nativePtr(d_index):nullptr;
}
 */

HeapPointerBase::size_type HeapPointerBase::size() const
{
    return MemoryManager::instance().sizeOf(d_index);
}

std::ostream &scissum::operator<<(std::ostream &s, scissum::HeapPointerBase const &p)
{
    return s << "0x" << std::hex << std::setw(16) << std::setfill('0') << p.d_index << std::dec;
}
