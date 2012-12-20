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
#include <QSignalMapper>
#include <QFileInfo>

// SlicerQt includes
#include "qSlicerGalleryControlModuleWidget.h"
#include "ui_qSlicerGalleryControlModule.h"

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


#include  <Modules/Loadable/Volumes/qSlicerVolumesIO.h>






// Qt includes
#include <QFileDialog>
#include <QFileSystemWatcher>

// SlicerQt includes
#include "qSlicerApplication.h"
#include "qSlicerIOManager.h"

// vtkSlicerLogic includes
#include "vtkSlicerTransformLogic.h"

// MRMLWidgets includes
#include <qMRMLUtils.h>

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
  int elements=0;
  this->Layout=QString("No_Layout");
  this->GalleryTimepoints=0;
  this->GalleryContrasts=0;
  this->GalleryOrientations=0;
  this->LoadLabels=false;
  elements=(sizeof(Timepoints)/sizeof(bool));
  for(int i=0;i<elements;i++) {
    this->Timepoints[i]=0;
  }
  elements=(sizeof(Contrasts)/sizeof(bool));
  for(int i=0;i<elements;i++) {
    Contrasts[i]=0;
  }

  /* Load data paths */
  /* hardcode for now*/
  /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
  /* labels are at /DataPath/labels/completed/completed_good_header/nii/           */
  this->DataPath=QString("/Volumes/sandbox/devatlas");
  this->LabelPath=QString(DataPath+"/labels/completed/completed_good_header/nii/");
  //  this->DataPattern  << "p"   << "timepoint" << "_average_" << "contrast" << ".nii";
  //  this->LabelPattern << "pnd" << "timepoint" << "_average_labels.nii";
  this->DataPattern  =QString("ptimepoint_average_contrast.nii");
  this->LabelPattern =QString("pndtimepoint_average_labels.nii");
  

  // these connections were setup in the *.ui file, but that seems to have been broken
  /*  connect(d->setTimeContrastLayoutButton, SIGNAL(released()),
	  this, SLOT(SetTimeContrastLayout()));
  connect(d->SetOrthagonalLayoutButton, SIGNAL(released()), 
	  this, SLOT(SetOrthagonalLayout()));
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
  d->orientationComboBox->setEnabled(false);

  connect(d->DatasetLabelsOn, SIGNAL(clicked()), SLOT(SetLabels()));
  connect(d->setTimeContrastLayoutButton, SIGNAL(clicked()), SLOT(SetTimeContrastLayout()));
  connect(d->setMultiContrastLayoutButton, SIGNAL(clicked()), SLOT(SetMultiContrastLayout()));
  connect(d->setOrthagonalLayoutButton, SIGNAL(clicked()), SLOT(SetOrthagonalLayout()));

  //  connect(d->LoadDataButton, SIGNAL(released()), SLOT(CallPerlScriptAndLoadMRML()));
  connect(d->LoadDataButton, SIGNAL(released()), SLOT(BuildScene()));
  this->Superclass::setup();
}



void qSlicerGalleryControlModuleWidget::SetTimeContrastLayout() 
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->Layout=QString("time_contrast");
  this->GalleryTimepoints=3;
  this->GalleryContrasts=2;
  this->GalleryOrientations=1;
  
  this->PrintMethod(QString("TCLayout"));
  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
  return ;
}

void qSlicerGalleryControlModuleWidget::SetMultiContrastLayout() 
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(true);
  this->PrintMethod(QString("MCLayout"));
  this->GalleryTimepoints=1;
  this->GalleryContrasts=4;
  this->GalleryOrientations=1;

  this->Layout=QString("multicontrast");
  this->PrintMethod("Setting layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
  return ;
}
void qSlicerGalleryControlModuleWidget::SetOrthagonalLayout() 
{
  Q_D(qSlicerGalleryControlModuleWidget);
  d->DataSelectionGroupBox->setCollapsed(false);
  d->orientationComboBox->setEnabled(false);
  this->Layout=QString("orthagonal");
  this->GalleryTimepoints=1;
  this->GalleryContrasts=1;
  this->GalleryOrientations=3;
  
  this->PrintMethod(QString("OLayout"));
  this->PrintMethod("Setting Layout" +Layout +"t="+QString::number(this->GalleryTimepoints) + "c=" +QString::number(this->GalleryContrasts));
  return;
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



//-----------------------------------------------------------------------------
// build mrml and load datasests
void qSlicerGalleryControlModuleWidget::BuildScene() 
{
  this->PrintMethod(QString("BuildScene")); 
  Q_D(qSlicerGalleryControlModuleWidget); //i think the Q_D line connects us back to our parent widget

  // last minute settings read
  QStringList timepointList =this->GetTimepoints();
  QStringList contrastList  =this->GetContrasts();
  QString     orientation    =d->orientationComboBox->currentText(); //meaningless for orthagonal layout
    
  QString labelFile;
  QString imageFile;
  QString nodeName;
  QStringList imageList;
  QStringList labelList;

  //DataPath 
  //LabelPath
  //DataPattern (replace timeptint with numbers and contrast with abreviation)
  //LabelPattern(replace timepoint with numbers)
  
  ////
  // initialize scene
  ////
  int viewers=GalleryTimepoints*GalleryContrasts*GalleryOrientations;
  //create viewnode.
  //  vtkMRMLScene* display=new vtkMRMLScene; //dindnt work
  //qSlicerApplication * s_app_obj = qSlicerApplication::application();
  //status=s_app_obj->ioManager()->loadScene(out_path);   // out_path is a qstring here.

  //QFileInfo(QDir directory, QString fileName), QFileInfo::suffix(), QFileInfo::absoluteFilePath()...


  // Read the labelmap segmentation     
//   qSlicerIO::IOProperties parameters;     
//   parameters["fileName"] = QString(d_ptr->segFolder->text());      
//   parameters["labelmap"] = true;     
//   parameters["centered"] = true;    
//   qSlicerCoreApplication::application()->coreIOManager()->loadNodes(qSlicerIO::VolumeFile, parameters);

  int viewerNum=1;
  for (int c=0;c<contrastList.size();c++) {    
    //load labels
    for(int t=0;t<timepointList.size(); t++) {
      //add node, load data and assign to node, load labels and assign to node
      //volume
      //add vtkMRMLScalarVolumeNode  
      //label
      //add vtkMRMLVolumeNode
      //slice
      //add vtkMRMLSliceNode

      imageFile=DataPattern;
      imageFile.replace("timepoint",timepointList[t]);
      imageFile.replace("contrast",contrastList[c]);
      /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
      imageFile=DataPath+"/"+timepointList[t]+"/average/"+imageFile;
      //QFileInfo(QDir directory, QString fileName), QFileInfo::suffix(), QFileInfo::absoluteFilePath()...
      //imageFile=QFileInfo(DataPath,timepointList[t],average,imageFile);
      
      this->PrintText("image="+imageFile);
      //loaddata assign as data to viewer viewerNum
      //if nodename no exist
      qSlicerIO::IOProperties imParameters;     
      imParameters["fileName"] = imageFile;      
      imParameters["labelmap"] = false;     
      imParameters["center"] = true;    
      imParameters["autoWindowLevel"] = false;
      //imParameters["fileType"] = "VolumeFile";
      qSlicerCoreApplication::application()->coreIOManager()->loadNodes(QString("VolumeFile"), imParameters); //qSlicerIO::VolumeFile
     
      if( LoadLabels ){ 
	labelFile=LabelPattern;
	labelFile.replace("timepoint",timepointList[t]);
	labelFile=LabelPath+"/"+labelFile;
//      QDir 
//      labelFile=QFileInfo(LabelPath,labelFile);

	this->PrintText("label="+labelFile);
	//loaddata assign as labels to viewer viewerNum
	qSlicerIO::IOProperties laParameters;     
	laParameters["fileName"] = labelFile;      
	laParameters["labelmap"] = true;     
	laParameters["center"] = true;    
	laParameters["autoWindowLevel"] = false;
	qSlicerCoreApplication::application()->coreIOManager()->loadNodes(QString("VolumeFile"), laParameters); //qSlicerIO::VolumeFile, 
      }
      
      viewerNum++;
    }
    viewerNum++;    
  }

  //arrange data
  for(int t=0;t<timepointList.size(); t++) {
    for (int c=0;c<contrastList.size();c++) {    
      imageFile=DataPattern;
      imageFile.replace("timepoint",timepointList[t]);
      imageFile.replace("contrast",contrastList[c]);
      /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
      imageFile=DataPath+"/"+timepointList[t]+"/average/"+imageFile;
      //QFileInfo(QDir directory, QString fileName), QFileInfo::suffix(), QFileInfo::absoluteFilePath()...
      //imageFile=QFileInfo(DataPath,timepointList[t],average,imageFile);
      if( LoadLabels ){ 
	labelFile=LabelPattern;
	labelFile.replace("timepoint",timepointList[t]);
	labelFile=LabelPath+"/"+labelFile;
      }
    }
  }      

  //status=s_app_obj->ioManager()->loadNodes();



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
		<< "freq"
		<< "gre"
		<< "rd"
		<< ""
		<< ""
    ;
  this->Contrasts[0]=d->c_ad  ->isChecked();
  this->Contrasts[1]=d->c_adc ->isChecked();
  this->Contrasts[2]=d->c_b0  ->isChecked();
  this->Contrasts[3]=d->c_dwi ->isChecked();
  this->Contrasts[4]=d->c_fa  ->isChecked();
  this->Contrasts[5]=d->c_freq->isChecked();
  this->Contrasts[6]=d->c_gre ->isChecked();
  this->Contrasts[7]=d->c_rd  ->isChecked();
  //  this->Contrasts[8]=d->c_->isChecked();
  //  this->Contrasts[9]=d->c_->isChecked();

  QString contrast_string("");
  int contrasts_found=0;
  int elements=(sizeof(Contrasts)/sizeof(bool));
  this->PrintText("Contrasts:"+QString::number(this->GalleryContrasts));
  for(int i=0;i<elements;i++) {
    if ( Contrasts[i] == 1 && contrasts_found < this->GalleryContrasts ) {
      //      this->PrintText("checking contrast"+ QString::number(i)+" match <"+QString::number(Contrasts[i])+">");      
      //      QString num = all_contrasts[i];
      //      contrast_string = contrast_string + num + "," ;
      contrast_string = contrast_string + all_contrasts[i] + "," ;
      contrast_list << all_contrasts[i];
      contrasts_found++;
    } else { 
      //      this->PrintText("checking contrast"+ QString::number(i)+" nomatch <"+QString::number(Contrasts[i])+">");
    }
  }
  //  return contrast_string;
  return contrast_list;
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
