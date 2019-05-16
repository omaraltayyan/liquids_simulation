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
    SimulationCanvasGLWidget *openGLWidget;
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
    QWidget *tab_2;
    QWidget *widget;
    QPushButton *ResumeButton;
    QPushButton *ClearButton;
    QSlider *SlownessSlider;
    QLabel *label_5;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(742, 697);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(20);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        openGLWidget = new SimulationCanvasGLWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setMinimumSize(QSize(400, 300));
        openGLWidget->setCursor(QCursor(Qt::PointingHandCursor));
        openGLWidget->setMouseTracking(false);

        gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
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
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 70, 81, 21));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 100, 81, 21));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        ViscosityText = new QLineEdit(tab);
        ViscosityText->setObjectName(QString::fromUtf8("ViscosityText"));
        ViscosityText->setGeometry(QRect(100, 40, 141, 20));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 81, 21));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        ParticleMassText = new QLineEdit(tab);
        ParticleMassText->setObjectName(QString::fromUtf8("ParticleMassText"));
        ParticleMassText->setGeometry(QRect(100, 10, 141, 20));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ParticleMassText->sizePolicy().hasHeightForWidth());
        ParticleMassText->setSizePolicy(sizePolicy3);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);

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
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 410, 61, 21));

        gridLayout->addWidget(widget, 0, 2, 2, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 742, 21));
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
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
