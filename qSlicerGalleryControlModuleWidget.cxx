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
  QTextStream out(stdout);
  out << "qSlicerGalleyControlModuleWidget SETUP RUN";
  Q_D(qSlicerGalleryControlModuleWidget);
  d->setupUi(this);
  /*  connect(d->setTimeContrastLayoutButton, SIGNAL(released()),
	  this, SLOT(setTimeContrastLayout()));
  connect(d->setOrthagonalLayoutButton, SIGNAL(released()), 
	  this, SLOT(setOrthagonalLayout()));
  connect(d->setMultiContrastLayoutButton, SIGNAL(released()), 
  this, SLOT(setMultiContrastLayout()));*/
  

  this->Superclass::setup();
}



bool qSlicerGalleryControlModuleWidget::setTimeContrastLayout() 
{
  //  this->printMethod();
  //  Q_D(qSlicerGalleryControlModuleWidget);
  //  QTextStream out(stdout);
  //  out << "Signalclicked for toolbutton time contrast";
  this->printMethod(QString("TCLayout"));

  //return d->selectModule("SampleData");//Layout();
  //return;
  return true;
  //    this->Superclass::connect(&TimeXContrastButton,SIGNAL(clicked()),out << "Signalclicked for toolbutton",SLOT(update()));
}

bool qSlicerGalleryControlModuleWidget::setMultiContrastLayout() 
{
  //  QTextStream out(stdout);
  //  out << "Stuff happened";
  this->printMethod(QString("MCLayout"));
  return true;
}
bool qSlicerGalleryControlModuleWidget::setOrthagonalLayout() 
{
  //  QTextStream out(stdout);
  //  out << "Stuff happened";
  this->printMethod(QString("OLayout"));
  return true;
}

void qSlicerGalleryControlModuleWidget::callPerlScriptAndLoadMRML() 
{
  //somehow call console application and retrun the mrml file we need to load...
  this->printMethod(QString("cpsandloadMRML"));
  return;
}

void qSlicerGalleryControlModuleWidget::printMethod(const QString text)
{
  QTextStream out(stdout);
  out << "qSlicerGalleyControlModuleWidget method:"<<text<<"\n";
  return;
}
