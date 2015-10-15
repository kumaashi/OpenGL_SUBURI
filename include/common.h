#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//OpenGL
#include <gl/gl.h>
#include <gl/glext.h>

//STL
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "vecmat.h"

//--------------------------------------------------------------------------------------
// OpenGL Function
//--------------------------------------------------------------------------------------
extern PFNGLDEBUGMESSAGECALLBACKPROC           glDebugMessageCallback      ;
extern PFNGLCREATEPROGRAMPROC                  glCreateProgram             ;
extern PFNGLDELETEPROGRAMPROC                  glDeleteProgram             ;
extern PFNGLCREATESHADERPROC                   glCreateShader              ;
extern PFNGLSHADERSOURCEPROC                   glShaderSource              ;
extern PFNGLCOMPILESHADERPROC                  glCompileShader             ;
extern PFNGLATTACHSHADERPROC                   glAttachShader              ;
extern PFNGLDELETESHADERPROC                   glDeleteShader              ;
extern PFNGLLINKPROGRAMPROC                    glLinkProgram               ;
extern PFNGLUSEPROGRAMPROC                     glUseProgram                ;
extern PFNGLUNIFORM1FPROC                      glUniform1f                 ;
extern PFNGLUNIFORM1IPROC                      glUniform1i                 ;
extern PFNGLUNIFORM4FVPROC                     glUniform4fv                ;
extern PFNGLUNIFORMMATRIX4FVPROC               glUniformMatrix4fv          ;
extern PFNGLGENBUFFERSPROC                     glGenBuffers                ;
extern PFNGLDELETEBUFFERSPROC                  glDeleteBuffers             ;
extern PFNGLBINDBUFFERPROC                     glBindBuffer                ;
extern PFNGLBINDBUFFERBASEPROC                 glBindBufferBase            ;
extern PFNGLBUFFERDATAPROC                     glBufferData                ;
extern PFNGLGETUNIFORMLOCATIONPROC             glGetUniformLocation        ;
extern PFNGLGETBUFFERSUBDATAPROC               glGetBufferSubData          ;
extern PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog          ;
extern PFNGLGETSHADERIVPROC                    glGetShaderiv               ;
extern PFNGLGETPROGRAMIVPROC                   glGetProgramiv              ;
extern PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog         ;
extern PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation         ;
//extern PFNGLGETINTEGERVPROC                    glGetIntegerv               ;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC      glTransformFeedbackVaryings ;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC         glBeginTransformFeedback    ;
extern PFNGLENDTRANSFORMFEEDBACKPROC           glEndTransformFeedback      ;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC        glEnableVertexAttribArray   ;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC       glDisableVertexAttribArray  ;
extern PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer       ;
extern PFNGLTEXIMAGE3DPROC                     glTexImage3D                ;
extern PFNGLPROGRAMUNIFORM1IPROC               glProgramUniform1i          ;
extern PFNGLACTIVETEXTUREPROC                  glActiveTexture             ;
extern PFNGLBINDVERTEXARRAYPROC                glBindVertexArray           ;
extern PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation        ;
extern PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays           ;
extern PFNGLDELETEVERTEXARRAYSPROC             glDeleteVertexArrays        ;
extern PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers           ;
extern PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer           ;
extern PFNGLGENRENDERBUFFERSPROC               glGenRenderBuffers          ;
extern PFNGLBINDRENDERBUFFERPROC               glBindRenderBuffer          ;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC        glFramebufferRenderbuffer   ;
extern PFNGLRENDERBUFFERSTORAGEPROC            glRenderBufferStorage       ;
extern PFNGLFRAMEBUFFERTEXTUREPROC             glFramebufferTexture        ;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC           glFramebufferTexture2D      ;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC         glCheckFramebufferStatus    ;
extern PFNGLDRAWBUFFERSPROC                    glDrawBuffers               ;
extern PFNGLGENERATEMIPMAPPROC                 glGenerateMipmap            ;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC          glTexImage2DMultisample     ;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
extern PFNGLRENDERBUFFERSTORAGEPROC            glRenderbufferStorage;
extern PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer           ;
extern PFNGLDEBUGMESSAGECALLBACKPROC           glDebugMessageCallback      ;


void   glInitFunc();

#endif
