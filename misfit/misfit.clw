; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMisfitDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "misfit.h"

ClassCount=4
Class1=CMisfitApp
Class2=CMisfitDlg
Class3=CAboutDlg

ResourceCount=10
Resource1=IDD_QITFIT_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_MISFIT_DIALOG
Resource4=IDD_FREQ_DLG
Resource5=IDD_TRAP_EDIT
Resource6=IDD_BASIC_PROP
Resource7=IDD_TRAP_PROP
Resource8=IDD_DATA_PROP
Resource9=IDD_ABOUTBOX
Resource10=IDD_PLOT_PROP

[CLS:CMisfitApp]
Type=0
HeaderFile=misfit.h
ImplementationFile=misfit.cpp
Filter=N

[CLS:CMisfitDlg]
Type=0
HeaderFile=misfitDlg.h
ImplementationFile=misfitDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=misfitDlg.h
ImplementationFile=misfitDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MISFIT_DIALOG]
Type=1
Class=CMisfitDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_QITFIT_DIALOG]
Type=1
Class=?
ControlCount=38
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COX,edit,1350631554
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EOT,edit,1350633602
Control6=IDC_STATIC,button,1342177287
Control7=IDC_SUBTYPE,button,1342373897
Control8=IDC_RADIO3,button,1342242825
Control9=IDC_STATIC,static,1342308353
Control10=IDC_DOPING,edit,1350631554
Control11=IDC_STATIC,button,1342177287
Control12=IDC_QUANT,button,1342242819
Control13=IDC_EXACT,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_PLOTY_COMBO,combobox,1344339971
Control16=IDC_STATIC,static,1342308352
Control17=IDC_PLOTX_COMBO,combobox,1344339971
Control18=IDC_STATIC,button,1342177287
Control19=IDC_TRAP_LIST,listbox,1353777409
Control20=IDC_ADD_TRAP,button,1342242816
Control21=IDC_DEL_TRAP,button,1342242816
Control22=IDC_EDIT_TRAP,button,1342242816
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,static,1342308352
Control25=IDC_GS,edit,1350631554
Control26=IDC_STATIC,static,1342308352
Control27=IDC_GP,edit,1350631554
Control28=IDC_STATIC,static,1342308352
Control29=IDC_VFB,edit,1350631554
Control30=IDC_STATIC,button,1342177287
Control31=IDC_DATA_LIST,listbox,1353793793
Control32=IDC_LOAD,button,1342373888
Control33=IDC_UNLOAD,button,1342373888
Control34=IDC_SAVE,button,1342242816
Control35=IDC_SAVE2,button,1342242816
Control36=ID_CALC,button,1342242817
Control37=IDC_STATIC,static,1342308352
Control38=IDC_TRAP_SHIFT,edit,1350633602

[DLG:IDD_FREQ_DLG]
Type=1
Class=?
ControlCount=4
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_TRAP_EDIT]
Type=1
Class=?
ControlCount=27
Control1=IDC_TRAP_MODE,combobox,1344339971
Control2=IDC_TRAPTYPE,button,1342308361
Control3=IDC_RADIO4,button,1342177289
Control4=IDC_STATIC,static,1342308352
Control5=IDC_DIT,edit,1350631554
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SS,edit,1350631554
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_E0,edit,1350631554
Control11=IDC_STATIC,static,1342308352
Control12=IDC_DE,edit,1350631554
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_ET,edit,1350631554
Control16=IDC_ET2,edit,1350631554
Control17=IDC_ET3,edit,1350631554
Control18=IDC_ET4,edit,1350631554
Control19=IDC_ET5,edit,1350631554
Control20=IDC_STATIC,static,1342308352
Control21=IDC_DITM,edit,1350631554
Control22=IDC_DITM2,edit,1350631554
Control23=IDC_DITM3,edit,1350631554
Control24=IDC_DITM4,edit,1350631554
Control25=IDC_DITM5,edit,1350631554
Control26=IDOK,button,1342242817
Control27=IDCANCEL,button,1342242816

[DLG:IDD_BASIC_PROP]
Type=1
Class=?
ControlCount=22
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COX,edit,1350631554
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EOT,edit,1350633602
Control6=IDC_STATIC,button,1342177287
Control7=IDC_COMBO_SUB,combobox,1344339971
Control8=IDC_QUANT,button,1342242819
Control9=IDC_SUBTYPE,button,1342373897
Control10=IDC_RADIO3,button,1342242825
Control11=IDC_STATIC,static,1342308353
Control12=IDC_DOPING,edit,1350631554
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308353
Control15=IDC_GS,edit,1350631554
Control16=IDC_STATIC,static,1342308353
Control17=IDC_GP,edit,1350631554
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308352
Control20=IDC_VFB,edit,1350631554
Control21=IDC_STATIC,static,1342308352
Control22=IDC_TRAP_SHIFT,edit,1350633602

[DLG:IDD_TRAP_PROP]
Type=1
Class=?
ControlCount=4
Control1=IDC_TRAP_LIST,listbox,1353777409
Control2=IDC_ADD_TRAP,button,1342242816
Control3=IDC_DEL_TRAP,button,1342242816
Control4=IDC_EDIT_TRAP,button,1342242816

[DLG:IDD_DATA_PROP]
Type=1
Class=?
ControlCount=5
Control1=IDC_LOAD,button,1342373888
Control2=IDC_UNLOAD,button,1342373888
Control3=IDC_DATA_LIST,listbox,1353777409
Control4=IDC_NEW_FREQ,button,1342373888
Control5=IDC_SAVE,button,1342373888

[DLG:IDD_PLOT_PROP]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PLOTY_COMBO,combobox,1344339971
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PLOTX_COMBO,combobox,1344339971
Control5=IDC_CORR,button,1342242851

