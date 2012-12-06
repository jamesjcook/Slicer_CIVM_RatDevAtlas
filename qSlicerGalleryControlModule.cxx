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
#include <QTextStream>

// GalleryControl Logic includes
#include <vtkSlicerGalleryControlLogic.h>

// GalleryControl includes
#include "qSlicerGalleryControlModule.h"
#include "qSlicerGalleryControlModuleWidget.h"

//qSlicerGalleryControlModlue, class code
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
//----------------------------------------------------------------------------
//constructor
qSlicerGalleryControlModule
::qSlicerGalleryControlModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerGalleryControlModulePrivate)
{
}


//-----------------------------------------------------------------------------
//destructor
qSlicerGalleryControlModule::~qSlicerGalleryControlModule()
{
  
}

//-----------------------------------------------------------------------------
//helptext
QString qSlicerGalleryControlModule::helpText()const
{
  return "This is a loadable module bundled in an extension. Select a icon from the gallery list, then choose the datasets.\n";
}
//-----------------------------------------------------------------------------
//acknowledgetext
QString qSlicerGalleryControlModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}
//-----------------------------------------------------------------------------
//contributorstext
QStringList qSlicerGalleryControlModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("James Cook (CIVM Duke University), ");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)-Template");//default, added -template
  return moduleContributors;
}
//-----------------------------------------------------------------------------
//set icon
QIcon qSlicerGalleryControlModule::icon()const
{
  return QIcon(":/Icons/GalleryControl.tiff");
}
//-----------------------------------------------------------------------------
//set category in slicer menu
QStringList qSlicerGalleryControlModule::categories() const
{
  return QStringList() << "Work in Progress";
}
//-----------------------------------------------------------------------------
//set module dependencies, not sure how to do this one 
QStringList qSlicerGalleryControlModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
//called on module start
void qSlicerGalleryControlModule::setup()
{
  //  QTextStream out(stdout);
  //  out << "qSLicerGalleryControlModule SETUP RUN\n";
  this->printMethod(QString("setup"));
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerGalleryControlModule::createWidgetRepresentation()
{
  //  QTextStream out(stdout);
  //  out << "createWidgetRepresentation\n";
  this->printMethod(QString("createWidgetRepresentation"));
  return new qSlicerGalleryControlModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerGalleryControlModule::createLogic()
{
  //  QTextStream out(stdout);
  //  out << "createLogic Start\n";
  this->printMethod(QString("createLogic"));
  return vtkSlicerGalleryControlLogic::New();
}

void qSlicerGalleryControlModule::printMethod(const QString text)
{
  QTextStream out(stdout);
  out << "qSlicerGalleyControlModule method:"<<text<<"\n";
  return;
}


// QStringList qSlicerGalleryControlModule::checkboxes() // variable to hold on to the check boxes which are checked, just doing a simple string list for now
// {
//   this->printMethod(QString("checkboxes"));
//   QStringList checkboxes_("");
//   return checkboxes_;
// }
//   // some other data container would probbaly be better, but this works for now. 
// QString qSlicerGalleryControlModule::MRMLLayout()
// {
//   this->printMethod(QString("MRMLLayout"));
//   QString layoutname("Three over Three");
//   return layoutname;
// }
