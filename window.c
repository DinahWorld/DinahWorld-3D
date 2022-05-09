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

/*!\brief dimensions de la fenêtre */
static int _wW = 800, _wH = 600;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint _sphere = 0, _cube = 0, _quad = 0, _torus = 0 , _car = 0;
/*!\brief identifiant du (futur) Vertex Array Object */
static GLuint _vao = 0;
/*!\brief identifiant du (futur) buffer de data - VBO ou Vertex Buffer Object */
static GLuint _buffer[2] = { 0 };
static GLuint _buffer2 = 0;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId2 = 0;
static GLuint _pId3 = 0;
static float i = 0;
static float velocity = 0.01f;
static float acc = 0.001f;
static float xparticle[5] = {-5,-5,-5,-5,-5};
static float xparticle_i[5] = {0.5f,0.5f,0.5f,0.5f,0.5f};
static float yparticle[5] = {0,0,0,0,0};
GLfloat rouge[] = {1, 0, 0, 1}, vert[] = {0, 1, 0, 1}, bleu[] = {0.4f, 0.4f, 0.8f, 1}, jaune[] = {1, 1, 0, 1};
GLfloat orange[] = {1.0f, 0.6f, 0.6f, 5};
GLfloat rose[] = {1,0.6f, 1};


// create a constant

typedef struct spaceship spaceship;
/*!\brief a data structure for storing camera position and
 * orientation */
struct spaceship {
  GLfloat x, y, z;
};
static spaceship _spaceship = {0, 0, -20};

typedef struct stars stars;
/*!\brief a data structure for storing camera position and
 * orientation */
struct stars {
  GLfloat x, y, z,size_x,size_y,size_z;; 
};
/*!\brief the used camera */
static stars _stars[20];

typedef struct cam cam;
/*!\brief a data structure for storing camera position and
 * orientation */
struct cam {
  GLfloat x, y, z;
};

/*!\brief the used camera */
static cam _cam = {0, 0, 10};

/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  spaceship_init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwKeyDownFunc(key);
  gl4duwMainLoop();
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.96f, 3.52f, 7.05f, 1.0f);
  _pId  = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
  _pId3 = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d_2.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_wW, _wH);
  _cube = gl4dgGenCubef();
  _sphere = gl4dgGenSpheref(30, 30);
  //https://gl4d.api8.fr/doxygen/html/HTML/S/59.html#L551
  _quad = gl4dgGenQuadf();
  _torus = gl4dgGenTorusf(300, 30, 0.1f);
}
/*!\brief Cette fonction paramétre la vue (viewport) OpenGL en
 * fonction des dimensions de la fenêtre.*/
static void resize(int w, int h) {
  _wW  = w; _wH = h;
  glViewport(0, 0, _wW, _wH);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
  //gl4duOrthof(-3.5, 3.5, -3.5 * _wH / _wW, 3.5 * _wH / _wW, 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
}

/*!\brief initialise les paramètres OpenGL et les données. */
static void spaceship_init(void) {
  GLuint idata[696];
  for(int i = 0; i < 696; i++) {
    idata[i] = i;
  }
  for(int i = 0; i < 20; i++) {    
    // random x position between -20 and 20

    _stars[i].x = (float)rand()/RAND_MAX * 80 - 40;
    _stars[i].y = (float)rand()/RAND_MAX * 80 - 40;
    _stars[i].z = (float)rand()/RAND_MAX * 80 - 40;
    _stars[i].size_x = (float)rand()/RAND_MAX * 0.5 + 0.5;
    _stars[i].size_y = (float)rand()/RAND_MAX * 0.5 + 0.5;
    _stars[i].size_z = (float)rand()/RAND_MAX * 0.5 + 0.5;

  }
  /* données-sommets envoyée dans le VBO ARRAY_BUFFER */
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
  /* Création du programme shader (voir le dossier shader) */
  _pId2 = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/basic.fs", NULL);
  /* Set de la couleur (RGBA) d'effacement OpenGL */
  glClearColor(0.01f, 0.01f, 0.08f, 1.0f);

  /* activation du test de profondeur afin de prendre en compte la
   * notion de devant-derrière. */
  glEnable(GL_DEPTH_TEST);
  /* activation du lissage de dessin de lignes ; permet d'utiliser
   * aussi le glLineWidth (voir dans le draw) */
  glEnable(GL_LINE_SMOOTH);
  /* décommenter la ligne du bas pour l'activation de l'option
   * permettant de cacher les faces arrières */
  /* glEnable(GL_CULL_FACE); */
  /* décommenter la ligne du bas si inversion du sens pour face
   * avant */
  /* glFrontFace(GL_CW); */
  /* Création des matrices GL4Dummies, une pour la projection, une
   * pour la modélisation et une pour la vue */
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_wW, _wH);
  /* Génération d'un identifiant de VAO */
  glGenVertexArrays(1, &_vao);
  /* Lier le VAO-machine-GL à l'identifiant VAO généré */
  glBindVertexArray(_vao);
  /* Activation des 2 premiers indices d'attribut de sommet */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  /* Génération de deux identifiants de VBO un pour ARRAY_BUFFER, un
   * pour ELEMENT_ARRAY_BUFFER */
    glGenBuffers(2, _buffer);
  /* Lier le VBO-ARRAY_BUFFER à l'identifiant du premier VBO généré */
  glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
  /* Transfert des données VBO-ARRAY_BUFFER */
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
  /* Paramétrage 2 premiers indices d'attribut de sommet */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof *data, (const void *)0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof *data, (const void *)(3 * sizeof *data));
  /* Lier le VBO-ELEMENT_ARRAY_BUFFER à l'identifiant du second VBO généré */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[1]);
  /* Transfert des données d'indices VBO-ELEMENT_ARRAY_BUFFER */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idata, idata, GL_STATIC_DRAW);
  /* dé-lier le VAO puis les VAO */
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {
  // LookAt

      static GLfloat pos_ship = 0.0f;
    static GLfloat size = 0.1f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(_pId);
      gl4duLookAtf(10, 0, 0, 0 , 1.0, 0 , 0.0, 0.0,0.0);

  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  
  // Circle
 gl4duTranslatef(5, 5, -50);
  gl4duPushMatrix(); {
    gl4duScalef(2.0f, 2.0f, 2.0f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, orange);
  gl4dgDraw(_sphere);

  for(int i = 0; i < 10; i++) {
    // sphere with random values
    gl4duPushMatrix(); {
      gl4duTranslatef(_stars[i].x, _stars[i].y, _stars[i].z);
      gl4duScalef(0.5f, 0.5f, 0.5f);
      gl4duSendMatrices();
    } gl4duPopMatrix();
    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, rose);
    gl4dgDraw(_sphere);
  }
   for(int i = 10; i < 20; i++) {
    // sphere with random values
    gl4duPushMatrix(); {
      gl4duTranslatef(_stars[i].x, _stars[i].y, _stars[i].z);
      gl4duScalef(0.5f, 0.5f, 0.5f);
      gl4duSendMatrices();
    } gl4duPopMatrix();
    glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, orange);
    gl4dgDraw(_sphere);
  }
  //random max_particle between 0 and 2
  
  
  for(int i = 0;i < 3;i++){
    xparticle[i] -= xparticle_i[i]; 
    if (xparticle[i] < -5) {
      size = 0.0f;
    
      // random between 0.05 and 0.08 float
      xparticle_i[i] = (float)rand() / (float)RAND_MAX * 0.05f + 0.05f;      
     xparticle[i] = -3;
    
    //random yparticle between -1 and 2 float
      yparticle[i] = (float)rand()/RAND_MAX * 2 - 1;
      }
  }

  if(size < 0.1f){
  size += 0.005f;
  }

  /*
  _spaceship.z -= velocity;
  _spaceship.x += velocity;
  _spaceship.y += velocity;
  */
  i += velocity;
  velocity += acc;
  if(i > 10){
      velocity = 0.001f;
      acc = -acc;
      printf("%f\n", acc);
      i = 9.90;
  }else  if(i < -10){
          velocity = 0.001f;
          acc = -acc;
          i = -9.90;
      
  }
  /* lier (mettre en avant ou "courante") la matrice vue créée dans
   * init */
  gl4duLoadIdentityf();
  gl4duSendMatrices();
  glUseProgram(_pId2);

  gl4duTranslatef(_spaceship.x, _spaceship.y, _spaceship.z);
  gl4duPushMatrix(); {

    gl4duRotatef(280, 1, 0, 0);
    gl4duRotatef(60, 0, 0, 1);
    gl4duRotatef(i, 1, 0, 0);
    gl4duScalef(1,1, 1);
    for(int i = 0; i < 3; i++){
    gl4duPushMatrix(); {
      gl4duTranslatef(xparticle[i], yparticle[i], 0);
      gl4duScalef(size, size, size);
      gl4duSendMatrices();
    } gl4duPopMatrix();
    gl4dgDraw(_cube);
    }
    gl4duSendMatrices();
  } gl4duPopMatrix();

  glUniform4fv(glGetUniformLocation(_pId2, "couleur"), 1, bleu);

  gl4duBindMatrix("modelViewMatrix");

  glLineWidth(3.0f);

  /* Lier le VAO-machine-GL à l'identifiant VAO _vao */
  glBindVertexArray(_vao);


    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(0 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(4 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(8 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(12 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(16 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(20 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(24 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(28 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(32 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(36 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(40 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(44 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(48 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(52 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(56 * sizeof(GLuint))); 
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(60 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(64 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(68 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(72 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(76 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(80 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(84 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(88 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(92 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(96 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(100 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(104 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(108 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(112 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid *)(116 * sizeof(GLuint)));

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
  if(_vao)
    glDeleteVertexArrays(1, &_vao);
  /* suppression du VBO _buffer en GPU, maintenant il y en a deux */
  if(_buffer[0])
    glDeleteBuffers(2, _buffer);
  /* nettoyage des éléments utilisés par la bibliothèque GL4Dummies */
  gl4duClean(GL4DU_ALL);
}


void key(int keycode) {
  //move spaceship
  if(keycode == GL4DK_LEFT) {
        _cam.y -= 0.01;

  }
  if(keycode == GL4DK_RIGHT) {
        _cam.y += 0.01;

  }

  //move spaceship
  if(keycode == GL4DK_UP) {
        _cam.x += 2;
  }
  if(keycode == GL4DK_DOWN) {
    _cam.x += 2;
  }
  //move spaceship

}