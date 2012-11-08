/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerGalleryControlFooBarWidget.h"
#include "ui_qSlicerGalleryControlFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_GalleryControl
class qSlicerGalleryControlFooBarWidgetPrivate
  : public Ui_qSlicerGalleryControlFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerGalleryControlFooBarWidget);
protected:
  qSlicerGalleryControlFooBarWidget* const q_ptr;

public:
  qSlicerGalleryControlFooBarWidgetPrivate(
    qSlicerGalleryControlFooBarWidget& object);
  virtual void setupUi(qSlicerGalleryControlFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerGalleryControlFooBarWidgetPrivate
::qSlicerGalleryControlFooBarWidgetPrivate(
  qSlicerGalleryControlFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerGalleryControlFooBarWidgetPrivate
::setupUi(qSlicerGalleryControlFooBarWidget* widget)
{
  this->Ui_qSlicerGalleryControlFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerGalleryControlFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerGalleryControlFooBarWidget
::qSlicerGalleryControlFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerGalleryControlFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerGalleryControlFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerGalleryControlFooBarWidget
::~qSlicerGalleryControlFooBarWidget()
{
}
