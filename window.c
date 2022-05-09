/*!\file window.c
 * \brief géométries lumière diffuse et transformations de base en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 */
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>

/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void resize(int w, int h);
static void draw(void);
static void quit(void);
static void spaceship_init(void);
static void draw_car(void);
static void key(int keycode);

void animation(void);
static int _wW = 800, _wH = 600;
static GLuint _pId = 0;
static GLuint _sphere = 0, _cube = 0, _bullet;
static GLuint _vao = 0;
static GLuint _buffer[2] = {0};
static GLuint _buffer2 = 0;

static GLuint _pId2 = 0;
static GLuint _pId3 = 0;

static float i = 0;
static float velocity = 0.01f;
static float size_i;
static float acc = 0.001f;
GLfloat bleu[] = {0.4f,0.4f,0.8f,1}; 
GLfloat orange[] = {1.0f,0.6f,0.6f,5};
GLfloat rose[] = {1,0.6f,1,1};
GLfloat violet[] = {0.4f, 0.4f, 0.6f,1};

typedef struct spaceship spaceship;
struct spaceship {
    GLfloat x, y, z;
};
static spaceship _spaceship = {0,0,-20};
typedef struct particles particles;
struct particles {GLfloat x, y, z,i;};
static particles _particles[20];
typedef struct stars stars;
struct stars {GLfloat x, y, z, size_x, size_y, size_z;};
static stars _stars[20];
typedef struct cam cam;
struct cam {GLfloat a, b, c,d,e,f,g,h,i;};

static cam _cam = {0,0,-40,0,0,0,0,1,0};

int main(int argc, char ** argv) {
    if (!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0,
            _wW, _wH, GL4DW_SHOWN))
        return 1;
    init();
    spaceship_init();
    atexit(quit);
    gl4duwResizeFunc(resize);
    gl4duwDisplayFunc(draw);
    //gl4duwKeyDownFunc(key);
    gl4duwMainLoop();
    return 0;
}

static void init(void) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.96f, 3.52f, 7.05f, 1.0f);
    _pId = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
    _pId3 = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d_2.fs", NULL);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    resize(_wW, _wH);
    _cube = gl4dgGenCubef();
    _bullet = gl4dgGenCubef();
    _sphere = gl4dgGenSpheref(30, 30);
}

static void resize(int w, int h) {
    _wW = w;
    _wH = h;
    glViewport(0, 0, _wW, _wH);
    gl4duBindMatrix("projectionMatrix");
    gl4duLoadIdentityf();
    gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
    gl4duBindMatrix("modelViewMatrix");
}

static void spaceship_init(void) {
    GLuint idata[696];
    for (int i = 0; i < 696; i++) {
        idata[i] = i;
    }
    for (int i = 0; i < 20; i++) {
        _stars[i].x = (float) rand() / RAND_MAX * 80 - 40;
        _stars[i].y = (float) rand() / RAND_MAX * 80 - 40;
        _stars[i].z = (float) rand() / RAND_MAX * 80 - 40;
        _stars[i].size_x = (float) rand() / RAND_MAX * 0.5 + 0.5;
        _stars[i].size_y = (float) rand() / RAND_MAX * 0.5 + 0.5;
        _stars[i].size_z = (float) rand() / RAND_MAX * 0.5 + 0.5;
        _particles[i].i = (float) rand() / (float) RAND_MAX * 0.05f + 0.05f;
        _particles[i].y = (float) rand() / (float) RAND_MAX * 10 - 5;
        _particles[i].x = (float) rand() / (float) RAND_MAX * 10 - 5;

    }
    GLfloat data[] = {
    /* 4 sommets de la face de devant               0   */   
    2, -0.1, -0.1,  0.6f,0.6f,0.6f,
    2, 0.1, -0.1, 0.6f,0.6f,0.6f,
    2, -0.1, 0.1,  0.6f,0.6f,0.6f,
    2, 0.1, 0.1, 0.6f,0.6f,0.6f,
    /* 4 sommets de la face de devant coté gauche   4*/
    2, -0.1, 0.1,  0.4f, 0.4f, 0.6f,
    2, -0.1, -0.1,  0.4f, 0.4f, 0.6f,
    0, -0.4, 0.5,  0.3f, 0.3f, 0.4f,
    0, -0.5, -0.2,  0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face de devant coté droit    8*/
    2, 0.1, 0.1,  0.4f, 0.4f, 0.6f,
    2, 0.1, -0.1,  0.4f, 0.4f, 0.6f,
    0, 0.4, 0.5,  0.3f, 0.3f, 0.4f,
    0, 0.4, -0.2, 0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face de devant coté bas     12*/
    2, 0.1, -0.1,  0.4f, 0.4f, 0.6f,
    2, -0.1, -0.1,  0.4f, 0.4f, 0.6f,
    0, 0.5, -0.2,  0.3f, 0.3f, 0.4f,
    0, -0.5, -0.2,  0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face de devant coté haut     12*/
    2, 0.1, 0.1,  0.4f, 0.4f, 0.6f,
    2, -0.1, 0.1,  0.4f, 0.4f, 0.6f,
    0, 0.4, 0.5,   0.3f, 0.3f, 0.4f,
    0, -0.4, 0.5,   0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face du milieu gauche             16*/
    0, -0.4, 0.5, 0.3f, 0.3f, 0.4f,
    0, -0.5, -0.2, 0.3f, 0.3f, 0.4f,
    -1, -0.8, 0.8, 0.3f, 0.3f, 0.4f,
    -1, -0.8, -0.2, 0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face du milieu droite             20*/
    0, 0.4, 0.5, 0.3f, 0.3f, 0.4f,
    0, 0.5, -0.2, 0.3f, 0.3f, 0.4f,
    -1, 0.8, 0.8, 0.3f, 0.3f, 0.4f,
    -1, 0.8, -0.2, 0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face du milieu haut             24*/
    -1, -0.8, 0.8,  0.3f, 0.3f, 0.4f,
    0,-0.4,0.5, 0.3f, 0.3f, 0.4f,
    -1,0.8,0.8, 0.3f, 0.3f, 0.4f,
    0,0.4,0.5, 0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face du milieu bas             28*/
    -1, 0.8, -0.2, 0.3f, 0.3f, 0.4f,
    0,0.5,-0.2,0.3f, 0.3f, 0.4f,
    -1,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    0,-0.5,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets de la face du milieu                 32*/
    0,-0.4,0.5,0.3f, 0.3f, 0.4f,
    0,-0.5,-0.2,0.3f, 0.3f, 0.4f,
    0,0.4,0.5,0.3f, 0.3f, 0.4f,
    0,0.5,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets central haut    haut                     36*/
    -2.5,-0.8,0.8,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.8,0.3f, 0.3f, 0.4f,
    -2.5,0.8,0.8,0.3f, 0.3f, 0.4f,
    -1,0.8,0.8,0.3f, 0.3f, 0.4f,
    /* 4 sommets central haut bas                         40*/
    -2.5,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -2.5,0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,0.8,0.3,0.3f, 0.3f, 0.4f,
    /* 4 sommets central haut  gauche                       44*/
    -2.5,-0.8,0.8,0.3f, 0.3f, 0.4f,
    -2.5,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.8,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.3,0.3f, 0.3f, 0.4f,
    /* 4 sommets central haut droit                     48*/
    -2.5,0.8,0.8,0.3f, 0.3f, 0.4f,
    -2.5,0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,0.8,0.8,0.3f, 0.3f, 0.4f,
    -1,0.8,0.3,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile gauche haut                         52*/
    -2.5,-3,0.3,0.3f, 0.3f, 0.4f,
    -2,-3,0.3,0.3f, 0.3f, 0.4f,
    -2.5,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.3,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile gauche bas                         56*/
    -2.5,-3,0,0.3f, 0.3f, 0.4f,
    -2,-3,0,0.3f, 0.3f, 0.4f,
    -2.5,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    -1,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile gauche devant                         60*/
    -2,-3,0.3,0.3f, 0.3f, 0.4f,
    -2,-3,0,0.3f, 0.3f, 0.4f,
    -1,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile gauche derriere                         64*/
    -2.5,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -2.5,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,-3,0.3,0.3f, 0.3f, 0.4f,
    -2.5,-3,0,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile droit haut                         68*/
    -2.5,3,0.3,0.3f, 0.3f, 0.4f,
    -2,3,0.3,0.3f, 0.3f, 0.4f,
    -2.5,0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,0.8,0.3,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile droit bas                         56*/
    -2.5,3,0,0.3f, 0.3f, 0.4f,
    -2,3,0,0.3f, 0.3f, 0.4f,
    -2.5,0.8,-0.2,0.3f, 0.3f, 0.4f,
    -1,0.8,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile droit devant                         60*/
    -2,3,0.3,0.3f, 0.3f, 0.4f,
    -2,3,0,0.3f, 0.3f, 0.4f,
    -1,0.8,0.3,0.3f, 0.3f, 0.4f,
    -1,0.8,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets aile gauche derriere                         64*/
    -2.5,0.8,0.3,0.3f, 0.3f, 0.4f,
    -2.5,0.8,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,3,0.3,0.3f, 0.3f, 0.4f,
    -2.5,3,0,0.3f, 0.3f, 0.4f,
    /* 4 sommets central milieu bas                         40*/
    -2.5,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    -1,-0.8,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,0.8,-0.2,0.3f, 0.3f, 0.4f,
    -1,0.8,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets trou derriere                  44*/
    -3,-0.2,0.2,0.3f, 0.3f, 0.4f,
    -3,-0.2,-0.2,0.3f, 0.3f, 0.4f,
    -3,0.2,0.2,0.3f, 0.3f, 0.4f,
    -3,0.2,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets trou gauche                  48*/
    -3,-0.2,0.2,0.3f, 0.3f, 0.4f,
    -3,-0.2,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,-0.2,0.2,0.3f, 0.3f, 0.4f,
    -2.5,-0.2,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets trou droit                  52*/
    -3,0.2,0.2,0.3f, 0.3f, 0.4f,
    -3,0.2,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,0.2,0.2,0.3f, 0.3f, 0.4f,
    -2.5,0.2,-0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets trou haut                  56*/
    -3,-0.2,0.2,0.3f, 0.3f, 0.4f,
    -3,0.2,0.2,0.3f, 0.3f, 0.4f,
    -2.5,-0.2,0.2,0.3f, 0.3f, 0.4f,
    -2.5,0.2,0.2,0.3f, 0.3f, 0.4f,
    /* 4 sommets trou bas                  60*/
    -3,-0.2,-0.2,0.3f, 0.3f, 0.4f,
    -3,0.2,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,-0.2,-0.2,0.3f, 0.3f, 0.4f,
    -2.5,0.2,-0.2,0.3f, 0.3f, 0.4f,

    -2.5,-0.8,0.3,0.3f, 0.3f, 0.4f,
    -2.5, -0.8, -0.2,0.3f, 0.3f, 0.4f,
    -2.5, -3, 0.3,0.3f, 0.3f, 0.4f,
    -2.5, -3, 0,0.3f, 0.3f, 0.4f,

    -2.5, 0.8, 0.8,0.3f, 0.3f, 0.4f,
    -2.5, 0.8, -0.2,0.3f, 0.3f, 0.4f,
    -2.5, -0.8, 0.8,0.3f, 0.3f, 0.4f,
    -2.5, -0.8, -0.2,0.3f, 0.3f, 0.4f,
  };
    _pId2 = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
    glClearColor(0.01f, 0.01f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    resize(_wW, _wH);
    glGenVertexArrays(1, & _vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glGenBuffers(2, _buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof * data, (const void * ) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof * data, (const void * )(3 * sizeof * data));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idata, idata, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void animation(void){
 i += velocity;
    velocity -= acc;
    if (i > 10) {
        velocity = 0.001f;
        acc = -acc;
        printf("%f\n", acc);
        i = 9.90;
    } else if (i < -20) {
        velocity = 0.01f;
        acc = -acc;
        i = -19.99;
    }
     _spaceship.z -= 0.08f;
    _spaceship.x += 0.06f;

    _cam.c -= 0.08f;
    _cam.a += 0.09f;
    _cam.d -= 0.02f;
}

static void draw(void) {
    animation();
    static GLfloat pos_ship = 0.0f;
    static GLfloat size = 0.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duLookAtf(_cam.a, _cam.b, _cam.c, _cam.d, _cam.e, _cam.f, _cam.g, _cam.h, _cam.i);
    glUseProgram(_pId);

    gl4duTranslatef(5, 5, -50);
    gl4duPushMatrix(); {
          gl4duTranslatef(5, 5, 50);

        gl4duScalef(10.0f, 10.0f,10.0f);
        gl4duSendMatrices();
    }
    gl4duPopMatrix();
    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, orange);
    gl4dgDraw(_sphere);


    for (int i = 0; i < 10; i++) {
        gl4duPushMatrix(); {
            gl4duTranslatef(_stars[i].x, _stars[i].y, _stars[i].z);
            gl4duScalef(0.5f, 0.5f, 0.5f);
            gl4duSendMatrices();
        }
        gl4duPopMatrix();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, rose);
        gl4dgDraw(_sphere);
    }
    for (int i = 10; i < 20; i++) {
        gl4duPushMatrix(); {
            gl4duTranslatef(_stars[i].x, _stars[i].y, _stars[i].z);
            gl4duScalef(0.5f, 0.5f, 0.5f);
            gl4duSendMatrices();
        }
        gl4duPopMatrix();
        glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, orange);
        gl4dgDraw(_sphere);
    }
    //random max_particle between 0 and 2
    for (int i = 0; i < 20; i++) {
        _particles[i].x -= _particles[i].i;
        if (_particles[i].x < -8) {
            size = 0.0f;
            _particles[i].i = (float) rand() / (float) RAND_MAX * 0.05f + 0.05f;
            _particles[i].x = -5;

            //random yparticle between -5 and 5 float
            _particles[i].y = (float) rand() / (float) RAND_MAX * 10 - 5;
            
            if (size < 5) {
                size += 0.02f;
            }
        }
    }
    gl4duLoadIdentityf();
    gl4duSendMatrices();
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duLookAtf(_cam.a, _cam.b, _cam.c, _cam.d, _cam.e, _cam.f, _cam.g, _cam.h, _cam.i);
    glUseProgram(_pId2);
   
    gl4duTranslatef(_spaceship.x, _spaceship.y, _spaceship.z);
    gl4duPushMatrix(); {

        gl4duRotatef(280, 1, 0, 0);
        gl4duRotatef(60, 0, 0, 1);
        gl4duRotatef(i, 1, 0, 0);
        gl4duScalef(1, 1, 1);
        for (int i = 0; i < 10; i++) {
              glUseProgram(_pId3);
            gl4duPushMatrix(); {
                gl4duTranslatef(_particles[i].x, _particles[i].y, 0);
                gl4duScalef(size * 2, size * 2, size * 2);
                gl4duSendMatrices();
            }
            gl4duPopMatrix();
            glUniform4fv(glGetUniformLocation(_pId3, "couleur"), 1, violet);
            gl4dgDraw(_cube);
        }
            glUseProgram(_pId2);
        gl4duSendMatrices();
    }
    gl4duPopMatrix();

    glLineWidth(3.0f);

    glBindVertexArray(_vao);
    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(0 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(4 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(8 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(12 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(16 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(20 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(24 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(28 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(32 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(36 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(40 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(44 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(48 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(52 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(56 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(60 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(64 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(68 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(72 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(76 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(80 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(84 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(88 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(92 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(96 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(100 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(104 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(108 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(112 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid * )(116 * sizeof(GLuint)));

    gl4duSendMatrices();
    /* Dessiner le VAO comme une bande d'un triangle avec 3 sommets
     * commençant à 0
     *
     * Attention ! Maintenant nous dessinons avec DrawElement qui
     * utilise les indices des sommets poassés pour mailler */
    /* dé-lier le VAO */
    glBindVertexArray(0);
    /* désactiver le programme shader */
    glUseProgram(0);
}

/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
    /* suppression du VAO _vao en GPU */
    if (_vao)
        glDeleteVertexArrays(1, & _vao);
    /* suppression du VBO _buffer en GPU, maintenant il y en a deux */
    if (_buffer[0])
        glDeleteBuffers(2, _buffer);
    /* nettoyage des éléments utilisés par la bibliothèque GL4Dummies */
    gl4duClean(GL4DU_ALL);
}