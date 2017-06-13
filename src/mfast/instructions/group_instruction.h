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

#ifndef GROUP_INSTRUCTION_H_JFTB0YOV
#define GROUP_INSTRUCTION_H_JFTB0YOV

#include "field_instruction.h"
#include "../array_view.h"
#include <boost/foreach.hpp>

namespace mfast
{
  typedef const field_instruction*  const_instruction_ptr_t;
  typedef array_view<const const_instruction_ptr_t> instructions_view_t;

  class MFAST_EXPORT group_field_instruction
    : public field_instruction
    , public referable_instruction<group_field_instruction>
  {
  public:
    group_field_instruction(presence_enum_t     optional,
                            uint32_t            id,
                            const char*         name,
                            const char*         ns,
                            const char*         dictionary,
                            instructions_view_t subinstructions,
                            const char*         typeref_name,
                            const char*         typeref_ns,
                            const char*         cpp_ns,
                            instruction_tag     tag = instruction_tag())
      : field_instruction(operator_none,
                          field_type_group,
                          optional,
                          id,
                          name,
                          ns,
                          tag)
      , referable_instruction<group_field_instruction>(0, cpp_ns)
      , dictionary_(dictionary)
      , typeref_name_(typeref_name)
      , typeref_ns_(typeref_ns)
      , segment_pmap_size_(0)
      , subinstructions_(0,0)
    {
      set_subinstructions(subinstructions);
    }

    void construct_group_subfields(value_storage* group_content,
                                   allocator*     alloc) const;
    void destruct_group_subfields(value_storage* group_content,
                                  allocator*     alloc) const;

    void copy_group_subfields(const value_storage* src,
                              value_storage*       dest,
                              allocator*           alloc) const;

    /// Returns the number of bytes needed for the content of the group
    uint32_t group_content_byte_count() const
    {
      return static_cast<uint32_t>(subinstructions_.size() * sizeof(value_storage));
    }

    /// Returns the index for the subinstruction with specified id,
    /// or -1 if not found.
    int find_subinstruction_index_by_id(uint32_t id) const;

    /// Returns the index for the subinstruction with specified name,
    /// or -1 if not found.
    int find_subinstruction_index_by_name(const char* name) const;


    const field_instruction* subinstruction(std::size_t index) const;
    std::size_t segment_pmap_size() const
    {
      return segment_pmap_size_;
    }

    const instructions_view_t&  subinstructions() const
    {
      return subinstructions_;
    }

    void  set_subinstructions(instructions_view_t instructions);
    const char* typeref_name() const
    {
      return typeref_name_;
    }

    void typeref_name(const char* v)
    {
      typeref_name_ = v;
    }

    const char* typeref_ns() const
    {
      return typeref_ns_;
    }

    void typeref_ns(const char* v)
    {
      typeref_ns_ = v;
    }

    const char* dictionary() const
    {
      return dictionary_;
    }

    void dictionary(const char* v)
    {
      dictionary_ = v;
    }

    virtual void construct_value(value_storage& storage,
                                 allocator*     alloc) const;
    virtual void destruct_value(value_storage& storage,
                                allocator*     alloc) const;


    virtual void construct_value(value_storage& storage,
                                 value_storage* fields_storage,
                                 allocator*     alloc,
                                 bool           construct_subfields=true) const;
    // perform deep copy
    virtual void copy_construct_value(const value_storage& src,
                                      value_storage&       dest,
                                      allocator*           alloc,
                                      value_storage*       fields_storage=0) const;

    virtual void accept(field_instruction_visitor&, void*) const;
    virtual group_field_instruction* clone(arena_allocator& alloc) const;


    void link_value(value_storage& storage,
                    value_storage* fields_storage,
                    allocator*     alloc) const;

  protected:

    const char* dictionary_;
    const char* typeref_name_;
    const char* typeref_ns_;
    std::size_t segment_pmap_size_;
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4251 )
#endif
    instructions_view_t subinstructions_;
#ifdef _MSC_VER
#pragma warning( pop )
#endif
  };

  template <typename T>
  class group_instruction_ex
    : public group_field_instruction
  {
  public:
    typedef T cref_type;
    group_instruction_ex(presence_enum_t     optional,
                         uint32_t            id,
                         const char*         name,
                         const char*         ns,
                         const char*         dictionary,
                         instructions_view_t subinstructions,
                         const char*         typeref_name,
                         const char*         typeref_ns,
                         const char*         cpp_ns,
                         instruction_tag     tag = instruction_tag())
      : group_field_instruction(optional,
                                id,
                                name,
                                ns,
                                dictionary,
                                subinstructions,
                                typeref_name,
                                typeref_ns,
                                cpp_ns,
                                tag)
    {
    }

    group_instruction_ex(presence_enum_t                optional,
                         uint32_t                       id,
                         const char*                    name,
                         const char*                    ns,
                         const char*                    dictionary,
                         const group_field_instruction* ref_instruction,
                         const char*                    typeref_name,
                         const char*                    typeref_ns,
                         const char*                    cpp_ns,
                         instruction_tag                tag = instruction_tag())
      : group_field_instruction(optional,
                                id,
                                name,
                                ns,
                                dictionary,
                                ref_instruction->subinstructions(),
                                typeref_name,
                                typeref_ns,
                                cpp_ns,
                                tag)
    {
      this->ref_instruction(ref_instruction);
    }

    virtual group_instruction_ex<T>* clone(arena_allocator& alloc) const
    {
      return new (alloc) group_instruction_ex<T>(*this);
    }

  };


} /* mfast */
#endif /* end of include guard: GROUP_INSTRUCTION_H_JFTB0YOV */
