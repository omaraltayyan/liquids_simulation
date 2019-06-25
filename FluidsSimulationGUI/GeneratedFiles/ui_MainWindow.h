/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "SimulationCanvasGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QWidget *widget_2;
    QLineEdit *BuoyancyText;
    QLabel *label_2;
    QLineEdit *surfaceTensionText;
    QLineEdit *GasConstantText;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_3;
    QLineEdit *thresholdText;
    QLineEdit *RestDensityText;
    QLabel *label_12;
    QLabel *label_4;
    QLineEdit *ParticleMassText;
    QLabel *label;
    QLineEdit *ViscosityText;
    QLabel *label_11;
    QLineEdit *restitutionText;
    SimulationCanvasGLWidget *openGLWidget;
    QWidget *widget;
    QPushButton *ResumeButton;
    QPushButton *ClearButton;
    QSlider *SlownessSlider;
    QLabel *label_5;
    QSlider *emissionSlider;
    QLabel *label_6;
    QSlider *gravitySlider;
    QLabel *label_7;
    QSlider *timeDeltaSlider;
    QLabel *label_10;
    QPushButton *colorSelectButton;
    QLabel *label_13;
    QCheckBox *pauseOnSpawnCheckbox;
    QComboBox *CollisionObjectsComboBox;
    QLabel *label_14;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(788, 691);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(20);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(0, 130));
        BuoyancyText = new QLineEdit(widget_2);
        BuoyancyText->setObjectName(QString::fromUtf8("BuoyancyText"));
        BuoyancyText->setGeometry(QRect(340, 90, 131, 20));
        BuoyancyText->setReadOnly(false);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 30, 81, 21));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        surfaceTensionText = new QLineEdit(widget_2);
        surfaceTensionText->setObjectName(QString::fromUtf8("surfaceTensionText"));
        surfaceTensionText->setGeometry(QRect(342, 0, 131, 20));
        GasConstantText = new QLineEdit(widget_2);
        GasConstantText->setObjectName(QString::fromUtf8("GasConstantText"));
        GasConstantText->setGeometry(QRect(90, 90, 141, 20));
        label_9 = new QLabel(widget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(250, 30, 81, 21));
        label_8 = new QLabel(widget_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(250, 0, 81, 21));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 60, 81, 21));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        thresholdText = new QLineEdit(widget_2);
        thresholdText->setObjectName(QString::fromUtf8("thresholdText"));
        thresholdText->setGeometry(QRect(340, 30, 131, 20));
        thresholdText->setReadOnly(false);
        RestDensityText = new QLineEdit(widget_2);
        RestDensityText->setObjectName(QString::fromUtf8("RestDensityText"));
        RestDensityText->setGeometry(QRect(90, 60, 141, 20));
        label_12 = new QLabel(widget_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(250, 90, 81, 21));
        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 90, 81, 21));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        ParticleMassText = new QLineEdit(widget_2);
        ParticleMassText->setObjectName(QString::fromUtf8("ParticleMassText"));
        ParticleMassText->setGeometry(QRect(90, 0, 141, 20));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ParticleMassText->sizePolicy().hasHeightForWidth());
        ParticleMassText->setSizePolicy(sizePolicy1);
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 81, 21));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        ViscosityText = new QLineEdit(widget_2);
        ViscosityText->setObjectName(QString::fromUtf8("ViscosityText"));
        ViscosityText->setGeometry(QRect(90, 30, 141, 20));
        label_11 = new QLabel(widget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(250, 60, 81, 21));
        restitutionText = new QLineEdit(widget_2);
        restitutionText->setObjectName(QString::fromUtf8("restitutionText"));
        restitutionText->setGeometry(QRect(340, 60, 131, 20));
        restitutionText->setReadOnly(false);

        gridLayout->addWidget(widget_2, 1, 0, 1, 1);

        openGLWidget = new SimulationCanvasGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy2);
        openGLWidget->setMinimumSize(QSize(500, 400));
        openGLWidget->setCursor(QCursor(Qt::PointingHandCursor));
        openGLWidget->setMouseTracking(false);

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        widget->setMinimumSize(QSize(250, 0));
        widget->setBaseSize(QSize(320, 0));
        widget->setAutoFillBackground(false);
        ResumeButton = new QPushButton(widget);
        ResumeButton->setObjectName(QString::fromUtf8("ResumeButton"));
        ResumeButton->setGeometry(QRect(20, 570, 75, 23));
        ResumeButton->setCheckable(false);
        ResumeButton->setChecked(false);
        ResumeButton->setAutoDefault(false);
        ResumeButton->setFlat(false);
        ClearButton = new QPushButton(widget);
        ClearButton->setObjectName(QString::fromUtf8("ClearButton"));
        ClearButton->setGeometry(QRect(160, 570, 75, 23));
        ClearButton->setCheckable(false);
        ClearButton->setChecked(false);
        SlownessSlider = new QSlider(widget);
        SlownessSlider->setObjectName(QString::fromUtf8("SlownessSlider"));
        SlownessSlider->setGeometry(QRect(80, 410, 160, 22));
        SlownessSlider->setMinimum(1);
        SlownessSlider->setMaximum(20);
        SlownessSlider->setSingleStep(4);
        SlownessSlider->setOrientation(Qt::Horizontal);
        SlownessSlider->setInvertedAppearance(false);
        SlownessSlider->setTickPosition(QSlider::TicksAbove);
        SlownessSlider->setTickInterval(5);
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 410, 61, 21));
        emissionSlider = new QSlider(widget);
        emissionSlider->setObjectName(QString::fromUtf8("emissionSlider"));
        emissionSlider->setGeometry(QRect(80, 460, 160, 22));
        emissionSlider->setMinimum(1);
        emissionSlider->setMaximum(10);
        emissionSlider->setValue(10);
        emissionSlider->setOrientation(Qt::Horizontal);
        emissionSlider->setTickPosition(QSlider::TicksAbove);
        emissionSlider->setTickInterval(5);
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 460, 61, 21));
        gravitySlider = new QSlider(widget);
        gravitySlider->setObjectName(QString::fromUtf8("gravitySlider"));
        gravitySlider->setGeometry(QRect(80, 350, 160, 22));
        gravitySlider->setMaximum(20);
        gravitySlider->setOrientation(Qt::Horizontal);
        gravitySlider->setTickPosition(QSlider::TicksAbove);
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 350, 61, 21));
        timeDeltaSlider = new QSlider(widget);
        timeDeltaSlider->setObjectName(QString::fromUtf8("timeDeltaSlider"));
        timeDeltaSlider->setGeometry(QRect(80, 510, 160, 22));
        timeDeltaSlider->setMinimum(1);
        timeDeltaSlider->setMaximum(10);
        timeDeltaSlider->setSingleStep(1);
        timeDeltaSlider->setPageStep(1);
        timeDeltaSlider->setValue(10);
        timeDeltaSlider->setSliderPosition(10);
        timeDeltaSlider->setOrientation(Qt::Horizontal);
        timeDeltaSlider->setTickPosition(QSlider::TicksAbove);
        timeDeltaSlider->setTickInterval(5);
        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 510, 61, 21));
        colorSelectButton = new QPushButton(widget);
        colorSelectButton->setObjectName(QString::fromUtf8("colorSelectButton"));
        colorSelectButton->setGeometry(QRect(90, 290, 101, 23));
        colorSelectButton->setAutoFillBackground(false);
        colorSelectButton->setAutoDefault(false);
        label_13 = new QLabel(widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 290, 81, 16));
        pauseOnSpawnCheckbox = new QCheckBox(widget);
        pauseOnSpawnCheckbox->setObjectName(QString::fromUtf8("pauseOnSpawnCheckbox"));
        pauseOnSpawnCheckbox->setGeometry(QRect(10, 260, 231, 17));
        pauseOnSpawnCheckbox->setChecked(true);
        CollisionObjectsComboBox = new QComboBox(widget);
        CollisionObjectsComboBox->setObjectName(QString::fromUtf8("CollisionObjectsComboBox"));
        CollisionObjectsComboBox->setGeometry(QRect(88, 190, 101, 22));
        CollisionObjectsComboBox->setEditable(false);
        label_14 = new QLabel(widget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 190, 71, 20));

        gridLayout->addWidget(widget, 0, 2, 3, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 788, 21));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindowClass);

        ResumeButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", nullptr));
        BuoyancyText->setPlaceholderText(QApplication::translate("MainWindowClass", "Buoyancy", nullptr));
        label_2->setText(QApplication::translate("MainWindowClass", "Viscosity", nullptr));
        surfaceTensionText->setText(QString());
        surfaceTensionText->setPlaceholderText(QApplication::translate("MainWindowClass", "Surface Tension", nullptr));
        GasConstantText->setPlaceholderText(QApplication::translate("MainWindowClass", "Gas Constant", nullptr));
        label_9->setText(QApplication::translate("MainWindowClass", "Threshold", nullptr));
        label_8->setText(QApplication::translate("MainWindowClass", "Surface Tension", nullptr));
        label_3->setText(QApplication::translate("MainWindowClass", "Rest Density", nullptr));
        thresholdText->setPlaceholderText(QApplication::translate("MainWindowClass", "Threshold", nullptr));
        RestDensityText->setPlaceholderText(QApplication::translate("MainWindowClass", "Rest Density", nullptr));
        label_12->setText(QApplication::translate("MainWindowClass", "Buoyancy", nullptr));
        label_4->setText(QApplication::translate("MainWindowClass", "Gas Constant", nullptr));
        ParticleMassText->setPlaceholderText(QApplication::translate("MainWindowClass", "Mass", nullptr));
        label->setText(QApplication::translate("MainWindowClass", "Mass", nullptr));
        ViscosityText->setPlaceholderText(QApplication::translate("MainWindowClass", "Viscosity", nullptr));
        label_11->setText(QApplication::translate("MainWindowClass", "Restitution", nullptr));
        restitutionText->setPlaceholderText(QApplication::translate("MainWindowClass", "Restitution", nullptr));
        ResumeButton->setText(QApplication::translate("MainWindowClass", "Start", nullptr));
#ifndef QT_NO_SHORTCUT
        ResumeButton->setShortcut(QApplication::translate("MainWindowClass", "R", nullptr));
#endif // QT_NO_SHORTCUT
        ClearButton->setText(QApplication::translate("MainWindowClass", "Clear", nullptr));
#ifndef QT_NO_SHORTCUT
        ClearButton->setShortcut(QApplication::translate("MainWindowClass", "C", nullptr));
#endif // QT_NO_SHORTCUT
        label_5->setText(QApplication::translate("MainWindowClass", "Slower", nullptr));
        label_6->setText(QApplication::translate("MainWindowClass", "Emission", nullptr));
        label_7->setText(QApplication::translate("MainWindowClass", "Gravity", nullptr));
        label_10->setText(QApplication::translate("MainWindowClass", "Time Delta", nullptr));
        colorSelectButton->setText(QString());
        label_13->setText(QApplication::translate("MainWindowClass", "Choose Color:", nullptr));
        pauseOnSpawnCheckbox->setText(QApplication::translate("MainWindowClass", "Pause on spawn", nullptr));
        label_14->setText(QApplication::translate("MainWindowClass", "Collision", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
