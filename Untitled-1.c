#include <X11/IntrinsicP.h> // IntrinsicP.h has some faster macros than Intrinsic.h
#include <X11/ShellP.h> // Needed for Widget class definitions
#include <stdio.h> //fprintf, stderr
#include <stdlib.h> // exit


XtAppContext app; 
Widget main_widget; //Main Window
    //------------------------------------------------
    // This function is called by a ButtonPress event in the main widget
    // Button 1 is the left mouse button, Button 3 is the right button,
    // Button 2 is the middle button (or the left & right button pressed together)
    // Button 3 quits the program, Buttons 2 & 3 change the main widget's size
void EvProc(Widget w, XtPointer client_data, XEvent *event, char* CTmp){ 
	if (event->xbutton.button == Button1) 
	{ 
		fprintf(stderr, "Button 1 Pressed\n"); 
		XtMakeResizeRequest(main_widget, 200, 600, NULL, NULL); 
	} 
	if (event->xbutton.button == Button2) 
	{ 
		fprintf(stderr, "Button 2 Pressed\n"); 
		XtMakeResizeRequest(main_widget, 400, 100, NULL, NULL); 
	} 
	if (event->xbutton.button == Button3)
	{ 
		fprintf(stderr, "Button 3 Pressed\n");
		exit(0); 
	}
}

//------------------------------------------------

void DrawHellowString (Widget prWidget, XtPointer pData,
	XEvent  *prEvent, Boolean  *pbContinue)

{
    Display	*prDisplay = XtDisplay (prWidget);
    Window	nWindow = XtWindow (prWidget);
    GC		prGC;

    if (prEvent->type == Expose)
    {
       prGC = XCreateGC (prDisplay, nWindow, 0, NULL);
       XDrawString (prDisplay, nWindow, prGC, 10, 50,
	"Hello, world!", strlen ("Hello, world!") );
       XFreeGC (prDisplay, prGC);
    }
}

//------------------------------------------------
char str[64];
int counter=0;
int main (int argc, char **argv){ 
	
	
    short unsigned int xx,yy;
    //--- Create and initialize the top-level widget 
    main_widget = XtOpenApplication(&app, "My Application", NULL, 0, &argc, argv, NULL, applicationShellWidgetClass, NULL, 0);
    
    //--- Make it the desired size 
    XtMakeResizeRequest(main_widget, 300, 100, &xx, &yy);
    
    //--- Realize the main widget 
    XtRealizeWidget(main_widget);
    
	//--- Add Mouse Button Event Handler
    XtAddEventHandler(main_widget, ButtonPressMask, FALSE, EvProc, NULL);
    XtAddEventHandler(main_widget, ExposureMask, FALSE, DrawHellowString, NULL);
    
    //----- 
    XtAppMainLoop(app);
}
