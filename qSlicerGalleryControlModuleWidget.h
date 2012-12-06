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

//cpp includes
//#include <iostream> 
//#include <string>

//#include <map>
//#include <tr1/unordered_map> 



class qSlicerGalleryControlModuleWidgetPrivate;
class vtkMRMLNode;


// unordered map junk
//typedef std::unordered_map<std::int,std::bool> numbool_map;
/*stringmap merge (stringmap a,stringmap b) { 
  stringmap temp(a); temp.insert(b.begin(),b.end()); return temp; 
  } */



/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_GALLERYCONTROL_EXPORT qSlicerGalleryControlModuleWidget :  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerGalleryControlModuleWidget(QWidget *parent=0);
  virtual ~qSlicerGalleryControlModuleWidget();
  void printMethod(QString);//print to console with constant text+message
  void printText(QString); //print to console
  //  virtual std::unordered_map checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  //  virtual std::map<QString,bool> checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  //  std::multimap<QString,bool> checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  bool timepoints[100];// easy way to keep track of waht time tpoints, undefined elements(false ones) are not considered
  bool contrasts[10];
  int gallery_timepoints;
  int gallery_contrasts;
  QString layout;


public slots:

  void setTimeContrastLayout();
  void setMultiContrastLayout();
  void  setOrthagonalLayout();
  void callPerlScriptAndLoadMRML();
  void setCheckBox();
  QString getContrasts(); //will look up all the checkboxes in contrasts group and make a string of their values
  QString getTimepoints(); // will look up all the checkboxes in timepionts gorup and make a string of their values. 


protected:
  QScopedPointer<qSlicerGalleryControlModuleWidgetPrivate> d_ptr;
  virtual void setup();

private:
  //  QSignalMapper *signalMapper;
  Q_DECLARE_PRIVATE(qSlicerGalleryControlModuleWidget);
  Q_DISABLE_COPY(qSlicerGalleryControlModuleWidget);
};

#endif







