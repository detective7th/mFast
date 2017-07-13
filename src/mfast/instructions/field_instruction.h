// Copyright (c) 2013, 2014, Huang-Ming Huang,  Object Computing, Inc.
// All rights reserved.
//
// This file is part of mFAST.
//
//     mFAST is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Lesser General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     mFAST is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU Lesser General Public License
//     along with mFast.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FIELD_INSTRUCTION_H_E6BIAZAB
#define FIELD_INSTRUCTION_H_E6BIAZAB


#include <cstddef>
#include <cassert>
#include <boost/static_assert.hpp>
#include "mfast/value_storage.h"
#include "mfast/mfast_export.h"
#include "mfast/arena_allocator.h"
#include "mfast/allocator.h"
#include <algorithm>
#include <iostream>

namespace mfast {

  enum operator_enum_t {
    operator_none,
    operator_constant,
    operator_delta,
    operator_default,
    operator_copy,
    operator_increment,
    operator_tail,
    operators_count
  };

  enum presence_enum_t {
    presence_mandatory =0,
    presence_optional=1
  };

  enum field_type_enum_t {
    field_type_int32,
    field_type_uint32,
    field_type_int64,
    field_type_uint64,
    field_type_decimal,
    field_type_exponent,
    field_type_templateref,
    field_type_ascii_string, // using of_array start
    field_type_unicode_string,
    field_type_byte_vector,
    field_type_int32_vector,
    field_type_uint32_vector,
    field_type_int64_vector,
    field_type_uint64_vector,
    field_type_sequence, // using of_array end, codegen needed start
    field_type_group,
    field_type_template,
    field_type_enum
  };


  struct op_context_t {
    const char* key_;
    const char* ns_;
    const char* dictionary_;
  };


  template <typename T>
  struct field_type_trait;

  template <>
  struct field_type_trait<int32_t>
  {
    enum { id = field_type_int32};

  };

  template <>
  struct field_type_trait<uint32_t>
  {
    enum { id = field_type_uint32};

  };

  template <>
  struct field_type_trait<int64_t>
  {
    enum { id = field_type_int64};

  };

  template <>
  struct field_type_trait<uint64_t>
  {
    enum { id = field_type_uint64};

  };

  class field_instruction_visitor;

  class MFAST_EXPORT instruction_tag
  {
  public:
    instruction_tag(uint64_t v=0)
      : uint64_value_(v)
    {
    }

    uint64_t to_uint64() const
    {
      return uint64_value_;
    }
  private:
    uint64_t uint64_value_;
  };

  inline std::ostream&
  operator << (std::ostream& os, const instruction_tag& tag)
  {
    os << "mfast::instruction_tag(" << tag.to_uint64() <<  ")";
    return os;
  }


  class MFAST_EXPORT field_instruction
  {
  public:
    virtual void construct_value(value_storage& storage,
                                 allocator*     alloc) const=0;
    virtual void destruct_value(value_storage& storage,
                                allocator*     alloc) const;

    /// Perform deep copy
    virtual void copy_construct_value(const value_storage& src,
                                      value_storage&       dest,
                                      allocator*           alloc,
                                      value_storage*       fields_storage=0) const;

    virtual void accept(field_instruction_visitor&, void*) const=0;
    virtual field_instruction* clone(arena_allocator& alloc) const =0;
    virtual std::size_t pmap_size() const;

    bool is_nullable() const
    {
      return nullable_flag_;
    }

    const int32_t decimal_place() const 
    { 
      return decimal_place_; 
    }

    bool optional() const
    {
      return optional_flag_;
    }

    uint32_t id() const
    {
      return id_;
    }

    const char* name() const
    {
      return name_;
    }

    const char* ns() const
    {
      return ns_;
    }

    field_type_enum_t field_type() const
    {
      return static_cast<field_type_enum_t>(field_type_);
    }

    operator_enum_t field_operator() const
    {
      return static_cast<operator_enum_t>(operator_id_);
    }

    bool mandatory_without_initial_value() const
    {
      return mandatory_no_initial_value_;
    }

    /// @returns true if the field type is string, byteVector or sequence.
    bool is_array() const
    {
      return is_array_;
    }

    const char* field_type_name() const;

    field_instruction(operator_enum_t operator_id,
                      int             field_type,
                      presence_enum_t optional,
                      uint32_t        id,
                      const char*     name,
                      const char*     ns,
                      instruction_tag tag);

    field_instruction(const field_instruction& other);

    void optional(bool v)
    {
      optional_flag_ = v;
      update_invariant();
    }

    void decimal_place(int32_t v) 
    { 
      decimal_place_ = v; 
    }

    void id(uint32_t v)
    {
      id_ = v;
    }

    void name(const char* v)
    {
      name_ = v;
    }

    void ns(const char* v)
    {
      ns_ = v;
    }

    void field_operator(operator_enum_t v)
    {
      operator_id_ = v;
      update_invariant();
    }

    const instruction_tag& tag() const
    {
      return tag_;
    }

  protected:

    virtual void update_invariant()
    {
      nullable_flag_ =  optional_flag_ &&  (operator_id_ != operator_constant);
      has_pmap_bit_ = operator_id_ > operator_delta || ((operator_id_ == operator_constant) && optional_flag_);
    }

    uint16_t field_index_;
    uint16_t operator_id_ : 3;
    uint16_t is_array_ : 1;
    uint16_t optional_flag_ : 1;
    uint16_t nullable_flag_ : 1;
    uint16_t has_pmap_bit_ : 1;
    uint16_t mandatory_no_initial_value_ : 1;
    uint16_t field_type_ : 8;
    uint32_t id_;
    int32_t decimal_place_;
    const char* name_;
    const char* ns_;
    instruction_tag tag_;
  };

  template <typename T>
  class referable_instruction
  {
  public:
    referable_instruction(const T*    ref_instruction,
                          const char* cpp_ns)
      : ref_instruction_(ref_instruction)
      , cpp_ns_(cpp_ns)
    {
    }

    const field_instruction* ref_instruction() const
    {
      return ref_instruction_;
    }

    void ref_instruction(const T* r)
    {
      ref_instruction_ = r;
    }

    const char* cpp_ns() const
    {
      return cpp_ns_;
    }

  private:
    const T* ref_instruction_;
    const char* cpp_ns_;
  };
}

#endif /* end of include guard: FIELD_INSTRUCTION_H_E6BIAZAB */
