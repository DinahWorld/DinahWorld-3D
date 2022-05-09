
#version 330
in  vec4 vsoColor;
out vec4 fragColor;

void main(void) {
  /* la couleur de sortie est le vsoColor linéairement interpolé au
     fragment (voir attribut flat pour désactiver l'interpolation). */
  fragColor = vsoColor;
}
