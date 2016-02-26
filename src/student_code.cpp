/*
 * Student solution for UC Berkeley Project 2
 *
 * Implemented by ____ on ____.
 *
 */

#include "student_code.h"
#include "mutablePriorityQueue.h"

namespace CGL {

    void BezierPatch::preprocess() {
        // TODO Part 1.
        //called in bezierPatch.cpp where control points are already loaded, u can just call control points
        //turn control points into 3 x y z matrices
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                xMatrix(i,j) = controlPoints[i][j].x;
                yMatrix(i,j) = controlPoints[i][j].y;
                zMatrix(i,j) = controlPoints[i][j].z;
            }
        }
        //matrices used to compute the bezier matrices
        Matrix4x4 M;
        M(0,0) = 1; M(0,1) = 0; M(0,2) = 0; M(0,3) = 0;
        M(1,0) = -3; M(1,1) = 3; M(1,2) = 0; M(1,3) = 0;
        M(2,0) = 3; M(2,1) = -6; M(2,2) = 3; M(2,3) = 0;
        M(3,0) = -1; M(3,1) = 3; M(3,2) = -3; M(3,3) = 1;
    
        xBezierMatrix = M* xMatrix* M.T();
        yBezierMatrix = M* yMatrix* M.T();
        zBezierMatrix = M* zMatrix* M.T();
    }

    Vector3D BezierPatch::evaluate(double u, double v) const {
        // TODO Part 1.
        Vector4D uVector = Vector4D(1, u, u*u, u*u*u);
        Vector4D vVector = Vector4D(1, v, v*v, v*v*v);
        double x = dot(uVector, (xBezierMatrix*vVector));
        double y = dot(uVector, (yBezierMatrix*vVector));
        double z = dot(uVector, (zBezierMatrix*vVector));
        return Vector3D(x, y, z);
    }

    void BezierPatch::add2mesh(Polymesh* mesh) const {
        // TODO Part 1.
        Vector3D v1;
        Vector3D v2;
        Vector3D v3;
        Vector3D v4;

        //iterate through the 9x9 grid of Vectord3Ds
        for (double v = 0; v <= 1-0.125; v+=0.125) {
            for (double u = 0; u <= 1-0.125; u += 0.125) {
                v1 = evaluate(u, v);
                v2 = evaluate(u + 0.125, v);
                v3 = evaluate(u, v + 0.125);
                v4 = evaluate(u + 0.125, v + 0.125);
                addTriangle(mesh, v1, v2, v4);
                addTriangle(mesh, v1, v4, v3);
            }
        }

    }

    Vector3D Vertex::normal(void) const {
    // TODO Part 2.
        Vector3D vertNorm;
        HalfedgeCIter h = this->halfedge();
        do{
            if(!h->face()->isBoundary()) {
                vertNorm += h->face()->normal();
            }
            h = h->twin()->next();
        } while (h != this -> halfedge());
        return vertNorm.unit();
    }

    EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0) {
        // TODO Part 3.
        //return if boundary loops
        HalfedgeIter h0 = e0->halfedge();
        if(e0->isBoundary()) {
            return EdgeIter();
        }
        //get half edges
        HalfedgeIter h1 = h0->next();
        HalfedgeIter h2 = h1->next();
        HalfedgeIter h3 = h0->twin();
        HalfedgeIter h4 = h3->next();
        HalfedgeIter h5 = h4->next();
        HalfedgeIter h6 = h1->twin();
        HalfedgeIter h7 = h2->twin();
        HalfedgeIter h8 = h4->twin();
        HalfedgeIter h9 = h5->twin();
        //get vertices
        VertexIter v0 = h0->vertex();
        VertexIter v1 = h3->vertex();
        VertexIter v2 = h2->vertex();
        VertexIter v3 = h5->vertex();
        //get faces
        FaceIter f0 = h0->face();
        FaceIter f1 = h3->face();
        //get edges
        EdgeIter e1 = h1->edge();
        EdgeIter e2 = h2->edge();
        EdgeIter e3 = h4->edge();
        EdgeIter e4 = h5->edge();

        h0->setNeighbors(h1, h3, v3, e0, f0);
        h1->setNeighbors(h2, h7, v2, e2, f0);
        h2->setNeighbors(h0, h8, v0, e3, f0);
        h3->setNeighbors(h4, h0, v2, e0, f1);
        h4->setNeighbors(h5, h9, v3, e4, f1);
        h5->setNeighbors(h3, h6, v1, e1, f1);
        //Set outside edges (differently!)
        //set h6
        h6->next() = h6->next();
        h6->twin() = h5;
        h6->vertex() = v2;
        h6->edge() = e1;
        h6->face() = f1;
        //set h7
        h7->next() = h7->next();
        h7->twin() = h1;
        h7->vertex() = v0;
        h7->edge() = e2;
        h7->face() = h7->face();
        //set h8
        h8->next() = h8->next();
        h8->twin() = h2;
        h8->vertex() = v3;
        h8->edge() = e3;
        h8->face() = f0;
        //set h9
        h9->next() = h9->next();
        h9->twin() = h4;
        h9->vertex() = v1;
        h9->edge() = e4;
        h9->face() = h9->face();
        //set vertices
        v0->halfedge() = h2; 
        v1->halfedge() = h5;
        v2->halfedge() = h3;
        v3->halfedge()= h0;
        //set edges
        e0->halfedge() = h0;
        e1->halfedge() = h5;
        e2->halfedge() = h1;
        e3->halfedge() = h2;
        e4->halfedge() = h4;
        //set faces
        f0->halfedge() = h0; 
        f1->halfedge() = h3;

        return e0;
    }

    VertexIter HalfedgeMesh::splitEdge(EdgeIter e0) {
        // TODO Part 4.
        // TODO This method should split the given edge and return an iterator to the newly inserted vertex.
        // TODO The halfedge of this vertex should point along the edge that was split, rather than the new edges.
        //see if boundary
        if(e0->isBoundary()) {
            return VertexIter();
        }
        //First, gather all mesh elements as before
        //get half edges
        HalfedgeIter h0 = e0->halfedge();
        HalfedgeIter h1 = h0->next();
        HalfedgeIter h2 = h1->next();
        HalfedgeIter h3 = h0->twin();
        HalfedgeIter h4 = h3->next();
        HalfedgeIter h5 = h4->next();
        HalfedgeIter h6 = h1->twin();
        HalfedgeIter h7 = h2->twin();
        HalfedgeIter h8 = h4->twin();
        HalfedgeIter h9 = h5->twin();
        //get vertices
        VertexIter v0 = h4->vertex();
        VertexIter v1 = h1->vertex();
        VertexIter v2 = h2->vertex();
        VertexIter v3 = h5->vertex();
        //get faces, reset them here to the diagonals
        FaceIter f0 = h1->face();
        FaceIter f1 = h5->face();
        //get edges
        EdgeIter e1 = h1->edge();
        EdgeIter e2 = h2->edge();
        EdgeIter e3 = h4->edge();
        EdgeIter e4 = h5->edge();

        //Create new stuff, allocation
        //create the new vertex
        VertexIter v4 = newVertex();
        //top right triange
        EdgeIter e5 = newEdge();
        EdgeIter e6 = newEdge();
        HalfedgeIter h10 = newHalfedge();
        HalfedgeIter h11 = h10->next();
        e5->halfedge() = h10;
        e6->halfedge() = h11;
        //bottom right triangle
        EdgeIter e7 = newEdge();
        HalfedgeIter h12 = newHalfedge();
        HalfedgeIter h13 = h12->next(); 
        e7->halfedge() = h12;
        //bottom left triangle
        EdgeIter e8 = newEdge();
        HalfedgeIter h14 = newHalfedge();
        HalfedgeIter h15 = h14->next();
        e8->halfedge() = h14;
        //top left triangle
        HalfedgeIter h16 = newHalfedge();
        HalfedgeIter h17 = h16->next();

        //allocating 2 new faces as a result
        FaceIter f2 = newFace();
        f2->halfedge() = h2;
        FaceIter f3 = newFace();
        f3->halfedge() = h4;
        //put everything back together now
        //set new vertex position to be the midpoint of e0 and intialize it
        v4->position = 0.5*((v1->position) + (v2->position));
        v4->halfedge() = h11;
        //set up new horiziontal half edge pointers, dont worry about deleting (yet?)
        h10->next() = h11;
        h10->twin() = h13;
        h10->vertex() = v2;
        h10->edge() = e5;
        h10->face() = f0;

        h11->next() = h1;
        h11->twin() = h16;
        h11->vertex() = v4;
        h11->edge() = e6;
        h11->face() = f0;

        h12->next() = h13;
        h12->twin() = h15;
        h12->vertex() = v0;
        h12->edge() = e7;
        h12->face() = f2;

        h13->next() = h2;
        h13->twin() = h10;
        h13->vertex() = v4;
        h13->edge() = e5;
        h13->face() = f2;

        h14->next() = h15;
        h14->twin() = h17;
        h14->vertex() = v3;
        h14->edge() = e8;
        h14->face() = f3;

        h15->next() = h4;
        h15->twin() = h12;
        h15->vertex() = v4;
        h15->edge() = e7;
        h15->face() = f3;

        h16->next() = h17;
        h16->twin() = h11;
        h16->vertex() = v1;
        h16->edge() = e6;
        h16->face() = f1;

        h17->next() = h5;
        h17->twin() = h14;
        h17->vertex() = v4;
        h17->edge() = e8;
        h17->face() = f1;

        //reset vertices
        v0->halfedge() = h12;
        v1->halfedge() = h16;
        v2->halfedge() = h10;
        v3->halfedge() = h14;
        //deal with e0, h0, h3;
        h1 = h11->next();
        h1->next() = h10;
        h1->twin() = h6;
        h1->vertex() = v1;
        h1->edge() = e1;
        h1->face() = f0;

        h2->next() = h12;
        h2->twin() = h7;
        h2->vertex() = v2;
        h2->edge() = e2;
        h2->face() = f2;

        h4->next() = h14;
        h4->twin() = h8;
        h4->vertex() = v0;
        h4->edge() = e3;
        h4->face() = f3;

        h5->next() = h16;
        h5->twin() =  h9;
        h5->vertex() = v3;
        h5->edge() = e4;
        h5->face() = f1;

        //the outsides ones 6-9
        //set h6
        h6->next() = h6->next();
        h6->twin() = h1;
        h6->vertex() = v2;
        h6->edge() = e1;
        h6->face() = h6->face();
        //set h7
        h7->next() = h7->next();
        h7->twin() = h2;
        h7->vertex() = v0;
        h7->edge() = e2;
        h7->face() = f2;
        //set h8
        h8->next() = h8->next();
        h8->twin() = h4;
        h8->vertex() = v3;
        h8->edge() = e3;
        h8->face() = f3;
        //set h9
        h9->next() = h9->next();
        h9->twin() = h5;
        h9->vertex() = v1;
        h9->edge() = e4;
        h9->face() = h9->face();

        deleteHalfedge(h0);
        deleteHalfedge(h3);
        deleteEdge(e0);
        //reset edges
        e1->halfedge() = h1;
        e2->halfedge() = h2;
        e3->halfedge() = h4;
        e4->halfedge() = h5;

        //reset faces
        f0->halfedge() = h1;
        f1->halfedge() = h5;
        f2->halfedge() = h2;
        f3->halfedge() = h4;

        return v4;
    }

    void MeshResampler::upsample(HalfedgeMesh& mesh)
    // TODO Part 5.
    // This routine should increase the number of triangles in the mesh using Loop subdivision.
    {
        // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
        // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
        // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
        // the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
        // the new mesh based on the values we computed for the original mesh.


        // TODO Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
        // TODO and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
        // TODO a vertex of the original mesh.


        // TODO Next, compute the updated vertex positions associated with edges, and store it in Edge::newPosition.


        // TODO Next, we're going to split every edge in the mesh, in any order.  For future
        // TODO reference, we're also going to store some information about which subdivided
        // TODO edges come from splitting an edge in the original mesh, and which edges are new,
        // TODO by setting the flat Edge::isNew.  Note that in this loop, we only want to iterate
        // TODO over edges of the original mesh---otherwise, we'll end up splitting edges that we
        // TODO just split (and the loop will never end!)


        // TODO Now flip any new edge that connects an old and new vertex.


        // TODO Finally, copy the new vertex positions into final Vertex::position.

    }

    // TODO Part 6.
    // TODO There's also some code you'll need to complete in "Shader/frag" file.

}
