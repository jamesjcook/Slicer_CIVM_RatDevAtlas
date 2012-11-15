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

#ifndef __qSlicerGalleryControlModuleWidget_h
#define __qSlicerGalleryControlModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerGalleryControlModuleExport.h"

class qSlicerGalleryControlModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_GALLERYCONTROL_EXPORT qSlicerGalleryControlModuleWidget :  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerGalleryControlModuleWidget(QWidget *parent=0);
  virtual ~qSlicerGalleryControlModuleWidget();
  void printMethod(QString);
public slots:

  bool setTimeContrastLayout();
  bool setMultiContrastLayout();
  bool setOrthagonalLayout();
  void callPerlScriptAndLoadMRML();

protected:
  QScopedPointer<qSlicerGalleryControlModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerGalleryControlModuleWidget);
  Q_DISABLE_COPY(qSlicerGalleryControlModuleWidget);
};

#endif
