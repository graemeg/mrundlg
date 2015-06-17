/*
	mrundlg - A application / program launcher for Motif/X
	Copyright (C) 2005   Matthew Holder (sixxgate@hotmail.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <X11/Shell.h>
#include <X11/Xmu/Editres.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringhelp.h"

/* Public Variables */
static unsigned char	bTermStart = XmUNSET;	/* Start the command in a terminal if true */
static String		szCommand = NULL;	/* The command to execute */

/* Callbacks */
void btnCancel_Click();
void btnOK_Click();
void chkTerminal_Change();
void txtCommand_Change();

/* Actions */
void CancelAP(Widget, XEvent*, String*, Cardinal*);
void OKAP(Widget, XEvent*, String*, Cardinal*);

/* Fallback resources */
static char *fallbackResources[] = {
	"*XmForm*translations: #override\\n"
		"<KeyPress>Return: OK()\\n"
		"<KeyPress>osfCancel: Cancel()\\n",
};

XtActionsRec NewActions[] = {
    {"OK",	OKAP},
    {"Cancel",	CancelAP}
};


int main(int argc, char *argv[])
{
	Widget dlgRun, frmLayout, lblMessage;
	Widget txtCommand, chkTerminal, btnOK, btnCancel;
	XtAppContext xtAppContext;
	Arg args[10]; Cardinal n;
	XmString szGeneric;
	char szTitle[] = "Run\0";
	char szMessage[] = "Type the name of a program that\nFreeBSD will open for you.\0";
	
	dlgRun = XtVaOpenApplication(&xtAppContext, "MRunDlg",
				     (XrmOptionDescList)NULL, 0,
				     &argc, argv, fallbackResources,
				     applicationShellWidgetClass,
				     XmNmwmDecorations, MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MENU,
				     XmNmwmFunctions, MWM_FUNC_MOVE | MWM_FUNC_CLOSE,
				     XmNtitle, szTitle,
				     XmNallowShellResize, False,
				     NULL);
	
	/* EditRes Support */
	XtAddEventHandler(dlgRun, (EventMask) 0, True,
			  (XtEventHandler) _XEditResCheckMessages, NULL);

	n = 0;
	XtSetArg(args[n], XmNmarginHeight, 7); n++;
	XtSetArg(args[n], XmNmarginWidth, 7); n++;
	XtSetArg(args[n], XmNhorizontalSpacing, 3); n++;
	XtSetArg(args[n], XmNverticalSpacing, 3); n++;
	frmLayout = XmCreateForm(dlgRun, "frmLayout", args, n);
	
	n = 0;
	szGeneric = XmStringCreateLocalized(szMessage);
	XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
	XtSetArg(args[n], XmNlabelString, szGeneric); n++;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	lblMessage = XmCreateLabel(frmLayout, "lblMessage", args, n);
	XmStringFree(szGeneric);

	n = 0;
	XtSetArg(args[n], XmNcolumns, 40); n++;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNtopWidget, lblMessage); n++;
	XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	txtCommand = XmCreateTextField(frmLayout, "txtCommand", args, n);
	
	n = 0;
	szGeneric = XmStringCreateLocalized("Start in Terminal");
	XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
	XtSetArg(args[n], XmNlabelString, szGeneric); n++;
	XtSetArg(args[n], XmNindicatorType, XmN_OF_MANY); n++;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNtopWidget, txtCommand); n++;
	XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	chkTerminal = XmCreateToggleButton(frmLayout, "chkTerminal", args, n);
	XmStringFree(szGeneric);
	
	n = 0;
	szGeneric = XmStringCreateLocalized("Cancel");
	XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
	XtSetArg(args[n], XmNlabelString, szGeneric); n++;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNtopWidget, chkTerminal); n++;
	XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNdefaultButtonShadowThickness, 1); n++;
	btnCancel = XmCreatePushButton(frmLayout, "btnCancel", args, n);

	n = 0;
	szGeneric = XmStringCreateLocalized("OK");
	XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
	XtSetArg(args[n], XmNlabelString, szGeneric); n++;
	XtSetArg(args[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNtopWidget, chkTerminal); n++;
	XtSetArg(args[n], XmNrightAttachment, XmATTACH_WIDGET); n++;
	XtSetArg(args[n], XmNrightWidget, btnCancel); n++;
	XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg(args[n], XmNdefaultButtonShadowThickness, 1); n++;
	XtSetArg(args[n], XmNshowAsDefault, True); n++;
	btnOK = XmCreatePushButton(frmLayout, "btnOK", args, n);
	
	XtAddCallback(btnOK, XmNactivateCallback, btnOK_Click, NULL);
	XtAddCallback(btnCancel, XmNactivateCallback, btnCancel_Click, NULL);
	XtAddCallback(txtCommand, XmNactivateCallback, btnOK_Click, NULL);

	XtAddCallback(chkTerminal, XmNvalueChangedCallback, chkTerminal_Change, NULL);
	XtAddCallback(txtCommand, XmNvalueChangedCallback, txtCommand_Change, NULL);

	XtAppAddActions(xtAppContext, NewActions, XtNumber(NewActions));

	XtManageChild(btnOK);
	XtManageChild(btnCancel);
	XtManageChild(chkTerminal);
	XtManageChild(txtCommand);
	XtManageChild(lblMessage);
	XtManageChild(frmLayout);
	
	XtRealizeWidget(dlgRun);

	XmProcessTraversal(txtCommand, XmTRAVERSE_CURRENT);

	XtAppMainLoop(xtAppContext);

	return FALSE;
}

void CleanUp ()
{
	if (szCommand != NULL) XtFree(szCommand);
}

void OKProc() {
	char *szShellCommand;
	char *szTrimCommand;

	/* Make szShellCommand big enough for everything */
	szShellCommand = malloc(32 + strlen(szTrimCommand));
	*szShellCommand = 0;
	if (szCommand != NULL)
	{
		/* Trim the command of spaces */
		szTrimCommand = malloc(strlen(szCommand) + 1);
		strtrim(szTrimCommand, szCommand);
		
		/* See if we are starting in a terminal */
		if (bTermStart ==  XmSET)
		{
			if (getenv("XTERM") != NULL)
			{
				strcat(szShellCommand, "$XTERM -e ");
			} else {
				strcat(szShellCommand, "xterm -e ");
			}
		}		
		
		/* See if we are going to a web site */
		if
		(
			(memcmp("http://", szTrimCommand, 7) == 0) ||
			(memcmp("ftp://", szTrimCommand, 7) == 0) ||
			(memcmp("www.", szTrimCommand, 4) == 0) ||
			(memcmp("ftp.", szTrimCommand, 4) == 0)
		)
		{
			if (getenv("BROWSER") != NULL)
			{
				strcat(szShellCommand, "$BROWSER ");
			} else {
				strcat(szShellCommand, "mozilla ");
			}
		}
		
		/* Tack on the typed in command and a background operator */
		strcat(szShellCommand, szTrimCommand);
		strcat(szShellCommand, " &");

		/* Run the command throught a shell and then free all memory */
		system (szShellCommand);
		free(szTrimCommand);
		free(szShellCommand);
	}
}

void btnCancel_Click (Widget w, XtPointer client_data, XmAnyCallbackStruct *call)
{
	CleanUp();
	XtCloseDisplay(XtDisplay(w));
	exit(0);
}

void btnOK_Click (Widget w, XtPointer client_data, XmAnyCallbackStruct *call)
{
	OKProc();
	CleanUp();
	XtCloseDisplay(XtDisplay(w));
	exit(0);
}

void chkTerminal_Change (Widget w, XtPointer client_data, XmAnyCallbackStruct *call)
{
	Arg args; unsigned char bValue;
	XtSetArg(args, XmNset, &bValue);
	XtGetValues(w, &args, 1);
	bTermStart = bValue;
}

void txtCommand_Change (Widget w, XtPointer client_data, XmAnyCallbackStruct *call)
{
	Arg args; String szValue;
	if (szCommand != NULL) XtFree(szCommand);
	XtSetArg(args, XmNvalue, &szValue);
	XtGetValues(w, &args, 1);
	szCommand = szValue;
}

void CancelAP(Widget w, XEvent *event, String *args, Cardinal *nArgs)
{
	CleanUp();
	XtCloseDisplay(XtDisplay(w));
	exit(0);
}

void OKAP(Widget w, XEvent *event, String *args, Cardinal *nArgs)
{
	OKProc();
	CleanUp();
	XtCloseDisplay(XtDisplay(w));
	exit(0);
}
