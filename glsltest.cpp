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

#include "TestShaderProgram.h"
#include "VAOPositionColor.h"







TestShaderProgram *tsp;
VAOPositionColor<TestShaderProgram> *vao;


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
	
}



void display(void)
{
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	vao->display();
	
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}

void idle(void)
{
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
	tsp->init();
	
	
	projection = glm::perspective(3.141592f * 30.0f / 180.0f, 1.0f / 1.0f, 1.0f, 1000.f);
	
	//位置データ
	std::vector<float> position_data
		{
			-0.8f, -0.8f, 0.0f,
			 0.8f, -0.8f, 0.0f,
			 0.0f,  0.8f, 0.0f,
			-0.8f,  0.2f, 0.0f,
			 0.8f,  0.2f, 0.0f,
			 0.0f,  1.8f, 0.0f
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
			0,1,2,
			3,4,5
		};
	
	
	vao->init(position_data, color_data, element_data);
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
	
	tsp = new TestShaderProgram();
	vao = new VAOPositionColor<TestShaderProgram>(*tsp);
	
	//シーンの初期化
	initScene();
	//メインループ
	glutMainLoop();
	
	delete vao;
	delete tsp;
	
	return 0;
}




