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

// SlicerQt includes
#include "qSlicerGalleryControlModuleWidget.h"
#include "ui_qSlicerGalleryControlModule.h"


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
  //  this->printMethod(QString("private call"));// doesnt work, not a method...
}

//-----------------------------------------------------------------------------
// qSlicerGalleryControlModuleWidget methods

//-----------------------------------------------------------------------------
//constructor
qSlicerGalleryControlModuleWidget::qSlicerGalleryControlModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerGalleryControlModuleWidgetPrivate )
{
  this->printMethod(QString("constructor"));
}

//-----------------------------------------------------------------------------
//destructor
qSlicerGalleryControlModuleWidget::~qSlicerGalleryControlModuleWidget()
{
  this->printMethod(QString("destructor"));
}

//-----------------------------------------------------------------------------
void qSlicerGalleryControlModuleWidget::setup()
{
  this->printMethod(QString("setup"));
  Q_D(qSlicerGalleryControlModuleWidget);
  d->setupUi(this);
  int elements=0;
  this->layout=QString("No Layout");
  this->gallery_timepoints=0;
  this->gallery_contrasts=0;

  elements=(sizeof(timepoints)/sizeof(bool));
  for(int i=0;i<elements;i++) {
    this->timepoints[i]=0;
  }
  elements=(sizeof(contrasts)/sizeof(bool));
  for(int i=0;i<elements;i++) {
    contrasts[i]=0;
  }

  // these connections were setup in the *.ui file, but that seems to have been broken
  /*  connect(d->setTimeContrastLayoutButton, SIGNAL(released()),
	  this, SLOT(setTimeContrastLayout()));
  connect(d->setOrthagonalLayoutButton, SIGNAL(released()), 
	  this, SLOT(setOrthagonalLayout()));
  connect(d->setMultiContrastLayoutButton, SIGNAL(released()), 
  this, SLOT(setMultiContrastLayout()));*/
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
  /*  connect(d->t_02, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_04, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_08, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_12, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_18, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_24, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_40, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  connect(d->t_80, SIGNAL(toggled(bool)), SLOT(setCheckBox()));
  */

  /*  if (d->t_00->isChecked()) {; //just test setting of the checkbox
    printMethod(QString("checktrue"));
    }*/

  connect(d->setTimeContrastLayoutButton, SIGNAL(pressed()), SLOT(setTimeContrastLayout()));
  connect(d->setMultiContrastLayoutButton, SIGNAL(pressed()), SLOT(setMultiContrastLayout()));
  connect(d->setOrthagonalLayoutButton, SIGNAL(pressed()), SLOT(setOrthagonalLayout()));
  connect(d->LoadDataButton, SIGNAL(released()), SLOT(callPerlScriptAndLoadMRML()));
  this->Superclass::setup();
}



void qSlicerGalleryControlModuleWidget::setTimeContrastLayout() 
{
  //  this->printMethod();
  //  Q_D(qSlicerGalleryControlModuleWidget); //i think the Q_D line connects us back to our parent widget
  //  QTextStream out(stdout);
  //  out << "Signalclicked for toolbutton time contrast";
  this->printMethod(QString("TCLayout"));
  layout=QString("timecontrast");
  gallery_timepoints=3;
  gallery_contrasts=2;
  this->printMethod("Setting layout" +layout +"t="+QString::number(this->gallery_timepoints) + "c=" +QString::number(this->gallery_contrasts));
  //return d->selectModule("SampleData");//Layout();
  //return;
  return ;
  //    this->Superclass::connect(&TimeXContrastButton,SIGNAL(clicked()),out << "Signalclicked for toolbutton",SLOT(update()));
}

void qSlicerGalleryControlModuleWidget::setMultiContrastLayout() 
{
  // Q_D(qSlicerGalleryControlModuleWidget); 
  //  QTextStream out(stdout);
  //  out << "Stuff happened";
  this->printMethod(QString("MCLayout"));
  
  this->layout=QString("multicontrast");
  this->gallery_timepoints=1;
  this->gallery_contrasts=4;
  this->printMethod("Setting layout" +layout +"t="+QString::number(this->gallery_timepoints) + "c=" +QString::number(this->gallery_contrasts));
  return ;
}
void qSlicerGalleryControlModuleWidget::setOrthagonalLayout() 
{
  //  Q_D(qSlicerGalleryControlModuleWidget); 
  //  QTextStream out(stdout);
  //  out << "Stuff happened";
  this->printMethod(QString("OLayout"));
  this->layout=QString("orthagonal");
  this->gallery_timepoints=1;
  this->gallery_contrasts=1;
  this->printMethod("Setting layout" +layout +"t="+QString::number(this->gallery_timepoints) + "c=" +QString::number(this->gallery_contrasts));
  return;
}

void qSlicerGalleryControlModuleWidget::setCheckBox()
{
  //  Q_D(qSlicerGalleryControlModuleWidget); //i think the Q_D line connects us back to our parent widget
  this->printMethod(QString("setCheckBox"));
  //get checkbox name
  //d->Properties["t_02"]= d->t_02->isChecked();// derp not set derp. must declare something
  //this->checkboxes.insert(std::pair<QString,bool>(QString("t_02"),true));

  // these checks work when we dont ahve any connection in the .ui file from qt designer.
//   this->timepoints[0]=d->t_00->isChecked();
//   this->timepoints[2]=d->t_02->isChecked();
//   this->timepoints[4]=d->t_04->isChecked();
//   this->timepoints[8]=d->t_08->isChecked();
//   this->timepoints[12]=d->t_12->isChecked();
//   this->timepoints[18]=d->t_18->isChecked();
//   this->timepoints[24]=d->t_24->isChecked();
//   this->timepoints[40]=d->t_40->isChecked();
//   this->timepoints[80]=d->t_80->isChecked();


  return;
}

//-----------------------------------------------------------------------------
// the dumb solution first, call perl to set up the mrml scene. 
// might be able to use the functions from addData to load the scene once perl generates it. 
// hav to set this up to depenbd on the data module. 
void qSlicerGalleryControlModuleWidget::callPerlScriptAndLoadMRML() 
{
  this->printMethod(QString("cpsandloadMRML")); 
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

//somehow call console application and retrun the mrml file we need to load...
//qSlicerApplication::application()->ioManager()->openLoadSceneDialog();// change dialog to just add the scene.
  
  QString time_string     =this->getTimepoints();
  QString contrast_string =this-> getContrasts();
  QString command         = QString("perl call") + " -c " + contrast_string + " -t " + time_string  + " ";
  this->printText(command);
  this->printText(layout);
  this->printText(QString::number(gallery_timepoints));
  this->printText(QString::number(gallery_contrasts));
  this->printText(QString::number(timepoints[40]));
  this->printText(QString::number(timepoints[80]));

  //this -> variable works same as without
  //    this->printText(layout);
  //    this->printText(QString::number(this->gallery_timepoints));
  //    this->printText(QString::number(this->gallery_contrasts));
  //    this->printText(QString::number(this->timepoints[40]));
  //    this->printText(QString::number(this->timepoints[80]));
  

  
  //get mrmlscene to use
  //get chekcboxes and clean up and split the strings 
  //do sort unique on it, then split, then based on the mrml only pass the correct number of data points to perl
    
  return;
}


QString qSlicerGalleryControlModuleWidget::getTimepoints()
{
  this->printMethod(QString("getTimepoints"));
  Q_D(qSlicerGalleryControlModuleWidget);

  //forms list of timepoints
  // not used for timepoints, but is for contrasts in getContrasts
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
  this->timepoints[0]=d->t_00->isChecked();
  this->timepoints[2]=d->t_02->isChecked();
  this->timepoints[4]=d->t_04->isChecked();
  this->timepoints[8]=d->t_08->isChecked();
  this->timepoints[12]=d->t_12->isChecked();
  this->timepoints[18]=d->t_18->isChecked();
  this->timepoints[24]=d->t_24->isChecked();
  this->timepoints[40]=d->t_40->isChecked();
  this->timepoints[80]=d->t_80->isChecked();
 
  QString outtimepoints("");
  int timepoints_found=0;
  //QString status_message = "Status=" + QString::number(status_code) + " State=" + QString::number(state_check);
  int elements=(sizeof(timepoints)/sizeof(bool));
  this->printText("Timepoints:"+QString::number(this->gallery_timepoints));
  for(int i=0;i<elements;i++) {
    //    this->printText();
    //    int *name=alltimepoints.at(i);
    if ( timepoints[i] == 1 && timepoints_found < this->gallery_timepoints ) {
      this->printText("checking timepoint"+ QString::number(i)+" match <"+QString::number(timepoints[i])+">");      
      //      QString num= QString("%1").arg(i,2,0,QChar('0')).toUpper();
      QString num = QString::number(i);
      outtimepoints = outtimepoints + num + "," ;
      timepoints_found++;
    } else { 
      this->printText("checking timepoint"+ QString::number(i)+" nomatch <"+QString::number(timepoints[i])+">");
    }
  }

  return outtimepoints;
  //  return "";
}

QString qSlicerGalleryControlModuleWidget::getContrasts()
{
  this->printMethod(QString("getContrasts"));
  Q_D(qSlicerGalleryControlModuleWidget);

  //forms comaseparated list of contrast strings
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

  this->contrasts[0]=d->c_ad  ->isChecked();
  this->contrasts[1]=d->c_adc ->isChecked();
  this->contrasts[2]=d->c_b0  ->isChecked();
  this->contrasts[3]=d->c_dwi ->isChecked();
  this->contrasts[4]=d->c_fa  ->isChecked();
  this->contrasts[5]=d->c_freq->isChecked();
  this->contrasts[6]=d->c_gre ->isChecked();
  this->contrasts[7]=d->c_rd  ->isChecked();
  //  this->contrasts[8]=d->c_->isChecked();
  //  this->contrasts[9]=d->c_->isChecked();

  QString outcontrasts("");
  int contrasts_found=0;
  int elements=(sizeof(contrasts)/sizeof(bool));
  this->printText("Contrasts:"+QString::number(this->gallery_contrasts));
  for(int i=0;i<elements;i++) {
    //    this->printText();
    //    int *name=allcontrasts.at(i);
    if ( contrasts[i] == 1 && contrasts_found < this->gallery_contrasts ) {
      this->printText("checking contrast"+ QString::number(i)+" match <"+QString::number(contrasts[i])+">");      
      //      QString num= QString("%1").arg(i,2,0,QChar('0')).toUpper();
      //      QString num = QString::number(i);
      QString num = all_contrasts[i];
      outcontrasts = outcontrasts + num + "," ;
      contrasts_found++;
    } else { 
      this->printText("checking contrast"+ QString::number(i)+" nomatch <"+QString::number(contrasts[i])+">");
    }
  }

  
  return outcontrasts;
}


void qSlicerGalleryControlModuleWidget::printMethod(const QString text)
{

  QString pass="qSlicerGalleyControlModuleWidget method:"+text;

  this->printText(pass);
  
  return;
}


void qSlicerGalleryControlModuleWidget::printText(const QString text)
{
  QTextStream out(stdout);
  out << text<<"\n";
  return;
}
