//
//  mapped_structs.hpp
//  
// Contains implementations of classic data structures for use in a memory-mapped area.
//

#ifndef BDSG_MAPPED_STRUCTS_HPP_INCLUDED
#define BDSG_MAPPED_STRUCTS_HPP_INCLUDED

#include <cstdio>
#include <cstdint>
#include <iostream>

namespace bdsg {
    
using namespace std;

/**
 * Context in which memory mapping happens. Needs to be passed down through all
 * the reference object constructors so that more reference objects can be
 * made.
 */
class MappingContext {
    // TODO
};


// We work with "reference" types which have a ::body_t that exists in the
// memory mapping, an offset, and possibly other fields that exist outside it.
// The reference type can have members that e.g. allocate, while the body
// cannot.
// These are meant to be value types, and should expose some accessor methods
// that operate on the body.

class base_ref_t {
public:
    MappingContext& context;
    size_t offset;
    
    struct body_t {};
    
protected:
    body_t& body();
    const body_t& body() const;
};


/**
 * Wrapper that stores a number in a defined endian-ness.
 */
class big_endian<T> {
    // TODO
};

/**
 * Offset pointer to a primitive type, in the memory mapping.
 * Exists in the memory mapping.
 */
template<typename T>
class offset_ptr {
private:
    big_endian<size_t> offset;
public:
    T& operator*();
    const T& operator*() const;
    
    T* operator->();
    const T* operator->() const;
    
    offset_ptr& operator=(T* addr);
};

/**
 * Offset pointer to the body of a reference type, in the memory mapping.
 * Exists in the memory mapping.
 */
template<typename ref_t>
class offset_to : public offset_ptr<ref_t::body_t> {
public:
    /// Get a ref_t to the body pointed to.
    ref_t get(MappingContext& context);
};


/**
 * Reference type for a vector of reference type items in the mapping.
 */
template<typename item_ref_t>
class MappedVector : public base_ref_t {
public:
    struct body_t {
        big_endian<size_t> length;
        offset_to<item_ref_t> first;
    };
    
    size_t size() const;
    void resize(size_t new_size);
    item_ref_t at(size_t index);
    const item_ref_t at(size_t index) const;
};

class IntVectorRef : public base_ref_t {
    // TODO
};

class PackedVectorRef : public base_ref_t {
    // TODO
};

class PagedVectorRef : public base_ref_t {
    // TODO
};

// Then on top of a PagedVector or something we might have:

/**
 * Context for accessing ref_t type things that live inside an int vector isntead of inside a memory mapping.
 */
class ArrayContext {
public:
    /// What is the int vector a vector of?
    using entry_t = size_t;
    
    /**
     * Our numbers may be compressed, so accesses to indices use these instead of real references.
     * See: std::vector<bool>
     */
    class ArrayEntryRef {
        friend class ArrayContext;
    private:
        ArrayContext& context;
        size_t offset;
    public:
        operator entry_t () const;
        ArrayEntryRef& operator= (const entry_t x);
        ArrayEntryRef& operator= (const ArrayEntryRef& x);
    };
    
    size_t size() const;
    void resize(size_t new_size);
    ArrayEntryRef at(size_t index);
    const ArrayEntryRef at(size_t index) const;
};

}

#endif
