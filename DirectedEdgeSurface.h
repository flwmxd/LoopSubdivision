///////////////////////////////////////////////////
//
//  Hamish Carr
//  September, 2020
//
//  ------------------------
//  DirectedEdgeSurface.h
//  ------------------------
//  
//  Base code for rendering assignments.
//
//  Minimalist (non-optimised) code for reading and 
//  rendering an object file
//  
//  We will make some hard assumptions about input file
//  quality. We will not check for manifoldness or 
//  normal direction, &c.  And if it doesn't work on 
//  all object files, that's fine.
//
//  While I could set it up to use QImage for textures,
//  I want this code to be reusable without Qt, so I 
//  shall make a hard assumption that textures are in 
//  ASCII PPM and use my own code to read them
//  
///////////////////////////////////////////////////

// include guard for DirectedEdgeSurface
#ifndef _DIRECTED_EDGE_SURFACE_H
#define _DIRECTED_EDGE_SURFACE_H

// include the C++ standard libraries we need for the header
#include <vector>
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// include the unit with Cartesian 3-vectors
#include "Cartesian3.h"
// the render parameters
#include "RenderParameters.h"
// the image class for a texture
#include "RGBAImage.h" 

#include <set>



class DirectedEdgeSurface
{ // class DirectedEdgeSurface

public:
    // vector of vertices
    std::vector<Cartesian3> vertices;

    // vector of normals
    std::vector<Cartesian3> normals;
    
	// vector of first directed edge IDs
	std::vector<unsigned int> firstDirectedEdge;

    // vector of faces - stored as a single long array
    std::vector<unsigned int> faceVertices;

	// vector of other half edge IDs
	std::vector<unsigned int> otherHalf;

    // centre of gravity - computed after reading
    Cartesian3 centreOfGravity;

    // size of object - i.e. radius of circumscribing sphere centred at centre of gravity
    float objectSize;

    // constructor will initialise to safe values
    DirectedEdgeSurface();
    
    // read routine returns true on success, failure otherwise
    bool ReadObjectStream(std::istream &geometryStream);

    // write routine
    void WriteObjectStream(std::ostream &geometryStream);

    // routine to render
    void Render(RenderParameters *renderParameters);



//add by Tian Zeng
    void generateDirectedEdge();
    void loopSubdivision();
    Cartesian3 adjust(int32_t vertexId);
    Cartesian3& getVertex(int32_t index);
    int32_t getAdjacentFaceId(int32_t index);
    int32_t getNextEdge(int32_t index);
    int32_t getPrevEdge(int32_t index);
    void addNewFace( std::vector<uint32_t> & face,const std::vector<uint32_t> & vertices);
    void save(const std::string & name);

}; // class DirectedEdgeSurface

// end of include guard for DirectedEdgeSurface
#endif
