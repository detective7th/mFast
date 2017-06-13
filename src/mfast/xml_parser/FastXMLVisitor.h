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
//
#ifndef FASTXMLVISITOR_H_ZMKQ7SDU
#define FASTXMLVISITOR_H_ZMKQ7SDU
#include <stdint.h>
#include <vector>
#include <exception>
#include <fstream>
#include <string>
#include <cstring>
#include <deque>

#include "tinyxml2.h"

using namespace tinyxml2;

class FastXMLVisitor
  : public XMLVisitor
{
protected:
  struct instruction_context
  {
    std::string type_;
    std::string ns_;
    std::string dictionary_;
  };

  std::vector<uint32_t> num_fields_;
  std::deque<instruction_context> context_stack_;

  instruction_context& current_context();
  void save_context(const XMLElement & element);
  const char* get_optional_attr(const XMLElement & element, const char* attr_name, const char* default_value) const;
  bool is_mandatory_constant(const XMLElement & element);

  // if element has only a child except the length element, the child is returned; otherwise, it return 0.
  const XMLElement* only_child(const XMLElement & element);

  // if element has only child  (except length element) and the child element is templateRef, it returns
  // the address of the child element; otherwise it returns 0.
  const XMLElement* only_child_templateRef(const XMLElement & element);

public:
  FastXMLVisitor();
  virtual bool VisitEnterTemplates(const XMLElement & /* element */);
  virtual bool VisitExitTemplates(const XMLElement & /* element */, std::size_t /* numFields */);
  virtual bool VisitEnterTemplate(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitExitTemplate(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t /* numFields */, std::size_t index);
  virtual bool VisitTemplateRef(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitEnterGroup(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitExitGroup(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t /* numFields */, std::size_t index);
  virtual bool VisitEnterSequence(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitExitSequence(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t /* numFields */, std::size_t index);
  virtual bool VisitInteger(const XMLElement & /* element */, int integer_bits, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitDecimal(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitString(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitByteVector(const XMLElement & /* element */, const std::string& /* name_attr */, std::size_t index);
  virtual bool VisitEnterDefine(const XMLElement & /* element */, const std::string& /* name_attr */);
  virtual bool VisitExitDefine(const XMLElement & /* element */, const std::string& /* name_attr */);
  virtual bool VisitIntVector(const XMLElement & /* element */, int integer_bits, const std::string& /* name_attr */, std::size_t index);

  virtual bool  VisitEnter (const XMLElement & element, const XMLAttribute* attr);
  virtual bool  VisitExit (const XMLElement & element);
};


#endif /* end of include guard: FASTXMLVISITOR_H_ZMKQ7SDU */
