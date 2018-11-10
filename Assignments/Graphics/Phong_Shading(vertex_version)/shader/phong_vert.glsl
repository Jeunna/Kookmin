uniform mat4 u_pvm_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;

uniform vec3 u_light_vector;
uniform vec4 u_light_ambient;
uniform vec4 u_light_diffuse;
uniform vec4 u_light_specular;

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_material_shininess;

//model coordinate
attribute vec3 a_vertex;
attribute vec3 a_normal;

varying vec4 v_color;

//빛을 이용해서 계산한 컬러값
vec4 directional_light()
{
  vec4 color = vec4(0); //검정색

  //model -> world coordinate
  vec3 vertex_wc = (u_model_matrix * vec4(a_vertex, 1.0)).xyz;
  vec3 normal_wc = u_normal_matrix * a_normal;
  normal_wc = normalize(normal_wc);

  vec3 light_vector_wc = normalize(u_light_vector);
  vec3 light_incident_vector_wc = -light_vector_wc;   //(입사방향인데 무한히 멀리 있다고 생각하기 때문에 그냥 반대..?) 빛이 들어오는 방향
  vec3 reflect_vector_wc = reflect(light_incident_vector_wc, normal_wc);  //빛의 반사 방향. 들어오는 빛을 기준으로 어디로 반사할건지(normal)

  vec3 view_position_wc = vec3(u_view_matrix * vec4(0, 0, 0, 1)); //시점 위치
  vec3 view_vector_wc = view_position_wc - vertex_wc; //시점위치 - 현재 포인트 위치 = 시점위치로 향하는 방향 벡터
  view_vector_wc = normalize(view_vector_wc);

  color += u_material_ambient * u_light_ambient;
  
  float ndotl = max(0.0, dot(normal_wc, light_vector_wc)); // 빛은 - 될 수 없으니까
  color += u_material_diffuse * ndotl * u_light_diffuse;

  float rdotv = max(0.0, dot(reflect_vector_wc, view_vector_wc));
  color += u_material_specular * pow(rdotv, u_material_shininess) * u_light_specular;

  return color;
}

void main() 
{
  v_color = directional_light();
  
  gl_Position = u_pvm_matrix * vec4(a_vertex, 1);
}
