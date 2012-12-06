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

#ifndef __qSlicerGalleryControlModule_h
#define __qSlicerGalleryControlModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerGalleryControlModuleExport.h"
//qSlicerGalleryControlModlue, class definition
class qSlicerGalleryControlModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_GALLERYCONTROL_EXPORT
qSlicerGalleryControlModule
  : public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerGalleryControlModule(QObject *parent=0);
  virtual ~qSlicerGalleryControlModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  virtual QIcon icon()const;

  virtual QStringList categories()const;
  virtual QStringList dependencies() const;

  //these state variable should probably be private but again, we'll just do another for now.
/*   virtual QStringList checkboxes(QString ); // variable to hold on to the check boxes which are checked, just doing a simple string list for now */
/*   // some other data container would probbaly be better, but this works for now.  */
/*   virtual QString MRMLLayout(); */
  void printMethod(QString);
  
protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerGalleryControlModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerGalleryControlModule);
  Q_DISABLE_COPY(qSlicerGalleryControlModule);

};

#endif
