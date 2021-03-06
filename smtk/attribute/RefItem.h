//=========================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//=========================================================================
// .NAME RefItem.h -
// .SECTION Description
// .SECTION See Also

#ifndef __smtk_attribute_RefItem_h
#define __smtk_attribute_RefItem_h

#include "smtk/SMTKCoreExports.h"
#include "smtk/PublicPointerDefs.h"
#include "smtk/attribute/Item.h"
#include <vector>

namespace smtk
{
  namespace attribute
  {
    class Attribute;
    class RefItemDefinition;
    class ValueItemDefinition;
    class SMTKCORE_EXPORT RefItem : public Item
    {
      friend class RefItemDefinition;
      friend class ValueItemDefinition;
    public:
      smtkTypeMacro(RefItem);
      virtual ~RefItem();
      virtual Item::Type type() const;
      std::size_t numberOfValues() const
      {return this->m_values.size();}
      bool  setNumberOfValues(std::size_t newSize);
      std::size_t numberOfRequiredValues() const;
      smtk::attribute::AttributePtr value(std::size_t element=0) const
      {return this->m_values[element].lock();}
      bool setValue( smtk::attribute::AttributePtr val)
      {return this->setValue(0, val);}
      bool setValue(std::size_t element, smtk::attribute::AttributePtr val);
      bool appendValue(smtk::attribute::AttributePtr val);
      bool removeValue(std::size_t element);
      virtual void reset();
      virtual std::string valueAsString(const std::string &format="") const
      {return this->valueAsString(0, format);}
      virtual std::string valueAsString(std::size_t element, const std::string &format="") const;
      virtual bool isSet(std::size_t element=0) const
      {return this->m_values[element].lock().get() != NULL;}
      virtual void unset(std::size_t element=0);
      virtual void copyFrom(const smtk::attribute::ItemPtr sourceItem,
                            smtk::attribute::Item::CopyInfo& info);

    protected:
      RefItem(Attribute *owningAttribute, int itemPosition);
      RefItem(Item *owningItem, int myPosition, int mySubGroupPosition);
      virtual bool setDefinition(smtk::attribute::ConstItemDefinitionPtr def);
      void clearAllReferences();
      std::vector<attribute::WeakAttributePtr>m_values;
    private:
    };
  }
}


#endif /* __smtk_attribute_RefItem_h */
