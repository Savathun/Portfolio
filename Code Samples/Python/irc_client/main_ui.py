# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'main.ui'
##
## Created by: Qt User Interface Compiler version 5.14.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import (QCoreApplication, QMetaObject, QObject, QPoint,
    QRect, QSize, QUrl, Qt)
from PySide2.QtGui import (QBrush, QColor, QConicalGradient, QFont,
    QFontDatabase, QIcon, QLinearGradient, QPalette, QPainter, QPixmap,
    QRadialGradient)
from PySide2.QtWidgets import *

class MainUI(object):
    def setupUi(self, MainUI):
        if MainUI.objectName():
            MainUI.setObjectName(u"MainUI")
        MainUI.setEnabled(True)
        MainUI.resize(1051, 658)
        self.centralwidget = QWidget(MainUI)
        self.centralwidget.setObjectName(u"centralwidget")
        self.submit_button = QPushButton(self.centralwidget)
        self.submit_button.setObjectName(u"submit_button")
        self.submit_button.setGeometry(QRect(764, 578, 103, 31))
        self.chat_listview = QListWidget(self.centralwidget)
        self.chat_listview.setObjectName(u"chat_listview")
        self.chat_listview.setGeometry(QRect(20, 20, 731, 521))
        self.channel_listview = QListWidget(self.centralwidget)
        self.channel_listview.setObjectName(u"channel_listview")
        self.channel_listview.setGeometry(QRect(770, 20, 256, 521))
        self.to_textedit = QLineEdit(self.centralwidget)
        self.to_textedit.setObjectName(u"to_textedit")
        self.to_textedit.setGeometry(QRect(20, 550, 131, 21))
        self.message_textedit = QLineEdit(self.centralwidget)
        self.message_textedit.setObjectName(u"message_textedit")
        self.message_textedit.setGeometry(QRect(20, 580, 741, 21))
        MainUI.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainUI)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1051, 22))
        self.menubar.setNativeMenuBar(True)
        MainUI.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainUI)
        self.statusbar.setObjectName(u"statusbar")
        MainUI.setStatusBar(self.statusbar)

        self.retranslateUi(MainUI)

        QMetaObject.connectSlotsByName(MainUI)
    # setupUi

    def retranslateUi(self, MainUI):
        MainUI.setWindowTitle(QCoreApplication.translate("MainUI", u"PyRC", None))
        self.submit_button.setText(QCoreApplication.translate("MainUI", u"Submit", None))
    # retranslateUi

