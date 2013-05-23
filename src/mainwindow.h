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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include "ui_mainwindow.h"
#include "disk.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget *parent=0);
		~MainWindow();
		void closeEvent(QCloseEvent *event);
        enum Status {STATUS_IDLE=0, STATUS_READING, STATUS_WRITING, STATUS_EXIT, STATUS_CANCELED};
		bool winEvent ( MSG * msg, long * result );
	protected slots:
		void on_tbBrowse_clicked();
		void on_bCancel_clicked();
		void on_bWrite_clicked();
		void on_bRead_clicked();
        void on_leFile_textChanged();
		void on_leFile_editingFinished();
		void on_md5CheckBox_stateChanged();
		void on_cbWEP_stateChanged();
		void on_cbHidden_stateChanged();
		void on_leSSID_textChanged();
		void on_lePassword_textChanged();
		void on_leTarget_textChanged();
	private:
		// find attached devices
		void getLogicalDrives();
		void setReadWriteButtonState();

		void disableWriteAndReadButtons();
		void disableCancelButton();
		void setStatusIdleAndDisableCancel();
		void closeHandle(HANDLE handle);
		void closeHandles(HANDLE handle1, HANDLE handle2);
		void removeLockAndCloseHandle(HANDLE handle2);
		void closeVolumeHandle();
		void removeLockAndCloseVolumeHandle();
		void idleStatusAndCloseVolumeHandle();
		void idleStatusRemoveLockAndCloseVolumeHandle();
		void closeFileHandle();
		void closeRawDiskHandle();
		void deleteData();
		void deleteDataIdleStatusRemoveLockAndCloseVolumeHandles();
		bool needsInsertion(QString leValue, QString &value);
		bool needsURLInsertion(QString leValue, QString &value);
		bool configurationShouldBeWritten();
		bool writeOSConfiguration(char *ltr);
		bool updateConfigurationFile(QString configFileName);
		void setParameter(QStringList &parameters, QString key, QString value);
		QStringList trimList(QStringList list);
		void removeParameter(QStringList &parameters, QString keyAndValue);
		QString setParameters(QString line, bool insertSSID, QString SSID, bool insertPassword, QString password, bool replaceURL, QString newURL);
		void setSSIDParameter(QStringList &parameters, bool insertSSID, QString SSID);
		void setPasswordParameter(QStringList &parameters, bool insertPassword, QString password);
		void setURLParameter(QStringList &parameters, bool replaceURL, QString newURL);

		HANDLE hVolume;
		HANDLE hFile;
		HANDLE hRawDisk;
		unsigned long long sectorsize;
		int status;
		char *sectorData;
		QTime timer;
		void generateMd5(char *filename);
        QString myFile;
        QString myHomeDir;
};

#endif // MAINWINDOW_H
