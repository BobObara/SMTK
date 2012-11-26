/*=========================================================================

Copyright (c) 1998-2003 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
// .NAME qtAttributeSection - the Attribute Section
// .SECTION Description
// .SECTION See Also
// qtSection

#ifndef __slctk_attribute_qtAttributeSection_h
#define __slctk_attribute_qtAttributeSection_h

#include "qtSection.h"

class qtAttributeSectionInternals;
class QTableWidgetItem;
class QKeyEvent;

namespace slctk
{
  namespace attribute
  {
    class SLCTKATTRIBUTE_EXPORT qtAttributeSection : public qtSection
    {
      Q_OBJECT

    public:
      qtAttributeSection(slctk::SectionPtr, QWidget* p);
      virtual ~qtAttributeSection();

      QTableWidgetItem* getSelectedItem();
      int currentViewBy();
      int currentCategory();
      void showUI(int viewBy, int category);
      virtual void createNewAttribute(slctk::AttributeDefinitionPtr attDef);

      enum enumViewBy
        {
        VIEWBY_Attribute = 0,
        VIEWBY_PROPERTY
        };

    public slots:
      void onViewBy(int);
      void onViewByWithDefinition(
        int viewBy, slctk::AttributeDefinitionPtr attDef);
      void onShowCategory(int category);
      void onListBoxSelectionChanged();
      void onAttributeValueChanged(QTableWidgetItem*);
      void onAttributeNameChanged(QTableWidgetItem*);
      void onCreateNew();
      void onCopySelected();
      void onDeleteSelected();
      void onAttributeModified();

      void showAdvanced(int show);

    protected:
      virtual void createWidget( );
      slctk::ValueItemPtr getArrayDataFromItem(QTableWidgetItem * item);
      slctk::AttributePtr getAttributeFromItem(QTableWidgetItem * item);
      slctk::ValueItemPtr getSelectedArrayData();
      slctk::AttributePtr getSelectedAttribute();
      slctk::ValueItemPtr getAttributeArrayData(slctk::AttributePtr aAttribute);
      QTableWidgetItem* addAttributeListItem(slctk::AttributePtr childData);
      void addAttributePropertyItems(
        slctk::AttributePtr childData, const QString& group);
      void updateTableWithAttribute(slctk::AttributePtr dataItem, const QString& group);
      void updateTableWithProperty(QString& propertyName);
      void addTableValueItems(
        slctk::AttributeItemPtr childData, int& numRows, bool bEnabled);

      void updateChildWidgetsEnableState(
        slctk::ValueItemPtr linkedData, QTableWidgetItem* item);
      virtual void getAllDefinitions();
      bool hasMultiDefinition(const QString& group);

    private:

      qtAttributeSectionInternals *Internals;

    }; // class
  }; // namespace attribute
}; // namespace slctk


#endif