from OpenGL.GLUT import *
from OpenGL.GL import *
import sys
import ctypes
import numpy as np
scale = 1.0
def voidNumber(num):
    return ctypes.c_void_p(0
    )
vertex_code='''
attribute vec3 position;
attribute vec4 color;
uniform float scale;
varying vec4 v_color;
void main()
{
    gl_Position = vec4(position*scale,1.0);
    v_color = color;
}
'''
fragment_code = '''

varying vec4 v_color;
void main()
{
gl_FragColor = v_color;
}
'''
def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) # clear the screen
    glLoadIdentity()
    print "Hello"
    glutSwapBuffers()
def reshape(width,height):
    glViewport(0,0,width,height)
def keyboard(key,x,y):
    if key=='\033':
        sys.exit()
def createShaders():
    program = glCreateProgram()
    vertex = glCreateShader(GL_VERTEX_SHADER)
    fragment=glCreateShader(GL_FRAGMENT_SHADER)
    glShaderSource(vertex,vertex_code)
    glShaderSource(fragment,fragment_code)
    glCompileShader(vertex)
    result = glGetShaderiv(vertex,GL_COMPILE_STATUS)
    if not(result):
        raise RuntimeError(glGetShaderInfoLog(vertex))
    glCompileShader(fragment)
    result = glGetShaderiv(fragment,GL_COMPILE_STATUS)
    if not(result):
            raise RuntimeError(glGetShaderInfoLog(fragment))
    glAttachShader(program,vertex)
    glAttachShader(program,fragment)
    glLinkProgram(program)
    result = glGetProgramiv(program,GL_LINK_STATUS)
    if not result:
        raise RuntimeError(glGetProgramInfoLog(program))
    glDetachShader(program,vertex)
    glDetachShader(program,fragment)
    return program
glutInit()

glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA)
glutCreateWindow("Hello World")
glutReshapeWindow(600,600)
glutReshapeFunc(reshape)
glutDisplayFunc(display)
glutKeyboardFunc(keyboard)
program = createShaders()

glutMainLoop()
