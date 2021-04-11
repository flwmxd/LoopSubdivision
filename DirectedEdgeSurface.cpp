///////////////////////////////////////////////////
//
//  Hamish Carr
//  September, 2020
//
//  ------------------------
//  DirectedEdgeSurface.cpp
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

// include the header file
#include "DirectedEdgeSurface.h"

// include the C++ standard libraries we want
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <limits>
#include <algorithm>
// include the Cartesian 3- vector class
#include "Cartesian3.h"
#include "SphereVertices.h"



#define MAXIMUM_LINE_LENGTH 1024

// constructor will initialise to safe values
DirectedEdgeSurface::DirectedEdgeSurface()
    : centreOfGravity(0.0,0.0,0.0)
{ // DirectedEdgeSurface()
// force arrays to size 0
    vertices.resize(0);
    normals.resize(0);
    firstDirectedEdge.resize(0);
    faceVertices.resize(0);
    otherHalf.resize(0);
} // DirectedEdgeSurface()

// read routine returns true on success, failure otherwise
bool DirectedEdgeSurface::ReadObjectStream(std::istream &geometryStream)
    { // ReadObjectStream()
    
    // create a read buffer
    char readBuffer[MAXIMUM_LINE_LENGTH];
    
    // the rest of this is a loop reading lines & adding them in appropriate places
    while (true)
        { // not eof
		// token for identifying meaning of line
		std::string token;

        // character to read
        geometryStream >> token;
        
        // check for eof() in case we've run out
        if (geometryStream.eof())
            break;

        // otherwise, switch on the token we read
		if (token == "#")
			{ // comment 
			// read and discard the line
			geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
            } // comment
		else if (token == "Vertex")
			{ // vertex
			// variables for the read
			unsigned int vertexID;
			geometryStream >> vertexID;
			// it has to be next valid 0-based ID, so
			// reject line if it isn't
			if (vertexID != vertices.size())
				{ // bad vertex ID
				// read and discard the line
				geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
				} // bad vertex ID				
			
			// read in the new vertex position
			Cartesian3 newVertex;
			geometryStream >> newVertex;
			
			// and add it to the vertices
			vertices.push_back(newVertex);
			} // vertex
		else if (token == "Normal")
			{ // normal
			// variables for the read
			unsigned int normalID;
			geometryStream >> normalID;
			// it has to be next valid 0-based ID, so
			// reject line if it isn't
			if (normalID != normals.size())
				{ // bad ID
				// read and discard the line
				geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
				} // bad ID				
			
			// read in the new normal
			Cartesian3 newNormal;
			geometryStream >> newNormal;
			
			// and add it to the vertices
			normals.push_back(newNormal);
			} // normal
		else if (token == "FirstDirectedEdge")
			{ // first directed edge
			// variables for the read
			unsigned int FDEID;
			geometryStream >> FDEID;
			// it has to be next valid 0-based ID, so
			// reject line if it isn't
			if (FDEID != firstDirectedEdge.size())
				{ // bad ID
				// read and discard the line
				geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
				} // bad ID				
			
			// read in the new FDE
			unsigned int newFDE;
			geometryStream >> newFDE;
			
			// and add it to the vertices
			firstDirectedEdge.push_back(newFDE);
			} // first directed edge
		else if (token == "Face")
			{ // face
			// variables for the read
			unsigned int faceID;
			geometryStream >> faceID;
			// it has to be next valid 0-based ID, so
			// reject line if it isn't
			if (faceID != faceVertices.size()/3)
				{ // bad face ID
				// read and discard the line
				geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
				} // bad face ID				
			
			// read in the new face vertex (3 times)
			unsigned int newFaceVertex;
			geometryStream >> newFaceVertex;
			faceVertices.push_back(newFaceVertex);
			geometryStream >> newFaceVertex;
			faceVertices.push_back(newFaceVertex);
			geometryStream >> newFaceVertex;
			faceVertices.push_back(newFaceVertex);
			} // face
		else if (token == "OtherHalf")
			{ // other half
			// variables for the read
			unsigned int otherHalfID;
			geometryStream >> otherHalfID;
			// it has to be next valid 0-based ID, so
			// reject line if it isn't
			if (otherHalfID != otherHalf.size())
				{ // bad ID
				// read and discard the line
				geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
				} // bad ID				
			
			// read in the new face vertex (3 times)
			unsigned int newOtherHalf;
			geometryStream >> newOtherHalf;
			otherHalf.push_back(newOtherHalf);
			} // other half
        } // not eof

    // compute centre of gravity
    // note that very large files may have numerical problems with this
    centreOfGravity = Cartesian3(0.0, 0.0, 0.0);

    // if there are any vertices at all
    if (vertices.size() != 0)
        { // non-empty vertex set
        // sum up all of the vertex positions
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
            centreOfGravity = centreOfGravity + vertices[vertex];
        
        // and divide through by the number to get the average position
        // also known as the barycentre
        centreOfGravity = centreOfGravity / vertices.size();

        // start with 0 radius
        objectSize = 0.0;

        // now compute the largest distance from the origin to a vertex
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
            { // per vertex
            // compute the distance from the barycentre
            float distance = (vertices[vertex] - centreOfGravity).length();         
            
            // now test for maximality
            if (distance > objectSize)
                objectSize = distance;
            } // per vertex
        } // non-empty vertex set

    // return a success code
    return true;
    } // ReadObjectStream()

// write routine
void DirectedEdgeSurface::WriteObjectStream(std::ostream &geometryStream)
    { // WriteObjectStream()
	geometryStream << "#" << std::endl; 
	geometryStream << "# Created for Leeds COMP 5821M Autumn 2020" << std::endl; 
	geometryStream << "#" << std::endl; 
	geometryStream << "#" << std::endl; 
	geometryStream << "# Surface vertices=" << vertices.size() << " faces=" << faceVertices.size()/3 << std::endl; 
	geometryStream << "#" << std::endl; 

	// output the vertices
    for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
        geometryStream << "Vertex " << vertex << " " << std::fixed << vertices[vertex] << std::endl;

    // and the normal vectors
    for (unsigned int normal = 0; normal < normals.size(); normal++)
        geometryStream << "Normal " << normal << " " << std::fixed << normals[normal] << std::endl;

	// and the first directed edges
    for (unsigned int vertex = 0; vertex < firstDirectedEdge.size(); vertex++)
        geometryStream << "FirstDirectedEdge " << vertex<< " " << std::fixed << firstDirectedEdge[vertex] << std::endl;

    // and the faces - increment is taken care of internally
    for (unsigned int face = 0; face < faceVertices.size(); )
        { // per face
        geometryStream << "Face " << face / 3<< " ";
        
        // read in three vertices
        geometryStream << faceVertices[face++] << " ";
        geometryStream << faceVertices[face++] << " ";
        geometryStream << faceVertices[face++];
            
        geometryStream << std::endl;
        } // per face

	// and the other halves
	for (unsigned int dirEdge = 0; dirEdge < otherHalf.size(); dirEdge++)
		geometryStream << "OtherHalf " << dirEdge << " " << otherHalf[dirEdge] << std::endl;
    } // WriteObjectStream()


void DirectedEdgeSurface::loopSubdivision()
{
//      b                b
//     / \              / \
//    /   \            p---q
//   /     \          / \ / \
//  a ----- c   ->   a---r--c
//   \     /          \     /
//    \   /            \   /
//     \ /              \ /
//      d                d
// an edge will be shared with two triangles at most,
// an edge is a boundary if an edge was shared with an triangle.
// an edge is a inner edge if it was shared with two edges.
// here is a formula
// r = 3/8(a+c) + 1/8(b+d)

//used to save center face in order to keep the original index
    std::vector<uint32_t> centerFacesVertices;
    std::vector<uint32_t> newFaceVertices;
    std::vector<Cartesian3> newVertices;

//       b         c
//      /\--------/\
//     /  \      /  \
//    /    p    q    \
//   /      \  /      \
//  /        \/        \
// e --------a----------d


    //for the face calculate point p and q.
    // faceCount = faceVertices.size()/3

    for(auto faceId = 0;faceId<faceVertices.size()/3;faceId++)
    {
         std::vector<uint32_t> newFace;
        //foreach face;
         for(auto j = 0;j<3;j++){
            auto a = (faceId * 3) + (j % 3);
            auto b = (faceId * 3) + (j + 2) % 3;
            auto c = (faceId * 3) + (j + 1) % 3;

            //getAdjacentFaceId(c) * 3 means get vertices index in the adjacent face;
            auto d = getAdjacentFaceId(c) * 3 + (getNextEdge(otherHalf[c]) % 3);
            auto e = getAdjacentFaceId(a) * 3 + (getNextEdge(otherHalf[a]) % 3);

//let's calculate the new pos.
            Cartesian3 currNewPos[3];
//now, update the point a with adjancent points;
            currNewPos[0] = adjust(faceVertices[a]);
// q = 3/8(a+c) + 1/8(b+d)
            currNewPos[1] = 0.375f * (getVertex(a)+getVertex(c)) + 0.125 * (getVertex(b)+ getVertex(d));//q
// p = 3/8(a+b) + 1/8(c+e)
            currNewPos[2] = 0.375f * (getVertex(a)+getVertex(b)) + 0.125 * (getVertex(c)+ getVertex(e));//p
//we get a new face aqp;
            for (auto k = 0; k < 3; k++)
            {
                auto & cur = currNewPos[k];
                auto index = -1;
                for (auto i = 0; i < newVertices.size(); i++)
                {
                    if (cur == newVertices[i])
                    {
                        index = i;
                        break;
                    }
                }

//if not found in new added vertices;
                if (index == -1)
                {
                    index = newVertices.size();
                    newVertices.emplace_back(cur);
                }
// zero is the origin point, so we need to add new point into vector.
// in this stage, we construct every new surface with one old point and two new points,
// but we don't construct the new surface with all new points
// so we add into the list and construct it later;
                if (k != 0){
                    newFace.emplace_back(index);
                }
               newFaceVertices.emplace_back(index);
            }
        }
        addNewFace(centerFacesVertices,newFace);
    }

    //keep the order from the requirements.
    //remaining faces are added sequentially at the end of the array
    for(auto & index : newFaceVertices){
        centerFacesVertices.emplace_back(index);
    }

    vertices= newVertices;
    faceVertices = centerFacesVertices;
    std::cout<<"new face size : "<<faceVertices.size()/3<<std::endl;
    generateDirectedEdge();
}

Cartesian3& DirectedEdgeSurface::getVertex(int32_t index)
{
    return vertices[faceVertices[index]];
}

//save file
void DirectedEdgeSurface::save(const std::string & name)
{
    std::ofstream out(name);
    WriteObjectStream(out);
    out.close();
}

//we need to get newA,newB,newC as the newFace

void DirectedEdgeSurface::addNewFace( std::vector<uint32_t> & face,const std::vector<uint32_t> & vertices)
{
    for (auto i = 2;i >= 0;i--)
    {
        face.emplace_back(vertices[i]);
    }
}

Cartesian3 DirectedEdgeSurface::adjust(int32_t vertexId)
{

//      u    u
//     /\----/\
//    /  \  /  \
//   /    \/    \
//   ------N-----
//   \    /\    /
//    \  /  \  /
//     \/____\/
//
// N = (1-nu)
// u = 1/n[5/8 - (3/8 + 1/4 * cos(2pi/n)) ^2]

    Cartesian3 sum = {0,0,0};
    int32_t n = 0;

    //try to find all adjancent points
    auto edgeId = firstDirectedEdge[vertexId];
    do
    {
        //accumulate all vertices;
        sum =sum + vertices[faceVertices[getNextEdge(edgeId)]];
        edgeId = getNextEdge(otherHalf[edgeId]);
        n++;
    }
    while (edgeId != firstDirectedEdge[vertexId]);

    //calculate  weight
    auto weight = (1 / (float)n) * ((5 / 8.f) - std::pow((3/8.f + 1/4.f * std::cos((2 * M_PI) / (float)n)), 2));
    return vertices[vertexId] * (float)(1 - n * weight) + sum * weight;
}
//in single face...
//
//    b(2)____c(1)
//        \  /
//         \/
//          a(0
int32_t DirectedEdgeSurface::getNextEdge(int32_t index)
{
    return (index % 3) == 2 ? index - 2 : index + 1;
}

int32_t DirectedEdgeSurface::getPrevEdge(int32_t index)
{
    return (index % 3) != 0 ? index - 1 : index + 2;
}

//index : the vertex id
int32_t DirectedEdgeSurface::getAdjacentFaceId(int32_t index)
{
    return otherHalf[index]/3;
}

void DirectedEdgeSurface::generateDirectedEdge()
{
    constexpr auto INIT_VALUE = std::numeric_limits<uint32_t>::max();
    //clear the prev data;
    firstDirectedEdge.clear();
    otherHalf.clear();
    //init wit a unreachable value to mark it is not used
    otherHalf.resize(faceVertices.size(),INIT_VALUE);
    firstDirectedEdge.resize(vertices.size(),INIT_VALUE);

    //generate the half data structure;
    for(auto edgeId = 0;edgeId<faceVertices.size();edgeId++)
    {
        auto faceId = edgeId / 3;
        //to -> from

        // Hint 1 :
        // if a face with vertices was interpreted as from
        // we can do like that
        // auto endVert = faceVertices[getNextEdge(edgeId)];
        auto startVert = faceVertices[edgeId];
        auto endVert = faceVertices[getPrevEdge(edgeId)];


        if(startVert != endVert)
        {
            if(firstDirectedEdge[startVert] == INIT_VALUE){
                firstDirectedEdge[startVert] = edgeId;
            }

            if(otherHalf[edgeId] == INIT_VALUE){
                //foreach the remain faces;
                for(auto nextEdgeId = 3 * (faceId + 1); nextEdgeId < faceVertices.size();nextEdgeId++)
                {
                    auto nextStart = faceVertices[nextEdgeId];
                    //same with Hint1
                    auto nextEnd= faceVertices[getPrevEdge(nextEdgeId)];

                    if(startVert == nextEnd && nextStart == endVert)
                    {
                        otherHalf[edgeId] = nextEdgeId;
                        otherHalf[nextEdgeId] = edgeId;
                    }
                }
            }
        }
    }
}


// routine to render
void DirectedEdgeSurface::Render(RenderParameters *renderParameters)
    { // Render()
    // Ideally, we would apply a global transformation to the object, but sadly that breaks down
    // when we want to scale things, as unless we normalise the normal vectors, we end up affecting
    // the illumination.  Known solutions include:
    // 1.   Normalising the normal vectors
    // 2.   Explicitly dividing the normal vectors by the scale to balance
    // 3.   Scaling only the vertex position (slower, but safer)
    // 4.   Not allowing spatial zoom (note: sniper scopes are a modified projection matrix)
    //
    // Inside a game engine, zoom usually doesn't apply. Normalisation of normal vectors is expensive,
    // so we will choose option 2.  

    // Scale defaults to the zoom setting
    float scale = renderParameters->zoomScale;
    scale /= objectSize;
        
    //  now scale everything
    glScalef(scale, scale, scale);

    // apply the translation to the centre of the object if requested
    glTranslatef(-centreOfGravity.x, -centreOfGravity.y, -centreOfGravity.z);

    // start rendering
    glBegin(GL_TRIANGLES);


	// set colour for pick render - ignored for regular render
    //glColor3f(1.0, 1.0, 1.0);


    // loop through the faces
	for (unsigned int face = 0; face < faceVertices.size(); face +=3)
		{ // per face

        if(renderParameters->currentSelected == face/3){
            glColor4f(1,0,0,1);

            GLfloat emission[4] = { 1, 0.2, 0.2, 1.0 };
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);

        }else{

            GLfloat emission[4] = { 0.3, 0.2, 0.2, 1.0 };
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);

        }

		// if we want flat normals, compute them here
		if (renderParameters->useFlatNormals)
			{ // flat normals
			// find two vectors along edges of the triangle
			Cartesian3 pq = vertices[faceVertices[face+1]] - vertices[faceVertices[face]];
			Cartesian3 pr = vertices[faceVertices[face+2]] - vertices[faceVertices[face]];

			// take their cross product and normalise
			Cartesian3 faceNormal = pq.cross(pr).unit();

			// and use it to set the glNormal
			glNormal3f(faceNormal.x * scale, faceNormal.y * scale, faceNormal.z * scale);
			} // flat normals


		// we have made a HARD assumption that we have enough normals
		for (unsigned int vertex = face; vertex < face+3; vertex++)
			{ // per vertex
		
			// if we are using smooth normals
			if (!renderParameters->useFlatNormals)
				// set the normal vector
				glNormal3f
					(
					normals[faceVertices[vertex]].x * scale,
					normals[faceVertices[vertex]].y * scale,
					normals[faceVertices[vertex]].z * scale
					);
			

			// and set the vertex position
			glVertex3f
				(
				vertices[faceVertices[vertex]].x,
				vertices[faceVertices[vertex]].y,
				vertices[faceVertices[vertex]].z
				);

			} // per vertex

		} // per face

    // close off the triangles
    glEnd();
    
    // now we add a second loop to render the vertices if desired
    if (!renderParameters->showVertices)
    	return;

	glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
	// loop through the vertices
	for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
		{ // per vertex
		// use modelview matrix (not most efficient solution, but quickest to code)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(vertices[vertex].x, vertices[vertex].y, vertices[vertex].z);
		glScalef(0.1 * renderParameters->vertexSize, 0.1 * renderParameters->vertexSize, 0.1 * renderParameters->vertexSize);
		renderTriangulatedSphere();
		glPopMatrix();
		} // per vertex 
    

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glLineWidth(5);
    for (auto face = 0; face < faceVertices.size(); face +=3)
    {
        auto & a = vertices[faceVertices[face]];
        auto & b = vertices[faceVertices[face + 1]];
        auto & c = vertices[faceVertices[face + 2]];
        glColor3f(1,0,0);

        glVertex3f
            (
            a.x,
            a.y,
            a.z
            );

        glVertex3f
            (
            b.x,
            b.y,
            b.z
            );
        glColor3f(0,1,0);
        glVertex3f
            (
            b.x,
            b.y,
            b.z
            );

        glVertex3f
            (
            c.x,
            c.y,
            c.z
            );
        glColor3f(0,0,1);
        glVertex3f
            (
            c.x,
            c.y,
            c.z
            );

        glVertex3f
            (
            a.x,
            a.y,
            a.z
            );
    }

    glEnd();

} // Render()







