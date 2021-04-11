/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  -----------------------------
//  Render Controller
//  -----------------------------
//  
//  We're using the Model-View-Controller pattern
//  so most of the control logic goes here
//  which means we need a slot for substantially
//  every possible UI manipulation
//
/////////////////////////////////////////////////////////////////

// include guard
#ifndef _RENDER_CONTROLLER_H
#define _RENDER_CONTROLLER_H

// QT headers
#include <QtGui>
#include <QMouseEvent>

// Local headers
#include "RenderWindow.h"
#include "DirectedEdgeSurface.h"
#include "RenderParameters.h"

// class for the render controller
class RenderController : public QObject
    { // class RenderController
    Q_OBJECT
    private:
    // the geometric object to be rendered
    DirectedEdgeSurface *directedEdgeSurface; 

    // the render parameters to use
    RenderParameters *renderParameters;
    
    // the render window with all of the controls
    RenderWindow *renderWindow;
    
    // local variable for tracking mouse-drag in shared widgets
    int dragButton;
    
    // local variable for x,y coordinates of initial position
    int startx, starty;
    int vertexID;
    
    int32_t currentSelected = -1;

    public:
    // constructor
    RenderController
        (
        // the geometric object to show
        DirectedEdgeSurface *newDirectedEdgeSurface,
        // the render parameters to use
        RenderParameters    *newRenderParameters,
        // the render window that it controls
        RenderWindow        *newRenderWindow
        );
    
    public slots:
    // slot for responding to arcball rotation for object
    void objectRotationChanged();
    void lightRotationChanged();

    // slots for responding to zoom & translate sliders
    void zoomChanged(int value);
    void xTranslateChanged(int value);
    void yTranslateChanged(int value);
    
    // slot for responding to vertex size slider
    void vertexSizeChanged(int value);
    
    // slots for responding to check boxes
    void showVerticesCheckChanged(int state);
    void flatNormalsCheckChanged(int state);

    void btnClick();
    void  saveFile();

    void cellSelected(QListWidgetItem * item);

    // slots for responding to widget manipulations
    void MouseDown(int whichButton, int x, int y);
    // note that Continue & End assume the button has already been set
    void MouseDrag(int x, int y);
    void MouseUp(int x, int y);




    }; // class RenderController

// end of include guard
#endif
