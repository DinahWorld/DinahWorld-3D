/*!\file dep3d.fs
 *
 * \brief rendu avec lumière directionnelle diffuse et couleur.
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr 
 * \date April 15 2016
 */
#version 330
uniform vec4 couleur;
in  vec3 vsoNormal;
out vec4 fragColor;

void main(void) {
  vec3 N = normalize(vsoNormal);
  vec3 L = normalize(vec3(-1, -1, 0)); /*vers le bas vers la gauche*/
  float diffuse = dot(N, -L);
  fragColor = vec4((couleur.rgb), couleur.a);
}
