/*=========================================================================

   Program: ParaView
   Module:    qtColorButton.h

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef pq_ColorChooserButton_h
#define pq_ColorChooserButton_h

#include "smtk/Qt/qtItem.h"

#include <QToolButton>
#include <QColor>

namespace smtk
{
  namespace attribute
  {
     class QTSMTK_EXPORT qtColorButton : public QToolButton
    {
      Q_OBJECT
      Q_PROPERTY(QColor chosenColor READ chosenColor WRITE setChosenColor)
    public:
      /// constructor requires a QComboBox
      qtColorButton(QWidget* p);
      /// get the color
      QColor chosenColor() const;

    signals:
      /// signal color changed
      void chosenColorChanged(const QColor&);
      /// signal color selected
      void validColorChosen(const QColor&);

    public slots:
      /// set the color
      virtual void setChosenColor(const QColor&);

      /// show a dialog to choose the color
      virtual void chooseColor();

    protected:
      /// overridden to resize the color icon.
      virtual void resizeEvent(QResizeEvent *rEvent);

      /// renders an icon for the color.
      QIcon renderColorSwatch(const QColor&);

      QColor Color;
    };
  };
};
#endif
