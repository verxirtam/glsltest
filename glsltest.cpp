/*
 * =====================================================================================
 *
 *       Filename:  glsltest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月24日 19時53分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "BasicShaderProgram.h"
#include "VAOPositionColor.h"

#include "Texture2D.h"
#include "TextureShaderProgram.h"


#include "glsltest_cuda.h"


BasicShaderProgram *tsp;
BasicShaderProgram::vaoType *vao;

VAOPositionColorDynamic<BasicShaderProgram> *vaod;

Texture2D *tex;
TextureShaderProgram *texsp;
TextureShaderProgram::vaoType *vaot;


glm::mat4 projection;



void setMatrix(void)
{
	//回転する角度
	static int degree = 0;
	//ビュー変換行列の設定
	glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 5.0f),//カメラの位置
			glm::vec3(0.0f, 0.0f, 0.0f),//視点の位置
			glm::vec3(0.0f, 1.0f, 0.0f) //カメラの上方向の向き
			);
	//モデル変換行列の設定
	//y軸周りに回転させる行列
	float d = 3.141592f * static_cast<float>(degree) / 180.0f;
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), d, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//モデル・ビュー・プロジェクション行列を設定
	glm::mat4 mvp = projection * view * model;
	
	//角度の更新
	degree += 1;
	degree = (degree >= 360) ? (degree - 360) : degree;
	
	tsp->setMVPMatrix(mvp);
	texsp->setMVPMatrix(mvp);
}

void moveVAO()
{
	vaod->map();
	
	float* v = vaod->getPositionDevicePointer();
	int vc = vaod->getVertexCount();
	//cudaの関数を呼ぶ
	moveVAO_cuda(v, vc);
	
	vaod->unmap();
}

void display(void)
{
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	vao->display();
	
	vaot->display();
	
	//moveVAO();
	
	vaod->display();
	
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}

void idle(void)
{
	//moveVAO();

	glutPostRedisplay();
}

void resize(int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);
	
	//プロジェクション行列の設定
	projection = glm::perspective( 3.141592f * 30.0f / 180.0f, static_cast<float>(w) / static_cast<float>(h), 1.0f, 1000.f);
}

void initCallbacks(void)
{
	//画面更新関数の設定
	glutDisplayFunc(display);
	//アイドル時に実行される関数の設定
	glutIdleFunc(idle);
	//画面リサイズ時に実行される関数の設定
	glutReshapeFunc(resize);
}


void initScene(void)
{
	
	projection = glm::perspective(3.141592f * 30.0f / 180.0f, 1.0f / 1.0f, 1.0f, 1000.f);
	
	{
		tsp->init();
		//位置データ
		std::vector<float> position_data
			{
				-0.8f,  0.2f, -0.5f,
				 0.8f,  0.2f,  0.5f,
				 0.0f,  1.8f,  0.0f
			};
		//色データ
		std::vector<float> color_data
			{
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			};
		
		std::vector<unsigned int> element_data
			{
				0,1,2
			};
		
		
		vao->init(position_data, color_data, element_data, GL_TRIANGLES);
	}
	
	{
		//位置データ
		std::vector<float> position_data
			{
				-0.8f, -0.8f, 0.0f,
				 0.8f, -0.8f, 0.0f,
				 0.0f,  0.8f, 0.0f
			};
		//色データ
		std::vector<float> texcoord_data
			{
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f
			};
		
		std::vector<unsigned int> element_data
			{
				0,1,2
			};
		texsp->init();
		tex->init
			(
				"/home/daisuke/programs/ATFViewer/res/JP-ENR-6.2-en-JP_20160204.raw",
				6928,
				4331
			);
		vaot->init(position_data, texcoord_data, element_data, GL_TRIANGLES);
	}
	{
		
		//位置データ
		std::vector<float> position_data
			{
				-1.0f, -1.0f, 0.5f,
				 1.0f, -1.0f, 0.5f,
				-1.0f,  0.0f, 0.5f,
				 1.0f,  0.0f, 0.5f,
				-1.0f,  1.0f, 0.5f,
				 1.0f,  1.0f, 0.5f
			};
		//色データ
		std::vector<float> color_data
			{
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			};
		
		std::vector<unsigned int> element_data
			{
				0,1,2,3,4,5
			};
		vaod->init(position_data, color_data, element_data, GL_TRIANGLE_STRIP);
	}
}

int main(int argc, char** argv)
{
	
	
	glutInit(&argc,argv);
	//ディスプレイモードの設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//ウィンドウの生成
	glutCreateWindow("glsltest");
	//glewの初期化
	GLenum err;
	err = glewInit();
	if(err != GLEW_OK)
	{
		std::cout << "error at glewInit()." << std::endl;
		return 1;
	}
	//コールバック関数の設定
	initCallbacks();
	
	tsp = new BasicShaderProgram();
	vao = new BasicShaderProgram::vaoType(*tsp);
	vaod = new VAOPositionColorDynamic<BasicShaderProgram>(*tsp);

	tex = new Texture2D(GL_TEXTURE0);
	texsp = new TextureShaderProgram();
	vaot = new TextureShaderProgram::vaoType(*texsp, *tex);
	
	//デプスバッファを有効にする
	glEnable(GL_DEPTH_TEST);
	
	//シーンの初期化
	initScene();
	//メインループ
	glutMainLoop();
	
	delete vaod;
	delete vao;
	delete tsp;
	delete vaot;
	delete texsp;
	
	return 0;
}




