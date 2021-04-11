/////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  -----------------------------
//  SphereVertices.cpp
//  -----------------------------
//  
//  A unit with a standardised sphere for rendering
//
/////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "SphereVertices.h"

#define N_SPHERE_VERTICES 84

float sphereVert[N_SPHERE_VERTICES][3] = 
	{
		{   0,  0,  -1  },  
		{   0.5,    0,  -0.86603    },  
		{   0.86603,    0,  -0.5    },  
		{   1,  0,  0   },  
		{   0.86603,    0,  0.5 },  
		{   0.5,    0,  0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0.43301,    0.25,   -0.86603    },  
		{   0.75,   0.43301,    -0.5    },  
		{   0.86603,    0.5,    0   },  
		{   0.75,   0.43301,    0.5 },  
		{   0.43301,    0.25,   0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0.25,   0.43301,    -0.86603    },  
		{   0.43301,    0.75,   -0.5    },  
		{   0.5,    0.86603,    0   },  
		{   0.43301,    0.75,   0.5 },  
		{   0.25,   0.43301,    0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0,  0.5,    -0.86603    },  
		{   0,  0.86603,    -0.5    },  
		{   0,  1,  0   },  
		{   0,  0.86603,    0.5 },  
		{   0,  0.5,    0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   -0.25,  0.43301,    -0.86603    },  
		{   -0.43301,   0.75,   -0.5    },  
		{   -0.5,   0.86603,    0   },  
		{   -0.43301,   0.75,   0.5 },  
		{   -0.25,  0.43301,    0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   -0.43301,   0.25,   -0.86603    },  
		{   -0.75,  0.43301,    -0.5    },  
		{   -0.86603,   0.5,    0   },  
		{   -0.75,  0.43301,    0.5 },  
		{   -0.43301,   0.25,   0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   -0.5,   0,  -0.86603    },  
		{   -0.86603,   0,  -0.5    },  
		{   -1, 0,  0   },  
		{   -0.86603,   0,  0.5 },  
		{   -0.5,   0,  0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   -0.43301,   -0.25,  -0.86603    },  
		{   -0.75,  -0.43301,   -0.5    },  
		{   -0.86603,   -0.5,   0   },  
		{   -0.75,  -0.43301,   0.5 },  
		{   -0.43301,   -0.25,  0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   -0.25,  -0.43301,   -0.86603    },  
		{   -0.43301,   -0.75,  -0.5    },  
		{   -0.5,   -0.86603,   0   },  
		{   -0.43301,   -0.75,  0.5 },  
		{   -0.25,  -0.43301,   0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0,  -0.5,   -0.86603    },  
		{   0,  -0.86603,   -0.5    },  
		{   0,  -1, 0   },  
		{   0,  -0.86603,   0.5 },  
		{   0,  -0.5,   0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0.25,   -0.43301,   -0.86603    },  
		{   0.43301,    -0.75,  -0.5    },  
		{   0.5,    -0.86603,   0   },  
		{   0.43301,    -0.75,  0.5 },  
		{   0.25,   -0.43301,   0.86603 },  
		{   0,  0,  1   },  
		{   0,  0,  -1  },  
		{   0.43301,    -0.25,  -0.86603    },  
		{   0.75,   -0.43301,   -0.5    },  
		{   0.86603,    -0.5,   0   },  
		{   0.75,   -0.43301,   0.5 },  
		{   0.43301,    -0.25,  0.86603 },  
		{   0,  0,  1   }   
    };
    
// three routines to render a sphere
void renderWireframeSphereOutline()
	{ // renderWireframeSphereOutline()
    // draw a circle around the edge of the sphere
    // reusing data from some of the elements of the sphere
//     glScalef(0.8, 0.8, 0.8);
    glBegin(GL_LINE_LOOP);
    for (int j = 3; j < 84; j += 7)
        glVertex3f(sphereVert[j][0], sphereVert[j][1], 0.5);
    glEnd();
	} // renderWireframeSphereOutline()

void renderWireframeSphere()
	{ // renderWireframeSphere()
    // loop through verticals of sphere
    for (int i = 0; i < 12; i++)
        { // loop drawing verticals
        glBegin(GL_LINE_STRIP);
        for (int j = i*7; j < 7+i*7; j++)
            glVertex3fv(sphereVert[j]);
        glEnd();        
        } // loop drawing verticals
    // and loop through horizontals
    for (int i = 1; i < 6; i++)
        { // loop for horizontals
        glBegin(GL_LINE_LOOP);
        for (int j = i; j < 84; j += 7)
            glVertex3fv(sphereVert[j]);
        glEnd();
        } // loop for horizontals   
	} // renderWireframeSphere()
	
void renderTriangulatedSphere()
	{ // renderTriangulatedSphere()
	glBegin(GL_TRIANGLES);
	// start with the triangles at the N Pole
	for (int segment = 0; segment < 12; segment++)
		{ // per segment
		int nextSegment = (segment+1) % 12;
		// start with a single triangle at the beginning
		glVertex3fv(sphereVert[7*segment+0]);
		glVertex3fv(sphereVert[7*segment+1]);
		glVertex3fv(sphereVert[7*nextSegment+1]);

		// now loop down the next few, drawing quads
		for (int layer = 1; layer < 6; layer++)
			{ // per layer
			glVertex3fv(sphereVert[7*segment+layer]);
			glVertex3fv(sphereVert[7*segment+layer+1]);
			glVertex3fv(sphereVert[7*nextSegment+layer+1]);
			glVertex3fv(sphereVert[7*segment+layer]);
			glVertex3fv(sphereVert[7*nextSegment+layer+1]);
			glVertex3fv(sphereVert[7*nextSegment+layer]);
			} // per layer

		// and now a triangle at the bottom
		// start with a single triangle at the beginning
		glVertex3fv(sphereVert[7*segment+5]);
		glVertex3fv(sphereVert[7*segment+6]);
		glVertex3fv(sphereVert[7*nextSegment+5]);
		
		} // per segment

	glEnd();	
	} // renderTriangulatedSphere()