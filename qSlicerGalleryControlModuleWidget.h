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
  //  virtual std::unordered_map checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  //  virtual std::map<QString,bool> checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  //  std::multimap<QString,bool> checkboxes();  // holds checkbox states. will name checkboxes according to the QT UI, which is c_[a-z]+ and t_[0-9]{2} 
  bool Timepoints[100];// easy way to keep track of waht time tpoints, undefined elements(false ones) are not considered
  bool Contrasts[10];
  bool LoadLabels;
  //  int Orientation; // not used commented
  int GalleryTimepoints;  //number of time points to load
  int GalleryContrasts;   //number of contrasts to load
  int GalleryOrientations;//number of orientations to display(generally 1 or 3). 
  int Gallery2DViews;     //number of 2D views on screen. 
  QStringList SceneNodes;
  //  QStringList Scene2DNodes; 
  //  QStringList Scene3DNodes;
  int Gallery3DViews;     //number of 3d views on screen.
  QString Layout;         //String name of the layout to use.
  QString DataPath;       //path to data set in the setup function, will have to be 
                          //intelligently modified for the demo version. perhaps we'll just use system links for now.
  QString LabelPath;      //see datapath.
  QString DataPattern;    //pattern for data file to have parts replacd wit actual contrast and timepoint information. 
  QString LabelPattern;   //see datapattern
  void PrintMethod(QString);//print to console with constant text+message
  void PrintText(QString); //print to console
  QStringList SetLayout();       //defunct. function to change the layout.
  void ShowNodesInScene(); // defunct. Debug function to print node names in the scene. 
  
  const char * NodeID(QString); //defunct.

public slots:
  void SetTimeContrastLayout();  //set variables for given layout
  void SetMultiContrastLayout(); //set variables for given layout
  void SetOrthogonalLayout();    //set variables for given layout
  void SetDual3DLayout();        //set variables for given layout
  void SetLabels();              
  void CallPerlScriptAndLoadMRML(); //only function w hich relied onc alling the crummy piece of substitution perl. 
  void BuildScene();             //sets up mrml nodes loads data
  void OrientationTest();        //defunct.
  void SetCheckBox();            //defunct.
  QStringList GetContrasts();    //will look up all the checkboxes in contrasts group and make a qstringlist of values
  QStringList GetTimepoints();   // will look up all the checkboxes in timepionts gorup and make a qstringlist of values. 
  bool NodeExists(QString);      // checks scene for existing node with given name. Used in the build scene fucntion to avoid reloading data.

protected:
  QScopedPointer<qSlicerGalleryControlModuleWidgetPrivate> d_ptr;
  virtual void setup();

private:
  //  QSignalMapper *signalMapper;
  Q_DECLARE_PRIVATE(qSlicerGalleryControlModuleWidget);
  Q_DISABLE_COPY(qSlicerGalleryControlModuleWidget);
};

#endif







