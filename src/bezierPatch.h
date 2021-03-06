/* 
 * File:   BezierPatch.h
 * Author: swl
 *
 * Created on January 30, 2016, 4:59 PM
 */

#ifndef BEZIERPATCH_H
#define	BEZIERPATCH_H

#include "CGL/CGL.h"
#include "mesh.h"

namespace CGL {

    class BezierPatch {
    public:
        void loadControlPoints(FILE* file);
        void add2mesh(Polymesh* mesh) const;
    private:
        Vector3D controlPoints[4][4];
        void addTriangle(Polymesh* mesh,
                const Vector3D& v0, const Vector3D& v1, const Vector3D& v2) const;

        void preprocess();
        Vector3D evaluate(double u, double v) const;

        // TODO Part 1 - add member variables?
        Matrix4x4 xMatrix;
        Matrix4x4 yMatrix;
        Matrix4x4 zMatrix; //no need to import matrix4x4.h because CGL
        Matrix4x4 xBezierMatrix; 
        Matrix4x4 yBezierMatrix;
        Matrix4x4 zBezierMatrix;
       
    };

}

#endif	/* BEZIERPATCH_H */

