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
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QFileSystemWatcher>

#include <QSignalMapper>
//#include <QComboBox> //not needed, we already used one without this include

// SlicerQt includes
#include "qSlicerGalleryControlModuleWidget.h"
#include "ui_qSlicerGalleryControlModule.h"

#include "qSlicerApplication.h"
#include "qSlicerIOManager.h"
#include "qSlicerLayoutManager.h"

//#include "qSlicerCoreApplication.h"
#include "qSlicerApplication.h"
#include "qSlicerCoreIOManager.h"
#include "qSlicerIOManager.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLCoreTestingMacros.h"
#include "qSlicerFileReader.h"
#include "qSlicerAbstractModule.h"
//#include "qSlicerAppAboutDialog.h"
#include "qSlicerActionsDialog.h"
#include "qSlicerLayoutManager.h"

//#include "qSlicerAppMainWindowCore_p.h"
#include "qSlicerModuleManager.h"
//#include "qSlicerSlicer2SceneReader.h"

// #include "Modules/Loadable/Data/qSlicerSceneIO.h"

//cmdline includes
//# include "tclap/CmdLine.h"


//#include  <Modules/Loadable/Volumes/qSlicerVolumesIO.h>

// vtkSlicerLogic includes
#include "vtkSlicerTransformLogic.h"

// MRMLWidgets includes
#include <qMRMLUtils.h>
#include <qMRMLSliceWidget.h>
#include <qMRMLSliceControllerWidget.h>
#include <qMRMLThreeDWidget.h>
#include <qMRMLThreeDViewControllerWidget.h>


// MRML includes
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLAnnotationFiducialNode.h"
#include "vtkMRMLFiducial.h"
#include "vtkMRMLFiducialListNode.h"
#include "vtkMRMLFiducialListStorageNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLSliceNode.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLViewNode.h"
//#include <Libs/MRML/Core/vtkMRMLViewNode.h> //testing this here for science that failed....
#include "vtkMRMLAbstractViewNode.h"
#include "vtkMRMLScalarVolumeNode.h"

// MRMLLogic includes
#include "vtkMRMLLayoutLogic.h"

// VTK includes
#include <vtkCallbackCommand.h>
#include "vtkPolyData.h"
#include <vtkLineSource.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkCellArray.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include "vtkCollection.h"
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include "vtkSTLReader.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataNormals.h"
#include <vtkExodusIIReader.h>
#include <vtkCollection.h>
#include <vtkSmartPointer.h>

#include <vtkMRMLLayoutNode.h>
#include <vtkMRMLSliceLogic.h>

// CTK includes
//#include <ctkComboBox.h> //not needed we already used one without this include

//layout definitions


const char* twoSliceView =
  "<layout type=\"vertical\">"
  " <item>"
  "    <view class=\"vtkMRMLSliceNode\" singletontag=\"Red\">"
  "     <property name=\"orientation\" action=\"default\">Axial</property>"
  "     <property name=\"viewlabel\" action=\"default\">R</property>"
  "     <property name=\"viewcolor\" action=\"default\">#F34A33</property>"
  "    </view>"
  " </item>"
  " <item>"
  "    <view class=\"vtkMRMLSliceNode\" singletontag=\"Green\">"
  "     <property name=\"orientation\" action=\"default\">Sagittal</property>"
  "     <property name=\"viewlabel\" action=\"default\">Y</property>"
  "     <property name=\"viewcolor\" action=\"default\">#EDD54C</property>"
  "    </view>"
  " </item>"
  "</layout>";

//qSlicerGalleryControlModlueWidget, class code
//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate

class qSlicerGalleryControlModuleWidgetPrivate: public Ui_qSlicerGalleryControlModule
{
public:
  qSlicerGalleryControlModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerGalleryControlModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerGalleryControlModuleWidgetPrivate::qSlicerGalleryControlModuleWidgetPrivate()
{
  //  this->PrintMethod(QString("private call"));// doesnt work, not a method...
}

//-----------------------------------------------------------------------------
// qSlicerGalleryControlModuleWidget methods

//-----------------------------------------------------------------------------
//constructor
qSlicerGalleryControlModuleWidget::qSlicerGalleryControlModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerGalleryControlModuleWidgetPrivate )
{
  this->PrintMethod(QString("constructor"));
}

//-----------------------------------------------------------------------------
//destructor
qSlicerGalleryControlModuleWidget::~qSlicerGalleryControlModuleWidget()
{
  this->PrintMethod(QString("destructor"));
}

//-----------------------------------------------------------------------------
void qSlicerGalleryControlModuleWidget::setup()
{
  this->PrintMethod(QString("setup"));
  Q_D(qSlicerGalleryControlModuleWidget);
  d->setupUi(this);

  this->ClearCheckboxes();
  this->Layout=QString("No_Layout");
  this->GalleryTimepoints=0;
  this->GalleryContrasts=0;
  this->GalleryOrientations=0;
  this->LoadLabels=false;


  /* Load data paths */
  /* hardcode for now*/
  /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
  /* labels are at /DataPath/labels/completed/completed_good_header/nii/           */
  
  this->DataPath=QString("/ratdevatlas/timepoint/average/");
  this->LabelPath=QString("/ratdevatlas/labels/completed/completed_good_header/nii/");
  //  this->DataPattern  << "p"   << "timepoint" << "_average_" << "contrast" << ".nii";
  //  this->LabelPattern << "pnd" << "timepoint" << "_average_labels.nii";
  this->DataPattern  =QString("ptimepoint_average_contrast.nii");
  this->LabelPattern =QString("pndtimepoint_average_labels.nii");
  this->MSProcessLoadOrder << "_Rss" << "_R" << "_ss" << "";
  this->ContrastLoadOrder << "FLAIR" << "T2" ;
  this->MSGender="Female";
  this->CenterVolumeOnLoad=true;

  // using this mrml to initalize the 3d views for now, and then set to the first gallery setup.
  QString out_path = DataPath+"/DoubleBlank.mrml";
  qSlicerApplication * app = qSlicerApplication::application();
  app->ioManager()->loadScene(out_path);
  out_path = DataPath+"/Blank.mrml";
  app->ioManager()->loadScene(out_path);  


  // these connections were setup in the *.ui file, but that seems to have been broken
  /*  connect(d->setTimeContrastLayoutButton, SIGNAL(released()),
      this, SLOT(SetTimeContrastLayout()));
      connect(d->SetOrthogonalLayoutButton, SIGNAL(released()),
      this, SLOT(SetOrthogonalLayout()));
      connect(d->SetMultiContastLayoutButton, SIGNAL(released()),
      this, SLOT(SetMultiContastLayout()));*/
  QSignalMapper* signalMapper;
  
  signalMapper = new QSignalMapper(this);

  /*  signalMapper = new QSignalMapper(this);
      signalMapper->setMapping(taxFileButton, QString("taxfile.txt"));
      signalMapper->setMapping(accountFileButton, QString("accountsfile.txt"));
      signalMapper->setMapping(reportFileButton, QString("reportfile.txt"));
      
      connect(taxFileButton, SIGNAL(clicked()),
      signalMapper, SLOT (map()));
      connect(accountFileButton, SIGNAL(clicked()),
      signalMapper, SLOT (map()));
      connect(reportFileButton, SIGNAL(clicked()),
      signalMapper, SLOT (map()));*/
  
  //  this works so long as we dont try to connect in QtDesigner
  /*  connect(d->t_02, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_04, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_08, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_12, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_18, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_24, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_40, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
      connect(d->t_80, SIGNAL(toggled(bool)), SLOT(SetCheckBox()));
  */

  /*  if (d->t_00->isChecked()) {; //just test setting of the checkbox
    PrintMethod(QString("checktrue"));
    }*/
  d->GallerySelectionComboBox->setCurrentIndex(0);
  d->GallerySelectionStack->setCurrentIndex(d->GallerySelectionComboBox->currentIndex());
  d->orientationComboBox->setEnabled(false);

  connect(d->DatasetLabelsOn, SIGNAL(clicked()), SLOT(SetLabels()));
  connect(d->setTimeContrastLayoutButton, SIGNAL(clicked()), SLOT(SetTimeContrastLayout()));
  connect(d->setMultiContrastLayoutButton, SIGNAL(clicked()), SLOT(SetMultiContrastLayout()));
  connect(d->setOrthogonalLayoutButton, SIGNAL(clicked()), SLOT(SetOrthogonalLayout()));
  connect(d->setDual3DLayoutButton, SIGNAL(clicked()), SLOT(SetDual3DLayout()));
  connect(d->setMSSingleLayoutButton, SIGNAL(clicked()), SLOT(SetMSSingleLayout()));
  connect(d->setMSDualLayoutButton, SIGNAL(clicked()), SLOT(SetMSDualLayout()));
  connect(d->setMSComparisonLayoutButton, SIGNAL(clicked()), SLOT(SetMSComparisonLayout()));
  connect(d->GallerySelectionComboBox,SIGNAL(currentIndexChanged(int)),SLOT(ChangeGallery())); 

  //  connect(d->LoadDataButton, SIGNAL(released()), SLOT(CallPerlScriptAndLoadMRML()));
  connect(d->LoadDataButton, SIGNAL(released()), SLOT(BuildScene()));
//  d->GallerySelectionComboBox->setCurrentIndex(0); // this should signal change gallery to run
//  this->ChangeGallery();
//  d->GallerySelectionStack->setCurrentIndex(d->GallerySelectionComboBox->currentIndex());  

//  connect(d->LoadDataButton, SIGNAL(released()), SLOT(OrientationTest()));

  //this has to happen after we've changed the combobox index or called change gallery
  this->GalleryName = d->GallerySelectionComboBox->currentText();
  this->Superclass::setup();
//  this->ChangeGallery(); //this cannot work and i dont know why
}


QString qSlicerGalleryControlModuleWidget::LookupMS(QString  age, QString gender) 
{ 
//  Q_D(qSlicerGalleryControlModuleWidget);
  this->PrintText("Age:"+age+" Gender:"+gender);
  if (age=="34"&&gender=="Female") 
    {
    return "MS001";
    }
  if (age=="33"&&gender=="Female")
    {
    return "MS002";
    }
  age=gender;
  return "MS002"; 
}

bool qSlicerGalleryControlModuleWidget::fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
}

QString qSlicerGalleryControlModuleWidget::LookupControl(QString  age, bool match_gender) 
{ 
//  Q_D(qSlicerGalleryControlModuleWidget);
  age="";
  match_gender=false;
  return "Control002";
}
void qSlicerGalleryControlModuleWidget::ChangeGallery()
{// set the relevent data paths and pattern for each gallery
 // this relies on the combobox and the stacked widget using the same page number/id number for galleries. 
  Q_D(qSlicerGalleryControlModuleWidget);  
  this->PrintMethod("ChangeGallery");
  this->GalleryName = d->GallerySelectionComboBox->currentText();
  d->GallerySelectionStack->setCurrentIndex(d->GallerySelectionComboBox->currentIndex());  
  // assign the index of the selection box to the widget stack
  this->ClearCheckboxes();//doesnt clear the ui, just the settings behind the scene the ui connects with, this will let us use the same timepoint and contrast array for any library.
  if(this->GalleryName=="Rat Development Atlas")
    {//Rat Development Atlas
    this->DataPath=QString("/ratdevatlas/timepoint/average/");
    this->LabelPath=QString("/ratdevatlas/labels/completed/completed_good_header/nii/");
    //  this->DataPattern  << "p"   << "timepoint" << "_average_" << "contrast" << ".nii";
    //  this->LabelPattern << "pnd" << "timepoint" << "_average_labels.nii";
    this->DataPattern  =QString("ptimepoint_average_contrast.nii");
    this->LabelPattern =QString("pndtimepoint_average_labels.nii");
    this->CenterVolumeOnLoad=true;
    }
  else if (this->GalleryName=="Multiple Sclerosis")
    {//Multiple Sclerosis

    this->DataPath=QString("/MS_Library/ptypepid/");
    this->LabelPath=QString(DataPath);
    // ptype is patient type, MS or CONTROL, 
    // pid is number, might modifiy to just have pid in future contining the ptype
    // pstatus is processin status, nothing, _R, _ss or _Rss, 
    // contrast is MR ADC FA L1 L2 L3 RD
    //            MR is loading preference Flair/t2, etc
    this->DataPattern  =QString("ptypepidpstatus_contrast.hdr");
    this->LabelPattern =QString("ptypepid_regtype_regsub.hdr"); 
    this->MSGender="Female";
    this->CenterVolumeOnLoad=true;
    }
  else
    {
    this->PrintText("Cannout understand gallery selection, there must be a typo or something.");
    }
  this->SetLayout();
  return;
}

void qSlicerGalleryControlModuleWidget::SetTimeContrastLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("TimeContrast"));
  this->Layout=QString("TimeContrast");
  this->GalleryTimepoints=3;
  this->GalleryContrasts=2;
  this->GalleryOrientations=1;
  this->Gallery2DViews=6;
  this->Gallery3DViews=0;

  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return ;
}

void qSlicerGalleryControlModuleWidget::SetMultiContrastLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("MultiContrast"));
  this->Layout=QString("MultiContrast");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=4;
  this->GalleryOrientations=1;
  this->Gallery2DViews=4;
  this->Gallery3DViews=0;

  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return ;
}
void qSlicerGalleryControlModuleWidget::SetOrthogonalLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(false);
  this->PrintMethod(QString("Orthogonal"));
  this->Layout=QString("Orthogonal");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=1;
  this->GalleryOrientations=3;
  this->Gallery2DViews=3;
  this->Gallery3DViews=1;

  this->PrintMethod("Setting Layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return;
}
void qSlicerGalleryControlModuleWidget::SetDual3DLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("Dual3D"));
  this->Layout=QString("Dual3D");
  /* could use special code for GalleryTimepoints to check the 
     number of timepoints and contrasts in build mrml such that 
     we can do either 2 timepoints or two contrats*/
  this->GalleryTimepoints=2;
  this->GalleryContrasts=1;
  this->GalleryOrientations=1;
  this->Gallery2DViews=3;
  this->Gallery3DViews=2;
  this->PrintMethod("Setting Layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return;
}
void qSlicerGalleryControlModuleWidget::SetMSSingleLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("MSSingle"));
  this->Layout=QString("MSSingle");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=1;
  this->GalleryOrientations=1;
  this->Gallery2DViews=1;
  this->Gallery3DViews=0;

  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return ;
}
void qSlicerGalleryControlModuleWidget::SetMSDualLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("MSDual"));
  this->Layout=QString("MSDual");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=2;
  this->GalleryOrientations=1;
  this->Gallery2DViews=2;
  this->Gallery3DViews=0;

  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return ;
}
void qSlicerGalleryControlModuleWidget::SetMSComparisonLayout()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("MSComparison"));
  this->Layout=QString("MSComparison");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=2;
  this->GalleryOrientations=1;
  this->Gallery2DViews=4;
  this->Gallery3DViews=0;

  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
//  this->SceneNodes=this->SetLayout(this->Layout); 
  this->SceneNodes=this->SetLayout();
  return ;
}
void qSlicerGalleryControlModuleWidget::SetLabels()
{
  Q_D(qSlicerGalleryControlModuleWidget);
  LoadLabels=d->DatasetLabelsOn->isChecked();
//   if ( LoadLabels) {
//     this->PrintText(QString("labelson"));
//   } else  {
//     this->PrintText(QString("labelsoff"));
//   }
  return;
}

void qSlicerGalleryControlModuleWidget::SetCheckBox()
{
  //  Q_D(qSlicerGalleryControlModuleWidget); //i think the Q_D line connects us back to our parent widget
  this->PrintMethod(QString("SetCheckBox"));
  //get checkbox name
  //d->Properties["t_02"]= d->t_02->isChecked();// derp not set derp. must declare something
  //this->checkboxes.insert(std::pair<QString,bool>(QString("t_02"),true));

  // these checks work when we dont ahve any connection in the .ui file from qt designer.
//   this->Timepoints[0]=d->t_00->isChecked();
//   this->Timepoints[2]=d->t_02->isChecked();
//   this->Timepoints[4]=d->t_04->isChecked();
//   this->Timepoints[8]=d->t_08->isChecked();
//   this->Timepoints[12]=d->t_12->isChecked();
//   this->Timepoints[18]=d->t_18->isChecked();
//   this->Timepoints[24]=d->t_24->isChecked();
//   this->Timepoints[40]=d->t_40->isChecked();
//   this->Timepoints[80]=d->t_80->isChecked();


  return;
}

void qSlicerGalleryControlModuleWidget::OrientationTest() 
{
  QString orientation="Axial"; // this could be axial or sagittal. it gets set by a qt qui setting in my production code
  QStringList sceneNodes;
  sceneNodes << "Red"
             << "Yellow"
             << "Green"
             << "Slice4"
             << "Slice5"
             << "Slice6";
  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  int viewerNum=0;
  QString sliceNodeID=QString("vtkMRMLSlice")+sceneNodes[viewerNum]; // this line was the source of my error, it original said vtkMRMLSlice   instead of vtkMRMLSliceNode.
  vtkMRMLNode      *sn        = currentScene->GetNodeByID(sliceNodeID.toStdString());
  vtkMRMLSliceNode *sliceNode;
  if ( sn != NULL )
    {
    sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode                                                                
    }
  else
    {
    this->PrintText("Bad slice node id"+sliceNodeID);
    return;
    }
  sliceNode->SetOrientation(orientation.toLatin1());
  return;
}

//-----------------------------------------------------------------------------
// build mrml and load datasests
void qSlicerGalleryControlModuleWidget::BuildScene()
{
  this->PrintMethod(QString("BuildScene"));
  Q_D(qSlicerGalleryControlModuleWidget); //i think the Q_D line connects us back to our parent widget

  // last minute settings read
  QStringList timepointList = this->GetTimepoints();
  QStringList contrastList  = this->GetContrasts();
  QString     orientation   = d->orientationComboBox->currentText(); //meaningless for orthagonol layout
  QString     labelFile;     //singular label file var
  QString     labelPath;     //singular label path var
  QString     imageFile;     //singular image file var
  QString     imagePath;     //singular image path
  QString     nodeName;      //singular var for imagefile on load
  QString     labelNode;     //node name for the label node
  QList <qSlicerIO::IOProperties> imageProperties; // container to hold all the properties(including name,path,nodename) of images to have open 
  QList <qSlicerIO::IOProperties> labelProperties; // container to hold all the properties(including name,path,nodename) of labels to have open will have an entry for each image element of imageProperties
//  QStringList imageFileList; //list of image files to load.
//  QStringList imagePathList; //list of image file paths to load.
//  QStringList labelFileList; //list of label files to load.
//  QStringList labelPathList; //list of label files to load.
  QList <qSlicerIO::IOProperties> unloadedFiles; //container for the parameters of unloadedfiles. 
  //DataPath
  //LabelPath
  //DataPattern (replace timepoint with numbers and contrast with abreviation)
  //LabelPattern(replace timepoint with numbers)

  ////
  // initialize scene
  ////
  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.

  bool setSliceOrient=true;
  if (this->GalleryOrientations==3) 
    { 
    setSliceOrient=false;
    }

  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout. Will probably fail if more than one layout is allowed. Oh well for now.
  
  QStringList orthogonalOrientations;
  orthogonalOrientations << "Axial"
                         << "Sagittal"
                         << "Coronal";


  //build list of files to load.
  for (int c=0;c<contrastList.size();c++) 
    {
    for (int t=0;t<timepointList.size(); t++) 
      {
      qSlicerIO::IOProperties tParams; //temp params 
      // add data to file list
      imageFile=DataPattern;
      if(this->GalleryName=="Rat Development Atlas") 
        {
        imageFile.replace("timepoint",timepointList[t]);
        imageFile.replace("contrast",contrastList[c]);
        //confusingly DataPath is root to data directories
        imagePath = DataPath+imageFile;
        imagePath.replace("timepoint",timepointList[t]);
        nodeName=imageFile;
        nodeName.replace(".nii","");
        tParams["fileName"]       = imagePath;
        tParams["nodeName"]       = nodeName;
        tParams["labelmap"]       = false;
        tParams["center"]         = this->CenterVolumeOnLoad;
        tParams["autoWindowLevel"]= false;
        tParams["fileType"]="VolumeFile";
        labelFile=LabelPattern;
        labelFile.replace("timepoint",timepointList[t]);
        //confusingly LabelPath is root to label root
        labelPath = LabelPath+labelFile;
        labelNode=labelFile;
        labelNode.replace(".nii","");
        tParams["labelNode"]      =labelNode; //will end up as duplicate of nodeName in the labelProperties entries, but this is easier.
        imageProperties << tParams;
        this->PrintText("imageProperties << "+imageFile);
        this->PrintText("Added volume to load list:"+imagePath);
        if( LoadLabels )
          {
          nodeName                   = labelNode;
          tParams["fileName"]        = labelPath;
          tParams["nodeName"]        = nodeName;
          tParams["labelmap"]        = true;     
          tParams["center"]          = this->CenterVolumeOnLoad;
          tParams["autoWindowLevel"] = false;
          labelProperties << tParams;
          this->PrintText("labelProperties << "+labelFile);
          this->PrintText("Added labels to load list:"+labelPath);
          }
        }
      else if ( this->GalleryName=="Multiple Sclerosis") 
        // diseasemodel data, must load 2 datasets, with an ifcheck load labels. go through process twice once for patient and once for control
        {//imageFile=DataPattern; //set on start here.
        QString patient = this->LookupMS(timepointList[t],MSGender);//returns MS001 for now
        //eventually should migrate to lookupDiseaseModelData
        QString control = this->LookupControl(patient,0);           //returns Control001 for now
        QStringList pList;
        pList << patient << control;
        //leaves us with /MS_Library/MS001/MS001pstatuspcontrast.hdr 
        //must preferentially load pstatus, then pcontrast if contrast is MR
        QString tFile;
        QString tPath;//=imagePath;
        QString rtype="LDDMM";
        QString regsub="WMPM_II";
        //checks for data
        for (int pa=0;pa<pList.size();pa++) 
          {
          patient=pList[pa];
          imageFile=DataPattern;
          imageFile.replace("ptypepid",patient);
          imagePath = DataPath+imageFile;
          imagePath.replace("ptypepid",patient);
          for (int po=0;po<MSProcessLoadOrder.size();po++) 
            {//check each process order, using the conrast as is, 
            tFile=imageFile;
            tPath=imagePath;
            tFile.replace("pstatus",MSProcessLoadOrder[po]);
            tPath.replace("pstatus",MSProcessLoadOrder[po]);
            tFile.replace("contrast",contrastList[c]);
            tPath.replace("contrast",contrastList[c]);
            this->PrintText("Testing "+tPath);
            if(fexists(tPath.toStdString().c_str()) ) 
              {
              imageFile=tFile;
              imagePath=tPath;
              po=MSProcessLoadOrder.size();
              this->PrintText("   success");
              }
            else
              {
              tFile=imageFile;
              tPath=imagePath;
              tFile.replace("pstatus",MSProcessLoadOrder[po]);
              tPath.replace("pstatus",MSProcessLoadOrder[po]);
              if(contrastList[c]=="MR")
                {
                for (int co=0;co<ContrastLoadOrder.size();co++)
                  {
                  tFile=imageFile;
                  tPath=imagePath;
                  tFile.replace("pstatus",MSProcessLoadOrder[po]);
                  tPath.replace("pstatus",MSProcessLoadOrder[po]);
                  tFile.replace("contrast",ContrastLoadOrder[co]);
                  tPath.replace("contrast",ContrastLoadOrder[co]);
                  this->PrintText("Testing "+tPath);
                  if(fexists(tPath.toStdString().c_str()) ) 
                    {
                    imageFile=tFile;
                    imagePath=tPath;
                    po=MSProcessLoadOrder.size();
                    co=ContrastLoadOrder.size();
                    this->PrintText("   success");
                    }
                  else
                    {
                    this->PrintText("   fail");
                    }
                  }
                }
              else
                {
                tFile.replace("contrast",contrastList[c]);
                tPath.replace("contrast",contrastList[c]);              
                if(fexists(tPath.toStdString().c_str()) ) 
                  {
                  imageFile=tFile;
                  imagePath=tPath;
                  po=MSProcessLoadOrder.size();                
                  this->PrintText("   success");
                  }
                else
                  {
                  this->PrintText("   fail");
                  }
                }
              }
            }
        
          // 
          //confusingly DataPath is root to data directories
        
          nodeName=imageFile;
          nodeName.replace(".hdr","");
          tParams["fileName"]       = imagePath;
          tParams["nodeName"]       = nodeName;
          tParams["labelmap"]       = false;
          tParams["center"]         = this->CenterVolumeOnLoad;
          tParams["autoWindowLevel"]= false;
          tParams["fileType"]="VolumeFile";      

          labelFile=LabelPattern;  
          labelFile.replace("ptypepid",patient);
          labelFile.replace("regtype",rtype);
          labelFile.replace("regsub",regsub);
//        this->LabelPattern =QString("ptypepid_regtype_regsub.hdr"); 
          labelPath = LabelPath+labelFile;
          labelPath.replace("ptypepid",patient);
          labelNode=labelFile;
          labelNode.replace(".hdr","");
          tParams["labelNode"]      =labelNode; //will end up as duplicate of nodeName in the labelProperties entries, but this is easier.
          imageProperties << tParams;
          this->PrintText("imageProperties << "+imageFile);
          this->PrintText("Added volume to load list:"+imagePath);

          if( LoadLabels )
            {
            nodeName                   = labelNode;
            tParams["fileName"]        = labelPath;
            tParams["nodeName"]        = nodeName;
            tParams["labelmap"]        = true;     
            tParams["center"]          = this->CenterVolumeOnLoad;    
            tParams["autoWindowLevel"] = false;
            labelProperties << tParams;
            this->PrintText("labelProperties << "+labelFile);
            this->PrintText("Added labels to load list:"+labelPath);
            }
          }

        }
      else 
        {
        this->PrintText("Bad or unset GalleryName.");
        return;
        }

     
      }//timepoint foor loop end
    }//contrast timepoint loop end.

  int snCounter;
  //loop for all images to add only unloaded to load list
  for(snCounter=0;snCounter<imageProperties.size();snCounter++)
    {
    if ( ! NodeExists(imageProperties[snCounter]["nodeName"].toString()) )
      {
      unloadedFiles << imageProperties[snCounter];
      } 
    }
  //loop for all labels to add only unloaded to load list need to do a unique element on this list... (bleh), the qset might do the trick
  QSet<QString> labelNodes;
  for(snCounter=0;snCounter<labelProperties.size();snCounter++)
    {
    if ( ! NodeExists(labelProperties[snCounter]["nodeName"].toString()) )
      {
      this->PrintText("-");
      if( labelNodes.isEmpty() ) 
        {
        this->PrintText(" +");
        unloadedFiles << labelProperties[snCounter];
        labelNodes << labelProperties[snCounter]["nodeName"].toString(); 
        } 
      else 
        {
        this->PrintText("-- ");
        if( ! labelNodes.contains(labelProperties[snCounter]["nodeName"].toString()) ) 
          {
          this->PrintText("adding "+labelProperties[snCounter]["nodeName"].toString()+"to loadlist");
          unloadedFiles << labelProperties[snCounter];
          labelNodes << labelProperties[snCounter]["nodeName"].toString();
          }
        else 
          {
          this->PrintText("  XXXXXX");
          }
        }
      }
    else 
      {
      this->PrintText("Found label"+labelProperties[snCounter]["nodeName"].toString());
      }
    }
  //load data
  s_app_obj->coreIOManager()->loadNodes(unloadedFiles); // images
  
  ////
  //arrange objects.
  ////
  //assign slices
  //assign 3d windows, 
  //for now just set properties in 3d views, rather than setting the volume settings.
  snCounter=0;
  for (snCounter=0;snCounter<SceneNodes.size();snCounter++) 
    {
    QString sliceNodeID=QString("vtkMRMLSliceNode")+SceneNodes[snCounter];
    QString viewNodeID =QString("vtkMRMLViewNode") +SceneNodes[snCounter];
    QString sliceCompositeNodeID=QString("vtkMRMLSliceCompositeNode")+SceneNodes[snCounter];
    vtkMRMLNode      *sn        = currentScene->GetNodeByID(sliceNodeID.toStdString());
    vtkMRMLSliceNode *sliceNode = NULL;
    if ( sn != NULL ) 
      { 
      sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode
      } 
    else 
      {
      this->PrintText("Bad slice node or no slice node for SceneNode: "+sliceNodeID);
      }
    vtkMRMLNode      *scn       = currentScene->GetNodeByID(sliceCompositeNodeID.toStdString());
    vtkMRMLSliceCompositeNode *scNode = NULL;
//     if ( sliceNode != NULL ) 
//       { 
//       this->PrintText("Slicenode set");
//       }
//     else
//       {
//       this->PrintText("Error setting slicenode");
//       return;
//       }

    if ( scn != NULL )//node existed try to downcast
      {
      scNode = vtkMRMLSliceCompositeNode::SafeDownCast(scn); //Composite
      } 
    else 
      { 
      this->PrintText("Bad composite node or no composite node for SceneNode"+sliceCompositeNodeID);
      }
    
    //<>//
    if ( scNode != NULL )  //if we have a composinte node sucessfully downcast to assign volumes to
      { //vtkMRMLSliceCompositeNode
      this->PrintText("SliceComposite ready:"+sliceCompositeNodeID);
      if(setSliceOrient) 
        {
        scNode->SetLinkedControl(1);
        sliceNode->SetOrientation(orientation.toLatin1());
        }
      else 
        {
        if(snCounter<orthogonalOrientations.size())
          {
          sliceNode->SetOrientation(orthogonalOrientations[snCounter].toLatin1());
          }
        else 
          {
          this->PrintText("Do not know what to do with orientation. Not setting.");
          }
        }
      //<<.>>//
      if(snCounter<imageProperties.size()) //prevents bad index crash
        {
        scNode->SetBackgroundVolumeID(this->NodeID(imageProperties[snCounter]["nodeName"].toString())); //set image
        if( LoadLabels )
          { 
          scNode->SetLabelVolumeID(this->NodeID(labelProperties[snCounter]["nodeName"].toString()));	//set labels
          if(GalleryName=="Multiple Sclerosis") //MSSingle" || GalleryName=="MSDual" || GalleryName=="MSComparison" )
            {
            scNode->SetLabelOpacity(1.0); 
              sliceNode->SetUseLabelOutline(true);
              if (this-> Layout=="MSComparison")
                {
                if(SceneNodes[snCounter]=="Red"||SceneNodes[snCounter]=="Green")
                  {
                  this->PrintText("turning off labels for this viewer");
                  scNode->SetLinkedControl(0);
                  scNode->SetLabelOpacity(0); 
                  scNode->SetLinkedControl(1);
                  }
                }

            }
          else 
            {
            scNode->SetLabelOpacity(0.25); 
            sliceNode->SetUseLabelOutline(false);
            }
          } 
        else 
          {
          scNode->SetLabelVolumeID(NULL);
          }
        }
      else 
        { //there are more scenenodes than there are volumes. This happens for 2 of our galleries at current, the Dual3D compare and the othogonal.
        //ifnodis yellow, 
        if ( SceneNodes[snCounter]=="yellow" ) 
          {//should only run on dual3d
          scNode->SetForegroundVolumeID(this->NodeID(imageProperties[1]["nodeName"].toString())); // set overlay
          scNode->SetForegroundOpacity(0.50);
          }
        }
      //end check for good scNode
      }
    else 
      { //we'll enter this code if we are on orthogonal or dual3d for the 3d volumes
      this->PrintText("SliceComposite bad:"+sliceCompositeNodeID+" could not downcast!");
      }
    
    vtkMRMLNode     *vn       = currentScene->GetNodeByID(viewNodeID.toStdString());
//    vtkMRMLViewNode *vNode;
    if ( vn != NULL )
      {
//      vNode = vtkMRMLViewNode::SafeDownCast(vn); //Composite
      this->SetViewNodeProperties(viewNodeID);
      } 
    else
      {
      this->PrintText("Bad view node or not a view node:"+viewNodeID);
      }
    }
    
  if(this->GalleryName=="MSDual") 
    {
    this->PrintText("Fixing MSDual red slice");    
    QString sliceNodeID=QString("vtkMRMLSliceNode")+"Red";
    QString sliceCompositeNodeID=QString("vtkMRMLSliceCompositeNode")+SceneNodes[snCounter];
    vtkMRMLNode      *sn        = currentScene->GetNodeByID(sliceNodeID.toStdString());
    vtkMRMLSliceNode *sliceNode = NULL;
    sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode
    
    vtkMRMLNode      *scn       = currentScene->GetNodeByID(sliceCompositeNodeID.toStdString());
    vtkMRMLSliceCompositeNode *scNode = NULL;
    scNode = vtkMRMLSliceCompositeNode::SafeDownCast(scn); //Composite
    scNode->SetBackgroundVolumeID(NULL); //set image

    }

  return;
}



//-----------------------------------------------------------------------------
// the dumb solution first, call perl to set up the mrml scene. 
// might be able to use the functions from addData to load the scene once perl generates it. 
// hav to set this up to depenbd on the data module. 
void qSlicerGalleryControlModuleWidget::CallPerlScriptAndLoadMRML() 
{
  this->PrintMethod(QString("cpsandloadMRML")); 
  //  int state_check=0;
  //  int status_code=0;

// using qSlicerGalleryControlModule
//  Q_D(qSlicerGalleryControlModulePrivate); //fails, no compile

// using qSlicerGalleryControlModuleWidget
//   Q_D(qSlicerGalleryControlModuleWidget);
//   d->setupUi(this); // inserts another copy of module in control panel
//   state_check=d->t_00->isTristate();//fails in crash of slicer
//   status_code=d->t_00->isChecked(); //fails in crash of slicer
//   state_check=t_00->isTristate();   //fails, no compile
//   status_code=t_00->isChecked();    //fails, no compile
//   state_check=d->t_02->isTristate();//fails in crash of slicer
//   state_check=this->t_00->isTristate();//fails, no compile, no member.
//   status_code=this->t_00->isChecked(); //fials. no compile, no member


 
//   status_code=d->t_00->isChecked();//no longer neeeded
//  QString status_message = "Status=" + QString::number(status_code) + " State=" + QString::number(state_check);

  //get chekcboxes and clean up and split the strings   
  QString time_string     =QString(""); //this->GetTimepoints();
  QString contrast_string =QString(""); //this->GetContrasts();
  if ( LoadLabels) {
    this->PrintText(QString("  found labelson"));
  } else  {
    this->PrintText(QString("  found labelsoff"));
  }
  
  
//   if ( Layout == "time_contrast") {
//   } else if(Layout  == "")  { 
//   } else if(Layout  == "")  { 
//   } else if ( Layout == "No_Layout"){
//   }
  QString perl_program    = QString("slicer_generate") + "_" + Layout + ".pl" ;
  QString perl_prefix = "/usr/bin/perl /Users/james/svnworkspaces/slicer_generate_mrml_via_template/";
  QString out_path = QString("/tmp/") + Layout + time_string + contrast_string + ".mrml";
  //  QString out_path = QString("/tmp/") + Layout+ ".mrml";
  QString command  = perl_prefix + 
    perl_program + 
    " -c " + contrast_string + 
    " -t " + time_string  + 
    " -o " + out_path ;
  this->PrintText(command);
  //   this->PrintText(Layout);
  //   this->PrintText(QString::number(GalleryTimepoints));
  //   this->PrintText(QString::number(GalleryContrasts));
  //   this->PrintText(QString::number(Timepoints[40]));
  //   this->PrintText(QString::number(Timepoints[80]));
  
  //this -> variable works same as without
  //   this->PrintText(Layout);
  //   this->PrintText(QString::number(this->GalleryTimepoints));
  //   this->PrintText(QString::number(this->GalleryContrasts));
  //   this->PrintText(QString::number(this->Timepoints[40]));
  //   this->PrintText(QString::number(this->Timepoints[80]));
  
  //somehow call console application and retrun the mrml file we need to load...
  // call command
  const char* myCommand = command.toStdString().c_str();
  system(myCommand);  
  const QString op = out_path;

  bool status = false;

  // load mrmlscene
  qSlicerIO::IOProperties properties;
  properties["fileName"]=out_path;
  properties["clear"]=true;

  /* does not seem to be working.NOt sure why not it appears as if i'm using this correctly. */
//   qSlicerCoreIOManager* sload;
//   status=sload->loadScene(op, true); // scene1, out_path); //, true);
//   status=sload->loadNodes(QString("SceneFile"),properties);

/* fails in crash
  //qSlicerSlicer2SceneReader* reader= new qSlicerSlicer2SceneReader();
  //reader->load(properties);*/
  //slicerdevel example
  qSlicerApplication * app = qSlicerApplication::application();
  status=app->ioManager()->loadScene(out_path);

  if ( status )  {
    this->PrintText(QString("Load Success"));
  } else {
    this->PrintText(QString("Load Failed"));
  }
  //qSlicerApplication::application()->ioManager()->openLoadSceneDialog();// change dialog to just add the scene.
  // doesnt workqSlicerApplication::application()->ioManager()->openLoadSceneDialog(out_path);// change dialog to just add the scene.
  return;
}

// simple function to check if a node is part of the scene already.
bool qSlicerGalleryControlModuleWidget::NodeExists(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);
  
  // Search the scene for the nodes and return true on match
  currentScene->InitTraversal();
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) {
    if (sn->GetName()==nodeName) { 
      this->PrintText(""+nodeName +" found.");
      return true;
    } else { 
      //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
    }
  }
  this->PrintText(""+nodeName+" not in open nodes.");
  return false;
}

const char * qSlicerGalleryControlModuleWidget::NodeID(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);
  
  // Search the scene for the nodes and return true on match
  currentScene->InitTraversal();
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) {
    if (sn->GetName()==nodeName) { 
      //this->PrintText("Found volume not reloading");
      return sn->GetID();
    } else { 
      //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
    }
  }
  this->PrintText("Did not find "+nodeName+" in open nodes.");
  return "";
}

void qSlicerGalleryControlModuleWidget::ClearCheckboxes()
{
  this->PrintMethod(QString("ClearCheckboxes"));
  Q_D(qSlicerGalleryControlModuleWidget);  
  int elements=0;
  elements=(sizeof(this->Timepoints)/sizeof(bool));
  for(int i=0;i<elements;i++) 
    {
    this->Timepoints[i]=false;
    }
  elements=(sizeof(this->Contrasts)/sizeof(bool));
  for(int i=0;i<elements;i++) 
    {
    this->Contrasts[i]=false;
    }

  d->c_ad       ->setChecked(false);
  d->c_adc      ->setChecked(false);
  d->c_b0       ->setChecked(false);
  d->c_dwi      ->setChecked(false);
  d->c_fa       ->setChecked(false);
  d->c_fa_color ->setChecked(false);
  d->c_freq     ->setChecked(false);
  d->c_gre      ->setChecked(false);
  d->c_rd       ->setChecked(false);
  d->MSC_ADC    ->setChecked(false);
  d->MSC_FA     ->setChecked(false);
  d->MSC_MR     ->setChecked(false);
  d->MSC_RD     ->setChecked(false);
  
  d->t_00->setChecked(false);
  d->t_02->setChecked(false);
  d->t_04->setChecked(false);
  d->t_08->setChecked(false);
  d->t_12->setChecked(false);
  d->t_18->setChecked(false);
  d->t_24->setChecked(false);
  d->t_33->setChecked(false);
  d->t_34->setChecked(false);
  d->t_40->setChecked(false);
  d->t_53->setChecked(false);
  d->t_54->setChecked(false);
  d->t_80->setChecked(false);

  return;
}

QStringList qSlicerGalleryControlModuleWidget::GetTimepoints()
{
  this->PrintMethod(QString("GetTimepoints"));
  Q_D(qSlicerGalleryControlModuleWidget);

  //forms list of timepoints
  // not used for timepoints, but is for contrasts in GetContrasts
  QStringList timepoint_list;
  QStringList all_timepoints;
  all_timepoints <<"00"
		 <<"02"
		 <<"04"
		 <<"08"
		 <<"12"
		 <<"18"
		 <<"24"
                 <<"33"
                 <<"34"
                 <<"53"
                 <<"54"
		 <<"40"
		 <<"80"
    ;

  // put all checkbox states in array
  this->Timepoints[0]=d->t_00->isChecked();
  this->Timepoints[2]=d->t_02->isChecked();
  this->Timepoints[4]=d->t_04->isChecked();
  this->Timepoints[8]=d->t_08->isChecked();
  this->Timepoints[12]=d->t_12->isChecked();
  this->Timepoints[18]=d->t_18->isChecked();
  this->Timepoints[24]=d->t_24->isChecked();
  this->Timepoints[40]=d->t_40->isChecked();
  this->Timepoints[33]=d->t_33->isChecked();
  this->Timepoints[34]=d->t_34->isChecked();
  this->Timepoints[53]=d->t_53->isChecked();
  this->Timepoints[54]=d->t_54->isChecked();
  this->Timepoints[80]=d->t_80->isChecked();
 
  QString timepoint_string("");
  int timepoints_found=0;
  //QString status_message = "Status=" + QString::number(status_code) + " State=" + QString::number(state_check);
  int elements=(sizeof(Timepoints)/sizeof(bool));
  this->PrintText("Timepoints:"+QString::number(this->GalleryTimepoints));
  for(int i=0;i<elements;i++) {
    if ( Timepoints[i] == 1 && timepoints_found < this->GalleryTimepoints ) {
//      this->PrintText("checking timepoint"+ QString::number(i)+" match <"+QString::number(Timepoints[i])+">");      
      QString num = QString("%1").arg(i, 2, 10, QChar('0')).toUpper();
      timepoint_string = timepoint_string + num + "," ;
      timepoint_list<<num;
      timepoints_found++;
    } else { 
      //      this->PrintText("checking timepoint"+ QString::number(i)+" nomatch <"+QString::number(Timepoints[i])+">");
    }
  }

  //  return timepoint_string;
  return timepoint_list;
}

QStringList qSlicerGalleryControlModuleWidget::GetContrasts()
{
  this->PrintMethod(QString("GetContrasts"));
  Q_D(qSlicerGalleryControlModuleWidget);

  //forms comaseparated list of contrast strings
  QStringList contrast_list;
  QStringList all_contrasts;
  all_contrasts << "ad" 
		<< "adc"
		<< "b0"
		<< "dwi"
		<< "fa"
		<< "fa_color"
		<< "freq"
		<< "gre"
		<< "rd"
		<< ""
		<< ""
    ;
  
  this->Contrasts[0]=d->c_ad       ->isChecked();
  this->Contrasts[1]=d->c_adc      ->isChecked();
  this->Contrasts[2]=d->c_b0       ->isChecked();
  this->Contrasts[3]=d->c_dwi      ->isChecked();
  this->Contrasts[4]=d->c_fa       ->isChecked();
  this->Contrasts[5]=d->c_fa_color ->isChecked();
  this->Contrasts[6]=d->c_freq     ->isChecked();
  this->Contrasts[7]=d->c_gre      ->isChecked();
  this->Contrasts[8]=d->c_rd       ->isChecked();
  //  this->Contrasts[8]=d->c_     ->isChecked();
  //  this->Contrasts[9]=d->c_     ->isChecked();
  
  //MS_LibraraySettings,(could do this better)
  if (this->GalleryName=="Multiple Sclerosis") 
    {
    this->Contrasts[1]=d->MSC_ADC ->isChecked();
    this->Contrasts[4]=d->MSC_FA  ->isChecked();
    this->Contrasts[7]=d->MSC_MR  ->isChecked();
    this->Contrasts[8]=d->MSC_RD  ->isChecked();
    all_contrasts[1]="ADC";
    all_contrasts[4]="FA";
    all_contrasts[7]="MR";
    all_contrasts[8]="RD";
    }
  
//  QString contrast_string("");
  int contrasts_found=0;
  int elements=(sizeof(Contrasts)/sizeof(bool));
  this->PrintText("Contrasts:"+QString::number(this->GalleryContrasts));
  for(int i=0;i<elements;i++) {
    if ( Contrasts[i] == 1 && contrasts_found < this->GalleryContrasts ) 
      {
      //      this->PrintText("checking contrast"+ QString::number(i)+" match <"+QString::number(Contrasts[i])+">");      
      //      QString num = all_contrasts[i];
      //      contrast_string = contrast_string + num + "," ;
      //contrast_string = contrast_string + all_contrasts[i] + "," ;
      contrast_list << all_contrasts[i];
      contrasts_found++;
    } else { 
      //      this->PrintText("checking contrast"+ QString::number(i)+" nomatch <"+QString::number(Contrasts[i])+">");
    }
  }
  //  return contrast_string;
  return contrast_list;
}

void qSlicerGalleryControlModuleWidget::SetViewNodeProperties(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);
  
  // Search the scene for the nodes set properties
  currentScene->InitTraversal();
  vtkMRMLNode *sn = currentScene->GetNodeByID(nodeName.toStdString());
  vtkMRMLViewNode *vNode = NULL;
  //qSlicerIO::IOProperties *viewNodeProps;
  
  if ( sn != NULL ) 
    {    
    vNode = vtkMRMLViewNode::SafeDownCast(sn); //Composite
    
    if ( vNode !=NULL ) 
      {
      this->PrintText("SET PROPERTIES FOR VIEWER HERE");
     //do work
      //vNode->id("vtkMRMLViewNode1");
      //vNode->name(View1);
      //vNode->hideFromEditors(false);
      //vNode->selectable(true);
      //vNode->selected(false);
      //vNode->attributes(MappedInLayout:1);
      //vNode->layoutLabel(1);
      //vNode->layoutName(1);
      //vNode->active(false);
      //vNode->visibility(false);
      double bg1[3]={0.0, 0.0, 0.0};//{0.756863, 0.764706, 0.909804};
      vNode->SetBackgroundColor(bg1);
      double bg2[3]={0.0, 0.0, 0.0};//{0.454902, 0.470588, 0.745098};
      vNode->SetBackgroundColor2(bg2);
      vNode->SetFieldOfView(45);
      //      vNode->SetLetterSize(0.05);
      vNode->SetBoxVisible(false);
//      vNode->SetFiducialsVisible(true);
//      vNode->SetFiducialLabelsVisible(true);
      vNode->SetAxisLabelsVisible(false);
//      vNode->SetAxisLabelsCameraDependent(true);
//      vNode->SetAnimationMode("Off");
//      vNode->SetNviewAxisMode("LookFrom");
//      vNode->SetSpinDegrees(2);
//      vNode->SetSpinMs(5);
//      vNode->SetSpinDirection("YawLeft");
//      vNode->SetRotateDegrees(5);
//      vNode->SetRockLength(200);
//      vNode->SetRockCount(0);
//      vNode->SetStereoType("NoStereo");
//      vNode->SetRenderMode("Perspective");



      }
    else
      {
      this->PrintText("View node did not set properly!");
      return;
      }


    }
  else
    {
    this->PrintText("Bad view node or not a view node");
    return;
    }


  
//   for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) {
//     if (sn->GetName()==nodeName) { 
//      this->PrintText(""+nodeName +" found.");
//       return;
//     } else { 
//       //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
//     }
//   }
  this->PrintText(""+nodeName+" Properties Set.");
  return;
}

void qSlicerGalleryControlModuleWidget::PrintMethod(const QString text)
{
  QString pass="qSlicerGalleyControlModuleWidget method:"+text;
  this->PrintText(pass);
  return;
}


void qSlicerGalleryControlModuleWidget::PrintText(const QString text)
{
  QTextStream out(stdout);
  out << text<<"\n";
  return;
}





// bool qSlicerGalleryControlModuleWidget::setMRMLScene(vtkMRMLScene *currentScene) {
//   Q_D(qSlicerGalleryControlModuleWidget);
//   vtkMRMLScene* currentScene = this->mrmlScene();
//   this->Superclass::setMRMLScene(currentScene);
  
//   qSlicerApplication * app = qSlicerApplication::application();
//   if (!app) {
//     return false;
//   }
//   qSlicerLayoutManager * layoutManager = app->layoutManager();
//   if (!layoutManager) {
//     return false;
//   }

//   // Search the scene for the available view nodes and create a
//   // Controller and connect it up
//   currentScene->InitTraversal();
//   for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNodeByClass("vtkMRMLSliceNode"));) {
//     vtkMRMLSliceNode *snode = vtkMRMLSliceNode::SafeDownCast(sn);
//     if (snode) { 
//       d->createController(snode, layoutManager);
//     }
//   }
  
//   currentScene->InitTraversal();
//   for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNodeByClass("vtkMRMLViewNode"));) {
//     vtkMRMLViewNode *vnode = vtkMRMLViewNode::SafeDownCast(sn);
//     if (vnode) {
//       d->createController(vnode, layoutManager);
//     }
//   }
  
//   // Need to listen for any new slice or view nodes being added
//   this->qvtkReconnect(currentScene, currentScene, vtkMRMLScene::NodeAddedEvent,
// 		      this, SLOT(onNodeAddedEvent(vtkObject*,vtkObject*)));
  
//   // Need to listen for any slice or view nodes being removed
//   this->qvtkReconnect(currentScene, currentScene, vtkMRMLScene::NodeRemovedEvent,
// 		      this, SLOT(onNodeRemovedEvent(vtkObject*,vtkObject*)));
  
//   // Listen to changes in the Layout so we only show controllers for
//   // the visible nodes
//   QObject::connect(layoutManager, SIGNAL(layoutChanged(int)), this,
// 		   SLOT(onLayoutChanged(int)));
  
// }


void qSlicerGalleryControlModuleWidget::ShowNodesInScene() //vtkMRMLScene *)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  qSlicerApplication * app = qSlicerApplication::application();
  if (!app)
    {
    return;
    }
  qSlicerLayoutManager * layoutManager = app->layoutManager();
  if (!layoutManager)
    {
    return;
    }
  currentScene->InitTraversal();
  vtkIndent indent= vtkIndent(5);
  //show slice nodes
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNodeByClass("vtkMRMLSliceNode"));)
    {
      sn->PrintSelf(std::cout,indent);
    }
  //show 3d view nodes
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNodeByClass("vtkMRMLViewNode"));)
    {
      sn->PrintSelf(std::cout,indent);
    }
}


QStringList qSlicerGalleryControlModuleWidget::SetLayout()  //QString layout
{
  vtkMRMLScene* currentScene = this->mrmlScene();  
  bool setSliceOrient=true;
//  qSlicerAppMainWindow main=
//  vtkMRMLNode currentScene->GetNextNodeByClass("vtkMRMLLayoutNode")
  currentScene->InitTraversal();
  vtkMRMLLayoutNode* sceneLayoutNode = vtkMRMLLayoutNode::SafeDownCast(
    currentScene->GetNextNodeByClass("vtkMRMLLayoutNode") );
  QStringList sceneNodes;
  if (this->Layout=="TimeContrast") 
    {
    //Three over three
    sceneNodes << "Red"
               << "Yellow"
               << "Green"
               << "Slice4"
               << "Slice5"
               << "Slice6";
    //vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView
    this->PrintText("Layout should have 6 volumes");
    //sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutInitialView);
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutThreeOverThreeView);
    } 
  else if ( this->Layout=="MSSingle")
    {
    //Two over Two
    sceneNodes << "Red";
    this->PrintText("Layout should have 1 volumes");
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutOneUpRedSliceView);
    } 
  else if ( this->Layout=="MSDual") 
    {
    //Two over Two loadorder for disease is patient, control, contrast 1, contrast2
//     sceneNodes << "Compare2"
//                << "Red"
//                << "Compare1";
//     //vtkMRMLLayoutNode::SlicerLayoutThreeOverThreeView
//     this->PrintText("Layout should have 2 volumes");
   
// //    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutCompareView);
// //    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView);//doeesnt help the trouble...
// //    sceneLayoutNode->SetNumberOfCompareViewRows(2);            //set to 2 viewers.
// //    sceneLayoutNode->SetNumberOfCompareViewLightboxColumns(1); //set to 1 column each
// //    sceneLayoutNode->SetMainPanelSize(5);//this didnt clear main panel
// //    sceneLayoutNode->SetSecondaryPanelSize(1200); //this didnt do it either
// //    sceneLayoutNode->SetTopPanelVisibility(0);    //there is no top panel macro?
// //    sceneLayoutNode->SetBottomPanelVisibility(0);
// //    sceneLayoutNode->SetGUIPanelVisibility(false);
// //    sceneLayoutNode->SetCollapseSliceControllers(1); //nope
// //    sceneLayoutNode->SetCollapseSliceControllers(0); //nope


// //    sceneLayoutNode->
//     int spSize=sceneLayoutNode->GetSecondaryPanelSize();
//     int mpSize=sceneLayoutNode->GetMainPanelSize();
//     int gV=sceneLayoutNode->GetGUIPanelVisibility();
//     int bV=sceneLayoutNode->GetBottomPanelVisibility();
// //    int mV=sceneLayoutNode->GetMainPanelVisibility();
//     this->PrintText("secondary size:"+QString::number(spSize)+
//                     " main size:"+QString::number(mpSize)+
//                     " Gui vis:"+QString::number(gV)+
//                     " bottom vis:"+QString::number(bV));//+
// //                    " main vis:"+QString::number(mV)+".";
//     sceneLayoutNode->SetBottomPanelVisibility(0);   // nope.
//     sceneLayoutNode->SetMainPanelSize(710);         // nope.
//     sceneLayoutNode->SetSecondaryPanelSize(90);     // percentage 0-100?99? nope
    sceneNodes << "Green"
               << ""//"Yellow"
               << "Red";
    
    sceneLayoutNode->AddLayoutDescription(vtkMRMLLayoutNode::SlicerLayoutUserView,twoSliceView);
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutUserView);
////    this->ConventionalLayoutRootElement = vtkMRMLLayoutNode::ParseLayout(conventionalView);
//    sceneLayoutNode->ParseLayout(twoSliceView);

    } 
  else if ( this->Layout=="MSComparison") 
    {
    //Two over Two 
    sceneNodes << "Yellow"
               << "Slice1"
               << "Red"
               << "Green";
    //vtkMRMLLayoutNode::SlicerLayoutThreeOverThreeView
    this->PrintText("Layout should have 4 volumes");
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView);
    } 
  else if ( this->Layout=="MultiContrast") 
    {
    //Two over Two 
    sceneNodes << "Red"
               << "Yellow"
               << "Slice1"
               << "Green";
    //vtkMRMLLayoutNode::SlicerLayoutThreeOverThreeView
    this->PrintText("Layout should have 4 volumes");
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView);
    } 
  else if( this->Layout=="Orthogonal") 
    {
    //Four-Up
    sceneNodes << "Red"
	       << "Yellow"
	       << "Green"
               << "1";
    setSliceOrient=false;
    //vtkMRMLLayoutNode::SlicerLayoutFourUpView
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutFourUpView);
    this->PrintText("Layout should have 1 volumes");
    }
  else if ( this->Layout=="Dual3D") 
    {
    sceneNodes << "Red"   
	       << "Green"  // cheap trick, change slice order, so left is dataset1, and right is dataset2.
	       << "Yellow"
	       << "1"
	       << "2";
    setSliceOrient=true;
    //vtkMRMLLayoutNode::SlicerLayoutFourUpView
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutDual3DView);
    this->PrintText("Layout should have 2 volumes");
    }
  else 
    {
    this->PrintText(QString("Bad layout selection "+Layout));
    }
  return sceneNodes;
}
