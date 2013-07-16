/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jul 16 11:42:51 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *leTimeHours;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *leTimeMinutes;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QComboBox *cbTimeZone;
    QGroupBox *groupBox_5;
    QHBoxLayout *hboxLayout;
    QProgressBar *progressbar;
    QGroupBox *gbWireless;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbSSID;
    QLineEdit *leSSID;
    QCheckBox *cbHidden;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbPassword;
    QLineEdit *lePassword;
    QComboBox *cbWPA_WEP;
    QHBoxLayout *hboxLayout1;
    QLabel *VerLabel;
    QPushButton *bCancel;
    QPushButton *bRead;
    QPushButton *bWrite;
    QPushButton *bExit;
    QHBoxLayout *hboxLayout2;
    QGroupBox *groupBox;
    QHBoxLayout *hboxLayout3;
    QLineEdit *leFile;
    QToolButton *tbBrowse;
    QGroupBox *groupBox_2;
    QHBoxLayout *hboxLayout4;
    QComboBox *cboxDevice;
    QHBoxLayout *hboxLayout5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *md5CheckBox;
    QLabel *md5header;
    QLabel *md5label;
    QGroupBox *gbTarget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbTarget;
    QLineEdit *leCIN;
    QLabel *label_4;
    QLineEdit *leChannelID;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(355, 452);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/Win32DiskImager.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        leTimeHours = new QLineEdit(groupBox_3);
        leTimeHours->setObjectName(QString::fromUtf8("leTimeHours"));
        leTimeHours->setMaxLength(2);

        horizontalLayout_5->addWidget(leTimeHours);


        gridLayout_2->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        leTimeMinutes = new QLineEdit(groupBox_3);
        leTimeMinutes->setObjectName(QString::fromUtf8("leTimeMinutes"));
        leTimeMinutes->setMaxLength(2);

        horizontalLayout_6->addWidget(leTimeMinutes);


        gridLayout_2->addLayout(horizontalLayout_6, 0, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        cbTimeZone = new QComboBox(groupBox_3);
        cbTimeZone->setObjectName(QString::fromUtf8("cbTimeZone"));

        horizontalLayout_7->addWidget(cbTimeZone);


        gridLayout_2->addLayout(horizontalLayout_7, 1, 0, 1, 2);


        gridLayout->addWidget(groupBox_3, 4, 0, 1, 1);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        hboxLayout = new QHBoxLayout(groupBox_5);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        progressbar = new QProgressBar(groupBox_5);
        progressbar->setObjectName(QString::fromUtf8("progressbar"));
        progressbar->setValue(0);
        progressbar->setTextVisible(true);

        hboxLayout->addWidget(progressbar);


        gridLayout->addWidget(groupBox_5, 6, 0, 1, 1);

        gbWireless = new QGroupBox(centralwidget);
        gbWireless->setObjectName(QString::fromUtf8("gbWireless"));
        verticalLayout_3 = new QVBoxLayout(gbWireless);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbSSID = new QLabel(gbWireless);
        lbSSID->setObjectName(QString::fromUtf8("lbSSID"));

        horizontalLayout_2->addWidget(lbSSID);

        leSSID = new QLineEdit(gbWireless);
        leSSID->setObjectName(QString::fromUtf8("leSSID"));

        horizontalLayout_2->addWidget(leSSID);

        cbHidden = new QCheckBox(gbWireless);
        cbHidden->setObjectName(QString::fromUtf8("cbHidden"));

        horizontalLayout_2->addWidget(cbHidden);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lbPassword = new QLabel(gbWireless);
        lbPassword->setObjectName(QString::fromUtf8("lbPassword"));

        horizontalLayout_3->addWidget(lbPassword);

        lePassword = new QLineEdit(gbWireless);
        lePassword->setObjectName(QString::fromUtf8("lePassword"));

        horizontalLayout_3->addWidget(lePassword);

        cbWPA_WEP = new QComboBox(gbWireless);
        cbWPA_WEP->setObjectName(QString::fromUtf8("cbWPA_WEP"));

        horizontalLayout_3->addWidget(cbWPA_WEP);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout_2);


        gridLayout->addWidget(gbWireless, 3, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        VerLabel = new QLabel(centralwidget);
        VerLabel->setObjectName(QString::fromUtf8("VerLabel"));
        VerLabel->setBaseSize(QSize(40, 20));
        VerLabel->setText(QString::fromUtf8(""));
        VerLabel->setAlignment(Qt::AlignCenter);

        hboxLayout1->addWidget(VerLabel);

        bCancel = new QPushButton(centralwidget);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));
        bCancel->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(bCancel->sizePolicy().hasHeightForWidth());
        bCancel->setSizePolicy(sizePolicy1);

        hboxLayout1->addWidget(bCancel);

        bRead = new QPushButton(centralwidget);
        bRead->setObjectName(QString::fromUtf8("bRead"));
        sizePolicy1.setHeightForWidth(bRead->sizePolicy().hasHeightForWidth());
        bRead->setSizePolicy(sizePolicy1);

        hboxLayout1->addWidget(bRead);

        bWrite = new QPushButton(centralwidget);
        bWrite->setObjectName(QString::fromUtf8("bWrite"));
        sizePolicy1.setHeightForWidth(bWrite->sizePolicy().hasHeightForWidth());
        bWrite->setSizePolicy(sizePolicy1);

        hboxLayout1->addWidget(bWrite);

        bExit = new QPushButton(centralwidget);
        bExit->setObjectName(QString::fromUtf8("bExit"));
        sizePolicy1.setHeightForWidth(bExit->sizePolicy().hasHeightForWidth());
        bExit->setSizePolicy(sizePolicy1);

        hboxLayout1->addWidget(bExit);


        gridLayout->addLayout(hboxLayout1, 7, 0, 1, 1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        hboxLayout3 = new QHBoxLayout(groupBox);
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout3->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        leFile = new QLineEdit(groupBox);
        leFile->setObjectName(QString::fromUtf8("leFile"));

        hboxLayout3->addWidget(leFile);

        tbBrowse = new QToolButton(groupBox);
        tbBrowse->setObjectName(QString::fromUtf8("tbBrowse"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/browse.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbBrowse->setIcon(icon1);

        hboxLayout3->addWidget(tbBrowse);


        hboxLayout2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        hboxLayout4 = new QHBoxLayout(groupBox_2);
#ifndef Q_OS_MAC
        hboxLayout4->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout4->setContentsMargins(9, 9, 9, 9);
#endif
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        cboxDevice = new QComboBox(groupBox_2);
        cboxDevice->setObjectName(QString::fromUtf8("cboxDevice"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cboxDevice->sizePolicy().hasHeightForWidth());
        cboxDevice->setSizePolicy(sizePolicy2);

        hboxLayout4->addWidget(cboxDevice);


        hboxLayout2->addWidget(groupBox_2);


        gridLayout->addLayout(hboxLayout2, 0, 0, 1, 1);

        hboxLayout5 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout5->setSpacing(6);
#endif
        hboxLayout5->setContentsMargins(0, 0, 0, 0);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, -1, -1, -1);
        md5CheckBox = new QCheckBox(centralwidget);
        md5CheckBox->setObjectName(QString::fromUtf8("md5CheckBox"));
        sizePolicy1.setHeightForWidth(md5CheckBox->sizePolicy().hasHeightForWidth());
        md5CheckBox->setSizePolicy(sizePolicy1);
        md5CheckBox->setMinimumSize(QSize(20, 0));

        horizontalLayout->addWidget(md5CheckBox);

        md5header = new QLabel(centralwidget);
        md5header->setObjectName(QString::fromUtf8("md5header"));
        md5header->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(md5header->sizePolicy().hasHeightForWidth());
        md5header->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(md5header);

        md5label = new QLabel(centralwidget);
        md5label->setObjectName(QString::fromUtf8("md5label"));
        md5label->setEnabled(false);
        md5label->setText(QString::fromUtf8(""));
        md5label->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(md5label);


        verticalLayout->addLayout(horizontalLayout);


        hboxLayout5->addLayout(verticalLayout);


        gridLayout->addLayout(hboxLayout5, 1, 0, 1, 1);

        gbTarget = new QGroupBox(centralwidget);
        gbTarget->setObjectName(QString::fromUtf8("gbTarget"));
        verticalLayout_4 = new QVBoxLayout(gbTarget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lbTarget = new QLabel(gbTarget);
        lbTarget->setObjectName(QString::fromUtf8("lbTarget"));

        horizontalLayout_4->addWidget(lbTarget);

        leCIN = new QLineEdit(gbTarget);
        leCIN->setObjectName(QString::fromUtf8("leCIN"));
        leCIN->setMaxLength(4);

        horizontalLayout_4->addWidget(leCIN);

        label_4 = new QLabel(gbTarget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        leChannelID = new QLineEdit(gbTarget);
        leChannelID->setObjectName(QString::fromUtf8("leChannelID"));
        leChannelID->setMaxLength(6);

        horizontalLayout_4->addWidget(leChannelID);


        verticalLayout_4->addLayout(horizontalLayout_4);


        gridLayout->addWidget(gbTarget, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(leFile, tbBrowse);
        QWidget::setTabOrder(tbBrowse, cboxDevice);
        QWidget::setTabOrder(cboxDevice, leCIN);
        QWidget::setTabOrder(leCIN, leChannelID);
        QWidget::setTabOrder(leChannelID, leSSID);
        QWidget::setTabOrder(leSSID, cbHidden);
        QWidget::setTabOrder(cbHidden, lePassword);
        QWidget::setTabOrder(lePassword, cbWPA_WEP);
        QWidget::setTabOrder(cbWPA_WEP, leTimeHours);
        QWidget::setTabOrder(leTimeHours, leTimeMinutes);
        QWidget::setTabOrder(leTimeMinutes, cbTimeZone);
        QWidget::setTabOrder(cbTimeZone, bWrite);
        QWidget::setTabOrder(bWrite, bExit);
        QWidget::setTabOrder(bExit, bCancel);
        QWidget::setTabOrder(bCancel, bRead);
        QWidget::setTabOrder(bRead, md5CheckBox);

        retranslateUi(MainWindow);
        QObject::connect(bExit, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PlugNPlayr", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Shutdown time", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Hours (0..23)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Minutes (00..59)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Timezone", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Progress", 0, QApplication::UnicodeUTF8));
        progressbar->setFormat(QApplication::translate("MainWindow", "%p%", 0, QApplication::UnicodeUTF8));
        gbWireless->setTitle(QApplication::translate("MainWindow", "Wireless", 0, QApplication::UnicodeUTF8));
        lbSSID->setText(QApplication::translate("MainWindow", "SSID", 0, QApplication::UnicodeUTF8));
        cbHidden->setText(QApplication::translate("MainWindow", "Hidden", 0, QApplication::UnicodeUTF8));
        lbPassword->setText(QApplication::translate("MainWindow", "Password", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bCancel->setStatusTip(QApplication::translate("MainWindow", "Cancel current process.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bCancel->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bRead->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        bRead->setStatusTip(QApplication::translate("MainWindow", "Read data from 'Device' to 'Image File'", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bRead->setText(QApplication::translate("MainWindow", "Read", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bWrite->setStatusTip(QApplication::translate("MainWindow", "Write data in 'Image File' to 'Device'", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bWrite->setText(QApplication::translate("MainWindow", "Write", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        bExit->setStatusTip(QApplication::translate("MainWindow", "Exit Win32 Disk Imager", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        bExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Image File", 0, QApplication::UnicodeUTF8));
        tbBrowse->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Device", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        md5CheckBox->setToolTip(QApplication::translate("MainWindow", "If checked, will generate the MD5 hash for the image file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        md5CheckBox->setText(QString());
        md5header->setText(QApplication::translate("MainWindow", "MD5 Hash: ", 0, QApplication::UnicodeUTF8));
        gbTarget->setTitle(QApplication::translate("MainWindow", "Playback address", 0, QApplication::UnicodeUTF8));
        lbTarget->setText(QApplication::translate("MainWindow", "http://playr.biz/", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "/", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
