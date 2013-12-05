/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, write to the Free Software       *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,                *
 *  Boston, MA  02110-1301, USA.                                      *
 *                                                                    *
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009, 2012 ImageWriter developers                   *
 *                           https://launchpad.net/~image-writer-devs *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <dbt.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
//#include <TimeStamp>

#include "disk.h"
#include "mainwindow.h"
#include "md5.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    getLogicalDrives();
    status = STATUS_IDLE;
    progressbar->reset();
    statusbar->showMessage(tr("Waiting for a task."));
    hVolume = INVALID_HANDLE_VALUE;
    hFile = INVALID_HANDLE_VALUE;
    hRawDisk = INVALID_HANDLE_VALUE;
    if (QCoreApplication::arguments().count() > 1)
    {
        QString filelocation = QApplication::arguments().at(1);
        QFileInfo fileInfo(filelocation);
        leFile->setText(fileInfo.absoluteFilePath());
    }

    setReadWriteButtonState();
    QString myver = tr("Version: %1").arg(VER);
    VerLabel->setText(myver);
    sectorData = NULL;
    sectorsize = 0ul;

    keepAppendKey = QString("keep_append");
    baseURL = QString("http://playr.biz/");
    channelPlaybackUrlPart = QString("/");
    playerRedirectUrlPart = QString("/pr/");

    myHomeDir = QDir::homePath();
    if (myHomeDir == NULL){
        myHomeDir = qgetenv("USERPROFILE");
    }
    QRegExp dir(tr("/Downloads$"));
    dir.setPatternSyntax(QRegExp::RegExp);
    QDirIterator it(myHomeDir, QDir::AllDirs|QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (it.fileInfo().filePath().contains(dir)){
            myHomeDir = it.filePath();
            break;
        }
    }

    // init time zone drop box
    allTimeZones = initTimeZones();
    for (int i = 0; i < allTimeZones.count(); i++)
        cbTimeZone->addItem(allTimeZones[i].description, QVariant(i));

    // init WPA WEP drop box
    cbWPA_WEP->addItem(QString("WPA/WPA2"), QVariant(0));
    cbWPA_WEP->addItem(QString("WEP"), QVariant(1));

    // init resolution drop box
    cbResolution->addItem(QString("Default"), QVariant(0));
    cbResolution->addItem(QString("1080p (1920x1080)"), QVariant(1));
    cbResolution->addItem(QString("720p (1280x720)"), QVariant(2));

    // make md5 fields invisible, they might confuse our users
    md5CheckBox->setVisible(false);
    md5header->setVisible(false);
    md5label->setVisible(false);
}

MainWindow::~MainWindow()
{
    if (hRawDisk != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
    }
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
    if (hVolume != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hVolume);
        hVolume = INVALID_HANDLE_VALUE;
    }
    if (sectorData != NULL)
    {
        delete sectorData;
        sectorData = NULL;
    }
}
QVector<TimeZone> MainWindow::initTimeZones()
{
    QVector<TimeZone> result = QVector<TimeZone>();

    result << TimeZone(QString(""), QString(" "),  QString(QString("00:00")), 0, 0, 0);
    result << TimeZone(QString("Pacific/Midway"), QString("(GMT-11:00) Midway Island, Samoa"),  QString(QString("-11:00")), -1, 11, 0);
    result << TimeZone(QString("America/Adak"), QString("(GMT-10:00) Hawaii-Aleutian"), QString("-10:00"), -1, 10, 0);
    result << TimeZone(QString("Pacific/Honolulu"), QString("(GMT-10:00) Hawaii"), QString("-10:00"), -1, 10, 0);
    result << TimeZone(QString("Pacific/Marquesas"), QString("(GMT-09:30) Marquesas Islands"), QString("-09:30"), -1, 9, 30);
    result << TimeZone(QString("Pacific/Gambier"), QString("(GMT-09:00) Gambier Islands"), QString("-09:00"), -1, 9, 0);
    result << TimeZone(QString("America/Anchorage"), QString("(GMT-09:00) Alaska"), QString("-09:00"), -1, 9, 0);
    result << TimeZone(QString("America/Tijuana"), QString("(GMT-08:00) Tijuana, Baja California"), QString("-08:00"), -1, 8, 0);
    result << TimeZone(QString("Pacific/Pitcairn"), QString("(GMT-08:00) Pitcairn Islands"), QString("-08:00"), -1, 8, 0);
    result << TimeZone(QString("America/Los_Angeles"), QString("(GMT-08:00) Pacific Time (US & Canada)"), QString("-08:00"), -1, 8, 0);
    result << TimeZone(QString("America/Denver"), QString("(GMT-07:00) Mountain Time (US & Canada)"), QString("-07:00"), -1, 7, 0);
    result << TimeZone(QString("America/Chihuahua"), QString("(GMT-07:00) Chihuahua, La Paz, Mazatlan"), QString("-07:00"), -1, 7, 0);
    result << TimeZone(QString("America/Dawson_Creek"), QString("(GMT-07:00) Arizona"), QString("-07:00"), -1, 7, 0);
    result << TimeZone(QString("America/Belize"), QString("(GMT-06:00) Saskatchewan, Central America"), QString("-06:00"), -1, 6, 0);
    result << TimeZone(QString("America/Cancun"), QString("(GMT-06:00) Guadalajara, Mexico City, Monterrey"), QString("-06:00"), -1, 6, 0);
    result << TimeZone(QString("Pacific/Easter"), QString("(GMT-06:00) Easter Island"), QString("-06:00"), -1, 6, 0);
    result << TimeZone(QString("America/Chicago"), QString("(GMT-06:00) Central Time (US & Canada)"), QString("-06:00"), -1, 6, 0);
    result << TimeZone(QString("America/New_York"), QString("(GMT-05:00) Eastern Time (US & Canada)"), QString("-05:00"), -1, 5, 0);
    result << TimeZone(QString("America/Havana"), QString("(GMT-05:00) Cuba"), QString("-05:00"), -1, 5, 0);
    result << TimeZone(QString("America/Bogota"), QString("(GMT-05:00) Bogota, Lima, Quito, Rio Branco"), QString("-05:00"), -1, 5, 0);
    result << TimeZone(QString("America/Caracas"), QString("(GMT-04:30) Caracas"), QString("-04:30"), -1, 4, 30);
    result << TimeZone(QString("America/Santiago"), QString("(GMT-04:00) Santiago"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("America/La_Paz"), QString("(GMT-04:00) La Paz"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("Atlantic/Stanley"), QString("(GMT-04:00) Faukland Islands"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("America/Campo_Grande"), QString("(GMT-04:00) Brazil"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("America/Goose_Bay"), QString("(GMT-04:00) Atlantic Time (Goose Bay)"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("America/Glace_Bay"), QString("(GMT-04:00) Atlantic Time (Canada)"), QString("-04:00"), -1, 4, 0);
    result << TimeZone(QString("America/St_Johns"), QString("(GMT-03:30) Newfoundland"), QString("-03:30"), -1, 3, 30);
    result << TimeZone(QString("America/Araguaina"), QString("(GMT-03:00) Araguaina"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Montevideo"), QString("(GMT-03:00) Montevideo"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Miquelon"), QString("(GMT-03:00) Miquelon, St. Pierre"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Godthab"), QString("(GMT-03:00) Greenland"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Argentina/Buenos_Aires"), QString("(GMT-03:00) Buenos Aires"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Sao_Paulo"), QString("(GMT-03:00) Brasilia"), QString("-03:00"), -1, 3, 0);
    result << TimeZone(QString("America/Noronha"), QString("(GMT-02:00) Mid-Atlantic"), QString("-02:00"), -1, 2, 0);
    result << TimeZone(QString("Atlantic/Cape_Verde"), QString("(GMT-01:00) Cape Verde Is."), QString("-01:00"), -1, 1, 0);
    result << TimeZone(QString("Atlantic/Azores"), QString("(GMT-01:00) Azores"), QString("-01:00"), -1, 1, 0);
    result << TimeZone(QString("Europe/Dublin"), QString("(GMT) Greenwich Mean Time : Dublin"), QString("00:00"), 1, 0, 0);
    result << TimeZone(QString("Europe/Lisbon"), QString("(GMT) Greenwich Mean Time : Lisbon"), QString("00:00"), 1, 0, 0);
    result << TimeZone(QString("Europe/London"), QString("(GMT) Greenwich Mean Time : London"), QString("00:00"), 1, 0, 0);
    result << TimeZone(QString("Africa/Abidjan"), QString("(GMT) Monrovia, Reykjavik"), QString("00:00"), 1, 0, 0);
    result << TimeZone(QString("Europe/Amsterdam"), QString("(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna"), QString("+01:00"), +1, 1, 0);
    result << TimeZone(QString("Europe/Belgrade"), QString("(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague"), QString("+01:00"), +1, 1, 0);
    result << TimeZone(QString("Europe/Brussels"), QString("(GMT+01:00) Brussels, Copenhagen, Madrid, Paris"), QString("+01:00"), +1, 1, 0);
    result << TimeZone(QString("Africa/Algiers"), QString("(GMT+01:00) West Central Africa"), QString("+01:00"), +1, 1, 0);
    result << TimeZone(QString("Africa/Windhoek"), QString("(GMT+01:00) Windhoek"), QString("+01:00"), +1, 1, 0);
    result << TimeZone(QString("Asia/Beirut"), QString("(GMT+02:00) Beirut"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Africa/Cairo"), QString("(GMT+02:00) Cairo"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Asia/Gaza"), QString("(GMT+02:00) Gaza"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Africa/Blantyre"), QString("(GMT+02:00) Harare, Pretoria"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Asia/Jerusalem"), QString("(GMT+02:00) Jerusalem"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Europe/Minsk"), QString("(GMT+02:00) Minsk"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Asia/Damascus"), QString("(GMT+02:00) Syria"), QString("+02:00"), +1, 2, 0);
    result << TimeZone(QString("Europe/Moscow"), QString("(GMT+03:00) Moscow, St. Petersburg, Volgograd"), QString("+03:00"), +1, 3, 0);
    result << TimeZone(QString("Africa/Addis_Ababa"), QString("(GMT+03:00) Nairobi"), QString("+03:00"), +1, 3, 0);
    result << TimeZone(QString("Asia/Tehran"), QString("(GMT+03:30) Tehran"), QString("+03:30"), +1, 3, 30);
    result << TimeZone(QString("Asia/Dubai"), QString("(GMT+04:00) Abu Dhabi, Muscat"), QString("+04:00"), +1, 4, 0);
    result << TimeZone(QString("Asia/Yerevan"), QString("(GMT+04:00) Yerevan"), QString("+04:00"), +1, 4, 0);
    result << TimeZone(QString("Asia/Kabul"), QString("(GMT+04:30) Kabul"), QString("+04:30"), +1, 4, 30);
    result << TimeZone(QString("Asia/Yekaterinburg"), QString("(GMT+05:00) Ekaterinburg"), QString("+05:00"), +1, 5, 0);
    result << TimeZone(QString("Asia/Tashkent"), QString("(GMT+05:00) Tashkent"), QString("+05:00"), +1, 5, 0);
    result << TimeZone(QString("Asia/Kolkata"), QString("(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi"), QString("+05:30"), +1, 5, 30);
    result << TimeZone(QString("Asia/Kathmandu"), QString("(GMT+05:45) Kathmandu"), QString("+05:45"), +1, 5, 45);
    result << TimeZone(QString("Asia/Dhaka"), QString("(GMT+06:00) Astana, Dhaka"), QString("+06:00"), +1, 6, 0);
    result << TimeZone(QString("Asia/Novosibirsk"), QString("(GMT+06:00) Novosibirsk"), QString("+06:00"), +1, 6, 0);
    result << TimeZone(QString("Asia/Rangoon"), QString("(GMT+06:30) Yangon (Rangoon)"), QString("+06:30"), +1, 6, 30);
    result << TimeZone(QString("Asia/Bangkok"), QString("(GMT+07:00) Bangkok, Hanoi, Jakarta"), QString("+07:00"), +1, 7, 0);
    result << TimeZone(QString("Asia/Krasnoyarsk"), QString("(GMT+07:00) Krasnoyarsk"), QString("+07:00"), +1, 7, 0);
    result << TimeZone(QString("Asia/Hong_Kong"), QString("(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi"), QString("+08:00"), +1, 8, 0);
    result << TimeZone(QString("Asia/Irkutsk"), QString("(GMT+08:00) Irkutsk, Ulaan Bataar"), QString("+08:00"), +1, 8, 0);
    result << TimeZone(QString("Australia/Perth"), QString("(GMT+08:00) Perth"), QString("+08:00"), +1, 8, 0);
    result << TimeZone(QString("Australia/Eucla"), QString("(GMT+08:45) Eucla"), QString("+08:45"), +1, 8, 45);
    result << TimeZone(QString("Asia/Tokyo"), QString("(GMT+09:00) Osaka, Sapporo, Tokyo"), QString("+09:00"), +1, 9, 0);
    result << TimeZone(QString("Asia/Seoul"), QString("(GMT+09:00) Seoul"), QString("+09:00"), +1, 9, 0);
    result << TimeZone(QString("Asia/Yakutsk"), QString("(GMT+09:00) Yakutsk"), QString("+09:00"), +1, 9, 0);
    result << TimeZone(QString("Australia/Adelaide"), QString("(GMT+09:30) Adelaide"), QString("+09:30"), +1, 9, 30);
    result << TimeZone(QString("Australia/Darwin"), QString("(GMT+09:30) Darwin"), QString("+09:30"), +1, 9, 30);
    result << TimeZone(QString("Australia/Brisbane"), QString("(GMT+10:00) Brisbane"), QString("+10:00"), +1, 10, 0);
    result << TimeZone(QString("Australia/Hobart"), QString("(GMT+10:00) Hobart"), QString("+10:00"), +1, 10, 0);
    result << TimeZone(QString("Asia/Vladivostok"), QString("(GMT+10:00) Vladivostok"), QString("+10:00"), +1, 10, 0);
    result << TimeZone(QString("Australia/Lord_Howe"), QString("(GMT+10:30) Lord Howe Island"), QString("+10:30"), +1, 10, 30);
    result << TimeZone(QString("Pacific/Noumea"), QString("(GMT+11:00) Solomon Is., New Caledonia"), QString("+11:00"), +1, 11, 0);
    result << TimeZone(QString("Asia/Magadan"), QString("(GMT+11:00) Magadan"), QString("+11:00"), +1, 11, 0);
    result << TimeZone(QString("Pacific/Norfolk"), QString("(GMT+11:30) Norfolk Island"), QString("+11:30"), +1, 11, 30);
    result << TimeZone(QString("Asia/Anadyr"), QString("(GMT+12:00) Anadyr, Kamchatka"), QString("+12:00"), +1, 12, 0);
    result << TimeZone(QString("Pacific/Auckland"), QString("(GMT+12:00) Auckland, Wellington"), QString("+12:00"), +1, 12, 0);
    result << TimeZone(QString("Pacific/Fiji"), QString("(GMT+12:00) Fiji, Kamchatka, Marshall Is."), QString("+12:00"), +1, 12, 0);
    result << TimeZone(QString("Pacific/Chatham"), QString("(GMT+12:45) Chatham Islands"), QString("+12:45"), +1, 12, 45);
    result << TimeZone(QString("Pacific/Tongatapu"), QString("(GMT+13:00) Nuku Alofa"), QString("+13:00"), +1, 13, 0);
    result << TimeZone(QString("Pacific/Kiritimati"), QString("(GMT+14:00) Kiritimati"), QString("+14:00"), +1, 14, 0);
    return result;
}
void MainWindow::setReadWriteButtonState()
{
    // set read and write buttons according to status of file/device
    //bRead->setEnabled(deviceSelected && fileSelected); disable, we only want to write
    bRead->setEnabled(false);
    //bWrite->setEnabled(deviceSelected && fileSelected); enable if either a file is selected or configuration data is present
    bWrite->setEnabled(deviceSelected() && timeSettingsCorrect() && (fileSelected() || wirelessConfigurationShouldBeWritten() || urlShouldBeWritten()));
}

bool MainWindow::fileSelected()
{
    return !(leFile->text().isEmpty());
}
bool MainWindow::deviceSelected()
{
    return (cboxDevice->count() > 0);
}
bool MainWindow::timeSettingsShouldBeWritten()
{
   return ( 
     ( !(leTimeHours->text().isEmpty()) || !(leTimeMinutes->text().isEmpty()) ) 
     && cbTimeZone->currentIndex() != 0
    );
 }
bool MainWindow::timeSettingsCorrect()
{
   return ( 
        (leTimeHours->text().isEmpty() && leTimeMinutes->text().isEmpty() && cbTimeZone->currentIndex() == 0) ||
        ( ( !(leTimeHours->text().isEmpty()) || !(leTimeMinutes->text().isEmpty()) ) 
          && cbTimeZone->currentIndex() != 0 
        )
    );
 }
bool MainWindow::isCINCorrect()
{
    QString CIN = leCIN->text(); 
    return ((CIN.length() == 4) && QRegExp("\\d*").exactMatch(CIN));
}
bool MainWindow::isRightUrlPartCorrect()
{
    QString channelID = leChannelID->text();
    return (cbEnterprise->isChecked() && channelID.length() == 6 && QRegExp("[\\da-zA-Z]*").exactMatch(channelID)) 
            || (!cbEnterprise->isChecked() && QRegExp("\\d*").exactMatch(channelID));
}
bool MainWindow::isURLCorrect()
{
    return isCINCorrect() && isRightUrlPartCorrect();
}
bool MainWindow::urlShouldBeWritten()
{
    return ( !leCIN->text().isEmpty() 
             || !leChannelID->text().isEmpty() 
    );
}
bool MainWindow::isWPASelected()
{
    return (cbWPA_WEP->currentIndex() == 0);
}
bool MainWindow::resolutionShouldBeWritten()
{
    return cbResolution->currentIndex() != 0;
}
int MainWindow::resolutionValue()
{
    return cbResolution->currentIndex();
}
bool MainWindow::wirelessConfigurationShouldBeWritten()
{
    return ( !leSSID->text().isEmpty() 
             || !lePassword->text().isEmpty() 
             || cbWPA_WEP->currentIndex() != 0 
             || cbHidden->checkState() != Qt::Unchecked
    );
}
bool MainWindow::configurationShouldBeWritten()
{
    return timeSettingsShouldBeWritten() || urlShouldBeWritten() || wirelessConfigurationShouldBeWritten();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (status == STATUS_READING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt image file.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
    else if (status == STATUS_WRITING)
    {
        if (QMessageBox::warning(NULL, tr("Exit?"), tr("Exiting now will result in a corrupt disk.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
}
void MainWindow::on_cbTimeZone_currentIndexChanged()
{
    myTimeZone = allTimeZones[cbTimeZone->itemData(cbTimeZone->currentIndex()).toInt()].name;
    setReadWriteButtonState();
}
void MainWindow::on_cbWPA_WEP_currentIndexChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_cbResolution_currentIndexChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_leTimeHours_textChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_leTimeMinutes_textChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_leCIN_textChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_leChannelID_textChanged()
{
    setReadWriteButtonState();
}
void MainWindow::on_tbBrowse_clicked()
{
    QString filelocation = QFileDialog::getOpenFileName(NULL, tr("Select a disk image"), myHomeDir, "*.img;*.IMG;;*.*",
                                                        0, QFileDialog::DontConfirmOverwrite);
    if (!filelocation.isNull())
    {
        leFile->setText(filelocation);
        md5label->clear();

        // if the md5 checkbox is checked, verify that it's a good file
        // and then generate the md5 hash
        if(md5CheckBox->isChecked())
        {
            QFileInfo fileInfo(filelocation);

            if (fileInfo.exists() && fileInfo.isFile() &&
                    fileInfo.isReadable() && (fileInfo.size() > 0) )
            {
                generateMd5(filelocation.toLatin1().data());
            }
        }
    }
}

// generates the md5 hash
void MainWindow::generateMd5(char *filename)
{
    md5label->setText(tr("Generating..."));
    QApplication::processEvents();

    MD5 md5;

    // may take a few secs - display a wait cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // "digestFile" computes the md5 - display it in the textbox
    md5label->setText(md5.digestFile(filename));

    // redisplay the normal cursor
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_leFile_textChanged()
{
    setReadWriteButtonState();

    // if the box was cleared, clear any existing md5 hash
    if( leFile->text().isEmpty() )
    {
        md5label->clear();
    }
}

void MainWindow::on_leSSID_textChanged()
{
    setReadWriteButtonState();
}

void MainWindow::on_lePassword_textChanged()
{
    setReadWriteButtonState();
}

void MainWindow::on_leTarget_textChanged()
{
    setReadWriteButtonState();
}

// on an "editingFinished" signal (IE: return press), if the lineedit
// contains a valid file, and generate the md5
void MainWindow::on_leFile_editingFinished()
{
    if(md5CheckBox->isChecked())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            generateMd5(leFile->text().toLatin1().data());
        }
    }
}

void MainWindow::on_bCancel_clicked()
{
    if ( (status == STATUS_READING) || (status == STATUS_WRITING) )

    {
        if (QMessageBox::warning(NULL, tr("Cancel?"), tr("Canceling now will result in a corrupt destination.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }
    }
}

// if the md5 checkbox becomes "checked", verify the file and generate md5
// when it's "unchecked", clear the md5 label
void MainWindow::on_md5CheckBox_stateChanged()
{
    bool state = md5CheckBox->isChecked();

    md5header->setEnabled(state);
    md5label->setEnabled(state);

    if(state)
    {
        // changed from unchecked to checked
        if( !(leFile->text().isEmpty()) )
        {
            QFileInfo fileinfo(leFile->text());
            if (fileinfo.exists() && fileinfo.isFile() &&
                    fileinfo.isReadable() && (fileinfo.size() > 0) )
            {
                generateMd5(leFile->text().toLatin1().data());
            }
        }

    }
    else
    {
        // changed from checked to unchecked
        md5label->clear();
    }
}
void MainWindow::on_cbEnterprise_stateChanged()
{
    if(cbEnterprise->isChecked())
    {
        lbMiddleOfURL->setText(playerRedirectUrlPart);
    }
    else
    {
        lbMiddleOfURL->setText(channelPlaybackUrlPart);
    }
    setReadWriteButtonState();
}
void MainWindow::on_cbHidden_stateChanged()
{
    setReadWriteButtonState();
}
void MainWindow::disableCancelButton()
{
    bCancel->setEnabled(false);
    setReadWriteButtonState();
}
void MainWindow::setStatusIdleAndDisableCancel()
{
    status = STATUS_IDLE;
    disableCancelButton();
}
void MainWindow::closeHandle(HANDLE handle)
{
    if (handle != NULL)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
}
void MainWindow::closeHandles(HANDLE handle1, HANDLE handle2)
{
    closeHandle(handle1);
    closeHandle(handle2);
}
void MainWindow::removeLockAndCloseHandle(HANDLE handle)
{
    removeLockOnVolume(handle);
    closeHandle(handle);
}
void MainWindow::closeVolumeHandle()
{
    CloseHandle(hVolume);
    hVolume = INVALID_HANDLE_VALUE;
}
void MainWindow::removeLockAndCloseVolumeHandle()
{
    removeLockOnVolume(hVolume);
    closeVolumeHandle();
}
void MainWindow::idleStatusAndCloseVolumeHandle()
{
    closeVolumeHandle();
    setStatusIdleAndDisableCancel();
}
void MainWindow::idleStatusRemoveLockAndCloseVolumeHandle()
{
    removeLockOnVolume(hVolume);
    idleStatusAndCloseVolumeHandle();
}
void MainWindow::closeFileHandle()
{
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
}
void MainWindow::closeRawDiskHandle()
{
    CloseHandle(hRawDisk);
    hRawDisk = INVALID_HANDLE_VALUE;
}
void MainWindow::deleteData()
{
    delete sectorData;
    sectorData = NULL;
}
void MainWindow::deleteDataIdleStatusRemoveLockAndCloseVolumeHandles()
{
    deleteData();
    idleStatusRemoveLockAndCloseVolumeHandle();
    closeRawDiskHandle();
    closeFileHandle();
}
void MainWindow::on_bWrite_clicked()
{
    bool passfail = true;

    if (!isURLCorrect())
    {
        if (cbEnterprise->isChecked())
        {
            QMessageBox::critical(NULL, tr("URL Error"), tr("The URL is incorrect; the first part should consists of 4 numbers, the second part should consist of 6 letters and numbers."));
        }
        else
        {
            QMessageBox::critical(NULL, tr("URL Error"), tr("The URL is incorrect; the first part should consists of 4 numbers, the second part should consist of numbers."));
        }
        return;
    }

    // build the drive letter as a const char *
    //   (without the surrounding brackets)
    QString qs = cboxDevice->currentText();
    qs.replace(QRegExp("[\\[\\]]"), "");
    QByteArray qba = qs.toLocal8Bit();
    char *ltr = qba.data();

    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (leFile->text().at(0) == cboxDevice->currentText().at(1))
            {
                QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }

            if (QMessageBox::warning(NULL, tr("Confirm overwrite"), tr("Writing to a physical device can corrupt the device.\n"
                                                                       "(Target Device: %1 \"%2\")\n"
                                                                       "Are you sure you want to continue?").arg(cboxDevice->currentText()).arg(getDriveLabel(ltr)),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            status = STATUS_WRITING;
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            double mbpersec;
            unsigned long long i, lasti, availablesectors, numsectors;
            int volumeID = cboxDevice->currentText().at(1).toAscii() - 'A';
            int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
            hVolume = getHandleOnVolume(volumeID, GENERIC_WRITE);
            if (hVolume == INVALID_HANDLE_VALUE)
            {
                setStatusIdleAndDisableCancel();
                return;
            }
            if (!getLockOnVolume(hVolume))
            {
                closeHandle(hVolume);
                setStatusIdleAndDisableCancel();
                return;
            }
            if (!unmountVolume(hVolume))
            {
                removeLockAndCloseHandle(hVolume);
                setStatusIdleAndDisableCancel();
                return;
            }
            hFile = getHandleOnFile(leFile->text().toAscii().data(), GENERIC_READ);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                removeLockAndCloseHandle(hVolume);
                setStatusIdleAndDisableCancel();
                return;
            }
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_WRITE);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                removeLockAndCloseHandle(hVolume);
                closeHandle(hFile);
                setStatusIdleAndDisableCancel();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            numsectors = getFileSizeInSectors(hFile, sectorsize);
            if (numsectors > availablesectors)
            {
                QMessageBox::critical(NULL, tr("Write Error"),
                                      tr("Not enough space on disk: Size: %1 sectors  Available: %2 sectors  Sector size: %3").arg(numsectors).arg(availablesectors).arg(sectorsize));
                removeLockAndCloseHandle(hVolume);
                closeHandles(hFile, hRawDisk);
                setStatusIdleAndDisableCancel();
                return;
            }

            progressbar->setRange(0, (numsectors == 0ul) ? 100 : (int)numsectors);
            lasti = 0ul;
            timer.start();
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += 1024ul)
            {
                sectorData = readSectorDataFromHandle(hFile, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
                if (sectorData == NULL)
                {
                    deleteData();
                    removeLockAndCloseHandle(hVolume);
                    closeHandles(hFile, hRawDisk);
                    setStatusIdleAndDisableCancel();
                    return;
                }
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
                {
                    deleteData();
                    removeLockAndCloseHandle(hVolume);
                    closeHandles(hFile, hRawDisk);
                    setStatusIdleAndDisableCancel();
                    return;
                }
                deleteData();
                QCoreApplication::processEvents();
                if (timer.elapsed() >= 1000)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * (1000.0 / timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                    timer.start();
                    lasti = i;
                }
                progressbar->setValue(i);
                QCoreApplication::processEvents();
            }
            removeLockAndCloseVolumeHandle();
            closeHandles(hFile, hRawDisk);
            sectorData = NULL;
            if (status == STATUS_CANCELED)
            {
                passfail = false;
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail)
        {
            QMessageBox::information(NULL, tr("Complete"), tr("Writing of image successful."));
        }

    }
    /*
     * Configuration of OS-startup by editing files written on USB device
     *
     */
    if (configurationShouldBeWritten())
    {
        if (passfail) 
        {
            passfail = writeOSConfiguration(ltr);
            if (passfail) 
            {
                QMessageBox::information(NULL, tr("Complete"), tr("Writing configuration data successful."));
            }
        }
        else
        {
            QMessageBox::critical(NULL, tr("Error"), tr("Configuration data not written because of previously occurred error(s)."));
        }
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
}

void MainWindow::disableWriteAndReadButtons()
{
    bCancel->setEnabled(true);
    bWrite->setEnabled(false);
    bRead->setEnabled(false);
}
QString MainWindow::urlEncode(QString leValue)
{
    return QUrl::toPercentEncoding(leValue);
}
QString MainWindow::replaceSpace(QString leValue)
{
    return leValue.replace(" ", "%20");
}
bool MainWindow::needsInsertion(QString leValue)
{
    return !leValue.isEmpty();
}
bool MainWindow::needsInsertion(QString leValue1, QString leValue2)
{
    return !leValue1.isEmpty() && !leValue2.isEmpty();
}
bool MainWindow::needsInsertion(int hours, int minutes)
{
    return ((hours >= 0) || (minutes >= 0));
}
bool MainWindow::needsURLInsertion(QString leValue)
{
    // regaxp for urls: /^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$/ 
    // see: http://net.tutsplus.com/tutorials/other/8-regular-expressions-you-should-know/
    return !leValue.isEmpty() && (leValue.indexOf(QRegExp("^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?$")) > -1);
}
bool MainWindow::writeOSConfiguration(char * ltr)
{
    bool result = true;
    
    if (configurationShouldBeWritten())
    {
        QString configFileName = QString(ltr + QString("boot\\live.cfg"));
        QFileInfo configfileinfo(configFileName);
        if (configfileinfo.exists() && configfileinfo.isFile() &&
                configfileinfo.isReadable() && (configfileinfo.size() > 0) )
        {
            result = updateConfigurationFile(configFileName);
        }
        else if (!configfileinfo.exists() || !configfileinfo.isFile())
        {
            configFileName = QString(ltr + QString("isolinux\\live.cfg"));
            QFileInfo secondConfigfileinfo(configFileName);
            if (secondConfigfileinfo.exists() && secondConfigfileinfo.isFile() &&
                    secondConfigfileinfo.isReadable() && (secondConfigfileinfo.size() > 0) )
            {
                result = updateConfigurationFile(configFileName);
            }
            else if (!secondConfigfileinfo.exists() || !secondConfigfileinfo.isFile())
            {
                QMessageBox::critical(NULL, tr("File Error"), tr("The expected config file (/boot/live.cfg or /isolinux/live.cfg) does not exist."));
                result = false;
            }
            else if (!secondConfigfileinfo.isReadable())
            {
                QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the config file."));
                result = false;
            }
            else if (secondConfigfileinfo.size() == 0)
            {
                QMessageBox::critical(NULL, tr("File Error"), tr("The config file contains no data."));
                result = false;
            }
        } 
        else if (!configfileinfo.isReadable())
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("You do not have permision to read the config file."));
            result = false;
        }
        else if (configfileinfo.size() == 0)
        {
            QMessageBox::critical(NULL, tr("File Error"), tr("The config file contains no data."));
            result = false;
        }
    }
    else
    {
        QMessageBox::critical(NULL, tr("Information"), tr("Default settings were configured."));
    }
    return result;
}
bool MainWindow::updateConfigurationFile(QString configFileName)
{
    bool result = true;
    QRegExp configLineToEditIndicator = QRegExp("^\\s*[Aa][Pp][Pp][Ee][Nn][Dd]");
    QRegExp configLineInstallSectionIndicator = QRegExp("^\\s*[Ll][Aa][Bb][Ee][Ll]\\s*[Ii][Nn][Ss][Tt][Aa][Ll][Ll]\\s*$");
    status = STATUS_WRITING;
    disableWriteAndReadButtons();

    // construct parameter strings to find and replace
    QString SSID = replaceSpace(leSSID->text());
    bool insertSSID = needsInsertion(leSSID->text());
    QString password = replaceSpace(lePassword->text());
    bool insertPassword = needsInsertion(lePassword->text());
    if (!insertPassword && (cbWPA_WEP->currentIndex() != 0 || cbHidden->checkState() != Qt::Unchecked || insertSSID))
    {
        insertPassword = true;
    }
    QString newURL;
    bool replaceURL = needsInsertion(leCIN->text(), leChannelID->text());
    if (replaceURL) newURL = createURL();
    int hours = toInt(leTimeHours->text());
    int minutes = toInt(leTimeMinutes->text());
    bool insertCron = needsInsertion(hours, minutes);
    bool insertResolution = resolutionShouldBeWritten();
    int resolution = resolutionValue();
    bool inInstallSection = false;

    // open the config file and a temporary new config file
    QString tmpConfigFileName = configFileName + QString(".tmp");
    QFile inFile(configFileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return result;
    QTextStream inStream(&inFile);
    QFile outFile(tmpConfigFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return result;
    QTextStream outStream(&outFile);

    // replace parameters
    while (!inStream.atEnd()) 
    {
        QString line = inStream.readLine();
        if (line.indexOf(configLineInstallSectionIndicator) > -1)
        {
            inInstallSection = true;
        }
        if (line.indexOf(configLineToEditIndicator) > -1)
        {
            // save indentation sincesetParameters returns the bare combined parameter string
            QString indentation = line.left(line.indexOf(configLineToEditIndicator));
            line = indentation + setParameters(line, insertSSID, SSID, 
                                               insertPassword, password, 
                                               replaceURL, newURL,
                                               insertCron, hours, minutes, myTimeZone,
                                               insertResolution, resolution,
                                               inInstallSection);
            inInstallSection = false;
        }
        outStream << line << endl;
    }
    // replace the config file with the new file
    inFile.close();
    inFile.remove();
    outFile.close();
    outFile.rename(configFileName);
    return result;
}
QString MainWindow::createURL()
{
    QString result = QString("");
    if (cbEnterprise->isChecked())
    {
        result = baseURL + leCIN->text() + playerRedirectUrlPart + leChannelID->text();
    }
    else
    {
        result = baseURL + leCIN->text() + channelPlaybackUrlPart + leChannelID->text();
    }
    return result;
}
QString MainWindow::setParameters(QString line, 
                                  bool insertSSID, QString SSID, 
                                  bool insertPassword, QString password, 
                                  bool replaceURL, QString newURL,
                                  bool insertCron, int hours, int minutes, QString timeZone,
                                  bool insertResolution, int resolution,
                                  bool keepParameter)
{
    QStringList items = trimList(line.split(" "));
    QStringList parameters = items.filter(QRegExp("\\S+"));
    setSSIDParameter(parameters, insertSSID, SSID, keepParameter);
    setPasswordParameter(parameters, insertPassword, password, keepParameter);
    setURLParameter(parameters, replaceURL, newURL, keepParameter);
    setCronParameter(parameters, insertCron, hours, minutes, timeZone, keepParameter);
    setResolutionParameter(parameters, insertResolution, resolution, keepParameter);
    QStringList nonEmptyParameters = parameters.filter(QRegExp("\\S+"));
    return nonEmptyParameters.join(QString(" "));
}
void MainWindow::setParameter(QStringList &parameters, QString key, QString value, bool keepParameter)
{
    setBaseParameter(parameters, key, value);
    if (keepParameter) setKeepParameter(parameters, key, value);
}
void MainWindow::setBaseParameter(QStringList &parameters, QString key, QString value)
{
    QRegExp toMatch = QRegExp(key + QString("=\\S*"));
    QString keyEquals = QString(key + QString("="));
    if (parameters.indexOf(toMatch) > -1)
    {
        parameters.replaceInStrings(toMatch, keyEquals + value);
    }
    else
    {
        parameters << QString(keyEquals + value);
    }
}
void MainWindow::setKeepParameter(QStringList &parameters, QString key, QString value)
{
    QRegExp toMatch = QRegExp(keepAppendKey + QString("=") + key + QString("=\\S*"));
    QString keyEquals = QString(keepAppendKey + QString("=") + key + QString("="));
    if (parameters.indexOf(toMatch) > -1)
    {
        parameters.replaceInStrings(toMatch, keyEquals + value);
    }
    else
    {
        parameters << QString(keyEquals + value);
    }
}
void MainWindow::removeParameter(QStringList &parameters, QString keyAndValue)
{
    removeKeepParameter(parameters, keyAndValue);
    removeBaseParameter(parameters, keyAndValue);
}
void MainWindow::removeBaseParameter(QStringList &parameters, QString keyAndValue)
{
    if (parameters.indexOf(QRegExp(keyAndValue)) > -1)
    {
        parameters.replaceInStrings(QRegExp(keyAndValue), QString(""));
    }
}
void MainWindow::removeKeepParameter(QStringList &parameters, QString keyAndValue)
{
    if (parameters.indexOf(QRegExp(keepAppendKey + QString("=") + keyAndValue)) > -1)
    {
        parameters.replaceInStrings(QRegExp(keepAppendKey + QString("=") + keyAndValue), QString(""));
    }
}
int MainWindow::toInt(QString string)
{
    int result = -1;
    if (string == NULL || string == QString()) return result;
    bool OK;
    result = string.toInt(&OK, 10);
    if (!OK) result = -1;
    return result; 
}
QStringList MainWindow::trimList(QStringList list)
{
    QStringList result;
    foreach (const QString &str, list) {
        result += str.trimmed();
    }
    return result;
}
void MainWindow::setSSIDParameter(QStringList &parameters, bool insertSSID, QString SSID, bool keepParameter)
{
    if (insertSSID)
    {
        setParameter(parameters, QString("wpa-ssid"), SSID, keepParameter);
    }
    else 
    {
        removeParameter(parameters, QString("wpa-ssid=\\S*"));
    }
}
void MainWindow::setPasswordParameter(QStringList &parameters, bool insertPassword, QString password, bool keepParameter)
{    
    if (insertPassword)
    {
        if (cbHidden->checkState() == Qt::Unchecked) 
        {
            // if present remove hidden settings
            removeParameter(parameters, QString("wpa-ap-scan=1"));
            removeParameter(parameters, QString("wpa-scan-ssid=1"));
        }
        else
        {
            // replace with correct setting
            setParameter(parameters, QString("wpa-ap-scan"), QString("1"), keepParameter);
            setParameter(parameters, QString("wpa-scan-ssid"), QString("1"), keepParameter);
        }
        if (!password.isEmpty())
        {
            if (isWPASelected()) 
            {
                // remove WEP key
                removeParameter(parameters, QString("wpa-key-mgmt=NONE"));
                removeParameter(parameters, QString("wpa-wep-key0=\\S*"));
                // add WPA key
                setParameter(parameters, QString("wpa-psk"), password, keepParameter);
            }
            else
            {
                // remove WPA key
                removeParameter(parameters, QString("wpa-psk=\\S*"));
                removeParameter(parameters, QString("wpa-key-mgmt=NONE"));
                // add WPA key
                // setParameter(parameters, QString("wpa-key-mgmt"), QString("NONE"));
                setParameter(parameters, QString("wpa-wep-key0"), password, keepParameter);
            }
        }
        else
        {
            // remove password key
            removeParameter(parameters, QString("wpa-wep-key0=\\S*"));
            removeParameter(parameters, QString("wpa-psk=\\S*"));
            // add setting indocating there is no security
            setParameter(parameters, QString("wpa-key-mgmt"), QString("NONE"), keepParameter);
        }
    }
    else
    {
        removeParameter(parameters, QString("wpa-ap-scan=1"));
        removeParameter(parameters, QString("wpa-scan-ssid=1"));
        removeParameter(parameters, QString("wpa-wep-key0=\\S*"));
        removeParameter(parameters, QString("wpa-psk=\\S*"));
        removeParameter(parameters, QString("wpa-key-mgmt=NONE"));
    }
}
void MainWindow::setURLParameter(QStringList &parameters, bool replaceURL, QString newURL, bool keepParameter)
{
    QString currentURL = QString("http://playr.biz/2400/74");

    if (replaceURL)
    {
        setParameter(parameters, QString("homepage"), newURL, keepParameter);
    }
    else
    {
        setParameter(parameters, QString("homepage"), currentURL, keepParameter);
    }
}
void MainWindow::setCronParameter(QStringList &parameters, bool insertCron, int hours, int minutes, QString timeZone, bool keepParameter)
{
    if (insertCron)
    {
        setParameter(parameters, QString("timezone"), timeZone, keepParameter);
        setParameter(parameters, QString("cron"), cronString(hours, minutes), keepParameter);
    }
    else
    {
        // remove Cron parameters
        removeParameter(parameters, QString("timezone=\\S*"));
        removeParameter(parameters, QString("cron=\\S*"));
    }
}
QString MainWindow::cronString(int hours, int minutes)
{
    QString result = replaceSpace( (minutes >= 0 && minutes < 10 ? QString("0") : QString("")) 
                                      + QString::number(((minutes < 0) || (minutes > 59)) ? 00 : minutes) 
                                      + QString(" ") 
                                      + (hours >= 0 && hours < 10 ? QString("0") : QString(""))
                                      + QString::number(((hours < 0) || (hours > 23)) ? 00 : hours)
                                      + QString(" * * * root poweroff")
                      );
    return result;
}
void MainWindow::setResolutionParameter(QStringList &parameters, bool insertResolution, int resolution, bool keepParameter)
{
    if (insertResolution)
    {
        setParameter(parameters, QString("xrandr"), resolutionString(resolution), keepParameter);
    }
    else
    {
        // remove Cron parameters
        removeParameter(parameters, QString("xrandr=\\S*"));
    }
}
QString MainWindow::resolutionString(int resolution)
{
    QString result = QString("");
    if (resolution == 1 || resolution == 2)
    {
        result = QString("-s%20") + (resolution == 1 ? QString("1920x1080") : QString("1280x720"));

    }
    return result;
}
void MainWindow::on_bRead_clicked()
{
    if (!leFile->text().isEmpty())
    {
        myFile = leFile->text();
        QFileInfo fileinfo(myFile);
        if (fileinfo.path()=="."){
            myFile=(myHomeDir + "/" + leFile->text());
            QFileInfo fileinfo(myFile);
        }
        if (myFile.at(0) == cboxDevice->currentText().at(1))
        {
            QMessageBox::critical(NULL, tr("Write Error"), tr("Image file cannot be located on the target device."));
            return;
        }
        if (fileinfo.exists())
        {
            if (QMessageBox::warning(NULL, tr("Confirm Overwrite"), tr("Are you sure you want to overwrite the specified file?"),
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
        }
        bCancel->setEnabled(true);
        bWrite->setEnabled(false);
        bRead->setEnabled(false);
        status = STATUS_READING;
        double mbpersec;
        unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
        int volumeID = cboxDevice->currentText().at(1).toAscii() - 'A';
        int deviceID = cboxDevice->itemData(cboxDevice->currentIndex()).toInt();
        hVolume = getHandleOnVolume(volumeID, GENERIC_READ);
        if (hVolume == INVALID_HANDLE_VALUE)
        {
            setStatusIdleAndDisableCancel();
            return;
        }
        if (!getLockOnVolume(hVolume))
        {
            closeHandle(hVolume);
            setStatusIdleAndDisableCancel();
            return;
        }
        if (!unmountVolume(hVolume))
        {
            removeLockAndCloseHandle(hVolume);
            setStatusIdleAndDisableCancel();
            return;
        }
        hFile = getHandleOnFile(myFile.toAscii().data(), GENERIC_WRITE);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            removeLockAndCloseHandle(hVolume);
            setStatusIdleAndDisableCancel();
            return;
        }
        hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ);
        if (hRawDisk == INVALID_HANDLE_VALUE)
        {
            removeLockAndCloseHandle(hVolume);
            closeHandle(hFile);
            setStatusIdleAndDisableCancel();
            return;
        }
        numsectors = getNumberOfSectors(hRawDisk, &sectorsize);
        filesize = getFileSizeInSectors(hFile, sectorsize);
        if (filesize >= numsectors)
        {
            spaceneeded = 0ull;
        }
        else
        {
            spaceneeded = (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
        }
        if (!spaceAvailable(myFile.left(3).replace(QChar('/'), QChar('\\')).toAscii().data(), spaceneeded))
        {
            QMessageBox::critical(NULL, tr("Write Error"), tr("Disk is not large enough for the specified image."));
            removeLockAndCloseHandle(hVolume);
            closeHandles(hFile, hRawDisk);
            setStatusIdleAndDisableCancel();
            sectorData = NULL;
            return;
        }
        if (numsectors == 0ul)
        {
            progressbar->setRange(0, 100);
        }
        else
        {
            progressbar->setRange(0, (int)numsectors);
        }
        lasti = 0ul;
        timer.start();
        for (i = 0ul; i < numsectors && status == STATUS_READING; i += 1024ul)
        {
            sectorData = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
            if (sectorData == NULL)
            {
                deleteData();
                removeLockAndCloseHandle(hVolume);
                closeHandles(hFile, hRawDisk);
                setStatusIdleAndDisableCancel();
                return;
            }
            if (!writeSectorDataToHandle(hFile, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
            {
                deleteData();
                removeLockAndCloseHandle(hVolume);
                closeHandles(hFile, hRawDisk);
                setStatusIdleAndDisableCancel();
                return;
            }
            delete sectorData;
            sectorData = NULL;
            if (timer.elapsed() >= 1000)
            {
                mbpersec = (((double)sectorsize * (i - lasti)) * (1000.0 / timer.elapsed())) / 1024.0 / 1024.0;
                statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                timer.start();
                lasti = i;
            }
            progressbar->setValue(i);
            QCoreApplication::processEvents();
        }
        removeLockAndCloseVolumeHandle();
        closeHandles(hFile, hRawDisk);
        sectorData = NULL;
        progressbar->reset();
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (status == STATUS_CANCELED){
            QMessageBox::information(NULL, tr("Complete"), tr("Read Canceled."));
        } else {
            QMessageBox::information(NULL, tr("Complete"), tr("Read Successful."));

        }
        if(md5CheckBox->isChecked())
        {
            QFileInfo fileinfo(myFile);
            if (fileinfo.exists() && fileinfo.isFile() &&
                    fileinfo.isReadable() && (fileinfo.size() > 0) )
            {
                generateMd5(myFile.toLatin1().data());
            }
        }
    }
    else
    {
        QMessageBox::critical(NULL, tr("File Info"), tr("Please specify a file to save data to."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
}

// getLogicalDrives sets cBoxDevice with any logical drives found, as long
// as they indicate that they're either removable, or fixed and on USB bus
void MainWindow::getLogicalDrives()
{
    // GetLogicalDrives returns 0 on failure, or a bitmask representing
    // the drives available on the system (bit 0 = A:, bit 1 = B:, etc)
    unsigned long driveMask = GetLogicalDrives();
    int i = 0;
    ULONG pID;

    cboxDevice->clear();

    while (driveMask != 0)
    {
        if (driveMask & 1)
        {
            // the "A" in drivename will get incremented by the # of bits
            // we've shifted
            char drivename[] = "\\\\.\\A:\\";
            drivename[4] += i;
            if (checkDriveType(drivename, &pID))
            {
                cboxDevice->addItem(QString("[%1:\\]").arg(drivename[4]), (qulonglong)pID);
            }
        }
        driveMask >>= 1;
        cboxDevice->setCurrentIndex(0);
        ++i;
    }
}

// support routine for winEvent - returns the drive letter for a given mask
//   taken from http://support.microsoft.com/kb/163503
char FirstDriveFromMask (ULONG unitmask)
{
    char i;

    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        {
            break;
        }
        unitmask = unitmask >> 1;
    }

    return (i + 'A');
}

// register to receive notifications when USB devices are inserted or removed
// adapted from http://www.known-issues.net/qt/qt-detect-event-windows.html
bool MainWindow::winEvent ( MSG * msg, long * result )
{
    if(msg->message == WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    // add device to combo box (after sanity check that
                    // it's not already there, which it shouldn't be)
                    QString qs = QString("[%1:\\]").arg(ALET);
                    if (cboxDevice->findText(qs) == -1)
                    {
                        ULONG pID;
                        char longname[] = "\\\\.\\A:\\";
                        longname[4] = ALET;
                        // checkDriveType gets the physicalID
                        if (checkDriveType(longname, &pID))
                        {
                            cboxDevice->addItem(qs, (qulonglong)pID);
                            setReadWriteButtonState();
                        }
                    }
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(DBTF_NET)
                {
                    char ALET = FirstDriveFromMask(lpdbv->dbcv_unitmask);
                    //  find the device that was removed in the combo box,
                    //  and remove it from there....
                    //  "removeItem" ignores the request if the index is
                    //  out of range, and findText returns -1 if the item isn't found.
                    cboxDevice->removeItem(cboxDevice->findText(QString("[%1:\\]").arg(ALET)));
                    setReadWriteButtonState();
                }
            }
            break;
        } // skip the rest
    } // end of if msg->message
    *result = 0; //get rid of obnoxious compiler warning
    return false; // let qt handle the rest
}
