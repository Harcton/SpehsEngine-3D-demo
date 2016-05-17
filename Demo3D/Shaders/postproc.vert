attribute vec2 vertexPosition;
varying vec2 texPos;

uniform sampler2D texture;

void main(void)
{
  gl_Position = vec4(vertexPosition, 0.0, 1.0);
  texPos = (vertexPosition + 1.0) / 2.0;
}