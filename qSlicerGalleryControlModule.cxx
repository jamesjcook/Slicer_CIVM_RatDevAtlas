/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QtPlugin>

// GalleryControl Logic includes
#include <vtkSlicerGalleryControlLogic.h>

// GalleryControl includes
#include "qSlicerGalleryControlModule.h"
#include "qSlicerGalleryControlModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerGalleryControlModule, qSlicerGalleryControlModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGalleryControlModulePrivate
{
public:
  qSlicerGalleryControlModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerGalleryControlModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerGalleryControlModulePrivate
::qSlicerGalleryControlModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerGalleryControlModule methods

//-----------------------------------------------------------------------------
qSlicerGalleryControlModule
::qSlicerGalleryControlModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerGalleryControlModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerGalleryControlModule::~qSlicerGalleryControlModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerGalleryControlModule::helpText()const
{
  return "This is a loadable module bundled in an extension. Select a icon from the gallery list, then choose the datasets.";
}

//-----------------------------------------------------------------------------
QString qSlicerGalleryControlModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerGalleryControlModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("James Cook (CIVM Duke University)");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)-Template");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerGalleryControlModule::icon()const
{
  return QIcon(":/Icons/GalleryControl.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerGalleryControlModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerGalleryControlModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerGalleryControlModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerGalleryControlModule
::createWidgetRepresentation()
{
  return new qSlicerGalleryControlModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerGalleryControlModule::createLogic()
{
  return vtkSlicerGalleryControlLogic::New();
}
