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

#ifndef TEMPLATE_INSTRUCTION_H_PARP21ZA
#define TEMPLATE_INSTRUCTION_H_PARP21ZA

#include "group_instruction.h"

namespace mfast
{
  class MFAST_EXPORT template_instruction
    : public group_field_instruction
  {
  public:
    template_instruction(uint32_t            id,
                         const char*         name,
                         const char*         ns,
                         const char*         template_ns,
                         const char*         dictionary,
                         instructions_view_t subinstructions,
                         bool                reset,
                         const char*         typeref_name,
                         const char*         typeref_ns,
                         const char*         cpp_ns,
                         instruction_tag     tag = instruction_tag(),
                         int32_t             decimal_place = 0)
      : group_field_instruction(presence_mandatory,
                                id,
                                name,
                                ns,
                                dictionary,
                                subinstructions,
                                typeref_name,
                                typeref_ns,
                                cpp_ns,
                                tag,
                                decimal_place)
      , template_ns_(template_ns)
      , reset_(reset)
    {
      field_type_ = field_type_template;
    }

    const char* template_ns() const
    {
      return template_ns_;
    }

    void copy_construct_value(value_storage&       storage,
                              value_storage*       fields_storage,
                              allocator*           alloc,
                              const value_storage* src_fields_storage) const;

    virtual void copy_construct_value(const value_storage& src,
                                      value_storage&       dest,
                                      allocator*           alloc,
                                      value_storage*       fields_storage=0) const;


    virtual void accept(field_instruction_visitor&, void*) const;
    virtual template_instruction* clone(arena_allocator& alloc) const;

    bool has_reset_attribute() const
    {
      return reset_;
    }

    // void ensure_valid_storage(value_storage& storage,
    //                           allocator*     alloc) const;

  private:
    const char* template_ns_;
    bool reset_;
  };

  template <typename T>
  class template_instruction_ex
    : public template_instruction
  {
  public:
    typedef T cref_type;
    template_instruction_ex(uint32_t            id,
                            const char*         name,
                            const char*         ns,
                            const char*         template_ns,
                            const char*         dictionary,
                            instructions_view_t subinstructions,
                            bool                reset,
                            const char*         typeref_name,
                            const char*         typeref_ns,
                            const char*         cpp_ns,
                            instruction_tag     tag = instruction_tag(),
                            int32_t             decimal_place = 0)
      : template_instruction(id, name, ns, template_ns, dictionary,
                             subinstructions, reset, typeref_name, typeref_ns,cpp_ns, tag, decimal_place)
    {
    }

    virtual template_instruction_ex<T>* clone(arena_allocator& alloc) const
    {
      return new (alloc) template_instruction_ex<T>(*this);
    }

  };

} /* mfast */


#endif /* end of include guard: TEMPLATE_INSTRUCTION_H_PARP21ZA */
