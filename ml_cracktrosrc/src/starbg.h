#include "sys/msys.h"

#include "asset_particle.h"

#define NUM_STARS 256
sprite starsprite[NUM_STARS];

GLuint stars_rendertex;
int stars_shader;
FBOELEM stars_fbo;


const char render_frag[] = ""
	"uniform sampler2D tex;"
	"void main()"
	"{"
	"vec2 offset = vec2(1.0)/vec2(1024.0,760.0);"
	"vec2 tc=gl_TexCoord[0].rg;"
	"vec4 color=texture2D(tex,tc);"
	"gl_FragColor=color;"
	"}";

const char blur_frag[] = ""
 "uniform sampler2D tex;"
 "void main()"
 "{"
   "const float x=1./1024.;"
   "vec4 t=vec4(0.);"
   "vec2 v=gl_TexCoord[0].rg;"
   "t+=texture2D(tex,vec2(v.r-4.*x,v.g))*.05;"
   "t+=texture2D(tex,vec2(v.r-3.*x,v.g))*.09;"
   "t+=texture2D(tex,vec2(v.r-2.*x,v.g))*.12;"
   "t+=texture2D(tex,vec2(v.r-x,v.g))*.15;"
   "t+=texture2D(tex,vec2(v.r,v.g))*.16;"
   "t+=texture2D(tex,vec2(v.r+x,v.g))*.15;"
   "t+=texture2D(tex,vec2(v.r+2.*x,v.g))*.12;"
   "t+=texture2D(tex,vec2(v.r+3.*x,v.g))*.09;"
   "t+=texture2D(tex,vec2(v.r+4.*x,v.g))*.05;"
   "gl_FragColor=t;"
 "}";


void stars_init()
{
	GLuint stars_sprite = loadTGATextureMemory((unsigned char*)particle,particle_len,false);
	stars_fbo = init_fbo(XRES,YRES);
	initShader(  &stars_shader, (const char*)vert, (const char*)blur_frag);

	for (int i=0;i<NUM_STARS;i++)
	{
		float tSize = 128;
		starsprite[i].xsize = tSize;
		starsprite[i].ysize = tSize;
		starsprite[i].speed = rand_rangef(1.0,6.5);
		starsprite[i].x = rand_range(0,XRES);
		starsprite[i].y = rand_range(0,YRES);
		starsprite[i].texture = stars_sprite;
		starsprite[i].acol = rand_rangef(10.0,32.0)*0.01;
		starsprite[i].rcol = 90.0*.01;
		starsprite[i].gcol = 40.0*.01;
		starsprite[i].bcol = 150.0*.01;
	}


}


void draw_star(sprite spr, int xres, int yres, bool flip_y = false)
{
	float zPos = 0.0;
	float  tX=spr.xsize/2.0f;
	float  tY=spr.ysize/2.0f;
	if (flip_y)BeginOrtho2D(xres,yres,true);
	else
		BeginOrtho2D(xres,yres);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,spr.texture);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE,GL_ONE);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	//glBlendFunc(GL_DST_COLOR, GL_ONE);

	glPushMatrix();  // Save modelview matrix

	glTranslatef(spr.x,spr.y,0.0f);  // Position sprite
	glColor4f(spr.rcol,spr.gcol,spr.bcol,spr.acol);
	glBegin(GL_QUADS);                                   // Draw sprite
	glTexCoord2f(0.0f,0.0f); glVertex3i(-tX, tY,zPos);
	glTexCoord2f(0.0f,1.0f); glVertex3i(-tX,-tY,zPos);
	glTexCoord2f(1.0f,1.0f); glVertex3i( tX,-tY,zPos);
	glTexCoord2f(1.0f,0.0f); glVertex3i( tX, tY,zPos);
	glEnd();
	glPopMatrix();  // Restore modelview matrix
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	EndProjection();
}

void stars_render(float time)
{


	start_fbo(stars_fbo.fbo,XRES,YRES);
	
	Resize(XRES,YRES);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i=0;i<NUM_STARS;i++)
	{
		starsprite[i].y+=starsprite[i].speed;
		if(starsprite[i].y > YRES+40) starsprite[i].y = -24;
		draw_sprite(starsprite[i],XRES,YRES);

	}
	end_fbo();




}

void stars_draw()
{
    int shadertex =oglGetUniformLocation(stars_shader, "tex");
	oglUniform1i(shadertex, 0);
	oglUseProgram(stars_shader );
	draw_fbotexture(stars_fbo.texture,XRES,YRES);
	oglUseProgram( NULL );
}
