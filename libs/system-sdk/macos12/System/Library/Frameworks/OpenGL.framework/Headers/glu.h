/*
	Copyright:	(c) 1999-2012 Apple Inc. All rights reserved.
*/

#ifndef __glu_h__
#define __glu_h__

#include <OpenGL/OpenGLAvailability.h>
#include <OpenGL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************/

/* Extensions */
#define GLU_EXT_object_space_tess            1
#define GLU_EXT_nurbs_tessellator            1

/* Boolean */
#define GLU_FALSE                            0
#define GLU_TRUE                             1

/* Version */
#define GLU_VERSION_1_1                      1
#define GLU_VERSION_1_2                      1
#define GLU_VERSION_1_3                      1

/* StringName */
#define GLU_VERSION                          100800
#define GLU_EXTENSIONS                       100801

/* ErrorCode */
#define GLU_INVALID_ENUM                     100900
#define GLU_INVALID_VALUE                    100901
#define GLU_OUT_OF_MEMORY                    100902
#define GLU_INCOMPATIBLE_GL_VERSION          100903
#define GLU_INVALID_OPERATION                100904


/* NurbsDisplay */
/*      GLU_FILL */
#define GLU_OUTLINE_POLYGON                  100240
#define GLU_OUTLINE_PATCH                    100241

/* NurbsCallback */
#define GLU_NURBS_ERROR                      100103
#define GLU_ERROR                            100103
#define GLU_NURBS_BEGIN                      100164
#define GLU_NURBS_BEGIN_EXT                  100164
#define GLU_NURBS_VERTEX                     100165
#define GLU_NURBS_VERTEX_EXT                 100165
#define GLU_NURBS_NORMAL                     100166
#define GLU_NURBS_NORMAL_EXT                 100166
#define GLU_NURBS_COLOR                      100167
#define GLU_NURBS_COLOR_EXT                  100167
#define GLU_NURBS_TEXTURE_COORD              100168
#define GLU_NURBS_TEX_COORD_EXT              100168
#define GLU_NURBS_END                        100169
#define GLU_NURBS_END_EXT                    100169
#define GLU_NURBS_BEGIN_DATA                 100170
#define GLU_NURBS_BEGIN_DATA_EXT             100170
#define GLU_NURBS_VERTEX_DATA                100171
#define GLU_NURBS_VERTEX_DATA_EXT            100171
#define GLU_NURBS_NORMAL_DATA                100172
#define GLU_NURBS_NORMAL_DATA_EXT            100172
#define GLU_NURBS_COLOR_DATA                 100173
#define GLU_NURBS_COLOR_DATA_EXT             100173
#define GLU_NURBS_TEXTURE_COORD_DATA         100174
#define GLU_NURBS_TEX_COORD_DATA_EXT         100174
#define GLU_NURBS_END_DATA                   100175
#define GLU_NURBS_END_DATA_EXT               100175

/* NurbsError */
#define GLU_NURBS_ERROR1                     100251   /* spline order un-supported */
#define GLU_NURBS_ERROR2                     100252   /* too few knots */
#define GLU_NURBS_ERROR3                     100253   /* valid knot range is empty */
#define GLU_NURBS_ERROR4                     100254   /* decreasing knot sequence */
#define GLU_NURBS_ERROR5                     100255   /* knot multiplicity > spline order */
#define GLU_NURBS_ERROR6                     100256   /* endcurve() must follow bgncurve() */
#define GLU_NURBS_ERROR7                     100257   /* bgncurve() must precede endcurve() */
#define GLU_NURBS_ERROR8                     100258   /* ctrlarray or knot vector is NULL */
#define GLU_NURBS_ERROR9                     100259   /* can't draw pwlcurves */
#define GLU_NURBS_ERROR10                    100260   /* missing gluNurbsCurve() */
#define GLU_NURBS_ERROR11                    100261   /* missing gluNurbsSurface() */
#define GLU_NURBS_ERROR12                    100262   /* endtrim() must precede endsurface() */
#define GLU_NURBS_ERROR13                    100263   /* bgnsurface() must precede endsurface() */
#define GLU_NURBS_ERROR14                    100264   /* curve of improper type passed as trim curve */
#define GLU_NURBS_ERROR15                    100265   /* bgnsurface() must precede bgntrim() */
#define GLU_NURBS_ERROR16                    100266   /* endtrim() must follow bgntrim() */
#define GLU_NURBS_ERROR17                    100267   /* bgntrim() must precede endtrim()*/
#define GLU_NURBS_ERROR18                    100268   /* invalid or missing trim curve*/
#define GLU_NURBS_ERROR19                    100269   /* bgntrim() must precede pwlcurve() */
#define GLU_NURBS_ERROR20                    100270   /* pwlcurve referenced twice*/
#define GLU_NURBS_ERROR21                    100271   /* pwlcurve and nurbscurve mixed */
#define GLU_NURBS_ERROR22                    100272   /* improper usage of trim data type */
#define GLU_NURBS_ERROR23                    100273   /* nurbscurve referenced twice */
#define GLU_NURBS_ERROR24                    100274   /* nurbscurve and pwlcurve mixed */
#define GLU_NURBS_ERROR25                    100275   /* nurbssurface referenced twice */
#define GLU_NURBS_ERROR26                    100276   /* invalid property */
#define GLU_NURBS_ERROR27                    100277   /* endsurface() must follow bgnsurface() */
#define GLU_NURBS_ERROR28                    100278   /* intersecting or misoriented trim curves */
#define GLU_NURBS_ERROR29                    100279   /* intersecting trim curves */
#define GLU_NURBS_ERROR30                    100280   /* UNUSED */
#define GLU_NURBS_ERROR31                    100281   /* unconnected trim curves */
#define GLU_NURBS_ERROR32                    100282   /* unknown knot error */
#define GLU_NURBS_ERROR33                    100283   /* negative vertex count encountered */
#define GLU_NURBS_ERROR34                    100284   /* negative byte-stride */
#define GLU_NURBS_ERROR35                    100285   /* unknown type descriptor */
#define GLU_NURBS_ERROR36                    100286   /* null control point reference */
#define GLU_NURBS_ERROR37                    100287   /* duplicate point on pwlcurve */

/* NurbsProperty */
#define GLU_AUTO_LOAD_MATRIX                 100200
#define GLU_CULLING                          100201
#define GLU_SAMPLING_TOLERANCE               100203
#define GLU_DISPLAY_MODE                     100204
#define GLU_PARAMETRIC_TOLERANCE             100202
#define GLU_SAMPLING_METHOD                  100205
#define GLU_U_STEP                           100206
#define GLU_V_STEP                           100207
#define GLU_NURBS_MODE                       100160
#define GLU_NURBS_MODE_EXT                   100160
#define GLU_NURBS_TESSELLATOR                100161
#define GLU_NURBS_TESSELLATOR_EXT            100161
#define GLU_NURBS_RENDERER                   100162
#define GLU_NURBS_RENDERER_EXT               100162

/* NurbsSampling */
#define GLU_OBJECT_PARAMETRIC_ERROR          100208
#define GLU_OBJECT_PARAMETRIC_ERROR_EXT      100208
#define GLU_OBJECT_PATH_LENGTH               100209
#define GLU_OBJECT_PATH_LENGTH_EXT           100209
#define GLU_PATH_LENGTH                      100215
#define GLU_PARAMETRIC_ERROR                 100216
#define GLU_DOMAIN_DISTANCE                  100217

/* NurbsTrim */
#define GLU_MAP1_TRIM_2                      100210
#define GLU_MAP1_TRIM_3                      100211

/* QuadricDrawStyle */ 
#define GLU_POINT                            100010
#define GLU_LINE                             100011
#define GLU_FILL                             100012
#define GLU_SILHOUETTE                       100013
  
/* QuadricCallback */
/*      GLU_ERROR */

/* QuadricNormal */
#define GLU_SMOOTH                           100000
#define GLU_FLAT                             100001
#define GLU_NONE                             100002
 
/* QuadricOrientation */
#define GLU_OUTSIDE                          100020
#define GLU_INSIDE                           100021

/* TessCallback */
#define GLU_TESS_BEGIN                       100100
#define GLU_BEGIN                            100100
#define GLU_TESS_VERTEX                      100101
#define GLU_VERTEX                           100101
#define GLU_TESS_END                         100102
#define GLU_END                              100102
#define GLU_TESS_ERROR                       100103
#define GLU_TESS_EDGE_FLAG                   100104
#define GLU_EDGE_FLAG                        100104
#define GLU_TESS_COMBINE                     100105
#define GLU_TESS_BEGIN_DATA                  100106
#define GLU_TESS_VERTEX_DATA                 100107
#define GLU_TESS_END_DATA                    100108
#define GLU_TESS_ERROR_DATA                  100109
#define GLU_TESS_EDGE_FLAG_DATA              100110
#define GLU_TESS_COMBINE_DATA                100111

/* TessContour */
#define GLU_CW                               100120
#define GLU_CCW                              100121
#define GLU_INTERIOR                         100122
#define GLU_EXTERIOR                         100123
#define GLU_UNKNOWN                          100124

/* TessProperty */
#define GLU_TESS_WINDING_RULE                100140
#define GLU_TESS_BOUNDARY_ONLY               100141
#define GLU_TESS_TOLERANCE                   100142

/* TessError */
#define GLU_TESS_ERROR1                      100151
#define GLU_TESS_ERROR2                      100152
#define GLU_TESS_ERROR3                      100153
#define GLU_TESS_ERROR4                      100154
#define GLU_TESS_ERROR5                      100155
#define GLU_TESS_ERROR6                      100156
#define GLU_TESS_ERROR7                      100157
#define GLU_TESS_ERROR8                      100158
#define GLU_TESS_MISSING_BEGIN_POLYGON       100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR       100152
#define GLU_TESS_MISSING_END_POLYGON         100153
#define GLU_TESS_MISSING_END_CONTOUR         100154
#define GLU_TESS_COORD_TOO_LARGE             100155
#define GLU_TESS_NEED_COMBINE_CALLBACK       100156

/* TessWinding */
#define GLU_TESS_WINDING_ODD                 100130
#define GLU_TESS_WINDING_NONZERO             100131
#define GLU_TESS_WINDING_POSITIVE            100132
#define GLU_TESS_WINDING_NEGATIVE            100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO         100134

/*************************************************************/


#ifdef __cplusplus
class GLUnurbs;
class GLUquadric;
class GLUtesselator;

typedef class GLUnurbs GLUnurbsObj OPENGL_DEPRECATED(10.0, 10.9);
typedef class GLUquadric GLUquadricObj OPENGL_DEPRECATED(10.0, 10.9);
typedef class GLUtesselator GLUtesselatorObj OPENGL_DEPRECATED(10.0, 10.9);
typedef class GLUtesselator GLUtriangulatorObj OPENGL_DEPRECATED(10.0, 10.9);
#else
typedef struct GLUnurbs GLUnurbs OPENGL_DEPRECATED(10.0, 10.9);
typedef struct GLUquadric GLUquadric OPENGL_DEPRECATED(10.0, 10.9);
typedef struct GLUtesselator GLUtesselator OPENGL_DEPRECATED(10.0, 10.9);

typedef struct GLUnurbs GLUnurbsObj OPENGL_DEPRECATED(10.0, 10.9);
typedef struct GLUquadric GLUquadricObj OPENGL_DEPRECATED(10.0, 10.9);
typedef struct GLUtesselator GLUtesselatorObj OPENGL_DEPRECATED(10.0, 10.9);
typedef struct GLUtesselator GLUtriangulatorObj OPENGL_DEPRECATED(10.0, 10.9);
#endif

#define GLU_TESS_MAX_COORD 1.0e150

extern void gluBeginCurve (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluBeginPolygon (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluBeginSurface (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluBeginTrim (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluBuild1DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED_MSG(10_2, 10_9, "Use glGenerateMipmap");
extern GLint gluBuild1DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use glGenerateMipmap");
extern GLint gluBuild2DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED_MSG(10_2, 10_9, "Use glGenerateMipmap");
extern GLint gluBuild2DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use glGenerateMipmap");
extern GLint gluBuild3DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data) OPENGL_DEPRECATED_MSG(10_2, 10_9, "Use glGenerateMipmap");
extern GLint gluBuild3DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) OPENGL_DEPRECATED_MSG(10_2, 10_9, "Use glGenerateMipmap");
extern GLboolean gluCheckExtension (const GLubyte *extName, const GLubyte *extString) OPENGL_DEPRECATED(10.2, 10.9);
extern void gluCylinder (GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluDeleteNurbsRenderer (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluDeleteQuadric (GLUquadric* quad) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluDeleteTess (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluEndCurve (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluEndPolygon (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluEndSurface (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluEndTrim (GLUnurbs* nurb) OPENGL_DEPRECATED(10.0, 10.9);
extern const GLubyte * gluErrorString (GLenum error) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluGetNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat* data) OPENGL_DEPRECATED(10.0, 10.9);
extern const GLubyte * gluGetString (GLenum name) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluGetTessProperty (GLUtesselator* tess, GLenum which, GLdouble* data) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluLoadSamplingMatrices (GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakeLookAt");
extern GLUnurbs* gluNewNurbsRenderer (void) OPENGL_DEPRECATED(10.0, 10.9);
extern GLUquadric* gluNewQuadric (void) OPENGL_DEPRECATED(10.0, 10.9);
extern GLUtesselator* gluNewTess (void) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNextContour (GLUtesselator* tess, GLenum type) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNurbsCallback (GLUnurbs* nurb, GLenum which, GLvoid (*CallBackFunc)()) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNurbsCallbackData (GLUnurbs* nurb, GLvoid* userData) OPENGL_DEPRECATED(10.2, 10.9);
extern void gluNurbsCallbackDataEXT (GLUnurbs* nurb, GLvoid* userData) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNurbsCurve (GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat value) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluNurbsSurface (GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluOrtho2D (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakeOrtho");
extern void gluPartialDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakePerspective");
extern void gluPickMatrix (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluProject (GLdouble objX, GLdouble objY, GLdouble objZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* winX, GLdouble* winY, GLdouble* winZ) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMathProject");
extern void gluPwlCurve (GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluQuadricCallback (GLUquadric* quad, GLenum which, GLvoid (*CallBackFunc)()) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluQuadricNormals (GLUquadric* quad, GLenum normal) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluQuadricOrientation (GLUquadric* quad, GLenum orientation) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluQuadricTexture (GLUquadric* quad, GLboolean texture) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluScaleImage (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluSphere (GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessBeginContour (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessBeginPolygon (GLUtesselator* tess, GLvoid* data) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessCallback (GLUtesselator* tess, GLenum which, GLvoid (*CallBackFunc)()) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessEndContour (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessEndPolygon (GLUtesselator* tess) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessNormal (GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessProperty (GLUtesselator* tess, GLenum which, GLdouble data) OPENGL_DEPRECATED(10.0, 10.9);
extern void gluTessVertex (GLUtesselator* tess, GLdouble *location, GLvoid* data) OPENGL_DEPRECATED(10.0, 10.9);
extern GLint gluUnProject (GLdouble winX, GLdouble winY, GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX, GLdouble* objY, GLdouble* objZ) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMathUnproject");
extern GLint gluUnProject4 (GLdouble winX, GLdouble winY, GLdouble winZ, GLdouble clipW, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble nearPlane, GLdouble farPlane, GLdouble* objX, GLdouble* objY, GLdouble* objZ, GLdouble* objW) OPENGL_DEPRECATED_MSG(10_2, 10_9, "Use GLKMathUnproject");

#ifdef __cplusplus
}
#endif

#endif /* __glu_h__ */
