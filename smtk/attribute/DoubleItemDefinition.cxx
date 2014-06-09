/*=========================================================================

Copyright (c) 1998-2012 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO
PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
=========================================================================*/


#include "smtk/attribute/DoubleItemDefinition.h"
#include "smtk/attribute/DoubleItem.h"
using namespace smtk::attribute;

//----------------------------------------------------------------------------
DoubleItemDefinition::DoubleItemDefinition(const std::string &myName):
  ValueItemDefinitionTemplate<double>(myName)
{
}

//----------------------------------------------------------------------------
DoubleItemDefinition::~DoubleItemDefinition()
{
}
//----------------------------------------------------------------------------
Item::Type DoubleItemDefinition::type() const
{
  return Item::DOUBLE;
}
//----------------------------------------------------------------------------
smtk::attribute::ItemPtr
DoubleItemDefinition::buildItem(Attribute *owningAttribute,
                                int itemPosition) const
{
  return smtk::attribute::ItemPtr(new DoubleItem(owningAttribute,
                                                itemPosition));
}
//----------------------------------------------------------------------------
smtk::attribute::ItemPtr
DoubleItemDefinition::buildItem(Item *owningItem,
                                int itemPosition,
                                int subGroupPosition) const
{
  return smtk::attribute::ItemPtr(new DoubleItem(owningItem,
                                                itemPosition,
                                                subGroupPosition));
}
//----------------------------------------------------------------------------
smtk::attribute::ItemDefinitionPtr
smtk::attribute::DoubleItemDefinition::
createCopy(smtk::attribute::ItemDefinition::CopyInfo& info) const
{
  smtk::attribute::DoubleItemDefinitionPtr newDef =
    smtk::attribute::DoubleItemDefinition::New(this->name());

  ValueItemDefinitionTemplate<double>::copyTo(newDef, info);
  return newDef;
}
//----------------------------------------------------------------------------
