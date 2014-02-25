/* Copyright (c) 2008-2013, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include <avian/util/fixed-allocator.h>

#include <avian/common.h>

namespace avian {
namespace util {

FixedAllocator::FixedAllocator(Aborter* a, uint8_t* base, unsigned capacity)
    : a(a), base(base), capacity(capacity), offset(0)
{
}

void* FixedAllocator::tryAllocate(unsigned size)
{
  return allocate(size);
}

void* FixedAllocator::allocate(unsigned size, unsigned padAlignment)
{
  unsigned paddedSize = vm::pad(size, padAlignment);
  expect(a, offset + paddedSize < capacity);

  void* p = base + offset;
  offset += paddedSize;
  return p;
}

void* FixedAllocator::allocate(unsigned size)
{
  return allocate(size, vm::BytesPerWord);
}

void FixedAllocator::free(const void* p, unsigned size)
{
  if (p >= base and static_cast<const uint8_t*>(p) + size == base + offset) {
    offset -= size;
  } else {
    abort(a);
  }
}

}  // namespace util
}  // namespace avian
