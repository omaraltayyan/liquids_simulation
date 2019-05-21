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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "SimulationCanvasGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *GasConstantText;
    QLineEdit *RestDensityText;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *ViscosityText;
    QLabel *label_2;
    QLineEdit *ParticleMassText;
    QLineEdit *surfaceTensionText;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *thresholdText;
    QLabel *label_11;
    QLineEdit *restitutionText;
    QLabel *label_12;
    QLineEdit *BuoyancyText;
    QWidget *tab_2;
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
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(0, 200));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        GasConstantText = new QLineEdit(tab);
        GasConstantText->setObjectName(QString::fromUtf8("GasConstantText"));
        GasConstantText->setGeometry(QRect(100, 100, 141, 20));
        RestDensityText = new QLineEdit(tab);
        RestDensityText->setObjectName(QString::fromUtf8("RestDensityText"));
        RestDensityText->setGeometry(QRect(100, 70, 141, 20));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 81, 21));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 70, 81, 21));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 100, 81, 21));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        ViscosityText = new QLineEdit(tab);
        ViscosityText->setObjectName(QString::fromUtf8("ViscosityText"));
        ViscosityText->setGeometry(QRect(100, 40, 141, 20));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 81, 21));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        ParticleMassText = new QLineEdit(tab);
        ParticleMassText->setObjectName(QString::fromUtf8("ParticleMassText"));
        ParticleMassText->setGeometry(QRect(100, 10, 141, 20));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ParticleMassText->sizePolicy().hasHeightForWidth());
        ParticleMassText->setSizePolicy(sizePolicy2);
        surfaceTensionText = new QLineEdit(tab);
        surfaceTensionText->setObjectName(QString::fromUtf8("surfaceTensionText"));
        surfaceTensionText->setGeometry(QRect(352, 10, 131, 20));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(260, 10, 81, 21));
        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(260, 40, 81, 21));
        thresholdText = new QLineEdit(tab);
        thresholdText->setObjectName(QString::fromUtf8("thresholdText"));
        thresholdText->setGeometry(QRect(350, 40, 131, 20));
        thresholdText->setReadOnly(false);
        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(260, 70, 81, 21));
        restitutionText = new QLineEdit(tab);
        restitutionText->setObjectName(QString::fromUtf8("restitutionText"));
        restitutionText->setGeometry(QRect(350, 70, 131, 20));
        restitutionText->setReadOnly(false);
        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(260, 100, 81, 21));
        BuoyancyText = new QLineEdit(tab);
        BuoyancyText->setObjectName(QString::fromUtf8("BuoyancyText"));
        BuoyancyText->setGeometry(QRect(350, 100, 131, 20));
        BuoyancyText->setReadOnly(false);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);

        openGLWidget = new SimulationCanvasGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy3);
        openGLWidget->setMinimumSize(QSize(500, 400));
        openGLWidget->setCursor(QCursor(Qt::PointingHandCursor));
        openGLWidget->setMouseTracking(false);

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy4);
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
        timeDeltaSlider->setMaximum(100);
        timeDeltaSlider->setSingleStep(10);
        timeDeltaSlider->setPageStep(10);
        timeDeltaSlider->setValue(10);
        timeDeltaSlider->setOrientation(Qt::Horizontal);
        timeDeltaSlider->setTickPosition(QSlider::TicksAbove);
        timeDeltaSlider->setTickInterval(5);
        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 510, 61, 21));
        colorSelectButton = new QPushButton(widget);
        colorSelectButton->setObjectName(QString::fromUtf8("colorSelectButton"));
        colorSelectButton->setGeometry(QRect(130, 290, 75, 23));
        colorSelectButton->setAutoFillBackground(false);
        colorSelectButton->setAutoDefault(false);
        label_13 = new QLabel(widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(40, 290, 81, 16));

        gridLayout->addWidget(widget, 0, 2, 2, 1);

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

        tabWidget->setCurrentIndex(0);
        ResumeButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", nullptr));
        GasConstantText->setPlaceholderText(QApplication::translate("MainWindowClass", "Gas Constant", nullptr));
        RestDensityText->setPlaceholderText(QApplication::translate("MainWindowClass", "Rest Density", nullptr));
        label->setText(QApplication::translate("MainWindowClass", "Mass", nullptr));
        label_3->setText(QApplication::translate("MainWindowClass", "Rest Density", nullptr));
        label_4->setText(QApplication::translate("MainWindowClass", "Gas Constant", nullptr));
        ViscosityText->setPlaceholderText(QApplication::translate("MainWindowClass", "Viscosity", nullptr));
        label_2->setText(QApplication::translate("MainWindowClass", "Viscosity", nullptr));
        ParticleMassText->setPlaceholderText(QApplication::translate("MainWindowClass", "Mass", nullptr));
        surfaceTensionText->setText(QString());
        surfaceTensionText->setPlaceholderText(QApplication::translate("MainWindowClass", "Surface Tension", nullptr));
        label_8->setText(QApplication::translate("MainWindowClass", "Surface Tension", nullptr));
        label_9->setText(QApplication::translate("MainWindowClass", "Threshold", nullptr));
        thresholdText->setPlaceholderText(QApplication::translate("MainWindowClass", "Threshold", nullptr));
        label_11->setText(QApplication::translate("MainWindowClass", "Restitution", nullptr));
        restitutionText->setPlaceholderText(QApplication::translate("MainWindowClass", "Restitution", nullptr));
        label_12->setText(QApplication::translate("MainWindowClass", "Buoyancy", nullptr));
        BuoyancyText->setPlaceholderText(QApplication::translate("MainWindowClass", "Buoyancy", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindowClass", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindowClass", "Tab 2", nullptr));
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
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
