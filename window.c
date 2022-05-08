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
static void car(void);
static void draw_car(void);


/*!\brief dimensions de la fenêtre */
static int _wW = 800, _wH = 600;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint _sphere = 0, _cube = 0, _quad = 0, _torus = 0 , _car = 0;
/*!\brief identifiant du (futur) Vertex Array Object */
static GLuint _vao = 0;
/*!\brief identifiant du (futur) buffer de data - VBO ou Vertex Buffer Object */
static GLuint _buffer = 0;
/*!\brief identifiant du (futur) GLSL program */
static GLuint _pId2 = 0;
static int i = 0;
static int iz = 0;


/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;
  init();
  car();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(1.0f, 0.7f, 0.7f, 0.0f);
  _pId  = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_wW, _wH);
  _cube = gl4dgGenQuadf();
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
static void car(void) {
  /* données-sommets envoyée dans le VBO ARRAY_BUFFER */
  GLfloat data[] = {
    /* 4 sommets de la face de devant               0   */   
    2, -0.2, -0.1, 0, 0, 0,
    2, 0.2, -0.1, 0, 0, 0,
    2, -0.2, 0.2, 0, 0, 0,
    2, 0.2, 0.2, 0, 0, 0,
    /* 4 sommets de la face de devant coté gauche   4*/
    2, -0.2, 0.2, 0, 0, 0,
    2, -0.2, -0.1, 0, 0, 0,
    0, -0.5, -0.2, 0, 0, 0,
    0, -0.4, 0.5, 0, 0, 0,
    /* 4 sommets de la face de devant coté droit    8*/
    2, 0.2, 0.2, 0, 0, 0,
    2, 0.2, -0.1, 0, 0, 0,
    0, 0.5, -0.2, 0, 0, 0,
    0, 0.4, 0.5, 0, 0, 0,
    /* 4 sommets de la face de devant coté haut     12*/
    2, 0.2, 0.2, 0, 0, 0,
    2, -0.2, 0.2, 0, 0, 0,
    0, -0.4, 0.5, 0, 0, 0,
    0, 0.4, 0.5, 0, 0, 0,
    /* 4 sommets de du milieu                       14*/
    0, 0.4, 0.5, 0, 0, 0,
    0, 0.5, -0.2, 0, 0, 0,
    0, -0.5, -0.2, 0, 0, 0,
    0, -0.4, 0.5, 0, 0, 0,
    /* 4 sommets de la face du milieu gauche             16*/
    0, -0.4, 0.5, 0, 0, 0,
    0, -0.5, -0.2, 0, 0, 0,
    -1, -0.8, -0.2, 0, 0, 0,
    -1, -0.8, 0.8, 0, 0, 0,
    /* 4 sommets de la face du milieu droite             20*/
    0, 0.4, 0.5, 0, 0, 0,
    0, 0.5, -0.2, 0, 0, 0,
    -1, 0.8, -0.2, 0, 0, 0,
    -1, 0.8, 0.8, 0, 0, 0,
    /* 4 sommets de la face du milieu haut             24*/
    -1, -0.8, 0.8, 0, 0, 0,
    0,-0.4,0.5,0,0,0,
    0,0.4,0.5,0,0,0,
    -1,0.8,0.8,0,0,0,
    /* 4 sommets de la face du milieu bas             28*/
    -1, 0.8, -0.2, 0, 0, 0,
    0,0.5,-0.2,0,0,0,
    0,-0.5,-0.2,0,0,0,
    -1,-0.8,-0.2,0,0,0,
    /* 4 sommets de la face du milieu                 32*/
    0,-0.4,0.5,0,0,0,
    0,-0.5,-0.2,0,0,0,
    0,0.5,-0.2,0,0,0,
    0,0.4,0.5,0,0,0,
    /* 4 sommets central haut    haut                     36*/
    -2.5,-0.8,0.8,0,0,0,
    -1,-0.8,0.8,0,0,0,
    -1,0.8,0.8,0,0,0,
    -2.5,0.8,0.8,0,0,0,
    /* 4 sommets central haut bas                         40*/
    -2.5,-0.8,0.3,0,0,0,
    -1,-0.8,0.3,0,0,0,
    -1,0.8,0.3,0,0,0,
    -2.5,0.8,0.3,0,0,0,
    /* 4 sommets central haut  gauche                       44*/
    -2.5,-0.8,0.8,0,0,0,
    -2.5,-0.8,0.3,0,0,0,
    -1,-0.8,0.3,0,0,0,
    -1,-0.8,0.8,0,0,0,
    /* 4 sommets central haut droit                     48*/
    -2.5,0.8,0.8,0,0,0,
    -2.5,0.8,0.3,0,0,0,
    -1,0.8,0.3,0,0,0,
    -1,0.8,0.8,0,0,0,
    /* 4 sommets aile gauche haut                         52*/
    -2.5,-3,0.3,0,0,0,
    -2,-3,0.3,0,0,0,
    -1,-0.8,0.3,0,0,0,
    -2.5,-0.8,0.3,0,0,0,
    /* 4 sommets aile gauche bas                         56*/
    -2.5,-3,0,0,0,0,
    -2,-3,0,0,0,0,
    -1,-0.8,-0.2,0,0,0,
    -2.5,-0.8,-0.2,0,0,0,
    /* 4 sommets aile gauche devant                         60*/
    -2,-3,0.3,0,0,0,
    -2,-3,0,0,0,0,
    -1,-0.8,-0.2,0,0,0,
    -1,-0.8,0.3,0,0,0,
    /* 4 sommets aile gauche derriere                         64*/
    -2.5,-0.8,0.3,0,0,0,
    -2.5,-0.8,-0.2,0,0,0,
    -2.5,-3,0,0,0,0,
    -2.5,-3,0.3,0,0,0,
    /* 4 sommets aile droit haut                         68*/
    -2.5,3,0.3,0,0,0,
    -2,3,0.3,0,0,0,
    -1,0.8,0.3,0,0,0,
    -2.5,0.8,0.3,0,0,0,
    /* 4 sommets aile droit bas                         56*/
    -2.5,3,0,0,0,0,
    -2,3,0,0,0,0,
    -1,0.8,-0.2,0,0,0,
    -2.5,0.8,-0.2,0,0,0,
    /* 4 sommets aile droit devant                         60*/
    -2,3,0.3,0,0,0,
    -2,3,0,0,0,0,
    -1,0.8,-0.2,0,0,0,
    -1,0.8,0.3,0,0,0,
    /* 4 sommets aile gauche derriere                         64*/
    -2.5,0.8,0.3,0,0,0,
    -2.5,0.8,-0.2,0,0,0,
    -2.5,3,0,0,0,0,
    -2.5,3,0.3,0,0,0,
    /* 4 sommets central milieu bas                         40*/
    -2.5,-0.8,-0.2,0,0,0,
    -1,-0.8,-0.2,0,0,0,
    -1,0.8,-0.2,0,0,0,
    -2.5,0.8,-0.2,0,0,0,
    /* 4 sommets trou derriere                  44*/
    -3,-0.2,0.2,0,0,0,
    -3,-0.2,-0.2,0,0,0,
    -3,0.2,-0.2,0,0,0,
    -3,0.2,0.2,0,0,0,
    /* 4 sommets trou gauche                  48*/
    -3,-0.2,0.2,0,0,0,
    -3,-0.2,-0.2,0,0,0,
    -2.5,-0.2,-0.2,0,0,0,
    -2.5,-0.2,0.2,0,0,0,
    /* 4 sommets trou droit                  52*/
    -3,0.2,0.2,0,0,0,
    -3,0.2,-0.2,0,0,0,
    -2.5,0.2,-0.2,0,0,0,
    -2.5,0.2,0.2,0,0,0,
    /* 4 sommets trou haut                  56*/
    -3,-0.2,0.2,0,0,0,
    -3,0.2,0.2,0,0,0,
    -2.5,0.2,0.2,0,0,0,
    -2.5,-0.2,0.2,0,0,0,
    /* 4 sommets trou bas                  60*/
    -3,-0.2,-0.2,0,0,0,
    -3,0.2,-0.2,0,0,0,
    -2.5,0.2,-0.2,0,0,0,
    -2.5,-0.2,-0.2,0,0,0,

  };
  /* Création du programme shader (voir le dossier shader) */
  _pId2 = gl4duCreateProgram("<vs>shaders/dep3d.vs", "<fs>shaders/dep3d.fs", NULL);
  /* Set de la couleur (RGBA) d'effacement OpenGL */
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
  glGenBuffers(1, &_buffer);
  /* Lier le VBO-ARRAY_BUFFER à l'identifiant du premier VBO généré */
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  /* Transfert des données VBO-ARRAY_BUFFER */
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
  /* Paramétrage 2 premiers indices d'attribut de sommet */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof *data, (const void *)0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof *data, (const void *)(6 * sizeof *data));
  /* dé-lier le VAO puis les VAO */
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {
    /* une variable d'angle, maintenant elle passe ne degrés */
  static GLfloat a = 0;
  GLfloat rouge[] = {1, 0, 0, 1}, vert[] = {0, 1, 0, 1}, bleu[] = {0, 0, 1, 1}, jaune[] = {1, 1, 0, 1};
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  glUseProgram(_pId);
  gl4duTranslatef(0, 0, -10.0);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -2.0, 0.0);
    gl4duRotatef(-90, 1, 0, 0);
    gl4duScalef(3.5f, 3, 3);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, vert);
  gl4dgDraw(_quad);

  gl4duTranslatef(0, -1, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, rouge);
  gl4dgDraw(_sphere);
  gl4duPopMatrix();
  gl4dgDraw(_sphere);

  gl4duRotatef(a, 0, 1, 0);
  gl4duTranslatef(3, 0, 0);
  gl4duPushMatrix(); {
    gl4duScalef(0.4f, 0.4f, 0.4f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, bleu);
  gl4dgDraw(_cube);
  gl4duRotatef(-3 * a, 1, 0, 0);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, jaune);
  gl4dgDraw(_torus);
  /* lier (mettre en avant ou "courante") la matrice vue créée dans
   * init */
   gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  gl4duSendMatrices();
  glUseProgram(_pId2);
  gl4duTranslatef(0, 0, -10.0);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, 2.0, 0.0);
    gl4duRotatef(i, 1, 1, 0);
    gl4duScalef(1,1, 1);
    gl4duSendMatrices();
  } gl4duPopMatrix();

  i++;
  if(i > 360)
    i = 0;
  
  printf("%d\n", i);
  glLineWidth(3.0f);
  /* Lier le VAO-machine-GL à l'identifiant VAO _vao */
  glBindVertexArray(_vao);



    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    glDrawArrays(GL_LINE_LOOP, 8, 4);
    glDrawArrays(GL_LINE_LOOP, 12, 4);
    glDrawArrays(GL_LINE_LOOP, 16, 4);
    glDrawArrays(GL_LINE_LOOP, 20, 4);
    glDrawArrays(GL_LINE_LOOP, 24, 4);
    glDrawArrays(GL_LINE_LOOP, 28, 4);
    glDrawArrays(GL_LINE_LOOP, 32, 4);
    glDrawArrays(GL_LINE_LOOP, 36, 4);
    glDrawArrays(GL_LINE_LOOP, 40, 4);
    glDrawArrays(GL_LINE_LOOP, 44, 4);
    glDrawArrays(GL_LINE_LOOP, 48, 4);
    glDrawArrays(GL_LINE_LOOP, 52, 4);
    glDrawArrays(GL_LINE_LOOP, 56, 4);
    glDrawArrays(GL_LINE_LOOP, 60, 4);
    glDrawArrays(GL_LINE_LOOP, 64, 4);
    glDrawArrays(GL_LINE_LOOP, 68, 4);
    glDrawArrays(GL_LINE_LOOP, 72, 4);
    glDrawArrays(GL_LINE_LOOP, 76, 4);
    glDrawArrays(GL_LINE_LOOP, 80, 4);
    glDrawArrays(GL_LINE_LOOP, 84, 4);
    glDrawArrays(GL_LINE_LOOP, 88, 4);
    glDrawArrays(GL_LINE_LOOP, 92, 4);
    glDrawArrays(GL_LINE_LOOP, 96, 4);
    glDrawArrays(GL_LINE_LOOP, 100, 4);
    glDrawArrays(GL_LINE_LOOP, 104, 4);
    glDrawArrays(GL_LINE_LOOP, 108, 4);
    glDrawArrays(GL_TRIANGLES, 0, 3);
     
    
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
  if(_buffer)
    glDeleteBuffers(1, &_buffer);
  /* nettoyage des éléments utilisés par la bibliothèque GL4Dummies */
  gl4duClean(GL4DU_ALL);
}