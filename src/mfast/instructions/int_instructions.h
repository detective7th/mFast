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

#ifndef INT_INSTRUCTIONS_H_YUVZVDYB
#define INT_INSTRUCTIONS_H_YUVZVDYB

#include "field_instruction.h"

namespace mfast
{
  class dictionary_builder;

  class MFAST_EXPORT integer_field_instruction_base
    : public field_instruction
  {
  public:
    integer_field_instruction_base(operator_enum_t      operator_id,
                                   int                  field_type,
                                   presence_enum_t      optional,
                                   uint32_t             id,
                                   const char*          name,
                                   const char*          ns,
                                   const op_context_t*  context,
                                   const value_storage& initial_storage,
                                   instruction_tag      tag,
                                   int32_t              decimal_place = 0);

    integer_field_instruction_base(const integer_field_instruction_base& other);
    virtual void construct_value(value_storage& storage,
                                 allocator*     alloc) const;



    value_storage& prev_value()
    {
      return *prev_value_;
    }

    const value_storage& prev_value() const
    {
      return *prev_value_;
    }

    const op_context_t* op_context() const
    {
      return op_context_;
    }

    void op_context(const op_context_t* v)
    {
      op_context_ = v;
    }

    const value_storage& initial_value() const
    {
      return initial_value_;
    }

    const value_storage& initial_or_default_value() const
    {
      return *initial_or_default_value_;
    }

    void initial_value(const value_storage& v)
    {
      initial_value_ = v;
      initial_or_default_value_ = initial_value_.is_empty() ? &default_value_ : &initial_value_;
      mandatory_no_initial_value_ = !optional() && initial_value_.is_empty();
    }

  protected:
    friend class dictionary_builder;
    const op_context_t* op_context_;
    value_storage initial_value_;
    value_storage* prev_value_;
    value_storage prev_storage_;
    const value_storage* initial_or_default_value_;
    static const value_storage default_value_;

    virtual void update_invariant()
    {
      field_instruction::update_invariant();
      mandatory_no_initial_value_ = !optional() && initial_value_.is_empty();
    }

  };

  template <typename T>
  class int_field_instruction
    : public integer_field_instruction_base
  {
  public:
    int_field_instruction(operator_enum_t      operator_id,
                          presence_enum_t      optional,
                          uint32_t             id,
                          const char*          name,
                          const char*          ns,
                          const op_context_t*  context,
                          int_value_storage<T> initial_value,
                          instruction_tag      tag = instruction_tag(),
                          int32_t              decimal_place = 0);

    int_field_instruction(const int_field_instruction& other);
    virtual void accept(field_instruction_visitor& visitor, void* context) const;
    virtual int_field_instruction<T>* clone(arena_allocator& alloc) const;
  };



  template <typename T>
  int_field_instruction<T>::int_field_instruction(operator_enum_t      operator_id,
                                                  presence_enum_t      optional,
                                                  uint32_t             id,
                                                  const char*          name,
                                                  const char*          ns,
                                                  const op_context_t*  context,
                                                  int_value_storage<T> initial_value,
                                                  instruction_tag      tag,
                                                  int32_t              decimal_place)
    : integer_field_instruction_base(operator_id,
                                     field_type_trait<T>::id,
                                     optional,
                                     id,
                                     name,
                                     ns,
                                     context,
                                     initial_value.storage_,
                                     tag,
                                     decimal_place)
  {
  }

  template <typename T>
  inline int_field_instruction<T>::int_field_instruction(const int_field_instruction& other)
    : integer_field_instruction_base(other)
  {
  }

  template <typename T>
  int_field_instruction<T>*
  int_field_instruction<T>::clone(arena_allocator& alloc) const
  {
    return new (alloc) int_field_instruction<T>(*this);
  }



  typedef int_field_instruction<int32_t> int32_field_instruction;
  typedef int_field_instruction<uint32_t> uint32_field_instruction;
  typedef int_field_instruction<int64_t> int64_field_instruction;
  typedef int_field_instruction<uint64_t> uint64_field_instruction;

} /* mfast */

#endif /* end of include guard: INT_INSTRUCTIONS_H_YUVZVDYB */
