//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  -----------------------------
//  Render Widget
//  -----------------------------
//  
//  Since the render code is in the geometric object class
//  this widget primarily sets up the transformation matrices and 
//  lighting.
//
//  It implements the UI for an arcball controller, but in an
//  abstractable way that allows it to share an arcball with another
//  widget (the arcball controller) - thus you can manipulate the
//  object either directly in the widget or indirectly through the
//  arcball displayed visually.
//  
//  It also supports translation by visual dragging.  This results in
//  needing three mouse buttons, and not all systems have them, so it
//  will be up to the user to make sure that they can use it on their
//  own machine (especially on OSX)
//
//  Since the controls are (potentially) shared with other widgets, 
//  this widget is only responsible for scaling the x,y of mouse events
//  then passing them to the controller
//  
////////////////////////////////////////////////////////////////////////

// include guard
#ifndef _RENDER_WIDGET_H
#define _RENDER_WIDGET_H

// include the relevant QT headers
#include <QOpenGLWidget>
#include <QMouseEvent>

// and include all of our own headers that we need
#include "DirectedEdgeSurface.h"
#include "RenderParameters.h"

// class for a render widget with arcball linked to an external arcball widget
class RenderWidget : public QOpenGLWidget                                       
    { // class RenderWidget
    Q_OBJECT
    private:    
    // the geometric object to be rendered
    DirectedEdgeSurface *directedEdgeSurface; 

    // the render parameters to use
    RenderParameters *renderParameters;

    public:
    // constructor
    RenderWidget
            (
            // the geometric object to show
            DirectedEdgeSurface *newDirectedEdgeSurface,
            // the render parameters to use
            RenderParameters    *newRenderParameters,
            // parent widget in visual hierarchy
            QWidget             *parent
            );
    
    // destructor
    ~RenderWidget();
            
    protected:
    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();

	public:
	// routine that does actual rendering
	void render();
	
    // mouse-handling
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    // these signals are needed to support shared arcball control
    public:
    signals:
	// pass through signal to send mouse events to controller
    void MouseDown(int whichButton, int x, int y);
    // note that Continue & End assume the button has already been set
    void MouseDrag(int x, int y);
    void MouseUp(int x, int y);
    
    // tells the controller that the pick image is ready
    void PickImageGenerated();
    
    }; // class RenderWidget

#endif
